#pragma once
#include "stdafx.h"
#include "CConnectionPoint.h"

class CoPingEngine : public IPingable, public IConnectionPointContainer {
public:
	CoPingEngine();
	~CoPingEngine();
	static HRESULT CreateObject(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv);

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP FindConnectionPoint(REFIID riid, IConnectionPoint **pConnectionPoint);
	STDMETHODIMP EnumConnectionPoints(IEnumConnectionPoints **ppEnum);

	STDMETHODIMP Initialize();
	STDMETHODIMP Ping(SHORT pingCode, SHORT * statusCode);
private:
	LONG m_lRefCount;
	CConnectionPoint *m_pPoint;
};