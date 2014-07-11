#ifndef EXPORT_DEF_H_
#define EXPORT_DEF_H_


#ifdef NESSENGINE_EXPORTS
#define NESSENGINE_API __declspec(dllexport)
#elif defined(INFRALIB_EXPORTS_STATIC)
#define NESSENGINE_API 
#else
#define NESSENGINE_API __declspec(dllimport)
#endif 

#endif