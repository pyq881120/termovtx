#pragma once
/**
 *	@file logging.h
 *	@brief Logging routines
 **/

bool LogInit(const ntl::km::const_unicode_string& log_name);
void LogCleanup();
void LogPut(const void* p, uint32_t cb, bool timespamp = true);
inline void LogPut(const ntl::km::const_ansi_string& msg, bool timespamp = true)
{
	LogPut(msg.data(), (uint32_t) msg.length()*sizeof(ntl::km::const_ansi_string::value_type), timespamp);
}
