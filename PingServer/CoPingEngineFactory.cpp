#include "stdafx.h"

class CoPingEngineFactory : IClassFactory {
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {}
	STDMETHODIMP_(ULONG) AddRef(void) {}
	STDMETHODIMP_(ULONG) Release(void) {}

	STDMETHODIMP CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void **ppv) {}
};