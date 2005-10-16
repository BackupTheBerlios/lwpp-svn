#ifndef MASTER_ADAPTOR_INCLUDED
#define MASTER_ADAPTOR_INCLUDED


#include "HandlerItem.h"
//#include "lwmaster.h"

struct LWMasterAccess;

template <class T, int Type> class Master_Adaptor : 
public HandlerItem<T>
{
public:
   Master_Adaptor(const char *name);

   virtual ~Master_Adaptor();

   static double Event_Callback(LWInstance inst,const LWMasterAccess *ma);

   static unsigned int  Flags_Callback (LWInstance inst);

   static int Master_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData );
};

template <class T, int Type>
Master_Adaptor<T,Type>::Master_Adaptor(const char *name) : HandlerItem<T>(LWMASTER_HCLASS, name, Master_Callback) {}

template <class T, int Type>
Master_Adaptor<T,Type>::~Master_Adaptor() {}

template <class T, int Type>
double Master_Adaptor<T,Type>::Event_Callback(LWInstance inst,const LWMasterAccess *ma) 
{ 
   return ((T*)inst)->Event( ma ); 
}

template <class T, int Type>
unsigned int  Master_Adaptor<T,Type>::Flags_Callback (LWInstance inst) 
{ 
   return ((T*)inst)->Flags(); 
}

template <class T, int Type>
int Master_Adaptor<T,Type>::Master_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData )
{
   if(version != LWMASTER_VERSION)  
      return(AFUNC_BADVERSION);

   LWMasterHandler   *local=(LWMasterHandler *)INlocal;

   local->event=Event_Callback;

   local->flags=Flags_Callback;

   local->type=Type;

   HandlerItem_Callback(global, local->inst, local->item);

   return AFUNC_OK;
}


#endif // MASTER_ADAPTOR_INCLUDED
