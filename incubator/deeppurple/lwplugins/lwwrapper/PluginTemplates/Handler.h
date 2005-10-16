#ifndef HANDLER_INCLUDED
#define HANDLER_INCLUDED

#include "Activator.h"

/**
 * Base Template for all Lightwave Pluigns using Handler functions.
 * @author David Forstenlechner
 * @date   2002-2003
 */
template <class T> class Handler : 
public Activator<T>  
{
public:
   Handler(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo);
   virtual ~Handler();
   static LWInstance Create_Callback(void *priv,void *context, LWError *err);

   static void Destroy_Callback(LWInstance inst);

   static LWError Copy_Callback(LWInstance to,LWInstance from);

   static LWError Load_Callback( LWInstance inst, const LWLoadState *lState );

   static LWError Save_Callback( LWInstance inst, const LWSaveState *sState );

   static const char *Describe_Callback( LWInstance inst );

   static int Handler_Callback(GlobalFunc *global, LWInstanceFuncs  *inst);
};

template <class T>
Handler<T>::Handler(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo) 
: Activator<T> (classname, name, Starter, srvinfo) {} 

template <class T>
Handler<T>::~Handler() {}

template <class T>
LWInstance Handler<T>::Create_Callback(void *priv,void *context, LWError *err) 
{ 
   return new T(context); 
}

template <class T>
void Handler<T>::Destroy_Callback(LWInstance inst) { delete ((T *)inst); }

template <class T>
LWError Handler<T>::Copy_Callback(LWInstance to,LWInstance from) { *((T*)to)=*((T*)from); return NULL; }

template <class T>
LWError Handler<T>::Load_Callback( LWInstance inst, const LWLoadState *lState ) { ((T*)inst)->Load(lState); return NULL; }

template <class T>
LWError Handler<T>::Save_Callback( LWInstance inst, const LWSaveState *sState ) 
{ 
    ((T*)inst)->Save(sState); 
    return NULL; 
}

template <class T>
const char *Handler<T>::Describe_Callback( LWInstance inst ) 
{ 
   return ((T*)inst)->Describe(); 
}

template <class T>
int Handler<T>::Handler_Callback(GlobalFunc *global, LWInstanceFuncs  *inst)
{
   if(inst)
   {
      inst->create     = Create_Callback;
      inst->destroy    = Destroy_Callback;
      inst->load       = Load_Callback;
      inst->save       = Save_Callback;
      inst->copy       = Copy_Callback;
      inst->descln     = Describe_Callback;
   }
   return Initialize(global);
}

#endif // HANDLER_INCLUDED
