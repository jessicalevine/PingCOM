// PingServer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PingServer.h"
#include "CoPingEngineFactory.h"

wchar_t g_wszModuleName[MAX_PATH];
LONG g_lComponentRefCounts = 0;
CoPingEngineFactory * g_pPingEngineFactory;

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
	return (InterlockedIncrement(&g_lComponentRefCounts));
}

ULONG ComponentRelease() {
	return (InterlockedDecrement(&g_lComponentRefCounts));
}

inline long ByteLen(wchar_t *pwsz)
{
	return (sizeof(wchar_t) * (wcslen(pwsz) + 1));
}


STDAPI DllCanUnloadNow(void) {
	if (g_lComponentRefCounts == 0) {
		return S_OK;
	}
	else {
		return S_FALSE;
	}
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
	if (rclsid != CLSID_CoPingEngine) {
		return CLASS_E_CLASSNOTAVAILABLE; // We only support CoPingEngine
	}

	return g_pPingEngineFactory->QueryInterface(riid, ppv);
}

STDAPI DllRegisterServer(void) {
	wchar_t wszKey[MAX_PATH];
	wchar_t wszValue[MAX_PATH];
	HKEY hKey = 0;

	// CLSID_CoPingEngine = {5819432D-806B-42C1-97DE-DE5A015A8497}
	// HKEY_CLASSES_ROOT\CLSID\{5819432D-806B-42C1-97DE-DE5A015A8497}
	//      @="CoPingEngine"
	wcscpy(wszKey, TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, TEXT("CoPingEngine"));
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)TEXT("CoPingEngine"), ByteLen(TEXT("CoPingEngine")));
	RegCloseKey(hKey);

	// HKEY_CLASSES_ROOT\CLSID\{5819432D-806B-42C1-97DE-DE5A015A8497}\InprocServer32
	//      @= value of g_wszModuleName, set during DllMain
	//      "ThreadingModel" = "Both"
	wcscpy(wszKey, TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}") TEXT("\\InprocServer32"));
	RegCreateKey(HKEY_CLASSES_ROOT, wszKey, &hKey);
	wcscpy(wszValue, g_wszModuleName);
	RegSetValueEx(hKey, 0, 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	wcscpy(wszValue, TEXT("Both"));
	RegSetValueEx(hKey, TEXT("ThreadingModel"), 0, REG_SZ, (BYTE*)wszValue, ByteLen(wszValue));
	RegCloseKey(hKey);

	return S_OK;
}

STDAPI DllUnregisterServer(void) {
	long lCode;

	lCode = RegDeleteKey(HKEY_CLASSES_ROOT,
		TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}") TEXT("\\InprocServer32"));
	lCode = RegDeleteKey(HKEY_CLASSES_ROOT,
		TEXT("CLSID\\{5819432D-806B-42C1-97DE-DE5A015A8497}"));

	return S_OK;
}