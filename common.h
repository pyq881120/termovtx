#pragma once

#include <cstdio>
#include <cstring>

#include <sal.h>

#include <km/basedef.hxx>
#include <km/new.hxx>
#include <km/memory.hxx>
#include <km/driver_object.hxx>
#include <km/irp.hxx>

#include <km/process.hxx>
#include <km/image.hxx>
#include <nt/peb.hxx>
#include <km/object.hxx>

//#include "ntl-deprecated/osver.hxx"

#include <km/file.hxx>

#include "logging.h"
#include <km/debug.hxx>

#include "config.h"

using namespace ntl;
using namespace ntl::km;
using std::wstring;
using ntl::km::nonpaged;

// NtBuildNumber
#define NT_BUILD_W2K    2195
#define NT_BUILD_XP     2600
#define NT_BUILD_W2K3   3790
#define NT_BUILD_VISTA  6000
#define NT_BUILD_W2K8   6001


typedef std::basic_string<char, std::char_traits<char>, ntl::km::npallocator<char> > kmstring;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, ntl::km::npallocator<wchar_t> > kmwstring;
typedef uint8_t byte;

typedef ntl::atomic_exec<fast_mutex, void> fast_synchronize;

#define TraceFunction() dbg::trace(DN __FUNCTION__)
#define TRACE_STATUS(st, msg) dbg::trace.printf(DN __FUNCTION__ ": " msg " returned 0x%p\n", st);
#define CHECK_STATUS(st, msg) TRACE_STATUS(st, msg); if(!success(st)) return st;
#define CHECK_STATUSV(st, msg) TRACE_STATUS(st, msg); if(!success(st)) return;


template<class T, size_t size>
struct _SIZEOF_T
{
	char buf[0];

	_SIZEOF_T(){}
};
template<class T, size_t offset>
struct _OFFSETOF_T
{
	char buf[0];

	_OFFSETOF_T(){}
};


#define _SIZEOF(T) _SIZEOF_T<T, sizeof(T) >::_SIZEOF_T();
#define _OFFSETOF(T,F) _OFFSETOF_T<T, offsetof(T,F) >::_OFFSETOF_T();
#define sizeofa(a)(sizeof(a)/sizeof(*a))

namespace ptr
{
  template<typename T, typename U>
  inline T round_to_size(T size, U alignment)
  {
    return (T)( (uintptr_t(size) + uintptr_t(alignment) - 1) & ~(uintptr_t(alignment)-1) );
  }

  template<typename T, typename U>
  inline T padd(T p, U offset)
  {
    return (T)( uintptr_t(p) + uintptr_t(offset) );
  }

  template<typename R, typename T, typename U>
  inline R* padd(T p, U offset)
  {
    return (R*)( uintptr_t(p) + uintptr_t(offset) );
  }
}

//////////////////////////////////////////////////////////////////////////
// May by crete class???
struct drv_t
{
  enum CpuSupported {
    Unknown,
    P4,
    PM,
    Core,
    Core2,
    Athlon  = 0x10,
    AthlonX2,
  };

  bool log_enabled;
  wstring driver_key;
  wstring logfile;
  driver_object* driver;

  CpuSupported cpu;

  drv_t()
    :log_enabled(false),
    driver(),
    cpu()
  {}
};
extern drv_t* g_drv;

//////////////////////////////////////////////////////////////////////////
void GetTime(ntl::km::time_fields* tm);
#define TIME(tbuf) char tbuf[32]; ntl::km::time_fields tm; GetTime(&tm); _snprintf(tbuf, sizeof(tbuf), "%02d.%02d %02d:%02d:%02d", tm.Day, tm.Month, tm.Hour, tm.Minute, tm.Second);

//////////////////////////////////////////////////////////////////////////
uint32_t efilter(exception::pointers* einfo, const char* function);

////////////////////////////////////
// Fix Size
#define PAGE_SIZE 4096
