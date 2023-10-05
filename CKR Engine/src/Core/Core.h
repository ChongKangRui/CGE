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





