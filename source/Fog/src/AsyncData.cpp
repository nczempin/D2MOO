#include <Fog.h>
#include <Storm.h>
#include <Windows.h>
#include <cstring>

static int32_t gnPostponePriorityChangesDepth = 0;
static AsyncData* gpPostponedPriorityChangeList = nullptr;
static BOOL gbAsyncDataInitialized = FALSE;
static uint32_t gnAsyncEventSlots = 0;
static AsyncDataEventSlot* gpAsyncEventSlots = nullptr;

// 1.12a: 0x6FF64D60
static void AsyncData_FinishLoad(AsyncData* pAsyncData)
{
	if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_LOADED) != 0)
	{
		return;
	}

	if (pAsyncData->pEventSlot != nullptr)
	{
		ResetEvent(pAsyncData->pEventSlot->hEvent);
		pAsyncData->pEventSlot->bSlotAvailable = TRUE;
		pAsyncData->pEventSlot->pAsyncOp = nullptr;
		pAsyncData->pEventSlot = nullptr;
	}

	if (pAsyncData->hFile != nullptr)
	{
		SFileCloseFile(reinterpret_cast<HANDLE>(pAsyncData->hFile));
		pAsyncData->hFile = nullptr;
	}

	pAsyncData->nFlags |= ASYNC_DATA_FLAGS_LOADED;
	if (pAsyncData->pAsyncOpDesc.pData != nullptr)
	{
		FOG_FreePool(pAsyncData->pMemPool, pAsyncData->pBuffer, __FILE__, __LINE__, 0);
		pAsyncData->pBuffer = pAsyncData->pAsyncOpDesc.pData;
		pAsyncData->nFileSize = static_cast<uint32_t>(pAsyncData->pAsyncOpDesc.nFileSize);
		memset(&pAsyncData->pAsyncOpDesc, 0, sizeof(pAsyncData->pAsyncOpDesc));
	}
}

// 1.12a: 0x6FF64E90
static AsyncDataEventSlot* AsyncData_AllocEventSlot()
{
	if (!gbAsyncDataInitialized)
	{
		return nullptr;
	}

	for (uint32_t i = 0; i < gnAsyncEventSlots; ++i)
	{
		if (gpAsyncEventSlots[i].bSlotAvailable != 0)
		{
			gpAsyncEventSlots[i].bSlotAvailable = FALSE;
			return &gpAsyncEventSlots[i];
		}
	}

	for (uint32_t i = 0; i < gnAsyncEventSlots; ++i)
	{
		if (WaitForSingleObject(gpAsyncEventSlots[i].hEvent, 0) == WAIT_OBJECT_0)
		{
			AsyncData_FinishLoad(gpAsyncEventSlots[i].pAsyncOp);
			gpAsyncEventSlots[i].bSlotAvailable = FALSE;
			return &gpAsyncEventSlots[i];
		}
	}

	if (gnAsyncEventSlots >= 1024)
	{
		return nullptr;
	}

	AsyncDataEventSlot* pSlot = &gpAsyncEventSlots[gnAsyncEventSlots++];
	pSlot->hEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
	pSlot->bSlotAvailable = FALSE;
	return pSlot;
}

// 1.10f: Fog.0x6FF5C010 (#10089)
// 1.12a: Fog.0x6FF65520 (#10089)
void __fastcall FOG_AsyncDataInitialize(BOOL bEnableAsyncOperations)
{
	D2_ASSERT(!gbAsyncDataInitialized);

	SFileSetAsyncBudget(0x40000);
	SFileSetDataChunkSize(0x4000);

	if (bEnableAsyncOperations)
	{
		gpAsyncEventSlots = static_cast<AsyncDataEventSlot*>(D2_ALLOC(1024 * sizeof(AsyncDataEventSlot)));
		D2_ASSERT(gpAsyncEventSlots != nullptr);

		for (uint32_t i = 0; i < 256; ++i)
		{
			gpAsyncEventSlots[i].hEvent = CreateEventA(nullptr, TRUE, FALSE, nullptr);
			gpAsyncEventSlots[i].bSlotAvailable = TRUE;
			gpAsyncEventSlots[i].pAsyncOp = nullptr;
		}

		gnAsyncEventSlots = 256;
		gbAsyncDataInitialized = TRUE;
	}
}

// 1.10f: Fog.0x6FF5C0E0 (#10090)
// 1.12a: Fog.0x6FF64E10 (#10090)
void __fastcall FOG_AsyncDataDestroy()
{
	if (gpAsyncEventSlots != nullptr)
	{
		for (uint32_t i = 0; i < gnAsyncEventSlots; ++i)
		{
			if (gpAsyncEventSlots[i].hEvent != nullptr)
			{
				CloseHandle(gpAsyncEventSlots[i].hEvent);
				gpAsyncEventSlots[i].hEvent = nullptr;
			}
		}

		D2_FREE(gpAsyncEventSlots);
		gpAsyncEventSlots = nullptr;
		gnAsyncEventSlots = 0;
	}
	gbAsyncDataInitialized = FALSE;
}

// 1.10f: Fog.0x6FF5C2B0 (#10091)
// 1.12a: Fog.0x6FF652E0 (#10091)
AsyncData* __fastcall FOG_AsyncDataLoadFileEx(
	void* pMemPool,
	const char* pFileName,
	BOOL bAllowAsync,
	LONG nFileOffset,
	int nFileSize,
	void* pBuffer,
	AsyncDataLoadFileCallback* asyncOpDesc,
	int a8,
	const char* sourceFile,
	int sourceLine)
{
	AsyncData* pAsyncData = static_cast<AsyncData*>(FOG_AllocPool(pMemPool, sizeof(AsyncData), __FILE__, __LINE__, 0));
	memset(pAsyncData, 0, sizeof(AsyncData));
	pAsyncData->nFlags = ASYNC_DATA_FLAGS_DEBUG_TAG;

	if (!SFileOpenFile(const_cast<char*>(pFileName), reinterpret_cast<int>(&pAsyncData->hFile)))
	{
		FOG_FreePool(pMemPool, pAsyncData, __FILE__, __LINE__, 0);
		return nullptr;
	}

	pAsyncData->pMemPool = pMemPool;
	pAsyncData->nPriority = static_cast<uint32_t>(a8);

	if (nFileOffset != 0)
	{
		const int nSeekRes = SFileSetFilePointer(reinterpret_cast<int>(pAsyncData->hFile), nFileOffset, 0, FILE_BEGIN);
		D2_ASSERT(nSeekRes != -1);
	}

	if (nFileSize <= 0)
	{
		nFileSize = SFileGetFileSize(reinterpret_cast<int>(pAsyncData->hFile), nullptr);
	}
	pAsyncData->nFileSize = static_cast<uint32_t>(nFileSize);

	if (pBuffer != nullptr)
	{
		pAsyncData->pBuffer = pBuffer;
		pAsyncData->nFlags &= ~ASYNC_DATA_FLAGS_OWNS_BUFFER;
	}
	else
	{
		pAsyncData->pBuffer = FOG_AllocPool(pMemPool, static_cast<int>(pAsyncData->nFileSize), sourceFile, sourceLine, 0);
		pAsyncData->nFlags |= ASYNC_DATA_FLAGS_OWNS_BUFFER;
	}

	AsyncDataLoadFileCallback* pCallbackDesc = nullptr;
	if (asyncOpDesc != nullptr)
	{
		pAsyncData->pAsyncOpDesc = *asyncOpDesc;
		pCallbackDesc = &pAsyncData->pAsyncOpDesc;
	}

	if (bAllowAsync && gbAsyncDataInitialized)
	{
		AsyncDataEventSlot* pSlot = AsyncData_AllocEventSlot();
		pAsyncData->pEventSlot = pSlot;
		if (!pSlot)
		{
			if (pAsyncData->hFile != nullptr)
			{
				SFileCloseFile(reinterpret_cast<HANDLE>(pAsyncData->hFile));
				pAsyncData->hFile = nullptr;
			}
			if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_OWNS_BUFFER) != 0)
			{
				FOG_FreePool(pMemPool, pAsyncData->pBuffer, __FILE__, __LINE__, 0);
			}
			FOG_FreePool(pMemPool, pAsyncData, __FILE__, __LINE__, 0);
			return nullptr;
		}

		pSlot->pAsyncOp = pAsyncData;
		pAsyncData->tOverlapped.hEvent = pSlot->hEvent;
		pAsyncData->tOverlapped.Offset = static_cast<DWORD>(nFileOffset);

		SFileFileExistsEx(
			reinterpret_cast<int>(pAsyncData->hFile),
			reinterpret_cast<int>(pAsyncData->pBuffer),
			static_cast<int>(pAsyncData->nFileSize),
			0,
			reinterpret_cast<LONG>(&pAsyncData->tOverlapped),
			a8,
			reinterpret_cast<int>(pCallbackDesc));
		return pAsyncData;
	}

	SFileFileExistsEx(
		reinterpret_cast<int>(pAsyncData->hFile),
		reinterpret_cast<int>(pAsyncData->pBuffer),
		static_cast<int>(pAsyncData->nFileSize),
		0,
		0,
		a8,
		0);

	if (pCallbackDesc && pCallbackDesc->callback)
	{
		pCallbackDesc->callback(pAsyncData->pBuffer, pAsyncData->nFileSize, pCallbackDesc);
	}

	AsyncData_FinishLoad(pAsyncData);
	return pAsyncData;
}

// 1.10f: Fog.0x6FF5C480 (#10092)
// 1.12a: Fog.0x6FF65260 (#10092)
BOOL __fastcall FOG_AsyncDataTestLoaded(AsyncData* pAsyncData)
{
	if (!pAsyncData)
	{
		return FALSE;
	}

	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);

	if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_LOADED) == 0)
	{
		if (WaitForSingleObject(pAsyncData->pEventSlot->hEvent, 0) == WAIT_OBJECT_0)
		{
			AsyncData_FinishLoad(pAsyncData);
		}
	}

	return (pAsyncData->nFlags & ASYNC_DATA_FLAGS_LOADED) != 0;
}

// 1.10f: Fog.0x6FF5C630 (#10093)
// 1.12a: Fog.0x6FF65790 (#10093)
void __fastcall FOG_AsyncDataWaitAndGetBuffer(AsyncData* pAsyncData)
{
	D2_ASSERT(pAsyncData != nullptr);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);

	if (!FOG_AsyncDataTestLoaded(pAsyncData))
	{
		SFilePrioritizeRequest(reinterpret_cast<int>(pAsyncData->pBuffer), ASYNC_DATA_PRIORITY_HIGH);
		if (WaitForSingleObject(pAsyncData->pEventSlot->hEvent, INFINITE) != WAIT_OBJECT_0)
		{
			FOG_DisplayHalt("WaitForSingleObject", __FILE__, __LINE__);
			exit(-1);
		}
		AsyncData_FinishLoad(pAsyncData);
	}
}

// 1.10f: Fog.0x6FF5C700 (#10094)
// 1.12a: Fog.0x6FF65860 (#10094)
void* __fastcall FOG_AsyncDataGetBuffer(AsyncData* pAsyncData)
{
	D2_ASSERT(pAsyncData != nullptr);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);

	if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_LOADED) == 0)
	{
		FOG_AsyncDataWaitAndGetBuffer(pAsyncData);
	}

	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_LOADED) != 0);
	return pAsyncData->pBuffer;
}

// 1.10f: Fog.0x6FF5C790 (#10095)
// 1.12a: Fog.0x6FF65040 (#10095)
DWORD __fastcall FOG_AsyncDataGetBufferSize(AsyncData* pAsyncData)
{
	D2_ASSERT(pAsyncData != nullptr);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);
	return pAsyncData->nFileSize;
}

// 1.10f: Fog.0x6FF5C7E0 (#10096)
// 1.12a: Fog.0x6FF64F50 (#10096)
void __fastcall FOG_AsyncDataSetResults(AsyncData* pAsyncData, void* pBuffer, int nBufferSize)
{
	D2_ASSERT(pAsyncData != nullptr);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_LOADED) != 0);

	FOG_FreePool(pAsyncData->pMemPool, pAsyncData->pBuffer, __FILE__, __LINE__, 0);
	pAsyncData->pBuffer = pBuffer;
	pAsyncData->nFileSize = static_cast<uint32_t>(nBufferSize);
}

// 1.10f: Fog.0x6FF5C880 (#10097)
// 1.12a: Fog.0x6FF65630 (#10097)
void __fastcall FOG_AsyncDataFree(AsyncData* pAsyncData)
{
	D2_ASSERT(pAsyncData != nullptr);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);

	if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_POSTPONED_PRIORITY_CHANGE) != 0)
	{
		AsyncData** ppCurr = &gpPostponedPriorityChangeList;
		while (*ppCurr != nullptr)
		{
			if (*ppCurr == pAsyncData)
			{
				*ppCurr = pAsyncData->pNextPostponedPriorityChange;
				break;
			}
			ppCurr = &(*ppCurr)->pNextPostponedPriorityChange;
		}
	}

	if (!FOG_AsyncDataTestLoaded(pAsyncData))
	{
		SFileCancelRequest(reinterpret_cast<int>(pAsyncData->pBuffer));
		WaitForSingleObject(pAsyncData->pEventSlot->hEvent, INFINITE);
		AsyncData_FinishLoad(pAsyncData);
	}

	if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_OWNS_BUFFER) != 0)
	{
		D2_ASSERT(pAsyncData->pBuffer != nullptr);
		FOG_FreePool(pAsyncData->pMemPool, pAsyncData->pBuffer, __FILE__, __LINE__, 0);
		pAsyncData->pBuffer = nullptr;
		pAsyncData->nFlags &= ~ASYNC_DATA_FLAGS_DEBUG_TAG_MASK;
	}

	FOG_FreePool(pAsyncData->pMemPool, pAsyncData, __FILE__, __LINE__, 0);
}

// 1.10f: Fog.0x6FF5C4E0 (#10099)
// 1.12a: Fog.0x6FF65160 (#10099)
void __fastcall FOG_AsyncDataSetPriority(AsyncData* pAsyncData, int nPriority)
{
	D2_ASSERT(pAsyncData != nullptr);
	D2_ASSERT((pAsyncData->nFlags & ASYNC_DATA_FLAGS_DEBUG_TAG_MASK) == ASYNC_DATA_FLAGS_DEBUG_TAG);
	D2_ASSERT(nPriority <= 0);

	if (gnPostponePriorityChangesDepth > 0)
	{
		if ((pAsyncData->nFlags & ASYNC_DATA_FLAGS_POSTPONED_PRIORITY_CHANGE) != 0)
		{
			if (nPriority > static_cast<int>(pAsyncData->nPriority))
			{
				pAsyncData->nPriority = static_cast<uint32_t>(nPriority);
			}
			return;
		}

		pAsyncData->nPreviousPriority = pAsyncData->nPriority;
		pAsyncData->nPriority = static_cast<uint32_t>(nPriority);
		pAsyncData->nFlags |= ASYNC_DATA_FLAGS_POSTPONED_PRIORITY_CHANGE;
		pAsyncData->pNextPostponedPriorityChange = gpPostponedPriorityChangeList;
		gpPostponedPriorityChangeList = pAsyncData;
		return;
	}

	if (static_cast<uint32_t>(nPriority) != pAsyncData->nPriority)
	{
		pAsyncData->nPriority = static_cast<uint32_t>(nPriority);
		SFilePrioritizeRequest(reinterpret_cast<int>(pAsyncData->pBuffer), nPriority);
	}
}

// 1.10f: Fog.0x6FF5C5A0 (#10100)
// 1.12a: Fog.0x6FF650C0 (#10100)
void __fastcall FOG_AsyncDataHandlePriorityChanges(BOOL bPostpone)
{
	if (bPostpone)
	{
		++gnPostponePriorityChangesDepth;
		return;
	}

	D2_ASSERT(gnPostponePriorityChangesDepth > 0);
	if (--gnPostponePriorityChangesDepth == 0)
	{
		AsyncData* pCurr = gpPostponedPriorityChangeList;
		while (pCurr != nullptr)
		{
			AsyncData* pNext = pCurr->pNextPostponedPriorityChange;
			if (pCurr->nPriority != pCurr->nPreviousPriority)
			{
				SFilePrioritizeRequest(reinterpret_cast<int>(pCurr->pBuffer), static_cast<int>(pCurr->nPriority));
			}
			pCurr->nFlags &= ~ASYNC_DATA_FLAGS_POSTPONED_PRIORITY_CHANGE;
			pCurr->nPreviousPriority = 0;
			pCurr->pNextPostponedPriorityChange = nullptr;
			pCurr = pNext;
		}
		gpPostponedPriorityChangeList = nullptr;
	}
}
