#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/fmt/ostr.h"

// Use this instead of static_assert
#define ENGINE_LOG_ASSERT(x, ...) { if(!(x)) { ENGINE_CORE_ERROR("Program has crashed. Please check crash log for details."); ENGINE_CORE_ERROR("Assertion Failed! {0}", __VA_ARGS__); /*engine->GetWindow()->SetExit(true);*/ __debugbreak(); } }

namespace Eclipse
{
	class Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetConsoleLogger();
		static std::shared_ptr<spdlog::logger>& GetCrashLogger();
	private:
		static std::shared_ptr<spdlog::logger> sConsole;
		static std::shared_ptr<spdlog::logger> sLogCrash;
	};
}

/**************************************************************************************/
/*         ONLY USE THIS WHEN THINGS ARE DONE BEFORE EDITOR INITIALIZATION            */
/**************************************************************************************/
// Inclusive in Log Assert so use ENGINE_LOG_ASSERT instead
// TEXT COLOR -> RED
#define ENGINE_CORE_ERROR(...) Eclipse::Log::GetCrashLogger()->error(__VA_ARGS__);

// For normal messages like initialization or creating entities, etc
// TEXT COLOR -> GREEN
#define ENGINE_CORE_INFO(...) Eclipse::Log::GetConsoleLogger()->info(__VA_ARGS__);

// To predict undefined behaviour such as undefined tags, nan values
// Basically things that don't make the engine crash but may give undesirable results
// TEXT COLOR -> YELLOW
#define ENGINE_CORE_WARN(...) Eclipse::Log::GetConsoleLogger()->warn(__VA_ARGS__);

//#define ENGINE_CORE_FATAL(...) Transcend::Log::GetConsoleLogger()->fatal(__VA_ARGS__);
