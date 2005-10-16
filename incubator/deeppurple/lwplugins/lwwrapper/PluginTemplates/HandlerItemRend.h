// HandlerItemRend.h: interface for the HandlerItemRend class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HANDLERITEMREND_INCLUDED
#define HANDLERITEMREND_INCLUDED

#include "HandlerItem.h"

/**
 * Base Template for all Lightwave Pluigns using Handler and Item and Render functions.
 * @author David Forstenlechner
 * @date   2002-2003
 */
template <class T> class HandlerItemRend : 
public HandlerItem<T>
{
public:
   HandlerItemRend(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo);

   virtual ~HandlerItemRend();

   static LWError Init_Callback    ( LWInstance inst, int Mode );

   static void    Cleanup_Callback ( LWInstance inst );

   static LWError NewTime_Callback ( LWInstance inst, LWFrame Frame, LWTime Time );

   static int HandlerItemRend_Callback ( GlobalFunc *global, LWInstanceFuncs *inst, LWItemFuncs *item, LWRenderFuncs *rend );
};

template <class T>
HandlerItemRend<T>::HandlerItemRend(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo) 
    : HandlerItem<T>(classname, name, Starter, srvinfo) {}

template <class T>
HandlerItemRend<T>::~HandlerItemRend() {}

template <class T>
LWError HandlerItemRend<T>::Init_Callback    ( LWInstance inst, int Mode ) 
{ ((T*)inst)->Init(Mode); return NULL; }

template <class T>
void    HandlerItemRend<T>::Cleanup_Callback ( LWInstance inst ) 
{ ((T*)inst)->Cleanup(); }

template <class T>
LWError HandlerItemRend<T>::NewTime_Callback ( LWInstance inst, LWFrame Frame, LWTime Time ) 
{ ((T*)inst)->NewTime( Frame, Time ); return NULL; }

template <class T>
int HandlerItemRend<T>::HandlerItemRend_Callback ( GlobalFunc *global, LWInstanceFuncs *inst, LWItemFuncs *item, LWRenderFuncs *rend )
{
  if (rend)
  {
     rend->init=Init_Callback;
     rend->cleanup=Cleanup_Callback;
     rend->newTime=NewTime_Callback;
  }
  return HandlerItem_Callback( global, inst, item );
}

#endif // HANDLERITEMREND_INCLUDED