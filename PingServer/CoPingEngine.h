#pragma once
#include "stdafx.h"

class CoPingEngine : public IPingable {
public:
	CoPingEngine();
	~CoPingEngine();
	static HRESULT CreateObject(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv);

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP Initialize();
	STDMETHODIMP Ping(int pingCode, int * statusCode);
private:
	LONG m_lRefCount;
};