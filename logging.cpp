#define NTL_NO_NEW
#include "common.h"
#include "logging.h"


struct log_t: file
{
	fast_mutex guard;
}* g_log = NULL;

//static file* g_log = NULL;

bool LogInit(const const_unicode_string& log_name)
{
	delete g_log;
	g_log = new (nonpaged) log_t();
	if(!g_log){
		dbg::error("Failed allocate log file object");
		return false;
	}

	bool ok = g_log->create(log_name.get_string(), file::open_if, file::append_data);
	if(!ok){
		dbg::error("Failed to open log file");
		delete g_log;
		g_log = NULL;
	}else{
		LogPut(const_ansi_string("\n-- "), false);
	}
	return ok;
}

void LogPut(const void* p, uint32_t cb, bool timespamp)
{
	if(g_log){
		fast_synchronize lock(g_log->guard);
		if(timespamp){
			char tbuf[32];
			time_fields tm;
			GetTime(&tm);
			uint32_t tcb = (uint32_t)_snprintf(tbuf, sizeof(tbuf), "%02d.%02d %02d:%02d:%02d ", tm.Day, tm.Month, tm.Hour, tm.Minute, tm.Second);
			g_log->write(tbuf, tcb);
		}
		g_log->write(p, cb);
	}
}

void LogCleanup() 
{
	delete g_log;
	g_log = NULL;
}