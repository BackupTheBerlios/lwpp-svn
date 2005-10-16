#include "StdAfx.h"
#include "LWlib.h"

namespace LWLib
{

LWChannelID GetChannelByName(LWChanGroupID grp, const char *name)
{
   LWChannelID chan = Lightwave3D::chaninfo->nextChannel( grp, NULL );
   while ( chan ) {
      if ( !strcmp( Lightwave3D::chaninfo->channelName( chan ), name )) {
         break;
      }
      chan = Lightwave3D::chaninfo->nextChannel( grp, chan );
   }
   if (chan)
      return chan;
   else
      return NULL;
}

LWEnvelopeID GetEnvelopeByName(LWChanGroupID grp, const char *name)
{
   LWEnvelopeID envV=NULL;
   LWChannelID chan = Lightwave3D::chaninfo->nextChannel( grp, NULL );
   while ( chan ) {
      if ( !strcmp( Lightwave3D::chaninfo->channelName( chan ), name )) {
         envV = Lightwave3D::chaninfo->channelEnvelope( chan );
         break;
      }
      chan = Lightwave3D::chaninfo->nextChannel( grp, chan );
   }
   if (chan)
      return envV;
   else
      return NULL;
}

bool ItemIDExisting(string &name, LWItemType Type)
{
   if (Type>=0)
   {
      LWItemID runner=Lightwave3D::iteminfo->first(Type,NULL);
      while (runner)
      {
         if (name==Lightwave3D::iteminfo->name(runner))
            return true;
         runner=Lightwave3D::iteminfo->next(runner);
      }
   }
   else
   {
      LWItemID runner=Lightwave3D::iteminfo->first(LWI_OBJECT,NULL);
      while (runner)
      {
         if (name==Lightwave3D::iteminfo->name(runner))
            return true;
         runner=Lightwave3D::iteminfo->next(runner);
      }

      runner=Lightwave3D::iteminfo->first(LWI_LIGHT,NULL);
      while (runner)
      {
         if (name==Lightwave3D::iteminfo->name(runner))
            return true;
         runner=Lightwave3D::iteminfo->next(runner);
      }

      runner=Lightwave3D::iteminfo->first(LWI_CAMERA,NULL);
      while (runner)
      {
         if (name==Lightwave3D::iteminfo->name(runner))
            return true;
         runner=Lightwave3D::iteminfo->next(runner);
      }
   }
   return false;
}

LWItemID GetIDByName(string &ItemName, LWItemType Type)
{
   LWItemID runner=Lightwave3D::iteminfo->first(Type,NULL);
   while (runner)
   {
      if (ItemName==Lightwave3D::iteminfo->name(runner))
         return runner;
      runner=Lightwave3D::iteminfo->next(runner);
   }
   return NULL;
}

LWItemID itemFindID(LWItemInfo *iteminfo, char* name)
{
	LWItemID id, bone;
	int n;
	int objectTypes[] = {LWI_OBJECT, LWI_BONE, LWI_LIGHT, LWI_CAMERA };

	for(n=0; n < 4; n++)
	{
		id = iteminfo->first(objectTypes[n], LWITEM_NULL);
		while(id)
		{
			// OBJECT
 			if(strcmp(iteminfo->name(id), name) == 0)
			{
				return id;
			}

			// CHILD BONES
			if(n == 0)
			{
				bone = iteminfo->first(LWI_BONE, id);
				while (bone) 
				{
 					if(strcmp(iteminfo->name(bone), name) == 0 && iteminfo->type(bone) == LWI_BONE)
					{
						return bone;
					}
					bone = iteminfo->next(bone);					
				}
			}

			id = iteminfo->next(id);
		}
	}
	return NULL;
}

}

