#include "stdafx.h"
#include "CoPingEngine.h"
#include "PingServer.h"

CoPingEngine::CoPingEngine() : m_lRefCount(0) { ComponentAddRef(); }
CoPingEngine::~CoPingEngine() { ComponentRelease(); }

HRESULT CoPingEngine::CreateObject(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv) { return E_NOTIMPL; }

STDMETHODIMP CoPingEngine::QueryInterface(REFIID riid, void **ppv) { return E_NOTIMPL; }

STDMETHODIMP_(ULONG) CoPingEngine::AddRef(void) { return InterlockedIncrement(&m_lRefCount); }

STDMETHODIMP_(ULONG) CoPingEngine::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	if (refCount == 0) {
		delete this;
	}

	return refCount;
}


STDMETHODIMP CoPingEngine::Initialize() { return E_NOTIMPL; }
STDMETHODIMP CoPingEngine::Ping(int pingCode, int * statusCode) { return E_NOTIMPL; }
