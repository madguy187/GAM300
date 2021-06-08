#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"

#define ENGINE_LOG_ASSERT(x, ...) { if(!(x)) { ENGINE_CORE_ERROR("Program has crashed. Please check crash log for details."); ENGINE_CORE_ERROR("Assertion Failed! {0}", __VA_ARGS__); /*engine->GetWindow()->SetExit(true);*/ __debugbreak(); } }

namespace Eclipse
{
	class Log
	{
		static std::shared_ptr<spdlog::logger> sConsole;
		static std::shared_ptr<spdlog::logger> sLogCrash;
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetConsoleLogger();
		static std::shared_ptr<spdlog::logger>& GetCrashLogger();
	};
}

#define ENGINE_CORE_ERROR(...) Eclipse::Log::GetCrashLogger()->error(__VA_ARGS__);
#define ENGINE_CORE_INFO(...) Eclipse::Log::GetConsoleLogger()->info(__VA_ARGS__);
#define ENGINE_CORE_WARN(...) Eclipse::Log::GetConsoleLogger()->warn(__VA_ARGS__);
//#define ENGINE_CORE_FATAL(...) Transcend::Log::GetConsoleLogger()->fatal(__VA_ARGS__);
