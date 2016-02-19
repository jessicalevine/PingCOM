#include "stdafx.h"
#include "CoPingEngineFactory.h"
#include "PingServer.h"

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

	reinterpret_cast<IUnknown *>(ppv)->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CoPingEngineFactory::AddRef(void) { return 0; }
STDMETHODIMP_(ULONG) CoPingEngineFactory::Release(void) { return 0; }

STDMETHODIMP CoPingEngineFactory::CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void **ppv) { return E_NOTIMPL; }
STDMETHODIMP CoPingEngineFactory::LockServer(BOOL fLock) { return E_NOTIMPL;  }