#include "StdAfx.h"
#include "LWSceneGraph.h"

LWItemID LWSceneGraph::FindLastOfType(ItemType TypeToSearch)
{
   LWItemID Result=NULL;
   LWItemID runner=Lightwave3D::iteminfo->first(TypeToSearch,NULL);
   while (runner)
   {
      Result=runner;
      runner=Lightwave3D::iteminfo->next(runner);
   }
   return Result;
}



string LWSceneGraph::GetItemName(LWItemID nameFor)
{
   const char *ItemName=Lightwave3D::iteminfo->name(nameFor);
   if (ItemName)
   {
      return string(ItemName);
   }
   return NULL;
}

LWItemID LWSceneGraph::AddNull(string & ItemName)
{
   ostringstream command;
   command << "AddNull ";
   command << ItemName;
   Lightwave3D::cmdfunc(command.str().c_str());

   LWItemID LastObject=this->FindLastOfType(LWSceneGraph::Object);
   if (GetItemName(LastObject)==ItemName)
      return LastObject;
   else
      return NULL;
}

