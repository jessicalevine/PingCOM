// PingCli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int main() {
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	IPingable * ptrPingable;
	CoCreateInstance(CLSID_CoPingEngine, NULL, CLSCTX_INPROC, IID_IPingable, reinterpret_cast<void**>(&ptrPingable));
	ptrPingable->Initialize();

	int statusCode;
	ptrPingable->Ping(12345, &statusCode);
	std::cout << "Status code for ping code 12345: " << statusCode;
	ptrPingable->Ping(777, &statusCode);
	std::cout << "Status code for ping code 777: " << statusCode;
	ptrPingable->Ping(1800, &statusCode);
	std::cout << "Status code for ping code 1800: " << statusCode;

	ptrPingable->Release();
	CoUninitialize();
    return 0;
}