#pragma once

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/fmt/ostr.h"

// Use this instead of static_assert
#define ENGINE_LOG_ASSERT(x, ...) { if(!(x)) { ENGINE_CORE_WARN("Program has crashed. Please check crash log for details."); ENGINE_CORE_ERROR("Assertion Failed! {0}", my_strcat(__FILE__, " Line ", __LINE__, ": ", __VA_ARGS__).c_str()); std::stringstream ss; ss<<"File: "<< __FILE__<< "\n"<< "Line: "<< __LINE__<< "\n"<< "\n"<< "For more information, please check crash log for details."<< "\n"<< "\n"<< "(Click Retry to debug application)"; wchar_t* msg = charToWChar(ss.str().c_str()); int msgID = MessageBox( nullptr, msg, TEXT("Debug Assertion Failed!"), MB_ABORTRETRYIGNORE ); delete[] msg; if (msgID == IDRETRY) { __debugbreak(); } else if (msgID == IDABORT) { glfwSetWindowShouldClose(OpenGL_Context::GetWindow(), 1); } } }

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
