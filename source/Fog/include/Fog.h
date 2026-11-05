#pragma once

#include <D2Dll.h>
#include <D2BasicTypes.h>
#include <StormHandles.h>
#include <stdlib.h>

//1.10f Image base: 0x6FF50000
//1.12a Image base: 0x6FF50000

#if defined(D2_VERSION_110F) || defined(D2_VERSION_112A)
constexpr int FogImageBase = 0x6FF50000;
#endif


#pragma pack(1)

struct D2TxtLinkNodeStrc
{
	char szText[32];				//0x00
	int nLinkIndex;					//0x20
	D2TxtLinkNodeStrc* pPrevious;	//0x24
	D2TxtLinkNodeStrc* pNext;		//0x28
};

struct D2TxtLinkTblStrc
{
	union
	{
		char szCode[4];				//0x00
		uint32_t dwCode;			//0x00
	};
	int nLinkIndex;					//0x04
};

struct D2TxtLinkStrc
{
	int32_t nRecords;				//0x00
	int32_t nAllocatedCells;		//0x04
	D2TxtLinkTblStrc* pTbl;			//0x08
	D2TxtLinkNodeStrc* pFirstNode;	//0x0C
};


struct D2BinFileStrc
{
	uint8_t* pDataBuffer;			//0x00
	uint8_t* pData;					//0x04
	int32_t nRecordCount;			//0x08
	int32_t nCellCount;				//0x0C
};

enum D2C_TxtFieldTypes : uint32_t
{
	TXTFIELD_NONE,
	TXTFIELD_ASCII,
	TXTFIELD_DWORD,
	TXTFIELD_WORD,
	TXTFIELD_BYTE,
	TXTFIELD_UNKNOWN1,
	TXTFIELD_UNKNOWN2,
	TXTFIELD_BYTE2,
	TXTFIELD_DWORD2,
	TXTFIELD_RAW,
	TXTFIELD_ASCIITOCODE,
	TXTFIELD_UNKNOWN3,
	TXTFIELD_UNKNOWN4,
	TXTFIELD_CODETOBYTE,
	TXTFIELD_UNKNOWN5,
	TXTFIELD_CODETOWORD,
	TXTFIELD_UNKNOWN6,
	TXTFIELD_NAMETOINDEX,
	TXTFIELD_NAMETOINDEX2,
	TXTFIELD_NAMETODWORD,
	TXTFIELD_NAMETOWORD,
	TXTFIELD_NAMETOWORD2,
	TXTFIELD_KEYTOWORD,
	TXTFIELD_CUSTOMLINK,
	TXTFIELD_UNKNOWN7,
	TXTFIELD_CALCTODWORD,
	TXTFIELD_BIT
};

struct D2BinFieldStrc
{
	const char* szFieldName;		//0x00
	D2C_TxtFieldTypes nFieldType;	//0x04
	int32_t nFieldLength;			//0x08
	int32_t nFieldOffset;			//0x0C
	void* pLinkField;				//0x10
};

struct D2UnkExcelStrc
{
	D2UnkExcelStrc* pNext;			//0x00
	uint32_t dwHash;				//0x04
	D2BinFieldStrc* pBinField;		//0x08
};


enum D2C_AsyncDataPriority
{
	ASYNC_DATA_PRIORITY_NORMAL = 0,
	ASYNC_DATA_PRIORITY_HIGH = 1,
};

enum D2C_AsyncDataFlags : uint32_t
{
	ASYNC_DATA_FLAGS_LOADED                    = 0x1,
	ASYNC_DATA_FLAGS_OWNS_BUFFER               = 0x2,
	ASYNC_DATA_FLAGS_POSTPONED_PRIORITY_CHANGE = 0x8,

	ASYNC_DATA_FLAGS_DEBUG_TAG      = 0x41737960,
	ASYNC_DATA_FLAGS_DEBUG_TAG_MASK = 0xFFFFFFF0,
};

struct AsyncDataLoadFileCallback																			   // sizeof(0x1C)
{
	DWORD(__stdcall* callback)(void* pFileData, size_t nDataLength, AsyncDataLoadFileCallback* pCallbackData); // 0x00
	DWORD userData[4];																						   // 0x04
	void* pData;																							   // 0x14
	size_t nFileSize;																						   // 0x18
};

struct AsyncData							 // sizeof(0x54)
{
	HSFILE hFile;							 // 0x00
	void* pMemPool;							 // 0x04
	OVERLAPPED tOverlapped;					 // 0x08
	AsyncDataLoadFileCallback pAsyncOpDesc;	 // 0x1C
	struct AsyncDataEventSlot* pEventSlot;	 // 0x38
	void* pBuffer;							 // 0x3C
	uint32_t nFileSize;						 // 0x40
	uint32_t nPriority;						 // 0x44 D2C_AsyncDataPriority
	uint32_t nFlags;						 // 0x48
	uint32_t nPreviousPriority;				 // 0x4C
	AsyncData* pNextPostponedPriorityChange; // 0x50
};

struct AsyncDataEventSlot  // sizeof(0x0C)
{
	HANDLE hEvent;		   // 0x00
	DWORD bSlotAvailable;  // 0x04
	AsyncData* pAsyncOp;   // 0x08
};


struct QServer;

#pragma pack()

typedef int (*D2ExceptionCallback)();

FOG_DLL_DECL const char* __cdecl FOG_csprintf(char* szDest, const char* szFormat, ...);																//Fog.#10018
FOG_DLL_DECL void __fastcall FOG_InitErrorMgr(const char* szProgramName, D2ExceptionCallback pExceptionCallback, const char* szVersion, BOOL bLogToFile);	//Fog.#10019
FOG_DLL_DECL int __fastcall FOG_SetLogPrefix(const char* szLogPrefix);																				//Fog.#10021
FOG_DLL_DECL void* __cdecl FOG_GetSystemInfo();																										//Fog.#10022
FOG_DLL_DECL void __cdecl FOG_DisplayAssert(const char* szMsg, const char* szFile, int nLine);														//Fog.#10023
FOG_DLL_DECL void __cdecl FOG_DisplayHalt(const char* szMsg, const char* szFile, int nLine);														//Fog.#10024
FOG_DLL_DECL void __cdecl FOG_DisplayWarning(const char* szMsg, const char* szFile, int nLine);														//Fog.#10025
FOG_DLL_DECL void __cdecl FOG_DisplayError(int nCategory, const char* szMsg, const char* szFile, int nLine);										//Fog.#10026
// Log to default logfile with date and `\n` at end of message
FOG_DLL_DECL void __cdecl FOG_Trace(const char* szFormat, ...);																						//Fog.#10029
// Log to file. Will be prefixed with "D2" and suffixed by date. Log includes date prefix and `\n` at end of message.
FOG_DLL_DECL void __cdecl FOG_TraceF(const char* pFileSubName, const char* szFormat, ...);															//Fog.#10030
// Append to the default logfile. No date nor '\n'.
FOG_DLL_DECL void __cdecl FOG_TraceAppend(const char* szFormat, ...);																				//Fog.#10031
FOG_DLL_DECL void __cdecl FOG_TraceMemory(const char* pFileSubName, const char* szMessage, void* pMemory, size_t nByteCount, BOOL bLogOffset, BOOL bNoCopy, BOOL bNoASCII); //Fog.#10032
FOG_DLL_DECL void __fastcall FOG_CrashDeadlockDetected(HANDLE hThreadToWalk);																		//Fog.#10033
FOG_DLL_DECL void __fastcall FOG_CrashDumpThread(HANDLE hThreadToWalk);																				//Fog.#10034
FOG_DLL_DECL BOOL __cdecl FOG_IsHandlingError();																									//Fog.#10039
FOG_DLL_DECL void* __fastcall FOG_Alloc(int nSize, const char* szFile, int nLine, int n0);															//Fog.#10042
FOG_DLL_DECL void __fastcall FOG_Free(void* pFree, const char* szFile, int nLine, int n0);															//Fog.#10043
FOG_DLL_DECL void* __fastcall FOG_Realloc(void* pMemory, int nSize, const char* szFile, int nLine, int n0);											//Fog.#10044
FOG_DLL_DECL void* __fastcall FOG_AllocPool(void* pMemPool, int nSize, const char* szFile, int nLine, int n0);										//Fog.#10045
FOG_DLL_DECL void __fastcall FOG_FreePool(void* pMemPool, void* pFree, const char* szFile, int nLine, int n0);										//Fog.#10046
FOG_DLL_DECL void* __fastcall FOG_ReallocPool(void* pMemPool, void* pMemory, int nSize, const char* szFile, int nLine, int n0);						//Fog.#10047
FOG_DLL_DECL void __fastcall FOG_10050_EnterCriticalSection(_Acquires_lock_(*_Curr_) CRITICAL_SECTION* pCriticalSection, int nLine);				//Fog.#10050
FOG_DLL_DECL int32_t __fastcall FOG_10055_GetSyncTime();																							//Fog.#10055
// Noop, same as 10048, 10049, 10053, 10054, 10146, 10194, 10195, 10196, 10197, 10220, 10221, 10225, 10232, 10240, 10241, 10242
FOG_DLL_DECL void __fastcall FOG_10082_Noop();																										//Fog.#10082
FOG_DLL_DECL float __stdcall FOG_10083_Cos_LUT(int16_t index);																						//Fog.#10083
FOG_DLL_DECL float __stdcall FOG_10084_Sin_LUT(int16_t index);																						//Fog.#10084
FOG_DLL_DECL unsigned __stdcall FOG_Decode14BitsFromString(uint16_t* p2Characters);																	//Fog.#10085
FOG_DLL_DECL void __stdcall FOG_Encode14BitsToString(uint16_t* p2Characters, uint32_t value);														//Fog.#10086
// Note: works up to value 0xFC05FC00
FOG_DLL_DECL unsigned __stdcall FOG_Decode32BitsFromString(uint32_t* p4Characters);																	//Fog.#10087
FOG_DLL_DECL void __stdcall FOG_Encode32BitsToString(uint32_t* p4Characters, uint32_t value);														//Fog.#10088
FOG_DLL_DECL void __fastcall FOG_AsyncDataInitialize(BOOL bEnableAsyncOperations);																	//Fog.#10089
FOG_DLL_DECL void __fastcall FOG_AsyncDataDestroy();																								//Fog.#10090
FOG_DLL_DECL AsyncData* __fastcall FOG_AsyncDataLoadFileEx(void* pMemPool, const char* pFileName, BOOL bAllowAsync, LONG nFileOffset, int nFileSize, void* pBuffer, AsyncDataLoadFileCallback* asyncOpDesc, int a8, const char* sourceFile, int sourceLine);	//Fog.#10091
FOG_DLL_DECL BOOL __fastcall FOG_AsyncDataTestLoaded(AsyncData* pAsyncData);																		//Fog.#10092
FOG_DLL_DECL void __fastcall FOG_AsyncDataWaitAndGetBuffer(AsyncData* pAsyncData);																	//Fog.#10093
FOG_DLL_DECL void* __fastcall FOG_AsyncDataGetBuffer(AsyncData* pAsyncData);																		//Fog.#10094
FOG_DLL_DECL DWORD __fastcall FOG_AsyncDataGetBufferSize(AsyncData* pAsyncData);																	//Fog.#10095
FOG_DLL_DECL void __fastcall FOG_AsyncDataSetResults(AsyncData* pAsyncData, void* pBuffer, int nBufferSize);										//Fog.#10096
FOG_DLL_DECL void __fastcall FOG_AsyncDataFree(AsyncData* pAsyncData);																				//Fog.#10097
FOG_DLL_DECL void __fastcall FOG_AsyncDataSetPriority(AsyncData* pAsyncData, int nPriority);														//Fog.#10099
FOG_DLL_DECL void __fastcall FOG_AsyncDataHandlePriorityChanges(BOOL bPostpone);																	//Fog.#10100
FOG_DLL_DECL BOOL __fastcall FOG_MPQSetConfig(int dwDirectAccessFlags, int bEnableSeekOptimization);												//Fog.#10101
FOG_DLL_DECL BOOL __fastcall FOG_FOpenFile(const char* szFile, HSFILE* pFileHandle);																//Fog.#10102
FOG_DLL_DECL void __fastcall FOG_FCloseFile(HSFILE pFile);																							//Fog.#10103
FOG_DLL_DECL BOOL __fastcall FOG_FReadFile(HSFILE pFile, void* pBuffer, size_t nSize, int* nBytesRead, uint32_t, uint32_t, uint32_t);				//Fog.#10104
FOG_DLL_DECL uint32_t __fastcall FOG_FGetFileSize(HSFILE pFileHandle, uint32_t* lpFileSizeHigh);													//Fog.#10105
FOG_DLL_DECL size_t __fastcall FOG_FSetFilePointer(HSFILE hFile, int32_t lDistanceToMove, int32_t* lpDistanceToMoveHigh, uint32_t dwMoveMethod);	//Fog.#10106
FOG_DLL_DECL HANDLE __fastcall FOG_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile); //Fog.#10107
FOG_DLL_DECL BOOL __fastcall FOG_DeleteFileA(LPCSTR lpFileName);																					//Fog.#10108
FOG_DLL_DECL BOOL __fastcall FOG_CloseFile(HANDLE hFILE);																							//Fog.#10109
FOG_DLL_DECL BOOL __fastcall FOG_ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);		//Fog.#10110
FOG_DLL_DECL BOOL __fastcall FOG_WriteFile(HANDLE hFile, const void* lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);	//Fog.#10111
FOG_DLL_DECL BOOL __fastcall FOG_CreatePathHierarchy(char* szPath);																					//Fog.#10114
FOG_DLL_DECL size_t __fastcall FOG_GetSavePath(char* pPathBuffer, size_t nBufferSize);																//Fog.#10115
FOG_DLL_DECL BOOL __fastcall FOG_GetInstallPath(char* pPathBuffer, size_t nBufferSize);																//Fog.#10116
FOG_DLL_DECL BOOL __fastcall FOG_UseDirect();																										//Fog.#10117
FOG_DLL_DECL uint16_t __stdcall FOG_ComputeStringCRC16(const char* szString);																		//Fog.#10137
FOG_DLL_DECL void __cdecl FOG_CreateNewPoolSystem(void** pMemPoolSystem, const char* szName, uint32_t nPools, uint32_t nUnused);					//Fog.#10142
FOG_DLL_DECL void __cdecl FOG_DestroyMemoryPoolSystem(void* pMemoryPoolSystem);																		//Fog.#10143
FOG_DLL_DECL void __cdecl FOG_SetMemoryPoolSystemMode(void* pMemoryPoolSystem, BOOL bFreePoolAlloc);												//Fog.#10145
FOG_DLL_DECL DWORD __cdecl FOG_GetMemoryUsage(void*);																								//Fog.#10147
D2FUNC_DLL(FOG, InitializeServer, QServer*, __stdcall, (int, int, int, int, void*, void*, void*, void*), 0x4150)									//Fog.#10149
D2FUNC_DLL(FOG, SetMaxClientsPerGame, int, __stdcall, (QServer*, int), 0x4970)																		//Fog.#10151
D2FUNC_DLL(FOG, 10152, int, __stdcall, (void*, const uint8_t*, int), 0x44F0)																		//Fog.#10152
D2FUNC_DLL(FOG, WaitForSingleObject, int, __stdcall, (void*, int), 0x4A60)																			//Fog.#10154
D2FUNC_DLL(FOG, 10156, int, __stdcall, (void*, int, uint8_t*, int), 0x59D0)																			//Fog.#10156
D2FUNC_DLL(FOG, 10157, int, __stdcall, (void*, int, const uint8_t*, int), 0x5EA0)																	//Fog.#10157
D2FUNC_DLL(FOG, 10158, int, __stdcall, (void*, int), 0x6210)																						//Fog.#10158
D2FUNC_DLL(FOG, 10159, int, __stdcall, (void*, int, char*, int), 0x6180)																			//Fog.#10159
D2FUNC_DLL(FOG, 10161, int, __stdcall, (void*, int), 0x6310)																						//Fog.#10161
D2FUNC_DLL(FOG, 10162, int, __stdcall, (void*, int, const char*, int), 0x6380)																		//Fog.#10162
D2FUNC_DLL(FOG, 10163, int, __stdcall, (void*, int, const char*, int), 0x66E0)																		//Fog.#10163
D2FUNC_DLL(FOG, 10164, int, __stdcall, (void*, int, int, int), 0x69D0)																				//Fog.#10164
D2FUNC_DLL(FOG, 10165, int, __stdcall, (void*, int, const char*, int), 0x6A10)																		//Fog.#10165
D2FUNC_DLL(FOG, 10166, int, __stdcall, (void*, int, int, int), 0x6C90)																				//Fog.#10166
D2FUNC_DLL(FOG, 10170, int, __stdcall, (void*, int), 0x6F70)																						//Fog.#10170
D2FUNC_DLL(FOG, 10171, int, __stdcall, (void*, void*), 0x6FB0)																						//Fog.#10171
D2FUNC_DLL(FOG, 10172, int, __stdcall, (void*, int, int), 0x6FD0)																					//Fog.#10172
D2FUNC_DLL(FOG, 10173, int, __stdcall, (void*, int), 0x7040)																						//Fog.#10173
D2FUNC_DLL(FOG, 10175, int, __stdcall, (void*, const uint8_t*, int, int), 0x7420)																	//Fog.#10175
D2FUNC_DLL(FOG, 10177, int, __stdcall, (void*, int), 0x5E60)																						//Fog.#10177
D2FUNC_DLL(FOG, SetHackListEnabled, int, __stdcall, (QServer* pServer, BOOL bHacklistEnabled), 0x75A0)												//Fog.#10178
D2FUNC_DLL(FOG, 10180, int, __stdcall, (void*), 0x4920)																								//Fog.#10180
D2FUNC_DLL(FOG, 10181, int, __stdcall, (void*, const uint8_t*, int, int), 0x75C0)																	//Fog.#10181
FOG_DLL_DECL int __stdcall FOG_10182_Return(void*);																									//Fog.#10182
FOG_DLL_DECL int __stdcall FOG_10183_Return(void*, int);																							//Fog.#10183
D2FUNC_DLL(FOG, 10186, int, __stdcall, (void*, int, int), 0x44C0)																					//Fog.#10186
D2FUNC_DLL(FOG, 10187, int, __fastcall, (void*, int, int), 0x7630)																					//Fog.#10187
FOG_DLL_DECL void __stdcall FOG_10207(D2BinFileStrc* pBinFile, D2BinFieldStrc* pBinField, void* pTxt, int nRecordCount, int nRecordSize);			//Fog.#10207
FOG_DLL_DECL D2BinFileStrc* __stdcall FOG_CreateBinFile(void* pDataBuffer, int nBufferSize);														//Fog.#10208
FOG_DLL_DECL void __stdcall FOG_FreeBinFile(D2BinFileStrc* pBinFile);																				//Fog.#10209
FOG_DLL_DECL int __stdcall FOG_GetRecordCountFromBinFile(D2BinFileStrc* pBinFile);																	//Fog.#10210
FOG_DLL_DECL void* __stdcall FOG_AllocLinker(const char* szFile, int nLine);																		//Fog.#10211
FOG_DLL_DECL void __stdcall FOG_FreeLinker(void* pLinker);																							//Fog.#10212
FOG_DLL_DECL int __stdcall FOG_GetLinkIndex(void* pLink, uint32_t dwCode, BOOL bLogError);															//Fog.#10213
FOG_DLL_DECL int __stdcall FOG_GetStringFromLinkIndex(void* pLinker, int nIndex, char* szString);													//Fog.#10214
FOG_DLL_DECL int __stdcall FOG_10215(void* pBin, int a2);																							//Fog.#10215
FOG_DLL_DECL int __stdcall FOG_10216_AddRecordToLinkingTable(void* pBin, const char* a2);															//Fog.#10216
FOG_DLL_DECL int __stdcall FOG_GetRowFromTxt(void* pBin, char* szText, int nColumn);																//Fog.#10217
FOG_DLL_DECL int __fastcall FOG_10218();																											//Fog.#10218
FOG_DLL_DECL int __fastcall FOG_10219(uint8_t*);																									//Fog.#10219
FOG_DLL_DECL int __fastcall FOG_10222(const uint8_t*, int);																							//Fog.#10222
FOG_DLL_DECL int __fastcall FOG_10223(uint8_t*, int, const uint8_t*, int);																			//Fog.#10223
FOG_DLL_DECL int __fastcall FOG_10224(char*, int, uint8_t*, int);																					//Fog.#10224
FOG_DLL_DECL int __fastcall FOG_IsExpansion();																										//Fog.#10227
FOG_DLL_DECL uint32_t __stdcall FOG_ComputeChecksum(void* pData, size_t dwSize);																	//Fog.#10229
FOG_DLL_DECL uint32_t __stdcall FOG_PopCount(void* pData, size_t dwSize);																			//Fog.#10230
FOG_DLL_DECL void __fastcall FOG_10233(const char*, int);																							//Fog.#10233
FOG_DLL_DECL uint32_t __stdcall FOG_LeadingZeroesCount(uint32_t nValue);																			//Fog.#10252
FOG_DLL_DECL char* __stdcall FOG_10255(void* pLinker, int nId, int a3);																				//Fog.#10255


#define D2_ALLOC(size) FOG_Alloc((size), __FILE__, __LINE__, 0)
#define D2_CALLOC(size) memset(FOG_Alloc(size, __FILE__, __LINE__, 0), 0x00, size)
#define D2_ALLOC_STRC(type) (type*)FOG_Alloc(sizeof(type), __FILE__, __LINE__, 0)
#define D2_CALLOC_STRC(type) (type*)memset(FOG_Alloc(sizeof(type), __FILE__, __LINE__, 0), 0x00, sizeof(type))
#define D2_FREE(ptr) FOG_Free(ptr, __FILE__, __LINE__, 0)


#define D2_ALLOC_POOL(pMemPool, size) FOG_AllocPool(pMemPool, size, __FILE__, __LINE__, 0)
#define D2_CALLOC_POOL(pMemPool, size) memset(FOG_AllocPool(pMemPool, size, __FILE__, __LINE__, 0),0x00,size)
#define D2_REALLOC_POOL(pMemPool, pMem, size) FOG_ReallocPool(pMemPool, pMem, size, __FILE__, __LINE__, 0)
#define D2_ALLOC_STRC_POOL(pMemPool, type) (type*)FOG_AllocPool(pMemPool, sizeof(type), __FILE__, __LINE__, 0)
#define D2_CALLOC_STRC_POOL(pMemPool, type) (type*)memset(FOG_AllocPool(pMemPool, sizeof(type), __FILE__, __LINE__, 0), 0x00, sizeof(type))
#define D2_FREE_POOL(pMemPool, ptr) FOG_FreePool(pMemPool, ptr, __FILE__, __LINE__, 0)

#ifndef NDEBUG
// Assert that an expression must be true, otherwise assume the program state will not be recoverable.
// In debug builds, this will trigger a log and exit, in release this is used as a hint for performance optimization.
// Do NOT use this if the program can recover when expr if false, as it is used as a hint for performance and can impact generated code.
// For recoverable errors, use D2_VERIFY
#define D2_DISPLAY_ASSERT_THEN_BREAK(msg) (FOG_DisplayAssert(msg, __FILE__, __LINE__), __debugbreak())
#define D2_ASSERT(expr) (void)( (!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(#expr), exit(-1) , 0)); _Analysis_assume_(expr)
#define D2_ASSERTM(expr,msg) (void)( (!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(msg), exit(-1) , 0)); _Analysis_assume_(expr)
#define D2_CHECK(expr) (void)( (!!(expr)) || (FOG_DisplayWarning(#expr, __FILE__, __LINE__), 0)); _Analysis_assume_(expr)


// Assert that an expression must be true, even though the program may be recoverable.
// Contrary to D2_ASSERT, this is still evaluated in release builds, and can be used anywhere the expression would be valid.
// This is to be used when one can recover from an error
// Example:
// if(D2_VERIFY(ptr != nullptr)) ptr->method(); // The only difference between debug and release build is the logging and breakpoint
#define D2_VERIFY(expr) ((!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(#expr), false))
#define D2_VERIFYM(expr,msg) ((!!(expr)) || (D2_DISPLAY_ASSERT_THEN_BREAK(msg), false))
#else
  #if defined(__clang__)
    // the argument to '__builtin_assume' has side effects that will be discarded
    #pragma clang diagnostic ignored "-Wassume"
  #endif // defined(__clang__)
#define D2_ASSERT(expr) (__assume(expr), (void)0)
#define D2_ASSERTM(expr,msg) (__assume(expr), (void)0)
#define D2_VERIFY(expr) (!!(expr))
#define D2_VERIFYM(expr,msg) (!!(expr))
#define D2_CHECK(expr) (__assume(expr), (void)0)
#endif
#define D2_UNREACHABLE D2_ASSERT(false)


#define D2_LOCK(pCriticalSection) D2_ASSERT(pCriticalSection); FOG_10050_EnterCriticalSection(pCriticalSection, __LINE__);
#define D2_UNLOCK(pCriticalSection) D2_ASSERT(pCriticalSection); LeaveCriticalSection(pCriticalSection);
