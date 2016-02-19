#include "stdafx.h"
#include "CoPingEngine.h"
#include "PingServer.h"

CoPingEngine::CoPingEngine() : m_lRefCount(0) { ComponentAddRef(); }
CoPingEngine::~CoPingEngine() { ComponentRelease(); }

HRESULT CoPingEngine::CreateObject(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv) {
	*ppv = NULL;

	if (pUnkOuter != NULL) {
		return CLASS_E_NOAGGREGATION;
	}

	CoPingEngine * pPingEngine = new CoPingEngine();
	if (pPingEngine == NULL) {
		return E_OUTOFMEMORY;
	}

	HRESULT hr = pPingEngine->QueryInterface(riid, ppv);
	if (FAILED(hr)) {
		delete pPingEngine;
	}

	return hr;
}

STDMETHODIMP CoPingEngine::QueryInterface(REFIID riid, void **ppv) {
	if (ppv == NULL) {
		return E_INVALIDARG;
	}

	if (riid == IID_IUnknown || riid == IID_IPingable) {
		*ppv = static_cast<IPingable *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(ppv)->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CoPingEngine::AddRef(void) { return InterlockedIncrement(&m_lRefCount); }

STDMETHODIMP_(ULONG) CoPingEngine::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	if (refCount == 0) {
		delete this;
	}

	return refCount;
}


STDMETHODIMP CoPingEngine::Initialize() {
	printf("CoPingEngine initialized");
	return S_OK;
}

STDMETHODIMP CoPingEngine::Ping(int pingCode, int * statusCode) {
	if (pingCode == 12345) {
		*statusCode = 200;
	}
	else {
		*statusCode = 401;
	}

	return S_OK;
}
