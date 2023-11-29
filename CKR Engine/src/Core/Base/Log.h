#pragma once
#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace GE {
	class GE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() {
			return s_EngineLogger;
		}
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}


	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;




	};
}

//Engine log macros
#define GELog_Error(...) ::GE::Log::GetEngineLogger()->error(__VA_ARGS__)
#define GELog_Warn(...) ::GE::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define GELog_Trace(...) ::GE::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define GELog_Fatal(...) ::GE::Log::GetEngineLogger()->critical(__VA_ARGS__)
#define GELog_Info(...) ::GE::Log::GetEngineLogger()->info(__VA_ARGS__)


//Client log macros
#define Log_Error(...) ::GE::Log::GetClientLogger()->error(__VA_ARGS__)
#define Log_Warm(...) ::GE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define Log_Trace(...) ::GE::Log::GetClientLogger()->trace(__VA_ARGS__)
#define Log_Fatal(...) ::GE::Log::GetClientLogger()->critical(__VA_ARGS__)
#define Log_Info(...) ::GE::Log::GetClientLogger()->info(__VA_ARGS__)