#include "stdafx.h"
#include "CConnectionPoint.h"

CConnectionPoint::CConnectionPoint(IConnectionPointContainer *pContainer) : m_lRefCount(0) {
	m_pContainer = pContainer; // Weak reference, does not increment reference count
}

CConnectionPoint::~CConnectionPoint() {
	if (m_pPongable != NULL) {
		m_pPongable->Release();
		m_pPongable = NULL;
	}
}

STDMETHODIMP CConnectionPoint::QueryInterface(REFIID riid, void **ppv) {
	if (ppv == NULL) {
		return E_INVALIDARG;
	}

	if (riid == IID_IUnknown || riid == IID_IConnectionPoint) {
		*ppv = static_cast<IConnectionPoint *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	this->AddRef();

	return S_OK;
}

// The lifetime of the connection point is manually managed by the container
STDMETHODIMP_(ULONG) CConnectionPoint::AddRef(void) { return 1; }
STDMETHODIMP_(ULONG) CConnectionPoint::Release(void) { return 1; }

STDMETHODIMP CConnectionPoint::GetConnectionInterface(IID *piid) {
	if (piid == NULL) {
		return E_POINTER;
	}

	*piid = IID_IPongable;
	return S_OK;
}
STDMETHODIMP CConnectionPoint::GetConnectionPointContainer(IConnectionPointContainer **ppContainer) {
	return m_pContainer->QueryInterface(IID_IConnectionPointContainer, (void **)ppContainer);
}

STDMETHODIMP CConnectionPoint::Advise(IUnknown *pSink, DWORD *pCookie) {
	wprintf(L"Attempting to advise sink...\n");

	if (pSink == NULL || pCookie == NULL) {
		return E_POINTER;
	}

	if (m_pPongable == NULL) {
		LPOLESTR striid;
		StringFromIID(IID_IPongable, &striid);
		HRESULT hr = pSink->QueryInterface(IID_IPongable, (void **)&m_pPongable);

		if (FAILED(hr)) {
			*pCookie = 0;
			DisplayStatus(L"Cannot connect (QueryInterface failed)", hr);
			return CONNECT_E_CANNOTCONNECT;
		}

		wprintf(L"Advised sink...\n");

		// We only support one sink at a time, no need to generate a random unique ID
		*pCookie = COOKIE;

		return S_OK;
	}

	*pCookie = 0;
	return CONNECT_E_ADVISELIMIT;
}

STDMETHODIMP CConnectionPoint::Unadvise(DWORD cookie) {
	if (cookie == COOKIE) {
		m_pPongable->Release();
		m_pPongable = NULL;

		wprintf(L"Unadvised sink\n");

		return S_OK;
	}
	else {
		return E_POINTER;
	}
}

STDMETHODIMP CConnectionPoint::EnumConnections(IEnumConnections ** ppEnum) {
	return E_NOTIMPL;
}