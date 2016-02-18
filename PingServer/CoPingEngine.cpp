#include "stdafx.h"
#include "CoPingEngine.h"

CoPingEngine::CoPingEngine() { }
CoPingEngine::~CoPingEngine() { }

HRESULT CoPingEngine::CreateObject(LPUNKNOWN pUnkOuter, REFIID riid, void** ppv) { return E_NOTIMPL; }

STDMETHODIMP CoPingEngine::QueryInterface(REFIID riid, void **ppv) { return E_NOTIMPL; }
STDMETHODIMP_(ULONG) CoPingEngine::AddRef(void) { return 0; }
STDMETHODIMP_(ULONG) CoPingEngine::Release(void) { return 0;  }


STDMETHODIMP CoPingEngine::Initialize() { return E_NOTIMPL; }
STDMETHODIMP CoPingEngine::Ping(int pingCode, int * statusCode) { return E_NOTIMPL; }
