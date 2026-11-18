#include <Fog.h>
#include <Storm.h>
#include <cstring>

// 1.12a: Fog.0x6FF6FA50 (#10061)
void __fastcall DATATBLS_DestroyDataArray(D2FogDataArrayStrc* pArray)
{
	if (!pArray)
	{
		return;
	}
	std::memset(pArray, 0, sizeof(D2FogDataArrayStrc));
	FOG_Free(pArray, __FILE__, __LINE__, 0);
}

// 1.12a: Fog.0x6FF6FA80 (#10060)
D2FogDataArrayStrc* __fastcall DATATBLS_CreateDataArray(const char* szName, int32_t nCapacity, int32_t nElementSize)
{
	const int32_t nTotalBytes = static_cast<int32_t>(sizeof(D2FogDataArrayStrc)) + nCapacity * nElementSize;
	D2FogDataArrayStrc* pArray = static_cast<D2FogDataArrayStrc*>(FOG_Alloc(nTotalBytes, __FILE__, __LINE__, 0));
	if (!pArray)
	{
		FOG_DisplayError(2, __FILE__, "", __LINE__);
		return nullptr;
	}

	std::memset(pArray, 0, sizeof(D2FogDataArrayStrc));
	if (szName)
	{
		SStrCopy(pArray->szName, szName, sizeof(pArray->szName));
	}
	pArray->nCapacity = nCapacity;
	pArray->nElementSize = nElementSize;
	pArray->bInitialized = FALSE;
	pArray->pElements = reinterpret_cast<uint8_t*>(pArray) + sizeof(D2FogDataArrayStrc);
	return pArray;
}

// 1.12a: Fog.0x6FF6FB10 (#10067)
void* __fastcall DATATBLS_TryGetDataArrayElement(D2FogDataArrayStrc* pArray, int32_t hElement)
{
	if (!pArray || hElement == -1)
	{
		return nullptr;
	}

	const int32_t nIndex = static_cast<int16_t>(hElement >> 16);
	if (nIndex < 0 || nIndex >= pArray->nCapacity)
	{
		FOG_DisplayHalt("Invalid DataArray handle index", __FILE__, __LINE__);
		return nullptr;
	}
	if (!pArray->bInitialized)
	{
		FOG_DisplayHalt("DataArray not initialized", __FILE__, __LINE__);
		return nullptr;
	}

	uint8_t* pElement = pArray->pElements + nIndex * pArray->nElementSize;
	const uint16_t wSlotGen = *reinterpret_cast<const uint16_t*>(pElement);
	if (wSlotGen == 0)
	{
		return nullptr;
	}

	const uint16_t wHandleGen = static_cast<uint16_t>(hElement & 0xFFFF);
	if (wHandleGen != 0 && wSlotGen != wHandleGen)
	{
		return nullptr;
	}

	return pElement;
}

// 1.12a: Fog.0x6FF6FBC0 (#10064)
int32_t __fastcall DATATBLS_AllocDataArrayElement(D2FogDataArrayStrc* pArray)
{
	if (!pArray || !pArray->bInitialized)
	{
		FOG_DisplayHalt("DataArray not initialized", __FILE__, __LINE__);
		return -1;
	}

	const int32_t nStride = pArray->nElementSize;
	int32_t nIndex = pArray->nFirstFreeIndex;
	uint8_t* pElement = pArray->pElements + nIndex * nStride;

	while (nIndex < pArray->nCapacity)
	{
		if (*reinterpret_cast<const uint16_t*>(pElement) == 0)
		{
			std::memset(pElement, 0, static_cast<size_t>(nStride));
			const uint16_t wGen = pArray->wNextGeneration;
			*reinterpret_cast<uint16_t*>(pElement) = wGen;

			pArray->wNextGeneration = static_cast<uint16_t>(wGen + 1);
			if (pArray->wNextGeneration == 0)
			{
				pArray->wNextGeneration = 0x8000u;
			}

			pArray->nActiveCount++;
			pArray->nFirstFreeIndex = nIndex + 1;
			if (pArray->nLastUsedIndex < nIndex)
			{
				pArray->nLastUsedIndex = nIndex;
			}

			return (nIndex << 16) | static_cast<int32_t>(wGen);
		}
		++nIndex;
		pElement += nStride;
	}

	return -1;
}

// 1.12a: Fog.0x6FF6FCC0 (#10063)
void __fastcall DATATBLS_ResetDataArray(D2FogDataArrayStrc* pArray)
{
	if (!pArray || !pArray->bInitialized)
	{
		FOG_DisplayHalt("DataArray not initialized", __FILE__, __LINE__);
		return;
	}

	pArray->nLastUsedIndex = -1;
	pArray->nActiveCount = 0;
	pArray->nFirstFreeIndex = 0;
	pArray->wNextGeneration = 0x8000u;

	const int32_t nStride = pArray->nElementSize;
	uint8_t* pCursor = pArray->pElements;
	for (int32_t i = 0; i < pArray->nCapacity; ++i)
	{
		*reinterpret_cast<uint16_t*>(pCursor) = 0;
		pCursor += nStride;
	}
}

// 1.12a: Fog.0x6FF6FD60 (#10062)
void __fastcall DATATBLS_InitDataArray(D2FogDataArrayStrc* pArray)
{
	if (!pArray)
	{
		return;
	}
	pArray->bInitialized = TRUE;
	DATATBLS_ResetDataArray(pArray);
}

// 1.12a: Fog.0x6FF6FD70 (#10066)
void* __fastcall DATATBLS_GetDataArrayElementChecked(D2FogDataArrayStrc* pArray, int32_t hElement)
{
	void* pElement = DATATBLS_TryGetDataArrayElement(pArray, hElement);
	if (!pElement)
	{
		FOG_DisplayHalt("Invalid DataArray handle", __FILE__, __LINE__);
	}
	return pElement;
}

// 1.12a: Fog.0x6FF6FDB0 (#10065)
void __fastcall DATATBLS_FreeDataArrayElement(D2FogDataArrayStrc* pArray, int32_t hElement)
{
	uint8_t* pElement = static_cast<uint8_t*>(DATATBLS_TryGetDataArrayElement(pArray, hElement));
	if (!pElement)
	{
		FOG_DisplayHalt("Invalid DataArray handle on free", __FILE__, __LINE__);
		return;
	}

	*reinterpret_cast<uint16_t*>(pElement) = 0;
	const int32_t nIndex = static_cast<int16_t>(hElement >> 16);
	if (nIndex < pArray->nFirstFreeIndex)
	{
		pArray->nFirstFreeIndex = nIndex;
	}

	if (nIndex == pArray->nLastUsedIndex)
	{
		const int32_t nStride = pArray->nElementSize;
		uint8_t* pScan = pElement;
		for (;;)
		{
			const int32_t nPrev = pArray->nLastUsedIndex - 1;
			pArray->nLastUsedIndex = nPrev;
			pScan -= nStride;
			if (nPrev < 0 || *reinterpret_cast<const uint16_t*>(pScan) != 0)
			{
				break;
			}
		}
	}

	pArray->nActiveCount--;
}
