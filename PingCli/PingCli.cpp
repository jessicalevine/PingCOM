// PingCli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "CSink.h"

void DisplayStatus(wchar_t *pwszMsg, HRESULT hr)
{

	if (hr == S_OK) {
		wprintf(TEXT("%s\n"), pwszMsg);
		return;
	}

	if (HRESULT_FACILITY(hr) == FACILITY_WINDOWS) {
		hr = HRESULT_CODE(hr);
	}

	wchar_t *pwszStatus;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&pwszStatus,
		0,
		NULL);

	wprintf(TEXT("%s: %s (ECode: %lx)\n"), pwszMsg, pwszStatus, hr);

	LocalFree(pwszStatus);
}

void Ping(IPingable *ptrPingable, SHORT pingCode) {
	SHORT statusCode;
	wprintf(L"Sending ping with code %d\n", pingCode);
	ptrPingable->Ping(pingCode, &statusCode);
	wprintf(L"Received status reponse %d from pinging %d\n\n", statusCode, pingCode);
}

int main() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr)) {
		IPingable * ptrPingable;

		IClassFactory *ptrFactory = NULL;
		HRESULT hr = CoGetClassObject(
			CLSID_CoPingEngine,
			CLSCTX_SERVER,
			NULL, IID_IClassFactory,
			reinterpret_cast<void**>(&ptrFactory)
		);

		if (SUCCEEDED(hr)) {
			hr = ptrFactory->CreateInstance(NULL, IID_IPingable, reinterpret_cast<void**>(&ptrPingable));
			ptrFactory->Release();

			if (SUCCEEDED(hr)) {
				HRESULT hr = ptrPingable->Initialize();
				if (FAILED(hr)) {
					DisplayStatus(TEXT("IID_IPingable->Initialize() failed: "), hr);
				}

				IConnectionPointContainer* pContainer;
				hr = ptrPingable->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);

				if (SUCCEEDED(hr)) {
					IConnectionPoint* pPoint;
					hr = pContainer->FindConnectionPoint(IID_IPongable, &pPoint);
					if (SUCCEEDED(hr)) {

						// Instantiate the sink object.
						wprintf(L"Instantiating sink\n");
						CSink *sink = new CSink;

						// Give the connectable object a pointer to the sink.
						DWORD dwCookie;
						wprintf(L"Advising sink\n");
						pPoint->Advise((IUnknown*)sink, &dwCookie);


						Ping(ptrPingable, 12345);
						Ping(ptrPingable, 777);
						Ping(ptrPingable, 1800);

						wprintf(L"Unadvisising sink\n");
						pPoint->Unadvise(dwCookie);

						pPoint->Release();
					}
					else {
						DisplayStatus(TEXT("IConnectionPointContainer(FindConnectionPoint) failed: "), hr);
					}

					pContainer->Release();
				}
				else {
					DisplayStatus(TEXT("QueryInterface(IID_IConnectionPointContainer) failed: "), hr);
				}


				ptrPingable->Release();
			}
			else {
				DisplayStatus(TEXT("CreateInstance(IID_IPingable) failed: "), hr);
			}
		}
		else {
			DisplayStatus(TEXT("CoGetClassObject(CoPingEngine) failed: "), hr);
		}

		CoUninitialize();
	}
	else {
		DisplayStatus(TEXT("CoInitializeEx failed: "), hr);
	}


	return 0;
}

