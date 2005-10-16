#ifndef LWLIB_H
#define LWLIB_H

#include "Lightwave3D.h"

namespace LWLib
{
   static string StdEnvNames[9]={
      "Position.X",
      "Position.Y",
      "Position.Z",
      "Rotation.H",
      "Rotation.P",
      "Rotation.B",
      "Scale.X",
      "Scale.Y",
      "Scale.Z"
   };


   LWEnvelopeID GetEnvelopeByName(LWChanGroupID grp, const char *name);
   LWChannelID GetChannelByName(LWChanGroupID grp, const char *name);

   bool ItemIDExisting(string &name, LWItemType Type);
   LWItemID GetIDByName(string &ItemName, LWItemType Type);
   LWItemID itemFindID(LWItemInfo *iteminfo, char* name);
}

#endif