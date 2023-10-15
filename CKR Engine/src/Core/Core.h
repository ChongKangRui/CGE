#pragma once

#ifdef GE_PLATFORM_WINDOW
	//if build game engine library, should export
	#ifdef GE_BUILD_DLL
		#define GE_API __declspec(dllexport)
	//if build game application, should import
	#else 
		#define GE_API __declspec(dllimport)
	#endif
#else // GE_BUILD_DLL
	#error CKR Game Engine only support Window!
#endif;

#define BIT(x) (1 << x)

#ifdef GE_ENABLE_ASSERTS
#define GE_ASSERT(x,...){if(!(x)) {Log_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();)}}
#define GE_CORE_ASSERT(...){if(!(x)) {GELog_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();)}}
#else
#define GE_ASSERT(x,...)
#define GE_CORE_ASSERT(x,...)
#endif

#define GE_BEVENT_FN(Func) std::bind(&Func, this, std::placeholders::_1)



