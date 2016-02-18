#pragma once
#include "stdafx.h"

class CoPingEngineFactory : public IClassFactory {
public:
	CoPingEngineFactory();
	~CoPingEngineFactory();
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void **ppv);
	STDMETHODIMP LockServer(BOOL fLock);
};