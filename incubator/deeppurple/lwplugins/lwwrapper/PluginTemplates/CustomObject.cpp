#include "StdAfx.h"
#include "CustomObject.h"
#include "../LWcommon/SceneItem.h"

CustomObject::CustomObject(void *context) :
Item(context),
Time(0.0)
{
   this->ItemName = Lightwave3D::iteminfo->name(Item);
   theSceneItem = new SceneItem(Item);
}

CustomObject::~CustomObject()
{
   if (theSceneItem)
      delete theSceneItem;
}

const LWItemID *CustomObject::UseItems() 
{ 
   ToLW[0]=Item;
   ToLW[1]=LWITEM_NULL;
   return ToLW; 
}

void CustomObject::ChangeID(const LWItemID *itemID) 
{ 
   LWItemID old=this->Item;

   for (int i=0; itemID[i]!=NULL; i++)  
   {
      if (itemID[i]==Item)
      {
         Item=itemID[i+1];
         this->ItemName=Lightwave3D::iteminfo->name(Item);
      }
      i++;
   }

   if (old!=this->Item)
   {
      delete this->theSceneItem;
      this->theSceneItem = new SceneItem( this->Item );
   }
}

int    CustomObject::Flags   () 
{ 
   return 0; 
}

void   CustomObject::Evaluate( const LWCustomObjAccess *ca) 
{
   int debug=1;
}

void CustomObject::NewTime(LWFrame Frame, LWTime time)
{
   Time=time;
   this->theSceneItem->setTime(Time);
}
