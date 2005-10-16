#ifndef CUSTOMOBJECT_INCLUDED
#define CUSTOMOBJECT_INCLUDED

#include "HandlerImpl.h"
#include "../../lwinclude/lwcustobj.h"

class SceneItem;

/**
 * Base class for Custom Object type Lightwave Plugins.
 * @author David Forstenlechner
 * @date   2002-2003
 */
class CustomObject : 
public HandlerImpl  
{
public:
   CustomObject(void *context);
   virtual ~CustomObject();

   virtual void	Evaluate( const LWCustomObjAccess *ca);

   virtual int Flags   ();

   virtual const LWItemID *UseItems();

   virtual void ChangeID(const LWItemID *itemID);

   virtual void NewTime(LWFrame Frame, LWTime time);

 protected:
   string ItemName;
   double Time;
   LWItemID Item;
   SceneItem *theSceneItem;

 private:
   LWItemID ToLW[2];
};

#endif // CUSTOMOBJECT_INCLUDED
