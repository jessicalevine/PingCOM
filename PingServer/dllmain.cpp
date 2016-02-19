// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "CoPingEngineFactory.h"
#include "PingServer.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		g_pPingEngineFactory = new CoPingEngineFactory();

		GetModuleFileName(hModule, g_wszModuleName, MAX_PATH); // Needed for registration
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		delete g_pPingEngineFactory;
	}

	return TRUE;
}
