#include "stdafx.h"
#include "CoPingEngine.h"
#include "PingServer.h"
#include "CEnumConnectionPoints.h"

CoPingEngine::CoPingEngine() : m_lRefCount(0) {
	ComponentAddRef();

	IConnectionPointContainer *pContainer;
	QueryInterface(IID_IConnectionPointContainer, (void **)&pContainer);
	m_pPoint = new CConnectionPoint(pContainer);
}

CoPingEngine::~CoPingEngine() {
	ComponentRelease(); 
}

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
	else if (riid == IID_IConnectionPointContainer) {
		*ppv = static_cast<IConnectionPointContainer *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CoPingEngine::AddRef(void) {
	long refCount = InterlockedIncrement(&m_lRefCount);
	//wprintf(L"CoPingEngine::AddRef(%ld)\n", refCount);
	return refCount;
}

STDMETHODIMP_(ULONG) CoPingEngine::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	//wprintf(L"CoPingEngine::Release(%ld)\n", refCount);
	if (refCount == 0) {
		delete this;
	}

	return refCount;
}


STDMETHODIMP CoPingEngine::Initialize() {
	printf("CoPingEngine initialized\n");
	return S_OK;
}

STDMETHODIMP CoPingEngine::Ping(SHORT pingCode, SHORT * statusCode) {
	if (pingCode == 12345) {
		*statusCode = 200;
	}
	else {
		*statusCode = 401;
	}


	if (m_pPoint->m_pPongable != NULL) {
		SHORT pongCount;
		wprintf(L"Ponging with code: %hd\n", pingCode);
		m_pPoint->m_pPongable->Pong(pingCode, &pongCount);
		wprintf(L"Pong count: %hd\n", pongCount);
	}

	return S_OK;
}

STDMETHODIMP CoPingEngine::FindConnectionPoint(REFIID riid, IConnectionPoint **pConnectionPoint) {
	if (pConnectionPoint == NULL) {
		return E_POINTER;
	}

	if (riid == IID_IPongable) {
		HRESULT hr = m_pPoint->QueryInterface(IID_IConnectionPoint, (void **)pConnectionPoint);

		if (SUCCEEDED(hr)) {
			return S_OK;
		}
		else {
			*pConnectionPoint = NULL;
			return E_UNEXPECTED;
		}
	}

	*pConnectionPoint = NULL;
	return CONNECT_E_NOCONNECTION;
}

STDMETHODIMP CoPingEngine::EnumConnectionPoints(IEnumConnectionPoints **ppEnum) {
	if (ppEnum == NULL) {
		return E_POINTER;
	}

	IConnectionPoint *pInterfacePoint;
	m_pPoint->QueryInterface(IID_IConnectionPoint, (void **)&pInterfacePoint);

	CEnumConnectionPoints *pEnum = new CEnumConnectionPoints(pInterfacePoint);
	if (pEnum == NULL) {
		return E_OUTOFMEMORY;
	}

	*ppEnum = pEnum;

	return S_OK;
}