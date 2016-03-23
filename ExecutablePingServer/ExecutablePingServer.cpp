// ExecutablePingServer.cpp : Defines the entry point for the executable.
//

#define UNICODE
// This macro is a hack around the fact that we don't have a real install
// process and therefore no good way to point the registry to the right
// files needed to set up COM. It would be overkill to set up an install
// process for a test project like this, so the macro will do.
#define PROJECTPATH "C:\\Users\\Administrator\\Documents\\Visual Studio 2015\\Projects\\PingCOM"

#include <assert.h>
#include "PingServer.h"
#include "CoPingEngineFactory.h"

wchar_t g_wszModuleName[MAX_PATH];
LONG g_lComponentRefCounts = 0;

CoPingEngineFactory g_PingEngineFactory;
HANDLE g_hExitEvent;

void DisplayStatus(wchar_t *pwszMsg, HRESULT hr)
{

	if (hr == S_OK) {
		wprintf(TEXT("%s\n"), pwszMsg);
		return;
	}

	if (HRESULT_FACILITY(hr) == FACILITY_WINDOWS) {
		hr = HRESULT_CODE(hr);
	}

	wchar_t *pwszStatus;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&pwszStatus,
		0,
		NULL);

	wprintf(TEXT("%s: %s (ECode: %lx)\n"), pwszMsg, pwszStatus, hr);

	LocalFree(pwszStatus);
}

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

	// TypeLib Keys

	// HKEY_CLASSES_ROOT\CLSID\{5819432D-806B-42C1-97DE-DE5A015A8497}\TypeLib
	//      @="{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}"
	wcscpy(wszKey, TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}\\")
	TEXT("TypeLib"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}"));
	RegSetValueEx(hKey, TEXT("AppID"), 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\TypeLib\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}
	wcscpy(wszKey, TEXT("TypeLib\\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\TypeLib\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\1.0
	//      @="PingLib"
	wcscpy(wszKey, TEXT("TypeLib\\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("PingLib"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\TypeLib\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\1.0\FLAGS
	//      @="0"
	wcscpy(wszKey, TEXT("TypeLib\\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\FLAGS"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("0"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\TypeLib\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\1.0\0
	wcscpy(wszKey, TEXT("TypeLib\\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\0"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	RegCloseKey(hKey);


	wcscpy(wszValue, TEXT(PROJECTPATH) TEXT("\\PingProxyStubDll\\Debug\\PingProxyStubDll.tlb"));

	// HKEY_CLASSES_ROOT\TypeLib\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\1.0\0
	wcscpy(wszKey, TEXT("TypeLib\\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\0\\win32"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\TypeLib\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\1.0\0
	wcscpy(wszKey, TEXT("TypeLib\\{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\0\\win64"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);


	// HKEY_CLASSES_ROOT\Interface\{C3C38ECD-6377-4560-9D48-D9E493728F77}
	//      @="IPingable"
	wcscpy(wszKey, TEXT("Interface\\{C3C38ECD-6377-4560-9D48-D9E493728F77}"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("IPingable"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\Interface\{C3C38ECD-6377-4560-9D48-D9E493728F77}\ProxyStubClsid32
	//      @="{00020424-0000-0000-C000-000000000046}"
	wcscpy(wszKey, TEXT("Interface\\{C3C38ECD-6377-4560-9D48-D9E493728F77}\\")
		TEXT("ProxyStubClsid32"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("{00020424-0000-0000-C000-000000000046}"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\Interface\{C3C38ECD-6377-4560-9D48-D9E493728F77}\TypeLib
	//      @="{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}"
	wcscpy(wszKey, TEXT("Interface\\{C3C38ECD-6377-4560-9D48-D9E493728F77}\\")
		TEXT("TypeLib"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
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
		TEXT("{5819432D-806B-42C1-97DE-DE5A015A8497}\\")
		TEXT("TypeLib"));

	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\0\\win32"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\0\\win64"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\0"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0\\FLAGS"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\")
		TEXT("1.0"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("TypeLib\\")
		TEXT("{6D2530EA-D5D8-46EC-A994-3AD1036BDC9B}\\"));

	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\")
		TEXT("{C3C38ECD-6377-4560-9D48-D9E493728F77}\\")
		TEXT("TypeLib"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\")
		TEXT("{C3C38ECD-6377-4560-9D48-D9E493728F77}\\")
		TEXT("ProxyStubClsid32"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("Interface\\")
		TEXT("{C3C38ECD-6377-4560-9D48-D9E493728F77}\\"));

	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("AppID\\")
		TEXT("{C6D89C1D-8049-4857-8447-631F433A367D}"));
	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("AppID\\ExecutablePingServer.exe"));

	lRc = RegDeleteKey(HKEY_CLASSES_ROOT, TEXT("CLSID\\")
		TEXT("{5819432D-806B-42C1-97DE-DE5A015A8497}"));
}

int main(int argc, char *argv[], char *envp[])
{
	g_hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(g_hExitEvent);

	if (argc > 1) {
		if (_stricmp(argv[1], "-RegServer") == 0) {
			RegisterComponent();
			wprintf(TEXT("Register\n"));
			return 0;
		}
		if (_stricmp(argv[1], "-UnRegServer") == 0) {
			UnregisterComponent();
			wprintf(TEXT("Unregistered\n"));
			return 0;
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

	Sleep(60000);

	return 0;
}
