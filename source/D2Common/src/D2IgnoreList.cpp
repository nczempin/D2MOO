#include "D2IgnoreList.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#include <Fog.h>
#include <Storm.h>

namespace
{
std::string NormalizeLower(const char* szInput)
{
	if (!szInput)
	{
		return {};
	}
	std::string s(szInput);
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
	return s;
}

bool WildcardMatchCaseInsensitive(const char* szPattern, const char* szText)
{
	if (!szPattern || !szText)
	{
		return false;
	}
	const char* pPat = szPattern;
	const char* pTxt = szText;
	const char* pStarPat = nullptr;
	const char* pStarTxt = nullptr;

	while (*pTxt)
	{
		if (*pPat == '*')
		{
			pStarPat = ++pPat;
			pStarTxt = pTxt;
		}
		else if (*pPat == '?' || std::tolower(static_cast<unsigned char>(*pPat)) == std::tolower(static_cast<unsigned char>(*pTxt)))
		{
			++pPat;
			++pTxt;
		}
		else if (pStarPat)
		{
			pPat = pStarPat;
			pTxt = ++pStarTxt;
		}
		else
		{
			return false;
		}
	}

	while (*pPat == '*')
	{
		++pPat;
	}
	return *pPat == '\0';
}

class CIgnoreList final : public IIgnoreList
{
public:
	explicit CIgnoreList(BOOL bLoadFromDisk)
		: m_bPersist(FALSE)
	{
		DWORD dwPersist = 0;
		if (SRegLoadValue("Diablo II", "IgnorePersist", 0, &dwPersist))
		{
			m_bPersist = (dwPersist != 0);
		}
		if (bLoadFromDisk)
		{
			LoadFromDisk();
		}
	}

	~CIgnoreList() override = default;

	void LoadFromDisk() override
	{
		m_characterIgnores.clear();
		m_accountIgnores.clear();
		m_textFilters.clear();

		FILE* pFile = std::fopen("ignorelist", "r");
		if (!pFile)
		{
			return;
		}

		char szLine[512] = {};
		while (std::fgets(szLine, sizeof(szLine), pFile))
		{
			size_t nLen = std::strlen(szLine);
			while (nLen > 0 && (szLine[nLen - 1] == '\r' || szLine[nLen - 1] == '\n'))
			{
				szLine[--nLen] = '\0';
			}
			if (nLen < 3 || szLine[1] != ':')
			{
				continue;
			}
			const char cType = szLine[0];
			const char* szValue = szLine + 2;
			if (cType == 'c' || cType == 'C')
			{
				AddUnique(m_characterIgnores, NormalizeLower(szValue));
			}
			else if (cType == 'a' || cType == 'A')
			{
				AddUnique(m_accountIgnores, NormalizeLower(szValue));
			}
			else if (cType == 't' || cType == 'T')
			{
				AddUnique(m_textFilters, NormalizeLower(szValue));
			}
		}
		std::fclose(pFile);
	}

	void ClearCharacterIgnores() override
	{
		m_characterIgnores.clear();
		if (m_bPersist)
		{
			SaveToDisk();
		}
	}

	void ClearAll() override
	{
		m_characterIgnores.clear();
		m_accountIgnores.clear();
		m_textFilters.clear();
		if (m_bPersist)
		{
			SaveToDisk();
		}
	}

	void AddCharacterIgnore(const char* szCharacterName, BOOL bPersist) override
	{
		if (!szCharacterName || !*szCharacterName)
		{
			return;
		}
		AddUnique(m_characterIgnores, NormalizeLower(szCharacterName));
		if (bPersist || m_bPersist)
		{
			SaveToDisk();
		}
	}

	void AddAccountIgnore(const char* szAccountName, BOOL bPersist) override
	{
		if (!szAccountName || !*szAccountName)
		{
			return;
		}
		const char* pName = (szAccountName[0] == '*') ? (szAccountName + 1) : szAccountName;
		if (!*pName)
		{
			return;
		}
		AddUnique(m_accountIgnores, NormalizeLower(pName));
		if (bPersist || m_bPersist)
		{
			SaveToDisk();
		}
	}

	void AddTextFilter(const char* szPattern, BOOL bPersist) override
	{
		if (!szPattern || !*szPattern)
		{
			return;
		}
		AddUnique(m_textFilters, NormalizeLower(szPattern));
		if (bPersist || m_bPersist)
		{
			SaveToDisk();
		}
	}

	void RemoveCharacterIgnore(const char* szCharacterName, BOOL bPersist) override
	{
		if (!szCharacterName || !*szCharacterName)
		{
			return;
		}
		RemoveValue(m_characterIgnores, NormalizeLower(szCharacterName));
		if (bPersist || m_bPersist)
		{
			SaveToDisk();
		}
	}

	void RemoveAccountIgnore(const char* szAccountName, BOOL bPersist) override
	{
		if (!szAccountName || !*szAccountName)
		{
			return;
		}
		const char* pName = (szAccountName[0] == '*') ? (szAccountName + 1) : szAccountName;
		RemoveValue(m_accountIgnores, NormalizeLower(pName));
		if (bPersist || m_bPersist)
		{
			SaveToDisk();
		}
	}

	void RemoveTextFilter(const char* szPattern, BOOL bPersist) override
	{
		if (!szPattern || !*szPattern)
		{
			return;
		}
		RemoveValue(m_textFilters, NormalizeLower(szPattern));
		if (bPersist || m_bPersist)
		{
			SaveToDisk();
		}
	}

	BOOL IsCharacterIgnored(const char* szCharacterName) const override
	{
		if (!szCharacterName || !*szCharacterName)
		{
			return FALSE;
		}
		const std::string sKey = NormalizeLower(szCharacterName);
		return std::find(m_characterIgnores.begin(), m_characterIgnores.end(), sKey) != m_characterIgnores.end();
	}

	BOOL IsAccountIgnored(const char* szAccountName) const override
	{
		if (!szAccountName || !*szAccountName)
		{
			return FALSE;
		}
		const char* pName = (szAccountName[0] == '*') ? (szAccountName + 1) : szAccountName;
		const std::string sKey = NormalizeLower(pName);
		return std::find(m_accountIgnores.begin(), m_accountIgnores.end(), sKey) != m_accountIgnores.end();
	}

	BOOL IsTextFiltered(const char* szMessage) const override
	{
		if (!szMessage || !*szMessage)
		{
			return FALSE;
		}
		const std::string sMsg = NormalizeLower(szMessage);
		for (const std::string& sPattern : m_textFilters)
		{
			if (sPattern.find('*') != std::string::npos || sPattern.find('?') != std::string::npos)
			{
				if (WildcardMatchCaseInsensitive(sPattern.c_str(), sMsg.c_str()))
				{
					return TRUE;
				}
			}
			else if (sMsg.find(sPattern) != std::string::npos)
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL IsIgnored(const char* szCharacterName, const char* szAccountName, const char* szMessage) const override
	{
		if (IsCharacterIgnored(szCharacterName))
		{
			return TRUE;
		}
		if (IsAccountIgnored(szAccountName))
		{
			return TRUE;
		}
		if (IsTextFiltered(szMessage))
		{
			return TRUE;
		}
		return FALSE;
	}

	BOOL GetPersistence() const override
	{
		return m_bPersist;
	}

	void SetPersistence(BOOL bPersist) override
	{
		m_bPersist = (bPersist != 0);
		SRegSaveValue("Diablo II", "IgnorePersist", 0, m_bPersist ? 1 : 0);
		if (m_bPersist)
		{
			SaveToDisk();
		}
	}

private:
	static void AddUnique(std::vector<std::string>& vec, const std::string& val)
	{
		if (!val.empty() && std::find(vec.begin(), vec.end(), val) == vec.end())
		{
			vec.push_back(val);
		}
	}

	static void RemoveValue(std::vector<std::string>& vec, const std::string& val)
	{
		vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
	}

	void SaveToDisk() const
	{
		FILE* pFile = std::fopen("ignorelist", "w");
		if (!pFile)
		{
			return;
		}
		for (const std::string& sChar : m_characterIgnores)
		{
			std::fprintf(pFile, "c:%s\n", sChar.c_str());
		}
		for (const std::string& sAcc : m_accountIgnores)
		{
			std::fprintf(pFile, "a:%s\n", sAcc.c_str());
		}
		for (const std::string& sFilter : m_textFilters)
		{
			std::fprintf(pFile, "t:%s\n", sFilter.c_str());
		}
		std::fclose(pFile);
	}

	BOOL m_bPersist;
	std::vector<std::string> m_characterIgnores;
	std::vector<std::string> m_accountIgnores;
	std::vector<std::string> m_textFilters;
};

IIgnoreList* gpIgnoreListSingleton = nullptr;
} // namespace

//1.13d: D2Common.0x6FDDD0E0 (#10097)
void __stdcall IGNORELIST_Alloc(BOOL bLoadFromDisk)
{
	if (!gpIgnoreListSingleton)
	{
		gpIgnoreListSingleton = new CIgnoreList(bLoadFromDisk);
	}
}

//1.13d: D2Common.0x6FDDCC40 (#10735)
IIgnoreList* __stdcall IGNORELIST_GetSingleton()
{
	if (!gpIgnoreListSingleton)
	{
		IGNORELIST_Alloc(TRUE);
	}
	return gpIgnoreListSingleton;
}

//1.13d: D2Common.0x6FDDD150 (#10925)
void __stdcall IGNORELIST_Destroy()
{
	delete gpIgnoreListSingleton;
	gpIgnoreListSingleton = nullptr;
}
