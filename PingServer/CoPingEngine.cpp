#include "stdafx.h"

class CoPingEngine : IPingable {
public:
	static HRESULT CreateObject(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv) { }

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {}
	STDMETHODIMP_(ULONG) AddRef(void) {}
	STDMETHODIMP_(ULONG) Release(void) {}


	STDMETHODIMP Initialize() { }
	STDMETHODIMP Ping(int pingCode, int * statusCode) { }
};