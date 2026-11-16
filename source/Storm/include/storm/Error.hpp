#ifndef STORM_ERROR_HPP
#define STORM_ERROR_HPP

#include <D2Dll.h>
#include <WinError.h>
#include <cstdint>

#if defined(NDEBUG)
#define STORM_ASSERT(x) (void)0
#else
#define STORM_ASSERT(x)                          \
    if (!(x)) {                                  \
        SErrPrepareAppFatal(__FILE__, __LINE__); \
        SErrDisplayAppFatal(#x);                 \
    }                                            \
    (void)0
#endif

#define STORM_VALIDATE(x, y, ...)                \
    if (!(x)) {                                  \
        SErrSetLastError(y);                     \
        return __VA_ARGS__;                      \
    }                                            \
    (void)0

STORM_DLL_DECL int32_t __stdcall SErrDisplayError(uint32_t messageId, const char* filename, int32_t linenumber, const char* description, int32_t recoverable, uint32_t exitcode); // Storm.#461
STORM_DLL_DECL __declspec(noreturn) void __cdecl SErrDisplayAppFatal(char* Format, ...); // Storm.#561
inline __declspec(noreturn) void SErrDisplayAppFatal(const char* Format, ...) { SErrDisplayAppFatal(const_cast<char*>(Format)); }
STORM_DLL_DECL int32_t __cdecl SErrDisplayErrorFmt(uint32_t messageId, const char* filename, int32_t linenumber, int32_t recoverable, uint32_t uExitCode, char* Format, ...); // Storm.#562
STORM_DLL_DECL int32_t __stdcall SErrPrepareAppFatal(const char* filename, int32_t linenumber); // Storm.#564
STORM_DLL_DECL uint32_t __cdecl SErrGetLastError(); // Storm.#463
STORM_DLL_DECL void __stdcall SErrSetLastError(uint32_t dwErrCode); // Storm.#465

#endif
