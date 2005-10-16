#ifndef CUSTOMOBJECT_ADAPTOR_INCLUDED
#define CUSTOMOBJECT_ADAPTOR_INCLUDED

#include "HandlerItemRend.h"

#include "../../lwinclude/lwcustobj.h"

/**
 * Template class for the creation of Custom Object Plugins.
 * @author David Forstenlechner
 * @date   2002-2003
 */
template <class T> class CustomObject_Adaptor : 
public HandlerItemRend<T>
{
public:
   CustomObject_Adaptor(const char *name, ServerTagInfo *srvinfo);

   virtual ~CustomObject_Adaptor();

   static void Evaluate_Callback(LWInstance inst, const LWCustomObjAccess *ca);

   static unsigned int Flags_Callback( LWInstance inst );

   static int CustomObject_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData );
};

template <class T>
CustomObject_Adaptor<T>::CustomObject_Adaptor(const char *name, ServerTagInfo *srvinfo) 
    : HandlerItemRend<T>( LWCUSTOMOBJ_HCLASS, name, CustomObject_Callback, srvinfo ) {}

template <class T>
CustomObject_Adaptor<T>::~CustomObject_Adaptor() {}

template <class T>
void CustomObject_Adaptor<T>::Evaluate_Callback(LWInstance inst, const LWCustomObjAccess *ca) { ((T*)inst)->Evaluate(ca); }

template <class T>
unsigned int CustomObject_Adaptor<T>::Flags_Callback( LWInstance inst ) { return ((T*)inst)->Flags(); }

template <class T>
int CustomObject_Adaptor<T>::CustomObject_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData )
{
   if ( version != LWCUSTOMOBJ_VERSION )
      return AFUNC_BADVERSION;

   LWCustomObjHandler *local=(LWCustomObjHandler *)INlocal;      

   local->evaluate = Evaluate_Callback;

   local->flags    = Flags_Callback;

   return HandlerItemRend_Callback( global, local->inst, local->item, local->rend );
}


#endif // CUSTOMOBJECT_ADAPTOR_INCLUDED
