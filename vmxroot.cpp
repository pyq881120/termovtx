#define NTL_NO_NEW
#include "common.h"
#include "vmxroot.h"
#include "cpuarch.hxx"

namespace vmxroot
{
	using namespace ntl::intrinsic;
	namespace msrs = ntl::km::MSR::IA32;

	using namespace detail;

    cpu CheckCPU()
    {   
        TraceFunction();
        int32_t cpuid[4];

        // check cpu model
        __cpuid(cpuid, 0);
        // intel
        if(cpuid[regEbx] != 'uneG'){
            // amd
            if(cpuid[regEbx] != 'htuA'){
                dbg::error("Unsupported cpu type\n");
                return unsup;
            }
            return Athlon;
        }
        return Intel;
    }

    namespace vmxintel
    {
	    bool CheckHardware()
	    {
		    TraceFunction();

		    int32_t cpuid[4];

		    // check features
		    uint8_t model, family;
		    bool vmxpresent = false;

		    __cpuid(cpuid, 1);
		    model = uint8_t((cpuid[regEax] >> 4) & 0xF);
		    family =uint8_t((cpuid[regEax] >> 8) & 0xF);
		    vmxpresent = ((cpuid[regEcx]>>5) & 1) != 0; //VMX avalability
		    if(family == 0x06 && model >= 0x07){
			    // core2+
			    g_drv->cpu = drv_t::Core2;
		    }

		    dbg::info.printf("cpu family %X, model %X;", family, model);
            dbg::info.printf("VMX bit is: %d\n", vmxpresent);

		    return vmxpresent;
	    }

	    void Entering()
	    {
		    TraceFunction();
		    cpuarch::CR4 cr4; cr4.value = __readcr4();
		    if (cr4.SMXE)
		    {
			    dbg::error("SMXE bit is set\n");
			    return;
		    }
		    if (cr4.VMXE)
		    {
			    dbg::error("VMXE bit is set\n");
			    return;
		    }
		    cpuarch::CR0 cr0; cr0.value = __readcr0();
		    if (!cr0.PE&&!cr0.NE&&!cr0.PG)
		    {
			    dbg::error("VMX can`t be execute. CR0(PE,NE,PG) bit is clear\n");
			    return;
		    }
		    dbg::info.printf("cpu.CR4:: SMXE = %d, VMXE = %d\n", cr4.SMXE, cr4.VMXE);
		    dbg::info.printf("cpu CR0:: PE = %d, NE = %d, PG = %d\n", cr0.PE, cr0.NE, cr0.PG);

		    msrs::feature_control fctrl_msr;
		    fctrl_msr.value = __readmsr(msrs::Core2::FeatureControl);
		    dbg::info.printf("cpu MSR.feature_ctrl:: VmxLock = %X, EnableInsideSmx = %X, EnableOutsideSmx = %X",
			    fctrl_msr.VmxLock,fctrl_msr.EnableInsideSmx,fctrl_msr.EnableOutsideSmx);
		    if (fctrl_msr.VmxLock&fctrl_msr.EnableOutsideSmx)
		    {
			    dbg::error("VMX can`t be execute. MSR feature control bit is clear\n");
			    return;
		    }

		    msrs::vmx_basic vmx_basic_msr; 
		    vmx_basic_msr.value = __readmsr(msrs::VmxBasic);
		    dbg::info.printf("cpu MSR.vmx_basic:: RevID = %X",vmx_basic_msr.Revision);

		    //LONG_PTR pVMCS_VA;
		    //....

		    //memcpy(pVMCS_VA,&vmx_basic_msr.Revision,sizeof(vmx_basic_msr.Revision));
		    //physical_address VMCS_PA = MmGetPhysicalAddress(pVMCS_VA);
		    //__vmx_on(&VMCS_PA);
    		
		    //dbg::info.printf("VMX is ON");

	    }

	    void Leaving()
	    {
		    TraceFunction();
		    __vmx_off();
		    cpuarch::CR4 cr4; cr4.value = __readcr4();
		    cr4.VMXE = 0;
		    __writecr4(cr4.value);
		    dbg::info.printf("VMX is OFF\n cpu.CR4:: SMXE = %d, VMXE = %d\n", cr4.SMXE, cr4.VMXE);
	    }
    } //vmxintel

    namespace msrs_amd = ntl::km::AMD;

    namespace vmxamd
    {
        bool CheckHardware()
        {
            TraceFunction();

            int32_t cpuid[4];

            // check features
            uint8_t model, family;
            bool svmpresent = false;

            __cpuid(cpuid, 1);
            model = uint8_t((cpuid[regEax] >> 4) & 0xF);
            family =uint8_t((cpuid[regEax] >> 8) & 0xF);
            __cpuid(cpuid, 0x80000001);
            svmpresent = ((cpuid[regEcx]>>2) & 1) != 0; //VMX avalability

            if(family == 0x0F && model >= 0x08){
                // Athlon X2 +
                g_drv->cpu = drv_t::AthlonX2;
            }

            dbg::info.printf("cpu family %X, model %X;", family, model);
            dbg::info.printf("SVM bit is: %d\n", svmpresent);

            return svmpresent;
        }

        void Entering()
        {
            TraceFunction();

            cpuarch::CR0 cr0; cr0.value = __readcr0();
            if (!cr0.PE&&!cr0.NE&&!cr0.PG)
            {
                dbg::error("VMX can`t be execute. CR0(PE,NE,PG) bit is clear\n");
                return;
            }
            dbg::info.printf("cpu CR0:: PE = %d, NE = %d, PG = %d\n", cr0.PE, cr0.NE, cr0.PG);

            msrs_amd::vm_cr vm_cr_msr;
            vm_cr_msr.value = __readmsr(msrs_amd::VM_CR);
            if (vm_cr_msr.SVMDIS)
            {
                dbg::error("SVM is not allowed\n");
                return;
            }
    
            int32_t cpuid[4];
            __cpuid(cpuid, 0x8000000A);
            
            if (((cpuid[regEcx]>>2) & 1) !=0)
            {
                dbg::error("SVM is LOCK. Ñonsult the BIOS or TPM to unlock SVM\n");
                return;
            }

            msrs_amd::efer efer_msr;
            efer_msr.value = __readmsr(msrs_amd::Efer);
            dbg::info.printf("cpu.efer_msr:: SVME = %d\n", efer_msr.SVME);
            if (efer_msr.SVME==1)
            {
                dbg::error("SVME bit is set..exit\n");
                return;
            }
            dbg::info.printf("Write SVME bit...\n");
            efer_msr.SVME = 1;
            __writemsr(msrs_amd::Efer,efer_msr.value);
            if (efer_msr.SVME==0)
            {
                dbg::error("SVME bit set is falure\n");
                return;
            }

            dbg::info.printf("cpu.efer_msr:: SVME = %d after wtire bit\n", efer_msr.SVME);

            msrs_amd::vmcb* NewVMCB = new(nonpaged) msrs_amd::vmcb;
            uintptr_t VMCS_PA = (uintptr_t)MmGetPhysicalAddress(NewVMCB);
            std::memset(NewVMCB,0,sizeof(msrs_amd::vmcb));
            __svm_vmrun(VMCS_PA);

            dbg::info.printf("SVM is ON");
        }

        void Leaving()
        {
            TraceFunction();

            msrs_amd::efer efer_msr;
            efer_msr.value = __readmsr(msrs_amd::Efer);
            if (efer_msr.SVME==0)
            {
                dbg::error("SVME bit is alredy clear\n");
                return;
            }

            dbg::info.printf("Clears the global interrupt flag...\n");

            __svm_clgi();
            __sti();

            dbg::info.printf("Write SVME bit...\n");
            efer_msr.SVME = 0;
            __writemsr(msrs_amd::Efer,efer_msr.value); 
            if (efer_msr.SVME)
            {
                dbg::error("SVME bit set is falure, Can`t disable SVM\n");
                return;
            }
            dbg::info.printf("cpu.efer_msr:: SVME = %d after wtire bit\n", efer_msr.SVME);
        }
    }

} // vmxroot