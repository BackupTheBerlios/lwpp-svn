#ifndef PLUGINTEMPLATES_INTERFACE_H_HEADER_INCLUDED
#define PLUGINTEMPLATES_INTERFACE_H_HEADER_INCLUDED

#include "Activator.h"

template <class T>
class Interface_Adaptor : public Activator<T>
{
 public:
   Interface_Adaptor(const char *name, const char *PluginFor, ServerTagInfo *srvinfo);
   ~Interface_Adaptor();

   static int Interface_Callback(long version, GlobalFunc *global, void *INlocal, void *serverData);

   static LWError Options ( LWInstance inst );
};

template <class T>
Interface_Adaptor<T>::Interface_Adaptor(const char *name, const char *PluginFor, ServerTagInfo *srvinfo) :
   Activator<T>(PluginFor, name, Interface_Callback, srvinfo)
{
}

template <class T>
Interface_Adaptor<T>::~Interface_Adaptor()
{
}

template <class T>
int Interface_Adaptor<T>::Interface_Callback(long version, GlobalFunc *global, void *INlocal, void *serverData)
{
   if ( version != LWINTERFACE_VERSION ) return AFUNC_BADVERSION;
   int SucessCode=Initialize(global);
   if (SucessCode!=AFUNC_OK)
      return SucessCode;
   LWInterface *local=(LWInterface *)INlocal;

   local->command = NULL;
   if ( T::PanelClassName == "XPanel" )
   {
      T *InterfaceImpl=new T;
      InterfaceImpl->SetInstance(local->inst);
      local->panel=InterfaceImpl->GetPanelID();
      local->options = NULL;
   }
   else
   {
      local->options = Options;
      local->panel= NULL;
   }
   return AFUNC_OK;
}

template <class T>
LWError Interface_Adaptor<T>::Options ( LWInstance inst )
{
   T *InterfaceImpl = new T;
   InterfaceImpl->SetInstance( inst );
   InterfaceImpl->Open();
   return NULL;
}

#endif /* PLUGINTEMPLATES_INTERFACE_H_HEADER_INCLUDED */
