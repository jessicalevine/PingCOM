#include "stdafx.h"
#include "CSink.h"

CSink::CSink() : m_lRefCount(0), m_siPongCount(0) { }

STDMETHODIMP CSink::QueryInterface(REFIID riid, void **ppv) {
	if (ppv == NULL) {
		return E_INVALIDARG;
	}

	if (riid == IID_IUnknown || riid == IID_IPongable) {
		*ppv = static_cast<IPongable *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	this->AddRef();

	return S_OK;
}

STDMETHODIMP_(ULONG) CSink::AddRef(void) { return InterlockedIncrement(&m_lRefCount); }

STDMETHODIMP_(ULONG) CSink::Release(void) {
	long refCount = InterlockedDecrement(&m_lRefCount);

	if (refCount == 0) {
		delete this;
	}

	return refCount;
}

STDMETHODIMP CSink::ResetPongCount() {
	m_siPongCount = 0;
	return S_OK;
}

STDMETHODIMP CSink::Pong(SHORT receivedCode, SHORT *pongCount) {
	wprintf(TEXT("Received pong with code %d\n"), receivedCode);
	*pongCount = ++m_siPongCount;
	return S_OK;
}