#pragma once
#include "stdafx.h"

class CEnumConnections : public IEnumConnections {
public:
	CEnumConnections(IPongable *pPongable);
	~CEnumConnections();

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP Next(ULONG cConnections, CONNECTDATA *rgcd, ULONG *lpcFetched);
	STDMETHODIMP Skip(ULONG cConnections);
	STDMETHODIMP Reset();
	STDMETHODIMP Clone(IEnumConnections **ppEnum);
	bool finished;
private:
	LONG m_lRefCount;
	IPongable *m_pPongable;
};