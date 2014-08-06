/**
* ness-engine Log macros, print to the console window only when in debug mode.
* Author: Ronen Ness
*/

#pragma once

#ifdef _DEBUG

	#define NESS_LOG(__str_)				printf("NessLog: %s\r\n", __str_)
	#define NESS_ERROR(__str_)				printf("NessErr: %s\r\n", __str_)

#else

	#define NESS_LOG(__str_) 
	#define NESS_ERROR(__str_) 

#endif