#include "pch.h"
#include "Logger.h"

namespace Eclipse
{
	std::shared_ptr<spdlog::logger> Log::sConsole;
	std::shared_ptr<spdlog::logger> Log::sLogCrash;

	void Log::Init()
	{
		sConsole = spdlog::stdout_color_mt("sConsole");
		sConsole->set_level(spdlog::level::info);
		spdlog::set_pattern("%^[%D %T] [CORE] [%l] %n: %v%$");

		sLogCrash = spdlog::basic_logger_mt("Crash Report", "Logs/Crash Report.txt");
		spdlog::set_default_logger(sLogCrash);
		spdlog::flush_on(spdlog::level::critical);
	}

	std::shared_ptr<spdlog::logger>& Log::GetConsoleLogger()
	{
		return sConsole;
	}

	std::shared_ptr<spdlog::logger>& Log::GetCrashLogger()
	{
		return sLogCrash;
	}
}