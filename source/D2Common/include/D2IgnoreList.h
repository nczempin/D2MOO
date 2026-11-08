#pragma once

#include "D2CommonDefinitions.h"

class IIgnoreList
{
public:
	virtual ~IIgnoreList() = default;
	virtual void LoadFromDisk() = 0;
	virtual void ClearCharacterIgnores() = 0;
	virtual void ClearAll() = 0;
	virtual void AddCharacterIgnore(const char* szCharacterName, BOOL bPersist) = 0;
	virtual void AddAccountIgnore(const char* szAccountName, BOOL bPersist) = 0;
	virtual void AddTextFilter(const char* szPattern, BOOL bPersist) = 0;
	virtual void RemoveCharacterIgnore(const char* szCharacterName, BOOL bPersist) = 0;
	virtual void RemoveAccountIgnore(const char* szAccountName, BOOL bPersist) = 0;
	virtual void RemoveTextFilter(const char* szPattern, BOOL bPersist) = 0;
	virtual BOOL IsCharacterIgnored(const char* szCharacterName) const = 0;
	virtual BOOL IsAccountIgnored(const char* szAccountName) const = 0;
	virtual BOOL IsTextFiltered(const char* szMessage) const = 0;
	virtual BOOL IsIgnored(const char* szCharacterName, const char* szAccountName, const char* szMessage) const = 0;
	virtual BOOL GetPersistence() const = 0;
	virtual void SetPersistence(BOOL bPersist) = 0;
};

//1.13d: D2Common.0x6FDDD0E0 (#10097)
D2COMMON_DLL_DECL void __stdcall IGNORELIST_Alloc(BOOL bLoadFromDisk);
//1.13d: D2Common.0x6FDDCC40 (#10735)
D2COMMON_DLL_DECL IIgnoreList* __stdcall IGNORELIST_GetSingleton();
//1.13d: D2Common.0x6FDDD150 (#10925)
D2COMMON_DLL_DECL void __stdcall IGNORELIST_Destroy();
