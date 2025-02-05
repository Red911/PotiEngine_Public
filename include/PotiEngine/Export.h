#pragma once

#ifdef POTI_ENGINE_BUILD
	#define POTI_ENGINE_API __declspec(dllexport)
#else 
	#define POTI_ENGINE_API __declspec(dllimport)
#endif
