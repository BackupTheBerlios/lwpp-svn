#ifndef GENERICPLUGIN_INCLUDED
#define GENERICPLUGIN_INCLUDED

#include "Activator.h"
#include "../../lwinclude/lwgeneric.h"           /* for the LayoutGeneric class   */

template <class T> class GenericPlugin_Adaptor : 
public Activator<T> 
{
public:
   GenericPlugin_Adaptor(const char *name, ServerTagInfo *srvinfo);
   ~GenericPlugin_Adaptor(void);

   static int GenericPlugin_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData );
};


template <class T>
GenericPlugin_Adaptor<T>::GenericPlugin_Adaptor(const char *name, ServerTagInfo *srvinfo) 
    : Activator<T>(LWLAYOUTGENERIC_CLASS, name, GenericPlugin_Callback, srvinfo) {}

template <class T>
GenericPlugin_Adaptor<T>::~GenericPlugin_Adaptor(void) {}

template <class T>
int GenericPlugin_Adaptor<T>::GenericPlugin_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData )
{
   // If Header Version doesn´t match Lightwave Version return BADVERSION
   if ( version != LWLAYOUTGENERIC_VERSION )
      return AFUNC_BADVERSION;

   int SucessCode=Initialize(global);
   if (SucessCode!=AFUNC_OK)
      return SucessCode;

   // Instantiate and launch main panel      
   T *MyPanel=new T;
   MyPanel->SetGlobalLocal((LWLayoutGeneric *)INlocal);
   MyPanel->Open();
   
   return AFUNC_OK;
}

#endif  // GENERICPLUGIN_INCLUDED