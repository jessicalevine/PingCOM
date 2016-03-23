#pragma once
#include "stdafx.h"
#include "CoPingEngineFactory.h"

ULONG ComponentAddRef();
ULONG ComponentRelease();
void DisplayStatus(wchar_t *pwszMsg, HRESULT hr);

extern wchar_t g_wszModuleName[MAX_PATH];
extern CoPingEngineFactory * g_pPingEngineFactory;
