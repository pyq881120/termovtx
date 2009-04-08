#include "common.h"
#include "vmxroot.h"

// driver functions
void DrvUnload(__in driver_object*);
void DrvReinitialize(__in driver_object* DriverObject,__in_opt void* Context,__in uint32_t Count);

// I/O Dispatch
ntstatus DrvDispatchShutdown(__in device_object* DeviceObject, __in irp* Irp);
ntstatus DrvDeviceIoControl(__in device_object* DeviceObject, __in irp* Irp);

// Create Close
ntstatus DrvDispatchCreateClose(__in device_object* DeviceObject, __in irp* Irp);


//////////////////////////////////////////////////////////////////////////

// global object
drv_t* g_drv = NULL;


/************************************************************************/
/* ENTRY                                                                */
/************************************************************************/
ntstatus DrvEntry(__in driver_object* DriverObject, __in unicode_string* RegistryPath)
{
	TraceFunction();

	g_drv = new(nonpaged) drv_t();
	if(!g_drv)
		return status::insufficient_resources;

    using namespace vmxroot;   

	switch(CheckCPU())
	{
	case Intel:
        if(vmxintel::CheckHardware())
		{
            vmxintel::Entering();
		}
		break;
        
	case Athlon:
        if(vmxamd::CheckHardware())
        {
            vmxamd::Entering();
        }
        break;

	default:

		break;
	}


	DriverObject->MajorFunction[irp::mj_shutdown] = &DrvDispatchShutdown;

	DriverObject->MajorFunction[irp::mj_create] = &DrvDispatchCreateClose;
	DriverObject->MajorFunction[irp::mj_close] = &DrvDispatchCreateClose;

	DriverObject->DriverUnload = DrvUnload;

	return status::success;

	//#endif
}

void DrvUnload(__in driver_object*)
{
	TraceFunction();

    using namespace vmxroot; 

    switch(CheckCPU())
    {
    case Intel:
        if(vmxintel::CheckHardware())
        {
            vmxintel::Leaving();
        }
        break;

    case Athlon:
        if(vmxamd::CheckHardware())
        {
            vmxamd::Leaving();
        }
        break;

    default:

        break;
    }

	// delete global object
	delete g_drv;

	// bye
	TIME(tbuf);
	dbg::trace.printf("%s driver unloaded\n", tbuf);
}

/************************************************************************/
/* I/O Dispatch                                                         */
/************************************************************************/
ntstatus DrvDispatchShutdown(__in device_object* /*DeviceObject*/, __in irp* /*Irp*/)
{
	TraceFunction();
	return status::success;
}


/************************************************************************/
/* CREATE CLOSE                                                         */
/************************************************************************/
ntstatus DrvDispatchCreateClose(__in device_object* DeviceObject, __in irp* Irp)
{
	TraceFunction();

	Irp->IoStatus.Status = status::success;
	Irp->IoStatus.Information = 0;

	Irp->complete_request();

	return status::success;
} 

/************************************************************************/
/* WORK STUFF                                                           */
/************************************************************************/


/************************************************************************/
/* MISC                                                                 */
/************************************************************************/
uint32_t efilter(exception::pointers* einfo, const char* function)
{
	dbg::error.printf("*** "DN"-%s: Unhandled exception caught!\n\tcode: 0x%p, flags: 0x%X, at address: 0x%p\n", 
		function, einfo->ExceptionRecord->ExceptionCode, einfo->ExceptionRecord->ExceptionFlags,
		einfo->ExceptionRecord->ExceptionAddress);
	if(einfo->ExceptionRecord->ExceptionCode == status::access_violation){
		dbg::error.printf("\tAttempt to %s address 0x%p.\n", einfo->ExceptionRecord->ExceptionInformation[0] ? "write to" : "read from",
			einfo->ExceptionRecord->ExceptionInformation[1]);
	}
	//if(!*KdDebuggerNotPresent)
	//  dbg::bp();
	return exception_execute_handler;
}

void GetTime(ntl::km::time_fields* tm)
{
	int64_t st, lt;
	KeQuerySystemTime(&st);
	ExSystemTimeToLocalTime(&st, &lt);
	RtlTimeToTimeFields(&lt, tm);
}
