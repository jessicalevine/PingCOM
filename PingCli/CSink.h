#pragma once
#include "stdafx.h"
class CSink : public IPongable {
public:
	CSink();

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP ResetPongCount();
	STDMETHODIMP Pong(SHORT receivedCode, SHORT *pongCount);
private:
	LONG m_lRefCount;
	short m_siPongCount;
};