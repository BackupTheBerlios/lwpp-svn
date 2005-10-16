// HandlerItem.h: interface for the HandlerItem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HANDLERITEM_INCLUDED
#define HANDLERITEM_INCLUDED

#include "Handler.h"

/**
 * Base Template for all Lightwave Pluigns using Handler and Item functions.
 * @author David Forstenlechner
 * @date   2002-2003
 */
template <class T> class HandlerItem : 
public Handler<T>  
{
public:
   HandlerItem(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo);

   virtual ~HandlerItem();

   static const LWItemID *useItems (LWInstance INinst);

   static void changeID (LWInstance INinst, const LWItemID *itemID);

   static int HandlerItem_Callback(GlobalFunc *global, LWInstanceFuncs  *inst, LWItemFuncs      *item);
};

template <class T>
HandlerItem<T>::HandlerItem(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo) 
    : Handler<T>(classname, name, Starter, srvinfo) {}

template <class T>
HandlerItem<T>::~HandlerItem() {}

template <class T>
const LWItemID *HandlerItem<T>::useItems (LWInstance INinst) 
{ 
   T *inst=(T *)INinst; 
   return inst->UseItems(); 
}

template <class T>
void   HandlerItem<T>::changeID (LWInstance INinst, const LWItemID *itemID)
{ 
   T *inst=(T *)INinst; 
   inst->ChangeID(itemID);
}

template <class T>
int HandlerItem<T>::HandlerItem_Callback(GlobalFunc *global, LWInstanceFuncs  *inst, LWItemFuncs      *item)
{
   if(item)
   {
      item->useItems   = useItems;
      item->changeID   = changeID;
   }
      
   return Handler_Callback( global, inst );
}

#endif // HANDLERITEM_INCLUDED
