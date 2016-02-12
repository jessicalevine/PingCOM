// PingCli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

int main() {
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	IPingable * ptrPingable;
	CoCreateInstance(CLSID_CoPingEngine, NULL, CLSCTX_INPROC, IID_IPingable, reinterpret_cast<void**>(&ptrPingable));
	ptrPingable->Initialize();

	int * statusCode = NULL;
	ptrPingable->Ping(200, statusCode);
	std::cout << "Status code: " << *statusCode;

	ptrPingable->Release();
	CoUninitialize();
    return 0;
}