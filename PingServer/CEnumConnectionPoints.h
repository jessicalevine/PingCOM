#pragma once
#include "stdafx.h"

class CEnumConnectionPoints : public IEnumConnectionPoints {
public:
	CEnumConnectionPoints(IConnectionPoint *pPoint);
	~CEnumConnectionPoints();

	STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);

	STDMETHODIMP Next(ULONG cConnections, IConnectionPoint **rgpcn, ULONG *lpcFetched);
	STDMETHODIMP Skip(ULONG cConnections);
	STDMETHODIMP Reset();
	STDMETHODIMP Clone(IEnumConnectionPoints **ppEnum);
	bool finished;
private:
	LONG m_lRefCount;
	IConnectionPoint *m_pPoint;
};