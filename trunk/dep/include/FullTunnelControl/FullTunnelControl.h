/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jan 26 09:03:27 2010
 */
/* Compiler settings for C:\Documents and Settings\GTXY\桌面\企信通COM组件\企信通COM组件 C++调用COM20100125版本\FullTunnelControl\FullTunnelControl.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __FullTunnelControl_h__
#define __FullTunnelControl_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IFullTunnelServiceControl_FWD_DEFINED__
#define __IFullTunnelServiceControl_FWD_DEFINED__
typedef interface IFullTunnelServiceControl IFullTunnelServiceControl;
#endif 	/* __IFullTunnelServiceControl_FWD_DEFINED__ */


#ifndef __FullTunnelServiceControl_FWD_DEFINED__
#define __FullTunnelServiceControl_FWD_DEFINED__

#ifdef __cplusplus
typedef class FullTunnelServiceControl FullTunnelServiceControl;
#else
typedef struct FullTunnelServiceControl FullTunnelServiceControl;
#endif /* __cplusplus */

#endif 	/* __FullTunnelServiceControl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IFullTunnelServiceControl_INTERFACE_DEFINED__
#define __IFullTunnelServiceControl_INTERFACE_DEFINED__

/* interface IFullTunnelServiceControl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IFullTunnelServiceControl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4E606E35-0F59-4AE3-8F63-10A263915F93")
    IFullTunnelServiceControl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSeqID( 
            /* [out] */ VARIANT __RPC_FAR *pvSeqID,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartAndManageConnect_User( 
            BSTR username,
            BSTR password,
            BSTR ip,
            BSTR port,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StartRecv_User( 
            VARIANT __MIDL_0015,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendBusinessPkg( 
            BSTR control_type,
            BSTR username,
            BSTR password,
            BSTR sourcecode,
            BSTR destcode,
            BSTR ip,
            BSTR port,
            BSTR content,
            BSTR sendtype,
            BSTR comid,
            BSTR BNeedReceipt,
            /* [out] */ VARIANT __RPC_FAR *pvSeqID,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CloseConnect( 
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendBusinessPkg_New( 
            BSTR control_type,
            BSTR username,
            BSTR password,
            BSTR sourcecode,
            BSTR destcode,
            BSTR ip,
            BSTR port,
            BSTR content,
            BSTR sendtype,
            BSTR comid,
            BSTR BNeedReceipt,
            BSTR mainAccessNum,
            /* [out] */ VARIANT __RPC_FAR *pvSeqID,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFullTunnelServiceControlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IFullTunnelServiceControl __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IFullTunnelServiceControl __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetSeqID )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [out] */ VARIANT __RPC_FAR *pvSeqID,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartAndManageConnect_User )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            BSTR username,
            BSTR password,
            BSTR ip,
            BSTR port,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartRecv_User )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            VARIANT __MIDL_0015,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendBusinessPkg )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            BSTR control_type,
            BSTR username,
            BSTR password,
            BSTR sourcecode,
            BSTR destcode,
            BSTR ip,
            BSTR port,
            BSTR content,
            BSTR sendtype,
            BSTR comid,
            BSTR BNeedReceipt,
            /* [out] */ VARIANT __RPC_FAR *pvSeqID,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseConnect )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SendBusinessPkg_New )( 
            IFullTunnelServiceControl __RPC_FAR * This,
            BSTR control_type,
            BSTR username,
            BSTR password,
            BSTR sourcecode,
            BSTR destcode,
            BSTR ip,
            BSTR port,
            BSTR content,
            BSTR sendtype,
            BSTR comid,
            BSTR BNeedReceipt,
            BSTR mainAccessNum,
            /* [out] */ VARIANT __RPC_FAR *pvSeqID,
            /* [retval][out] */ VARIANT __RPC_FAR *pvRE);
        
        END_INTERFACE
    } IFullTunnelServiceControlVtbl;

    interface IFullTunnelServiceControl
    {
        CONST_VTBL struct IFullTunnelServiceControlVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFullTunnelServiceControl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFullTunnelServiceControl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFullTunnelServiceControl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFullTunnelServiceControl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFullTunnelServiceControl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFullTunnelServiceControl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFullTunnelServiceControl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFullTunnelServiceControl_GetSeqID(This,pvSeqID,pvRE)	\
    (This)->lpVtbl -> GetSeqID(This,pvSeqID,pvRE)

#define IFullTunnelServiceControl_StartAndManageConnect_User(This,username,password,ip,port,pvRE)	\
    (This)->lpVtbl -> StartAndManageConnect_User(This,username,password,ip,port,pvRE)

#define IFullTunnelServiceControl_StartRecv_User(This,__MIDL_0015,pvRE)	\
    (This)->lpVtbl -> StartRecv_User(This,__MIDL_0015,pvRE)

#define IFullTunnelServiceControl_SendBusinessPkg(This,control_type,username,password,sourcecode,destcode,ip,port,content,sendtype,comid,BNeedReceipt,pvSeqID,pvRE)	\
    (This)->lpVtbl -> SendBusinessPkg(This,control_type,username,password,sourcecode,destcode,ip,port,content,sendtype,comid,BNeedReceipt,pvSeqID,pvRE)

#define IFullTunnelServiceControl_CloseConnect(This,pvRE)	\
    (This)->lpVtbl -> CloseConnect(This,pvRE)

#define IFullTunnelServiceControl_SendBusinessPkg_New(This,control_type,username,password,sourcecode,destcode,ip,port,content,sendtype,comid,BNeedReceipt,mainAccessNum,pvSeqID,pvRE)	\
    (This)->lpVtbl -> SendBusinessPkg_New(This,control_type,username,password,sourcecode,destcode,ip,port,content,sendtype,comid,BNeedReceipt,mainAccessNum,pvSeqID,pvRE)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFullTunnelServiceControl_GetSeqID_Proxy( 
    IFullTunnelServiceControl __RPC_FAR * This,
    /* [out] */ VARIANT __RPC_FAR *pvSeqID,
    /* [retval][out] */ VARIANT __RPC_FAR *pvRE);


void __RPC_STUB IFullTunnelServiceControl_GetSeqID_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFullTunnelServiceControl_StartAndManageConnect_User_Proxy( 
    IFullTunnelServiceControl __RPC_FAR * This,
    BSTR username,
    BSTR password,
    BSTR ip,
    BSTR port,
    /* [retval][out] */ VARIANT __RPC_FAR *pvRE);


void __RPC_STUB IFullTunnelServiceControl_StartAndManageConnect_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFullTunnelServiceControl_StartRecv_User_Proxy( 
    IFullTunnelServiceControl __RPC_FAR * This,
    VARIANT __MIDL_0015,
    /* [retval][out] */ VARIANT __RPC_FAR *pvRE);


void __RPC_STUB IFullTunnelServiceControl_StartRecv_User_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFullTunnelServiceControl_SendBusinessPkg_Proxy( 
    IFullTunnelServiceControl __RPC_FAR * This,
    BSTR control_type,
    BSTR username,
    BSTR password,
    BSTR sourcecode,
    BSTR destcode,
    BSTR ip,
    BSTR port,
    BSTR content,
    BSTR sendtype,
    BSTR comid,
    BSTR BNeedReceipt,
    /* [out] */ VARIANT __RPC_FAR *pvSeqID,
    /* [retval][out] */ VARIANT __RPC_FAR *pvRE);


void __RPC_STUB IFullTunnelServiceControl_SendBusinessPkg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFullTunnelServiceControl_CloseConnect_Proxy( 
    IFullTunnelServiceControl __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pvRE);


void __RPC_STUB IFullTunnelServiceControl_CloseConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFullTunnelServiceControl_SendBusinessPkg_New_Proxy( 
    IFullTunnelServiceControl __RPC_FAR * This,
    BSTR control_type,
    BSTR username,
    BSTR password,
    BSTR sourcecode,
    BSTR destcode,
    BSTR ip,
    BSTR port,
    BSTR content,
    BSTR sendtype,
    BSTR comid,
    BSTR BNeedReceipt,
    BSTR mainAccessNum,
    /* [out] */ VARIANT __RPC_FAR *pvSeqID,
    /* [retval][out] */ VARIANT __RPC_FAR *pvRE);


void __RPC_STUB IFullTunnelServiceControl_SendBusinessPkg_New_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFullTunnelServiceControl_INTERFACE_DEFINED__ */



#ifndef __FULLTUNNELCONTROLLib_LIBRARY_DEFINED__
#define __FULLTUNNELCONTROLLib_LIBRARY_DEFINED__

/* library FULLTUNNELCONTROLLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_FULLTUNNELCONTROLLib;

EXTERN_C const CLSID CLSID_FullTunnelServiceControl;

#ifdef __cplusplus

class DECLSPEC_UUID("11FCBB79-75DC-4C17-AF52-48B0D3FE0D6F")
FullTunnelServiceControl;
#endif
#endif /* __FULLTUNNELCONTROLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
