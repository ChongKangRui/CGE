#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"

namespace GEngine {
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
#define GELog_Error(...) ::GEngine::Log::GetEngineLogger()->error(__VA_ARGS__)
#define GELog_Warn(...) ::GEngine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define GELog_Trace(...) ::GEngine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define GELog_Fatal(...) ::GEngine::Log::GetEngineLogger()->critical(__VA_ARGS__)
#define GELog_Info(...) ::GEngine::Log::GetEngineLogger()->info(__VA_ARGS__)


//Client log macros
#define Log_Error(...) ::GEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define Log_Warm(...) ::GEngine::Log::GetClientLogger()->warm(__VA_ARGS__)
#define Log_Trace(...) ::GEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define Log_Fatal(...) ::GEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)
#define Log_Info(...) ::GEngine::Log::GetCClientogger()->info(__VA_ARGS__)