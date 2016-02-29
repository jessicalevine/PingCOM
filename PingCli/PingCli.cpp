// PingCli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

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
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&pwszStatus,
		0,
		NULL);

	wprintf(TEXT("%s: %s (ECode: %lx)\n"), pwszMsg, pwszStatus, hr);

	LocalFree(pwszStatus);
}

int main() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		IPingable * ptrPingable;
		HRESULT hr = CoCreateInstance(CLSID_CoPingEngine, NULL, CLSCTX_INPROC_SERVER, IID_IPingable, reinterpret_cast<void**>(&ptrPingable));
		if (SUCCEEDED(hr)) {
			ptrPingable->Initialize();

			SHORT statusCode;
			ptrPingable->Ping(12345, &statusCode);
			std::cout << "Status code for ping code 12345: " << statusCode << std::endl;
			ptrPingable->Ping(777, &statusCode);
			std::cout << "Status code for ping code 777: " << statusCode << std::endl;
			ptrPingable->Ping(1800, &statusCode);
			std::cout << "Status code for ping code 1800: " << statusCode << std::endl;

			ptrPingable->Release();
		}
		else {
			DisplayStatus(TEXT("CoCreateInstance(CoPingEngine) failed: "), hr);
		}
		CoUninitialize();
	}
	else {
		DisplayStatus(TEXT("CoInitializeEx failed: "), hr);
	}


	return 0;
}