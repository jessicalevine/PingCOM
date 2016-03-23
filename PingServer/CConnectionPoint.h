#pragma once
#include "stdafx.h"
#include "olectl.h"
#include "PingServer.h"

#define COOKIE 1 

class CConnectionPoint : public IConnectionPoint {
public:
	CConnectionPoint(IConnectionPointContainer *pContainer);

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP GetConnectionInterface(IID *piid);
	STDMETHODIMP GetConnectionPointContainer(IConnectionPointContainer **ppContainer);
	STDMETHODIMP Advise(IUnknown *pSink, DWORD *pCookie);
	STDMETHODIMP Unadvise(DWORD cookie);
	STDMETHODIMP EnumConnections(IEnumConnections **ppEnum);
private:
	LONG m_lRefCount;
	IConnectionPointContainer *m_pContainer;
	IPongable *m_pPongable;
};