#ifndef VOLUMEHANDLER_ADAPTOR_H
#define VOLUMEHANDLER_ADAPTOR_H

#include "HandlerItemRend.h"
#include "../../lwinclude/lwvolume.h"

template <class T> class VolumeHandler_Adaptor : 
public HandlerItemRend<T>
{
public:
   VolumeHandler_Adaptor(const char *name, ServerTagInfo *srvinfo);

   virtual ~VolumeHandler_Adaptor();

   static double Evaluate_Callback(LWInstance inst, st_LWVolumeAccess *va);
   
   static unsigned int Flags_Callback( LWInstance inst );

   static int VolumeHandler_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData );
};

template <class T>
VolumeHandler_Adaptor<T>::VolumeHandler_Adaptor(const char *name, ServerTagInfo *srvinfo) 
    : HandlerItemRend<T>( LWVOLUMETRIC_HCLASS, name, VolumeHandler_Callback, srvinfo ) {}

template <class T>
VolumeHandler_Adaptor<T>::~VolumeHandler_Adaptor() {}

template <class T>
double VolumeHandler_Adaptor<T>::Evaluate_Callback(LWInstance inst, st_LWVolumeAccess *va) { return ((T*)inst)->Evaluate(va); }

template <class T>
unsigned int VolumeHandler_Adaptor<T>::Flags_Callback( LWInstance inst ) { return ((T*)inst)->Flags(); }

template <class T>
int VolumeHandler_Adaptor<T>::VolumeHandler_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData )
{
   if ( version != LWVOLUMETRIC_VERSION )
      return AFUNC_BADVERSION;

   LWVolumetricHandler *local=(LWVolumetricHandler *)INlocal;      

   local->evaluate = Evaluate_Callback;

   local->flags    = Flags_Callback;

   return HandlerItemRend_Callback( global, local->inst, local->item, local->rend );
}

#endif
