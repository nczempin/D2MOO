#ifndef STORM_MEMORY_HPP
#define STORM_MEMORY_HPP

#include <D2Dll.h>
#include <cstdint>
#include <cstdlib>

STORM_DLL_DECL void* __stdcall SMemAlloc(size_t bytes, const char* logfilename, int logline, uint32_t flags); // Storm.#401
STORM_DLL_DECL int32_t __stdcall SMemFree(void* location, const char* logfilename, int logline, uint32_t flags); // Storm.#403
inline void SMemFree(void* ptr) { SMemFree(ptr, __FILE__, __LINE__, 0); }
STORM_DLL_DECL size_t __stdcall SMemGetSize(void* location, const char* logfilename, int logline); // Storm.#404
STORM_DLL_DECL void* __stdcall SMemReAlloc(void* location, int amount, const char* logfilename, int logline, uint32_t flags); // Storm.#405

#endif
