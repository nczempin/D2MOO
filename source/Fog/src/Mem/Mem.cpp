#include <Fog.h>
#include <Storm.h>

// 1.10f: 0x6FF58F50 (#10042)
// 1.12a: 0x6FF64C30 (#10042)
void* __fastcall FOG_Alloc(int nSize, const char* szFile, int nLine, int n0)
{
	D2_MAYBE_UNUSED(n0);
	void* pMem = SMemAlloc(nSize > 0 ? nSize : 1, szFile ? szFile : "", nLine, 0);
	if (!pMem)
	{
		FOG_DisplayError(2, szFile ? szFile : "", "", nLine);
	}
	return pMem;
}

// 1.10f: 0x6FF58F90 (#10043)
// 1.12a: 0x6FF64C10 (#10043)
void __fastcall FOG_Free(void* pFree, const char* szFile, int nLine, int n0)
{
	D2_MAYBE_UNUSED(n0);
	if (pFree)
	{
		SMemFree(pFree, szFile ? szFile : "", nLine, 0);
	}
}

// 1.10f: 0x6FF58FB0 (#10044)
// 1.12a: 0x6FF64D10 (#10044)
void* __fastcall FOG_Realloc(void* pMemory, int nSize, const char* szFile, int nLine, int n0)
{
	D2_MAYBE_UNUSED(n0);
	if (!pMemory)
	{
		return FOG_Alloc(nSize, szFile, nLine, n0);
	}
	if (nSize <= 0)
	{
		FOG_Free(pMemory, szFile, nLine, n0);
		return nullptr;
	}
	void* pResult = SMemReAlloc(pMemory, nSize, szFile ? szFile : "", nLine, 0);
	if (!pResult)
	{
		FOG_DisplayError(2, szFile ? szFile : "", "", nLine);
	}
	return pResult;
}

// 1.10f: 0x6FF58FF0 (#10045)
// 1.12a: 0x6FF64C70 (#10045)
void* __fastcall FOG_AllocPool(void* pMemPool, int nSize, const char* szFile, int nLine, int n0)
{
	D2_MAYBE_UNUSED(pMemPool);
	return FOG_Alloc(nSize, szFile, nLine, n0);
}

// 1.10f: 0x6FF59030 (#10046)
// 1.12a: 0x6FF64BE0 (#10046)
void __fastcall FOG_FreePool(void* pMemPool, void* pFree, const char* szFile, int nLine, int n0)
{
	D2_MAYBE_UNUSED(pMemPool);
	FOG_Free(pFree, szFile, nLine, n0);
}

// 1.10f: 0x6FF59060 (#10047)
// 1.12a: 0x6FF64CC0 (#10047)
void* __fastcall FOG_ReallocPool(void* pMemPool, void* pMemory, int nSize, const char* szFile, int nLine, int n0)
{
	D2_MAYBE_UNUSED(pMemPool);
	return FOG_Realloc(pMemory, nSize, szFile, nLine, n0);
}

// 1.10f: 0x6FF5A280 (#10142)
// 1.12a: 0x6FF6A870 (#10142)
void __cdecl FOG_CreateNewPoolSystem(void** pMemPoolSystem, const char* szName, uint32_t nPools, uint32_t nUnused)
{
	D2_MAYBE_UNUSED(szName);
	D2_MAYBE_UNUSED(nPools);
	D2_MAYBE_UNUSED(nUnused);
	if (pMemPoolSystem)
	{
		*pMemPoolSystem = nullptr;
	}
}

// 1.10f: 0x6FF5A100 (#10143)
// 1.12a: 0x6FF6B010 (#10143)
void __cdecl FOG_DestroyMemoryPoolSystem(void* pMemoryPoolSystem)
{
	D2_MAYBE_UNUSED(pMemoryPoolSystem);
}

// 1.10f: 0x6FF5A4A0 (#10145)
// 1.12a: 0x6FF6AFC0 (#10145)
void __cdecl FOG_SetMemoryPoolSystemMode(void* pMemoryPoolSystem, BOOL bFreePoolAlloc)
{
	D2_MAYBE_UNUSED(pMemoryPoolSystem);
	D2_MAYBE_UNUSED(bFreePoolAlloc);
}

// 1.10f: 0x6FF5A4E0 (#10147)
// 1.12a: 0x6FF6AF70 (#10147)
DWORD __cdecl FOG_GetMemoryUsage(void* pMemoryPoolSystem)
{
	D2_MAYBE_UNUSED(pMemoryPoolSystem);
	return 0;
}
