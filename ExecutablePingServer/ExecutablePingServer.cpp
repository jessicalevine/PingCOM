// PingServer.cpp : Defines the exported functions for the DLL application.
//

#define UNICODE

#include <assert.h>
#include "PingServer.h"
#include "CoPingEngineFactory.h"

wchar_t g_wszModuleName[MAX_PATH];
LONG g_lComponentRefCounts = 0;

CoPingEngineFactory g_PingEngineFactory;
HANDLE g_hExitEvent;


ULONG ComponentAddRef() {
	ULONG ul = CoAddRefServerProcess();
	wprintf(TEXT("ComponentAddRef(%ld)\n"), ul);
	return ul;
}

ULONG ComponentRelease() {
	ULONG ul = CoReleaseServerProcess();

	wprintf(TEXT("ComponentRelease(%ld)\n"), ul);
	if (ul == 0) {
		SetEvent(g_hExitEvent);
	}
	return ul;
}

inline long ByteLen(wchar_t *pwsz)
{
	return (sizeof(wchar_t) * (wcslen(pwsz) + 1));
}

//-------------------------------------------------------------------
void RegisterComponent()
{
	wchar_t wszKey[MAX_PATH];
	wchar_t wszValue[MAX_PATH];
	HKEY hKey = 0;

	// HKEY_CLASSES_ROOT\CLSID\{5819432D-806B-42C1-97DE-DE5A015A8497}
	//  @="CoPingEngine"
	wcscpy(wszKey, TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("CoPingEngine"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));

	//  "AppID"="{C6D89C1D-8049-4857-8447-631F433A367D}"
	wcscpy(wszValue, TEXT("{C6D89C1D-8049-4857-8447-631F433A367D}"));
	RegSetValueEx(hKey, TEXT("AppID"), 0, REG_SZ,
		(BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\CLSID\{5819432D-806B-42C1-97DE-DE5A015A8497}\LocalServer32
	//      @="...path...\ExecutablePingServer.exe"
	wcscpy(wszKey, TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}\\")
		TEXT("LocalServer32"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	GetModuleFileName(0, wszValue, MAX_PATH);
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\AppID\ExecutablePingServer.exe
	//      "AppID"="{C6D89C1D-8049-4857-8447-631F433A367D}"
	wcscpy(wszKey, TEXT("AppID\\ExecutablePingServer.exe"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("{C6D89C1D-8049-4857-8447-631F433A367D}"));
	RegSetValueEx(hKey, TEXT("AppID"), 0, REG_SZ,
		(BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\AppID\{C6D89C1D-8049-4857-8447-631F433A367D}
	//      @="CoPingEngine"
	wcscpy(wszKey, TEXT("AppID\\{C6D89C1D-8049-4857-8447-631F433A367D}"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("CoPingEngine"));
	RegSetValueEx(hKey, 0, 0, REG_SZ,
		(BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);
}

//-------------------------------------------------------------------
// Automatic unregistration; for out-of-process server support the
// -unregserver option
//-------------------------------------------------------------------
void UnregisterComponent()
{
	long lRc = 0;
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\")
		TEXT("{5819432D-806B-42C1-97DE-DE5A015A8497}\\")
		TEXT("LocalServer32"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\")
		TEXT("{5819432D-806B-42C1-97DE-DE5A015A8497}"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("AppID\\")
		TEXT("{C6D89C1D-8049-4857-8447-631F433A367D}"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("AppID\\ExecutablePingServer.exe"));
}

void main(int argc, char *argv[], char *envp[])
{
	g_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(g_hExitEvent);

	if (argc > 1) {
		if (_stricmp(argv[1], "-RegServer") == 0) {
			RegisterComponent();
			wprintf(TEXT("Register\n"));
			return;
		}
		if (_stricmp(argv[1], "-UnRegServer") == 0) {
			UnregisterComponent();
			wprintf(TEXT("Unregistered\n"));
			return;
		}
	}

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	DWORD dwRegister;
	hr = CoRegisterClassObject(CLSID_CoPingEngine,
		&g_PingEngineFactory,
		CLSCTX_SERVER,
		REGCLS_MULTIPLEUSE,
		&dwRegister);
	assert(SUCCEEDED(hr));
	g_PingEngineFactory.Release();

	WaitForSingleObject(g_hExitEvent, INFINITE);

	CoRevokeClassObject(dwRegister);

	CoUninitialize();

	Sleep(5000);

	return;
}
