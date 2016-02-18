#include "stdafx.h"
#include "CoPingEngineFactory.h"

STDMETHODIMP CoPingEngineFactory::QueryInterface(REFIID riid, void **ppv) { return E_NOTIMPL;  }

STDMETHODIMP_(ULONG) CoPingEngineFactory::AddRef(void) { return 0; }
STDMETHODIMP_(ULONG) CoPingEngineFactory::Release(void) { return 0; }

STDMETHODIMP CoPingEngineFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void **ppv) { return E_NOTIMPL; }
STDMETHODIMP CoPingEngineFactory::LockServer(BOOL fLock) { return E_NOTIMPL;  }