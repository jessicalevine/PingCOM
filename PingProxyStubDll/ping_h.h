

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Thu Feb 18 15:19:44 2016
 */
/* Compiler settings for ping.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ping_h_h__
#define __ping_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPingable_FWD_DEFINED__
#define __IPingable_FWD_DEFINED__
typedef interface IPingable IPingable;

#endif 	/* __IPingable_FWD_DEFINED__ */


#ifndef __CoPingEngine_FWD_DEFINED__
#define __CoPingEngine_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoPingEngine CoPingEngine;
#else
typedef struct CoPingEngine CoPingEngine;
#endif /* __cplusplus */

#endif 	/* __CoPingEngine_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IPingable_INTERFACE_DEFINED__
#define __IPingable_INTERFACE_DEFINED__

/* interface IPingable */
/* [uuid][object] */ 


EXTERN_C const IID IID_IPingable;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C3C38ECD-6377-4560-9D48-D9E493728F77")
    IPingable : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Ping( 
            /* [in] */ int pingCode,
            /* [out] */ int *statusCode) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IPingableVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPingable * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPingable * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPingable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IPingable * This);
        
        HRESULT ( STDMETHODCALLTYPE *Ping )( 
            IPingable * This,
            /* [in] */ int pingCode,
            /* [out] */ int *statusCode);
        
        END_INTERFACE
    } IPingableVtbl;

    interface IPingable
    {
        CONST_VTBL struct IPingableVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPingable_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPingable_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPingable_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPingable_Initialize(This)	\
    ( (This)->lpVtbl -> Initialize(This) ) 

#define IPingable_Ping(This,pingCode,statusCode)	\
    ( (This)->lpVtbl -> Ping(This,pingCode,statusCode) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IPingable_INTERFACE_DEFINED__ */



#ifndef __PingLib_LIBRARY_DEFINED__
#define __PingLib_LIBRARY_DEFINED__

/* library PingLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_PingLib;

EXTERN_C const CLSID CLSID_CoPingEngine;

#ifdef __cplusplus

class DECLSPEC_UUID("5819432D-806B-42C1-97DE-DE5A015A8497")
CoPingEngine;
#endif
#endif /* __PingLib_LIBRARY_DEFINED__ */

/* interface __MIDL_itf_ping_0000_0002 */
/* [local] */ 

DEFINE_GUID(PingLib, 0x5819432d, 0x806b, 0x42c1, 0x97, 0xde, 0xde, 0x5a, 0x01, 0x5a, 0x84, 0x97);
DEFINE_GUID(IID_IPingable, 0xc3c38ecd, 0x6377, 0x4560, 0x9d, 0x48, 0xd9, 0xe4, 0x93, 0x72, 0x8f, 0x77);
DEFINE_GUID(CLSID_CoPingEngine, 0x5819432d, 0x806b, 0x42c1, 0x97, 0xde, 0xde, 0x5a, 0x01, 0x5a, 0x84, 0x97);


extern RPC_IF_HANDLE __MIDL_itf_ping_0000_0002_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ping_0000_0002_v0_0_s_ifspec;

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


