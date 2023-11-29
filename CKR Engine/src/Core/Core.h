#pragma once
//#include "Log.h"
#include <Memory>

#ifdef GE_PLATFORM_WINDOW
#if  GE_DYNAMIC_LINK
//if build game engine library, should export
#ifdef GE_BUILD_DLL
#define GE_API __declspec(dllexport)
//if build game application, should import
#else 
#define GE_API __declspec(dllimport)
#endif
#else
#define GE_API
#endif //  GE_DYNAMIC_LINK
#else // GE_BUILD_DLL
	#error CKR Game Engine only support Window!
#endif;

#define BIT(x) (1 << x)



#ifdef GE_DEBUG
	#define GE_ENABLE_ASSERTS
#endif 

#include "Core/Base/Log.h"
#ifdef GE_ENABLE_ASSERTS
 #define GE_ASSERT(x, ...) { if(!(x)) { Log_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define GE_CORE_ASSERT(x, ...){if(!(x)) {GELog_Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define GE_ASSERT(x,...)
#define GE_CORE_ASSERT(x,...)
#endif

#define GE_BEVENT_FN(Func) std::bind(&Func, this, std::placeholders::_1)

namespace GE {
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}



