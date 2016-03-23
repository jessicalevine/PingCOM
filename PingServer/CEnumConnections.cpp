#pragma once
#include "stdafx.h"

#include "CEnumConnections.h"
#include "CConnectionPoint.h"

// Because we only support one connection, this enumerator is a facade over that connection

CEnumConnections::CEnumConnections(IPongable *pPongable) : m_lRefCount(0), finished(false) {
	// pPongable may be NULL, in which case CEnumConnections will behave like a 0-length enumerator
	m_pPongable = pPongable;
}

STDMETHODIMP CEnumConnections::QueryInterface(REFIID riid, void **ppv) {
	if (ppv == NULL) {
		return E_INVALIDARG;
	}

	if (riid == IID_IEnumConnections) {
		*ppv = static_cast<IEnumConnections *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	this->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CEnumConnections::AddRef(void) { return InterlockedIncrement(&m_lRefCount); }

STDMETHODIMP_(ULONG) CEnumConnections::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	if (refCount == 0) {
		delete this;
	}

	return refCount;
}

STDMETHODIMP CEnumConnections::Next(ULONG cConnections, CONNECTDATA *rgcd, ULONG *lpcFetched) {
	if (m_pPongable == NULL || rgcd == NULL || cConnections > 1 || finished) {
		return S_FALSE;
	}

	CONNECTDATA data = { NULL, COOKIE };
	m_pPongable->QueryInterface(IID_IUnknown, (void **) &data.pUnk);
	*rgcd = data;

	if (lpcFetched != NULL) {
		*lpcFetched = 1;
	}

	finished = true;

	return S_OK;
}

STDMETHODIMP CEnumConnections::Skip(ULONG cConnections) {
	return S_FALSE; // There's not enough connection points to skip any!
}

STDMETHODIMP CEnumConnections::Reset() {
	finished = false;
	return S_OK;
}

STDMETHODIMP CEnumConnections::Clone(IEnumConnections **ppEnum) {
	if (ppEnum == NULL) {
		return E_INVALIDARG;
	}

	CEnumConnections *pNew = new CEnumConnections(m_pPongable);
	if (pNew == NULL) {
		return E_OUTOFMEMORY;
	}

	pNew->AddRef();
	pNew->finished = finished;
	*ppEnum = pNew;
	return S_OK;
}