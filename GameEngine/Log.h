#pragma once

enum class LOGLEVEL
{
	LOG_ERRORS = 0,
	LOG_WARNINGS,
	LOG_DEBUG,
	NO_LOG
};

class Log
{
public:
	Log();
	virtual ~Log();

	Log(const Log&  l) = delete;
	Log(const Log&& l) = delete;
	Log& operator=(const Log&  l) = delete;
	Log& operator=(const Log&& l) = delete;

	LOGLEVEL m_level;
};

