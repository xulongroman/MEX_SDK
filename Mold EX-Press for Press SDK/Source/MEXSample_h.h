

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Fri Oct 19 10:20:11 2018
 */
/* Compiler settings for .\MEXSample.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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


#ifndef __MEXSample_h_h__
#define __MEXSample_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMEXSample_FWD_DEFINED__
#define __IMEXSample_FWD_DEFINED__
typedef interface IMEXSample IMEXSample;
#endif 	/* __IMEXSample_FWD_DEFINED__ */


#ifndef __MEXSample_FWD_DEFINED__
#define __MEXSample_FWD_DEFINED__

#ifdef __cplusplus
typedef class MEXSample MEXSample;
#else
typedef struct MEXSample MEXSample;
#endif /* __cplusplus */

#endif 	/* __MEXSample_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __MEXSample_LIBRARY_DEFINED__
#define __MEXSample_LIBRARY_DEFINED__

/* library MEXSample */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MEXSample;

#ifndef __IMEXSample_DISPINTERFACE_DEFINED__
#define __IMEXSample_DISPINTERFACE_DEFINED__

/* dispinterface IMEXSample */
/* [uuid] */ 


EXTERN_C const IID DIID_IMEXSample;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D91792D8-9C66-4C63-9FBD-B888B8652F52")
    IMEXSample : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMEXSampleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMEXSample * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMEXSample * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMEXSample * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMEXSample * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMEXSample * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMEXSample * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMEXSample * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMEXSampleVtbl;

    interface IMEXSample
    {
        CONST_VTBL struct IMEXSampleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMEXSample_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMEXSample_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMEXSample_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMEXSample_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMEXSample_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMEXSample_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMEXSample_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMEXSample_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_MEXSample;

#ifdef __cplusplus

class DECLSPEC_UUID("46A67CF8-8865-4DF4-9394-4CDC8BA24EF7")
MEXSample;
#endif
#endif /* __MEXSample_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


