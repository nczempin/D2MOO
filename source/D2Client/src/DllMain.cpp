#include <Windows.h>

extern HINSTANCE ghD2ClientInstance_6FBCBCC0;

// NOLINTBEGIN(bugprone-branch-clone)
BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, void* lpReserved)
{
	(void)lpReserved;
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		ghD2ClientInstance_6FBCBCC0 = hModule;
	}
	return TRUE;
}
// NOLINTEND(bugprone-branch-clone)
