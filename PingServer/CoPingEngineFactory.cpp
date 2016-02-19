#include "stdafx.h"
#include "CoPingEngineFactory.h"
#include "PingServer.h"
#include "CoPingEngine.h"

CoPingEngineFactory::CoPingEngineFactory() { ComponentAddRef(); }
CoPingEngineFactory::~CoPingEngineFactory() { ComponentRelease(); }

STDMETHODIMP CoPingEngineFactory::QueryInterface(REFIID riid, void **ppv) {
	if (ppv == NULL) {
		return E_INVALIDARG;
	}

	if (riid == IID_IUnknown || riid == IID_IClassFactory) {
		*ppv = static_cast<IClassFactory *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return S_OK;
}

// CoPingEngineFactory is a singleton whose lifetime is managed in DllMain
STDMETHODIMP_(ULONG) CoPingEngineFactory::AddRef(void) { return 1; }
STDMETHODIMP_(ULONG) CoPingEngineFactory::Release(void) { return 1; }

STDMETHODIMP CoPingEngineFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void **ppv) {
	return CoPingEngine::CreateObject(pUnkOuter, riid, ppv);
}

STDMETHODIMP CoPingEngineFactory::LockServer(BOOL fLock) {
	if (fLock) {
		ComponentAddRef();
	}
	else {
		ComponentRelease();
	}

	return S_OK;
}