#pragma once
#include "stdafx.h"
#include "CoPingEngineFactory.h"

ULONG ComponentAddRef();
ULONG ComponentRelease();

extern wchar_t g_wszModuleName[MAX_PATH];
extern CoPingEngineFactory * g_pPingEngineFactory;