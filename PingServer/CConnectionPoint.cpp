#include "stdafx.h"
#include "CConnectionPoint.h"
#include "CEnumConnections.h"

CConnectionPoint::CConnectionPoint(IConnectionPointContainer *pContainer) : m_lRefCount(0) {
	m_pContainer = pContainer;
}

CConnectionPoint::~CConnectionPoint() {
	m_pContainer->Release();
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

STDMETHODIMP_(ULONG) CConnectionPoint::AddRef(void) { return InterlockedIncrement(&m_lRefCount); }

STDMETHODIMP_(ULONG) CConnectionPoint::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	if (refCount == 0) {
		delete this;
	}

	return refCount;
}

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

		return S_OK;
	}
	else {
		return E_POINTER;
	}
}

STDMETHODIMP CConnectionPoint::EnumConnections(IEnumConnections ** ppEnum) {
	return E_NOTIMPL;

	if (ppEnum == NULL) {
		return E_POINTER;
	}

	CEnumConnections *pEnum = new CEnumConnections(m_pPongable);
	if (pEnum == NULL) {
		return E_OUTOFMEMORY;
	}

	*ppEnum = pEnum;

	return S_OK;
}