#include "stdafx.h"
#include "CEnumConnectionPoints.h"

// Because we only support one connection point, this enumerator is a facade over that object

CEnumConnectionPoints::CEnumConnectionPoints(IConnectionPoint *pPoint) : m_lRefCount(0), finished(false) {
	m_pPoint = pPoint;
}

CEnumConnectionPoints::~CEnumConnectionPoints() {
	m_pPoint->Release();
}

STDMETHODIMP CEnumConnectionPoints::QueryInterface(REFIID riid, void **ppv) {
	if (ppv == NULL) {
		return E_INVALIDARG;
	}

	if (riid == IID_IUnknown || riid == IID_IEnumConnectionPoints) {
		*ppv = static_cast<IEnumConnectionPoints *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	this->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CEnumConnectionPoints::AddRef(void) { return InterlockedIncrement(&m_lRefCount); }

STDMETHODIMP_(ULONG) CEnumConnectionPoints::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	if (refCount == 0) {
		delete this;
	}

	return refCount;
}

STDMETHODIMP CEnumConnectionPoints::Next(ULONG cConnections, IConnectionPoint **rgpcn, ULONG *lpcFetched) {
	if (rgpcn == NULL || cConnections > 1 || finished) {
		return S_FALSE;
	}

	*rgpcn = m_pPoint;
	m_pPoint->AddRef();

	if (lpcFetched != NULL) {
		*lpcFetched = 1;
	}

	finished = true;

	return S_OK;
}

STDMETHODIMP CEnumConnectionPoints::Skip(ULONG cConnections) {
	return S_FALSE; // There's not enough connection points to skip any!
}

STDMETHODIMP CEnumConnectionPoints::Reset() {
	finished = false;
	return S_OK;
}

STDMETHODIMP CEnumConnectionPoints::Clone(IEnumConnectionPoints **ppEnum) {
	if (ppEnum == NULL) {
		return E_INVALIDARG;
	}

	CEnumConnectionPoints *pNew = new CEnumConnectionPoints(m_pPoint);
	if (pNew == NULL) {
		return E_OUTOFMEMORY;
	}

	pNew->AddRef();
	pNew->finished = finished;
	*ppEnum = pNew;
	return S_OK;
}