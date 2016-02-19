#pragma once
#include "stdafx.h"
#include "CoPingEngineFactory.h"

ULONG ComponentAddRef();
ULONG ComponentRelease();

extern CoPingEngineFactory * g_pPingEngineFactory;