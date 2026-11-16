#include <Windows.h>

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, void* lpReserved)
{
	(void)hModule;
	(void)dwReason;
	(void)lpReserved;
	return TRUE;
}
