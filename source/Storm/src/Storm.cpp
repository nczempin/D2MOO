#include <Windows.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <D2Dll.h>

enum StormOrdinal : uint32_t
{
	ORD_SNetCreateGame = 101,
	ORD_SNetDestroy = 102,
	ORD_SNetEnumProviders = 103,
	ORD_SNetEnumDevices = 104,
	ORD_SNetEnumGames = 105,
	ORD_SNetDropPlayer = 106,
	ORD_SNetGetGameInfo = 107,
	ORD_SNetGetNetworkLatency = 108,
	ORD_SNetGetNumPlayers = 109,
	ORD_SNetGetOwnerTurnsWaiting = 110,
	ORD_SNetGetPerformanceData = 111,
	ORD_SNetGetPlayerCaps = 112,
	ORD_SNetGetPlayerName = 113,
	ORD_SNetGetProviderCaps = 114,
	ORD_SNetGetTurnsInTransit = 115,
	ORD_SNetInitializeDevice = 116,
	ORD_SNetInitializeProvider = 117,
	ORD_SNetJoinGame = 118,
	ORD_SNetLeaveGame = 119,
	ORD_SNetPerformUpgrade = 120,
	ORD_SNetReceiveMessage = 121,
	ORD_SNetReceiveTurns = 122,
	ORD_SNetRegisterEventHandler = 123,
	ORD_SNetResetLatencyMeasurements = 124,
	ORD_SNetSelectGame = 125,
	ORD_SNetSelectProvider = 126,
	ORD_SNetSendMessage = 127,
	ORD_SNetSendTurn = 128,
	ORD_SNetSetBasePlayer = 129,
	ORD_SNetSetGameMode = 130,
	ORD_SNetUnregisterEventHandler = 131,
	ORD_SNetEnumGamesEx = 133,
	ORD_SNetSendServerChatCommand = 134,
	ORD_SNetSend135 = 135,
	ORD_SNetReceive136 = 136,
	ORD_SNetGetPlayerNames = 137,
	ORD_SNetCreateLadderGame = 138,
	ORD_SNetReportGameResult = 139,
	ORD_SNetCheckDataFile = 140,
	ORD_SDlgBeginPaint = 201,
	ORD_SDlgBltToWindowI = 202,
	ORD_SDlgCheckTimers = 203,
	ORD_SDlgCreateDialogIndirectParam = 204,
	ORD_SDlgCreateDialogParam = 205,
	ORD_SDlgDefDialogProc = 206,
	ORD_SDlgDialogBoxIndirectParam = 208,
	ORD_SDlgDialogBoxParam = 209,
	ORD_SDlgDrawBitmap = 210,
	ORD_SDlgEndDialog = 211,
	ORD_SDlgEndPaint = 212,
	ORD_SDlgKillTimer = 213,
	ORD_SDlgSetBaseFont = 214,
	ORD_SDlgSetBitmapI = 215,
	ORD_SDlgSetControlBitmaps = 216,
	ORD_SDlgSetCursor = 217,
	ORD_SDlgSetSystemCursor = 218,
	ORD_SDlgSetTimer = 219,
	ORD_SDlgUpdateCursor = 220,
	ORD_SDlgBltToWindowE = 221,
	ORD_SDlgSetBitmapE = 222,
	ORD_SDlgSetLocale = 223,
	ORD_SFileAuthenticateArchive = 251,
	ORD_SFileCloseArchive = 252,
	ORD_SFileCloseFile = 253,
	ORD_SFileDdaBegin = 254,
	ORD_SFileDdaBeginEx = 255,
	ORD_SFileDdaDestroy = 256,
	ORD_SFileDdaEnd = 257,
	ORD_SFileDdaGetPos = 258,
	ORD_SFileDdaGetVolume = 259,
	ORD_SFileDdaInitialize = 260,
	ORD_SFileDdaSetVolume = 261,
	ORD_SFileDestroy = 262,
	ORD_SFileEnableDirectAccess = 263,
	ORD_SFileGetFileArchive = 264,
	ORD_SFileGetFileSize = 265,
	ORD_SFileOpenArchive = 266,
	ORD_SFileOpenFile = 267,
	ORD_SFileOpenFileEx = 268,
	ORD_SFileReadFile = 269,
	ORD_SFileSetBasePath = 270,
	ORD_SFileSetFilePointer = 271,
	ORD_SFileSetLocale = 272,
	ORD_SFileGetBasePath = 273,
	ORD_SFileSetIoErrorMode = 274,
	ORD_SFileGetArchiveName = 275,
	ORD_SFileGetFileName = 276,
	ORD_SFileGetArchiveInfo = 277,
	ORD_SFileSetPlatform = 278,
	ORD_SFileLoadFile = 279,
	ORD_SFileUnloadFile = 280,
	ORD_SFileLoadFileEx = 281,
	ORD_SFilePrioritizeRequest = 282,
	ORD_SFileCancelRequest = 283,
	ORD_SFileSetAsyncBudget = 284,
	ORD_SFileSetDataChunkSize = 285,
	ORD_SFileEnableSeekOptimization = 286,
	ORD_SFileReadFileEx = 287,
	ORD_SFileFileExists = 288,
	ORD_SFileFileExistsEx = 289,
	ORD_SFileReadFileEx2 = 290,
	ORD_StormDestroy = 301,
	ORD_StormGetInstance = 302,
	ORD_StormGetOption = 303,
	ORD_StormSetOption = 304,
	ORD_SBltGetSCode = 312,
	ORD_SBltROP3 = 313,
	ORD_SBltROP3Clipped = 314,
	ORD_SBltROP3Tiled = 315,
	ORD_SBmpDecodeImage = 321,
	ORD_SBmpLoadImage = 323,
	ORD_SBmpSaveImage = 324,
	ORD_SBmpAllocLoadImage = 325,
	ORD_SBmpSaveImageEx = 326,
	ORD_SCodeCompile = 331,
	ORD_SCodeDelete = 332,
	ORD_SCodeExecute = 334,
	ORD_SCodeGetPseudocode = 335,
	ORD_SDrawAutoInitialize = 341,
	ORD_SDrawCaptureScreen = 342,
	ORD_SDrawClearSurface = 343,
	ORD_SDrawDestroy = 344,
	ORD_SDrawFlipPage = 345,
	ORD_SDrawGetFrameWindow = 346,
	ORD_SDrawGetObjects = 347,
	ORD_SDrawGetScreenSize = 348,
	ORD_SDrawGetServiceLevel = 349,
	ORD_SDrawLockSurface = 350,
	ORD_SDrawManualInitialize = 351,
	ORD_SDrawMessageBox = 352,
	ORD_SDrawPostClose = 353,
	ORD_SDrawRealizePalette = 354,
	ORD_SDrawSelectGdiSurface = 355,
	ORD_SDrawUnlockSurface = 356,
	ORD_SDrawUpdatePalette = 357,
	ORD_SDrawUpdateScreen = 358,
	ORD_SDrawWaitForVerticalBlank = 359,
	ORD_SEvtDispatch = 372,
	ORD_SEvtRegisterHandler = 373,
	ORD_SEvtUnregisterHandler = 374,
	ORD_SEvtUnregisterType = 375,
	ORD_SEvtPopState = 376,
	ORD_SEvtPushState = 377,
	ORD_SEvtBreakHandlerChain = 378,
	ORD_SGdiBitBlt = 381,
	ORD_SGdiCreateFont = 382,
	ORD_SGdiDeleteObject = 383,
	ORD_SGdiExtTextOut = 385,
	ORD_SGdiImportFont = 386,
	ORD_SGdiLoadFont = 387,
	ORD_SGdiRectangle = 388,
	ORD_SGdiSelectObject = 389,
	ORD_SGdiSetPitch = 390,
	ORD_SGdiTextOut = 391,
	ORD_SGdi392 = 392,
	ORD_Ordinal393 = 393,
	ORD_SMemAlloc = 401,
	ORD_SMemFree = 403,
	ORD_SMemGetSize = 404,
	ORD_SMemReAlloc = 405,
	ORD_SMsgDispatchMessage = 412,
	ORD_SMsgDoMessageLoop = 413,
	ORD_SMsgRegisterCommand = 414,
	ORD_SMsgRegisterKeyDown = 415,
	ORD_SMsgRegisterKeyUp = 416,
	ORD_SMsgRegisterMessage = 417,
	ORD_SMsgPopRegisterState = 418,
	ORD_SMsgPushRegisterState = 419,
	ORD_SRegLoadData = 421,
	ORD_SRegLoadString = 422,
	ORD_SRegLoadValue = 423,
	ORD_SRegSaveData = 424,
	ORD_SRegSaveString = 425,
	ORD_SRegSaveValue = 426,
	ORD_SRegGetBaseKey = 427,
	ORD_SRegDeleteValue = 428,
	ORD_STransBlt = 431,
	ORD_STransBltUsingMask = 432,
	ORD_STransCreateI = 433,
	ORD_STransDelete = 434,
	ORD_STransDuplicate = 436,
	ORD_STransIntersectDirtyArray = 437,
	ORD_STransInvertMask = 438,
	ORD_STransLoadI = 439,
	ORD_STransSetDirtyArrayInfo = 440,
	ORD_STransUpdateDirtyArray = 441,
	ORD_STransPointInMask = 442,
	ORD_STransCombineMasks = 443,
	ORD_STransCreateI2 = 444,
	ORD_STransCreateE = 445,
	ORD_STrans446 = 446,
	ORD_STransLoadE = 447,
	ORD_SVidDestroy = 451,
	ORD_SVidGetSize = 452,
	ORD_SVidInitialize = 453,
	ORD_SVidPlayBegin = 454,
	ORD_SVidPlayBeginFromMemory = 455,
	ORD_SVidPlayContinue = 456,
	ORD_SVidPlayContinueSingle = 457,
	ORD_SVidPlayEnd = 458,
	ORD_SVidSetVolume = 459,
	ORD_SErrDisplayError = 461,
	ORD_SErrGetErrorStr = 462,
	ORD_SErrGetLastError = 463,
	ORD_SErrRegisterMessageSource = 464,
	ORD_SErrSetLastError = 465,
	ORD_SErrReportNamedResourceLeak = 466,
	ORD_SErrReportResourceLeak = 467,
	ORD_SErrSuppressErrors = 468,
	ORD_SErrRegisterHandler = 469,
	ORD_SErrUnregisterHandler = 470,
	ORD_SCmdGetBool = 472,
	ORD_SCmdGetNum = 473,
	ORD_SCmdGetString = 474,
	ORD_SCmdProcess = 475,
	ORD_SCmdRegisterArgList = 476,
	ORD_SCmdRegisterArgument = 477,
	ORD_SCmdStringExists = 478,
	ORD_SCmdProcessCommandLine = 479,
	ORD_SMemFindNextBlock = 481,
	ORD_SMemFindNextHeap = 482,
	ORD_SMemGetHeapByCaller = 483,
	ORD_SMemGetHeapByPtr = 484,
	ORD_SMemHeapAlloc = 485,
	ORD_SMemHeapCreate = 486,
	ORD_SMemHeapDestroy = 487,
	ORD_SMemHeapFree = 488,
	ORD_SMemHeapRealloc = 489,
	ORD_SMemHeapSize = 490,
	ORD_SMemCopy = 491,
	ORD_SMemFill = 492,
	ORD_SMemMove = 493,
	ORD_SMemZero = 494,
	ORD_SMemCmp = 495,
	ORD_SMem496 = 496,
	ORD_SMemDumpState = 497,
	ORD_SStrCopy = 501,
	ORD_SStrHash = 502,
	ORD_SStrNCat = 503,
	ORD_SStrTokenize = 504,
	ORD_SStrSearch = 505,
	ORD_SStrLen = 506,
	ORD_SStrDup = 507,
	ORD_SStrCmp = 508,
	ORD_SStrCmpI = 509,
	ORD_SStrUpper = 510,
	ORD_SMsgBreakHandlerChain = 511,
	ORD_SMsgUnregisterCommand = 512,
	ORD_SMsgUnregisterKeyDown = 513,
	ORD_SMsgUnregisterKeyUp = 514,
	ORD_SMsgUnregisterMessage = 515,
	ORD_SMsgGetDispatcher = 516,
	ORD_SMsgSetDefaultWindow = 517,
	ORD_SMsgGetDefaultWindow = 518,
	ORD_SRgn521 = 521,
	ORD_SRgn523 = 523,
	ORD_SRgnCreateRegion = 524,
	ORD_SRgnDeleteRegion = 525,
	ORD_SRgn527 = 527,
	ORD_SRgn528i = 528,
	ORD_SRgn529i = 529,
	ORD_SRgn530i = 530,
	ORD_SRgn531i = 531,
	ORD_SRgn532i = 532,
	ORD_SRgn533i = 533,
	ORD_SRgn534 = 534,
	ORD_SRgn535f = 535,
	ORD_SRgn536f = 536,
	ORD_SRgn537f = 537,
	ORD_SRgn538f = 538,
	ORD_SRgn539f = 539,
	ORD_SRgn540f = 540,
	ORD_SLogClose = 541,
	ORD_SLogCreate = 542,
	ORD_SLog543 = 543,
	ORD_SLogDump = 544,
	ORD_SLogFlush = 545,
	ORD_SLogFlushAll = 546,
	ORD_SLogPend = 547,
	ORD_SLogWrite = 548,
	ORD_SLog549 = 549,
	ORD_SLogCriticalLog = 550,
	ORD_SCompCompress = 551,
	ORD_SCompDecompress = 552,
	ORD_SErrDisplayAppFatal = 561,
	ORD_SErrDisplayErrorFmt = 562,
	ORD_SErrIsDisplayingError = 563,
	ORD_SErrPrepareAppFatal = 564,
	ORD_SStrChr = 571,
	ORD_SStrChrR = 572,
	ORD_SStrToDouble = 573,
	ORD_SStrToFloat = 574,
	ORD_SStrToInt = 575,
	ORD_SStrToUnsigned = 576,
	ORD_SStrToInt64 = 577,
	ORD_SStrPrintf = 578,
	ORD_SStrLower = 579,
	ORD_SStrHash64 = 580,
	ORD_SStrVPrintf = 581,
	ORD_SHashString = 582,
	ORD_SBigAdd = 601,
	ORD_SBigAnd = 602,
	ORD_SBigCompare = 603,
	ORD_SBigCopy = 604,
	ORD_SBigDec = 605,
	ORD_SBigDel = 606,
	ORD_SBigDiv = 607,
	ORD_SBigFindPrime = 608,
	ORD_SBigFromBinary = 609,
	ORD_SBigFromStr = 610,
	ORD_SBigFromStream = 611,
	ORD_SBigFromUnsigned = 612,
	ORD_SBigGcd = 613,
	ORD_SBigInc = 614,
	ORD_SBigInvMod = 615,
	ORD_SBigIsEven = 616,
	ORD_SBigIsOdd = 617,
	ORD_SBigIsOne = 618,
	ORD_SBigIsPrime = 619,
	ORD_SBigIsZero = 620,
	ORD_SBigMod = 621,
	ORD_SBigMul = 622,
	ORD_SBigMulMod = 623,
	ORD_SBigNew = 624,
	ORD_SBigNot = 625,
	ORD_SBigOr = 626,
	ORD_SBigPow = 627,
	ORD_SBigPowMod = 628,
	ORD_SBigRand = 629,
	ORD_SBigSet2Exp = 630,
	ORD_SBigSetOne = 631,
	ORD_SBigSetZero = 632,
	ORD_SBigShl = 633,
	ORD_SBigShr = 634,
	ORD_SBigSquare = 635,
	ORD_SBigSub = 636,
	ORD_SBigToBinaryArray = 637,
	ORD_SBigToBinaryBuffer = 638,
	ORD_SBigToBinaryPtr = 639,
	ORD_SBigToStrArray = 640,
	ORD_SBigToStrBuffer = 641,
	ORD_SBigToStrPtr = 642,
	ORD_SBigToStreamArray = 643,
	ORD_SBigToStreamBuffer = 644,
	ORD_SBigToStreamPtr = 645,
	ORD_SBigToUnsigned = 646,
	ORD_SBigXor = 647,
	ORD_SUniConvertUTF16to8Len = 901,
	ORD_SUniConvertUTF16to8 = 902,
	ORD_SUniConvertUTF8to16Len = 903,
	ORD_SUniConvertUTF8to16 = 904,
	ORD_SUniS905 = 905,
	ORD_SUniS906 = 906,
	ORD_SUniFindAfterUTF8Chr = 907,
	ORD_SUniFindUTF8ChrStart = 908,
	ORD_SUniConvertUTF16To909 = 909,
	ORD_SUniConvertUTF16To910 = 910,
	ORD_SUniConvertUTF16To911 = 911,
	ORD_SUniConvert912 = 912,
	ORD_SUniConvert913 = 913,
	ORD_SUniConvert914 = 914,
};

static FARPROC __stdcall StormResolveOrdinal(uint32_t nOrdinal, FARPROC pfnFallback)
{
	static HMODULE s_hStormBackend = nullptr;
	static bool s_bTriedLoad = false;
	if (!s_bTriedLoad)
	{
		s_bTriedLoad = true;
		char szPath[MAX_PATH] = {};
		if (GetModuleFileNameA(nullptr, szPath, MAX_PATH) != 0)
		{
			char* pSlash = std::strrchr(szPath, '\\');
			if (pSlash)
			{
				pSlash[1] = '\0';
				std::strncat(szPath, "Storm112.dll", MAX_PATH - std::strlen(szPath) - 1);
				s_hStormBackend = LoadLibraryA(szPath);
			}
		}
		if (!s_hStormBackend)
		{
			s_hStormBackend = LoadLibraryA("Storm112.dll");
		}
		if (s_hStormBackend)
		{
			// Storm.dll (1.12a) DllMain checks lpvReserved == nullptr (dynamic LoadLibrary load)
			// and deliberately calls 0x6FC015F0(0x10000100) to poison the MPQ crypt_table at
			// 0x6FC45358 with seed 0x10000100 instead of 0x100001.
			// Calling SFileDestroy (@262) immediately after LoadLibrary frees the poisoned table
			// and resets 0x6FC45358 to nullptr so SFileOpenArchive initializes it with 0x100001.
			using SFileDestroyFn = BOOL(__stdcall*)();
			if (auto pfnDestroy = reinterpret_cast<SFileDestroyFn>(GetProcAddress(s_hStormBackend, MAKEINTRESOURCEA(262))))
			{
				pfnDestroy();
			}
		}
	}
	if (s_hStormBackend)
	{
		FARPROC pfn = GetProcAddress(s_hStormBackend, MAKEINTRESOURCEA(nOrdinal));
		if (pfn)
		{
			return pfn;
		}
	}
	return pfnFallback;
}

#undef D2FUNC_DLL_NP
#define D2FUNC_DLL_NP(DLL, NAME, RETURN, CONV, ARGS, OFFSET) \
	static RETURN CONV NAME##_Fallback ARGS { \
		return default_or_void<RETURN>(); \
	} \
	static FARPROC g_pfn_##NAME = nullptr; \
	DLL##_DLL_DECL __declspec(naked) RETURN CONV NAME ARGS { \
		__asm { mov eax, g_pfn_##NAME } \
		__asm { test eax, eax } \
		__asm { jnz resolved_##NAME } \
		__asm { push ecx } \
		__asm { push edx } \
		__asm { push offset NAME##_Fallback } \
		__asm { push ORD_##NAME } \
		__asm { call StormResolveOrdinal } \
		__asm { pop edx } \
		__asm { pop ecx } \
		__asm { mov g_pfn_##NAME, eax } \
	resolved_##NAME: \
		__asm { jmp eax } \
	}

#include <Storm.h>

// 1.12a: 0x6FC03C70 (Storm.#290)
int __stdcall SFileReadFileEx2(int hArchive, char* szFileName, int ppBufferAddr, int pdwFileSizeAddr, int nExtraBytesToZero, char nFlags, LONG lpOverlapped, int a8)
{
	(void)a8;
	void** ppBuffer = reinterpret_cast<void**>(static_cast<uintptr_t>(static_cast<uint32_t>(ppBufferAddr)));
	DWORD* pdwFileSize = reinterpret_cast<DWORD*>(static_cast<uintptr_t>(static_cast<uint32_t>(pdwFileSizeAddr)));
	if (ppBuffer)
	{
		*ppBuffer = nullptr;
	}
	if (!szFileName || !ppBuffer)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (pdwFileSize)
	{
		*pdwFileSize = 0;
	}

	HANDLE hFile = nullptr;
	if (!SFileOpenFileEx(hArchive, szFileName, nFlags, reinterpret_cast<int>(&hFile)) || !hFile)
	{
		return 0;
	}

	const int nFileSize = SFileGetFileSize(reinterpret_cast<int>(hFile), nullptr);
	if (nFileSize < 0)
	{
		SFileCloseFile(hFile);
		return 0;
	}

	const int nExtra = (nExtraBytesToZero > 0) ? nExtraBytesToZero : 0;
	uint8_t* pBuf = static_cast<uint8_t*>(SMemAlloc(nFileSize + nExtra, __FILE__, __LINE__, 0));
	if (!pBuf)
	{
		SFileCloseFile(hFile);
		return 0;
	}

	DWORD dwBytesRead = 0;
	if (!SFileReadFile(reinterpret_cast<int>(hFile), reinterpret_cast<int>(pBuf), nFileSize, reinterpret_cast<int>(&dwBytesRead), lpOverlapped))
	{
		SMemFree(pBuf, __FILE__, __LINE__, 0);
		SFileCloseFile(hFile);
		return 0;
	}

	if (nExtra > 0)
	{
		std::memset(pBuf + nFileSize, 0, static_cast<size_t>(nExtra));
	}

	*ppBuffer = pBuf;
	if (pdwFileSize)
	{
		*pdwFileSize = static_cast<DWORD>(nFileSize);
	}

	SFileCloseFile(hFile);
	return 1;
}

// 1.12a: 0x6FC03DC0 (Storm.#281)
int __stdcall SFileLoadFileEx(int hArchive, char* szFileName, int ppBufferAddr, int pdwFileSizeAddr, int nExtraBytesToZero, char nFlags, LONG lpOverlapped)
{
	return SFileReadFileEx2(hArchive, szFileName, ppBufferAddr, pdwFileSizeAddr, nExtraBytesToZero, nFlags, lpOverlapped, 0);
}

// 1.12a: 0x6FC03DF0 (Storm.#279)
int __stdcall SFileLoadFile(char* szFileName, int ppBufferAddr, int pdwFileSizeAddr, int nExtraBytesToZero, LONG lpOverlapped)
{
	return SFileReadFileEx2(0, szFileName, ppBufferAddr, pdwFileSizeAddr, nExtraBytesToZero, 0, lpOverlapped, 0);
}

// 1.12a: 0x6FC00DB0 (Storm.#280)
BOOL __stdcall SFileUnloadFile(void* buffer)
{
	if (!buffer)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	return SMemFree(buffer, __FILE__, __LINE__, 0);
}

// 1.12a: 0x6FC082F0 (Storm.#302)
HMODULE __cdecl StormGetInstance()
{
	return GetModuleHandleA("Storm.dll");
}

// 1.12a: 0x6FC24330 (Storm.#491)
int __stdcall SMemCopy(void* dest, const void* source, size_t size)
{
	if (!dest)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (!source && size != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	std::memcpy(dest, source, size);
	return 1;
}

// 1.12a: 0x6FC24300 (Storm.#492)
int __stdcall SMemFill(void* location, size_t length, char fillWith)
{
	if (!location && length != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (length != 0)
	{
		std::memset(location, static_cast<unsigned char>(fillWith), length);
	}
	return 1;
}

// 1.12a: 0x6FC242E0 (Storm.#493)
int __stdcall SMemMove(void* Dst, void* Src, size_t Size)
{
	if (!Dst)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (!Src && Size != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	std::memmove(Dst, Src, Size);
	return 1;
}

// 1.12a: 0x6FC242C0 (Storm.#494)
int __stdcall SMemZero(void* location, size_t length)
{
	if (!location && length != 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (length != 0)
	{
		std::memset(location, 0, length);
	}
	return 1;
}

// 1.12a: 0x6FC24360 (Storm.#495)
int __stdcall SMemCmp(void* location1, void* location2, size_t size)
{
	if (!location1 || !location2)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	return std::memcmp(location1, location2, size);
}

// 1.12a: 0x6FC36758
static const uint32_t gdwStormHash32Table[16] = {
	0x486E26EEu, 0xDCAA16B3u, 0xE1918EEFu, 0x202DAFDBu,
	0x341C7DC7u, 0x1C365303u, 0x40EF2D37u, 0x65FD5E49u,
	0xD6057177u, 0x904ECE93u, 0x1C38024Fu, 0x98FD323Bu,
	0xE3061AE7u, 0xA39B0FA1u, 0x9797F25Fu, 0xE4444563u,
};

// 1.12a: 0x6FC05B50 (Storm.#502)
DWORD __stdcall SStrHash(const char* string, unsigned __int32 flags, unsigned __int32 Seed)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	if (Seed == 0)
	{
		Seed = 0x7FED7FEDu;
	}
	uint32_t dwAdd = 0xEEEEEEEEu;

	while (*string != '\0')
	{
		uint32_t c = static_cast<uint8_t>(*string++);
		if ((flags & 1) == 0)
		{
			if (c >= 'a' && c <= 'z')
			{
				c -= 0x20;
			}
			if (c == '/')
			{
				c = '\\';
			}
		}
		Seed = (Seed + dwAdd) ^ (gdwStormHash32Table[c >> 4] - gdwStormHash32Table[c & 0xF]);
		dwAdd = dwAdd * 33u + c + Seed + 3u;
	}

	return Seed ? Seed : 1u;
}

// 1.12a: 0x6FC05D40 (Storm.#503)
int __stdcall SStrNCat(char* base, const char* append, int max_length)
{
	if (!base || !append || max_length <= 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}

	const size_t nBaseLen = std::strlen(base);
	if (max_length == 0x7FFFFFFF)
	{
		std::strcat(base + nBaseLen, append);
		return static_cast<int>(nBaseLen + std::strlen(base + nBaseLen));
	}

	if (static_cast<int>(nBaseLen) < max_length - 1)
	{
		const size_t nRemain = static_cast<size_t>((max_length - 1) - static_cast<int>(nBaseLen));
		char* pDst = base + nBaseLen;
		size_t i = 0;
		while (i < nRemain && append[i] != '\0')
		{
			pDst[i] = append[i];
			++i;
		}
		pDst[i] = '\0';
		return static_cast<int>(nBaseLen + i);
	}

	base[max_length - 1] = '\0';
	return max_length - 1;
}

// 1.12a: 0x6FC06A40 (Storm.#505)
char* __stdcall SStrSearch(const char* string, char c, BOOL bReverse)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return nullptr;
	}
	return const_cast<char*>(bReverse ? SStrChrR(string, c) : SStrChr(string, c));
}

// 1.12a: 0x6FC06770 (Storm.#573)
double __stdcall SStrToDouble(const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0.0;
	}
	return std::atof(string);
}

// 1.12a: 0x6FC05C30 (Storm.#576)
unsigned int __stdcall SStrToUnsigned(const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	unsigned int result = 0;
	while (*string >= '0' && *string <= '9')
	{
		result = result * 10u + static_cast<unsigned int>(*string - '0');
		++string;
	}
	return result;
}

// 1.12a: 0x6FC064A0 (Storm.#577)
__int64 __stdcall SStrToInt64(const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	bool bNegative = false;
	if (*string == '-')
	{
		bNegative = true;
		++string;
	}
	else if (*string == '+')
	{
		++string;
	}
	uint64_t val = 0;
	while (*string >= '0' && *string <= '9')
	{
		val = val * 10u + static_cast<uint64_t>(*string - '0');
		++string;
	}
	return bNegative ? -static_cast<__int64>(val) : static_cast<__int64>(val);
}

// 1.12a: 0x6FC06940 (Storm.#581)
int __cdecl SStrVPrintf(char* DstBuf, size_t MaxCount, char* Format, va_list ArgList)
{
	if (!DstBuf || !Format || MaxCount == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return 0;
	}
	if (MaxCount == 0x7FFFFFFF)
	{
		return std::vsprintf(DstBuf, Format, ArgList);
	}
	int nWritten = std::vsnprintf(DstBuf, MaxCount, Format, ArgList);
	DstBuf[MaxCount - 1] = '\0';
	if (nWritten < 0 || static_cast<size_t>(nWritten) >= MaxCount)
	{
		return static_cast<int>(MaxCount - 1);
	}
	return nWritten;
}

#define STORM_JENKINS_MIX(a, b, c) \
	do { \
		a -= b; a -= c; a ^= (c >> 13); \
		b -= c; b -= a; b ^= (a << 8); \
		c -= a; c -= b; c ^= (b >> 13); \
		a -= b; a -= c; a ^= (c >> 12); \
		b -= c; b -= a; b ^= (a << 16); \
		c -= a; c -= b; c ^= (b >> 5); \
		a -= b; a -= c; a ^= (c >> 3); \
		b -= c; b -= a; b ^= (a << 10); \
		c -= a; c -= b; c ^= (b >> 15); \
	} while (0)

// 1.12a: 0x6FC057B0
static uint32_t StormJenkinsHash(const uint8_t* k, uint32_t length, uint32_t initval)
{
	uint32_t a = 0x9E3779B9u;
	uint32_t b = 0x9E3779B9u;
	uint32_t c = initval;
	uint32_t len = length;

	while (len >= 12)
	{
		a += k[0] + (static_cast<uint32_t>(k[1]) << 8) + (static_cast<uint32_t>(k[2]) << 16) + (static_cast<uint32_t>(k[3]) << 24);
		b += k[4] + (static_cast<uint32_t>(k[5]) << 8) + (static_cast<uint32_t>(k[6]) << 16) + (static_cast<uint32_t>(k[7]) << 24);
		c += k[8] + (static_cast<uint32_t>(k[9]) << 8) + (static_cast<uint32_t>(k[10]) << 16) + (static_cast<uint32_t>(k[11]) << 24);
		STORM_JENKINS_MIX(a, b, c);
		k += 12;
		len -= 12;
	}

	c += length;
	switch (len)
	{
	case 11: c += static_cast<uint32_t>(k[10]) << 24;
	case 10: c += static_cast<uint32_t>(k[9]) << 16;
	case 9:  c += static_cast<uint32_t>(k[8]) << 8;
	case 8:  b += static_cast<uint32_t>(k[7]) << 24;
	case 7:  b += static_cast<uint32_t>(k[6]) << 16;
	case 6:  b += static_cast<uint32_t>(k[5]) << 8;
	case 5:  b += k[4];
	case 4:  a += static_cast<uint32_t>(k[3]) << 24;
	case 3:  a += static_cast<uint32_t>(k[2]) << 16;
	case 2:  a += static_cast<uint32_t>(k[1]) << 8;
	case 1:  a += k[0]; break;
	default: break;
	}
	STORM_JENKINS_MIX(a, b, c);
	return c;
}

// 1.12a: 0x6FC06270 (Storm.#582)
uint32_t __stdcall SHashString(const char* szStr)
{
	char szNormalized[1024] = {};
	uint32_t nLen = 0;
	if (szStr)
	{
		while (szStr[nLen] != '\0' && nLen < 1023)
		{
			char ch = szStr[nLen];
			if (ch >= 'a' && ch <= 'z')
			{
				ch -= 0x20;
			}
			else if (ch == '/')
			{
				ch = '\\';
			}
			szNormalized[nLen] = ch;
			++nLen;
		}
	}
	szNormalized[nLen] = '\0';
	return StormJenkinsHash(reinterpret_cast<const uint8_t*>(szNormalized), nLen, 0);
}

static void StormBuildRegSubKey(const char* keyname, uint32_t flags, char* szOut, size_t nOutSize)
{
	szOut[0] = '\0';
	if ((flags & SREG_ABSOLUTE) == 0)
	{
		SStrCopy(szOut, (flags & SREG_BATTLE_NET) ? "Software\\Battle.net\\" : "Software\\Blizzard Entertainment\\", nOutSize);
	}
	if (keyname)
	{
		SStrNCat(szOut, keyname, static_cast<int>(nOutSize));
	}
}

// 1.12a: 0x6FC0C960 (Storm.#427)
BOOL __stdcall SRegGetBaseKey(char flags, char* buffer, size_t bufferchars)
{
	if (!buffer || bufferchars == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	StormBuildRegSubKey("", static_cast<uint8_t>(flags), buffer, bufferchars);
	return TRUE;
}

// 1.12a: 0x6FC0CA60 (Storm.#421)
BOOL __stdcall SRegLoadData(const char* keyname, const char* valuename, size_t nSize, LPBYTE lpData, uint32_t nFlags, LPDWORD lpcbData)
{
	if (!keyname || !valuename || (nSize > 0 && !lpData))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	char szSubKey[260] = {};
	StormBuildRegSubKey(keyname, nFlags, szSubKey, sizeof(szSubKey));

	HKEY hRoots[2] = {};
	int nRoots = 0;
	if ((nFlags & SREG_EXCLUDE_CURRENT_USER) == 0)
	{
		hRoots[nRoots++] = HKEY_CURRENT_USER;
	}
	if ((nFlags & SREG_EXCLUDE_LOCAL_MACHINE) == 0)
	{
		hRoots[nRoots++] = HKEY_LOCAL_MACHINE;
	}

	for (int i = 0; i < nRoots; ++i)
	{
		HKEY hKey = nullptr;
		if (RegOpenKeyExA(hRoots[i], szSubKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			DWORD dwType = 0;
			DWORD cbData = static_cast<DWORD>(nSize);
			const LSTATUS status = RegQueryValueExA(hKey, valuename, nullptr, &dwType, lpData, &cbData);
			RegCloseKey(hKey);
			if (lpcbData)
			{
				*lpcbData = cbData;
			}
			if (status == ERROR_SUCCESS)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

// 1.12a: 0x6FC0CBC0 (Storm.#422)
BOOL __stdcall SRegLoadString(const char* keyname, const char* valuename, uint32_t nFlags, LPSTR pBuffer, size_t nBufferSize)
{
	if (!keyname || !valuename || !pBuffer || nBufferSize == 0)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD cbRead = 0;
	if (SRegLoadData(keyname, valuename, nBufferSize, reinterpret_cast<LPBYTE>(pBuffer), nFlags, &cbRead))
	{
		pBuffer[nBufferSize - 1] = '\0';
		return TRUE;
	}
	pBuffer[0] = '\0';
	return FALSE;
}

// 1.12a: 0x6FC0CCA0 (Storm.#423)
BOOL __stdcall SRegLoadValue(const char* keyname, const char* valuename, uint32_t nFlags, DWORD* value)
{
	if (!keyname || !valuename || !value)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	DWORD cbRead = 0;
	return SRegLoadData(keyname, valuename, sizeof(DWORD), reinterpret_cast<LPBYTE>(value), nFlags, &cbRead);
}

static BOOL StormRegSaveInternal(const char* keyname, const char* valuename, BYTE flags, DWORD dwType, const BYTE* lpData, DWORD cbData)
{
	if (!keyname || !valuename || !lpData)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	char szSubKey[260] = {};
	StormBuildRegSubKey(keyname, flags, szSubKey, sizeof(szSubKey));

	HKEY hRoot = (flags & SREG_EXCLUDE_CURRENT_USER) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
	HKEY hKey = nullptr;
	DWORD dwDisposition = 0;
	if (RegCreateKeyExA(hRoot, szSubKey, 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, &dwDisposition) != ERROR_SUCCESS)
	{
		return FALSE;
	}

	const LSTATUS status = RegSetValueExA(hKey, valuename, 0, dwType, lpData, cbData);
	RegCloseKey(hKey);
	return status == ERROR_SUCCESS;
}

// 1.12a: 0x6FC0CD50 (Storm.#424)
BOOL __stdcall SRegSaveData(char* keyname, char* valuename, BYTE flags, BYTE* lpData, DWORD cbData)
{
	return StormRegSaveInternal(keyname, valuename, flags, REG_BINARY, lpData, cbData);
}

// 1.12a: 0x6FC0CEC0 (Storm.#425)
BOOL __stdcall SRegSaveString(const char* keyname, const char* valuename, BYTE flags, const char* string)
{
	if (!string)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
	return StormRegSaveInternal(keyname, valuename, flags, REG_SZ, reinterpret_cast<const BYTE*>(string), static_cast<DWORD>(std::strlen(string) + 1));
}

// 1.12a: 0x6FC0CFA0 (Storm.#426)
BOOL __stdcall SRegSaveValue(const char* keyname, const char* valuename, BYTE flags, DWORD result)
{
	return StormRegSaveInternal(keyname, valuename, flags, REG_DWORD, reinterpret_cast<const BYTE*>(&result), sizeof(DWORD));
}

// 1.12a: 0x6FC0CB30 (Storm.#428)
BOOL __stdcall SRegDeleteValue(char* keyname, char* valuename, HKEY phkResult)
{
	if (!keyname || !valuename)
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

	const uint32_t nFlags = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(phkResult) & 0xFFu);
	char szSubKey[260] = {};
	StormBuildRegSubKey(keyname, nFlags, szSubKey, sizeof(szSubKey));

	HKEY hRoot = (nFlags & SREG_EXCLUDE_CURRENT_USER) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
	HKEY hKey = nullptr;
	if (RegOpenKeyExA(hRoot, szSubKey, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
	{
		return FALSE;
	}
	const LSTATUS status = RegDeleteValueA(hKey, valuename);
	RegCloseKey(hKey);
	return status == ERROR_SUCCESS;
}

// 1.12a: 0x6FC354F8
static const uint8_t TrailingBytesForUTF8[256] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,
};

// 1.12a: 0x6FC354E0
static const uint32_t OffsetsFromUTF8[6] = {
	0x00000000u, 0x00003080u, 0x000E2080u,
	0x03C82080u, 0xFA082080u, 0x82082080u,
};

// 1.12a: 0x6FC14110 (Storm.#904)
int __stdcall SUniConvertUTF8to16(wchar_t* pBuffer, int nBufferCount, const char* szSrc, int nSrcCount, int* pOutLength, int a6)
{
	int* pBytesConsumed = reinterpret_cast<int*>(static_cast<uintptr_t>(a6));
	if (pBytesConsumed)
	{
		*pBytesConsumed = 0;
	}
	if (!pOutLength)
	{
		return -1;
	}
	*pOutLength = 0;
	if (!pBuffer || nBufferCount < 0 || !szSrc || nSrcCount < 0)
	{
		return -1;
	}

	const uint8_t* source = reinterpret_cast<const uint8_t*>(szSrc);
	const uint8_t* const sourceStart = source;
	const uint8_t* const sourceEnd = source + nSrcCount;
	wchar_t* target = pBuffer;
	wchar_t* const targetStart = target;
	wchar_t* const targetEnd = target + nBufferCount;

	int result = 0;
	while (source < sourceEnd && *source != 0)
	{
		uint32_t ch = 0;
		const uint16_t extraBytesToRead = TrailingBytesForUTF8[*source];
		if (source + extraBytesToRead >= sourceEnd)
		{
			result = 1;
			break;
		}
		bool bNullInMiddle = false;
		for (uint16_t i = 1; i <= extraBytesToRead; ++i)
		{
			if (source[i] == 0)
			{
				bNullInMiddle = true;
				break;
			}
		}
		if (bNullInMiddle)
		{
			result = 1;
			break;
		}

		switch (extraBytesToRead)
		{
		case 5: ch += *source++; ch <<= 6;
		case 4: ch += *source++; ch <<= 6;
		case 3: ch += *source++; ch <<= 6;
		case 2: ch += *source++; ch <<= 6;
		case 1: ch += *source++; ch <<= 6;
		case 0: ch += *source++; break;
		}
		ch -= OffsetsFromUTF8[extraBytesToRead];

		if (target >= targetEnd)
		{
			result = 2;
			break;
		}

		if (ch <= 0xFFFFu)
		{
			*target++ = static_cast<wchar_t>(ch);
		}
		else if (ch > 0x10FFFFu)
		{
			*target++ = static_cast<wchar_t>(0xFFFDu);
		}
		else
		{
			if (target + 1 >= targetEnd)
			{
				result = 2;
				break;
			}
			ch -= 0x0010000u;
			*target++ = static_cast<wchar_t>((ch >> 10) + 0xD800u);
			*target++ = static_cast<wchar_t>((ch & 0x3FFu) + 0xDC00u);
		}
	}

	if (result == 0)
	{
		if (source < sourceEnd && *source == 0)
		{
			if (target < targetEnd)
			{
				++source;
				*target = L'\0';
			}
			else
			{
				result = 2;
			}
		}
	}

	if (pBytesConsumed)
	{
		*pBytesConsumed = static_cast<int>(source - sourceStart);
	}
	*pOutLength = static_cast<int>(target - targetStart);
	return result;
}
