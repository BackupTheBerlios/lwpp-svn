#ifndef LWCOMMON_LWSCENEGRAPH_H_HEADER_INCLUDED_C136776D
#define LWCOMMON_LWSCENEGRAPH_H_HEADER_INCLUDED_C136776D

#include "Singleton.h"
#include "lwrender.h"
#include <string>

class LWSceneGraph;

typedef Singleton<LWSceneGraph> LWSceneGraphSingleton;

/** 
 * Singleton to provide common tasks of scene graph access.
 * @author David Forstenlechner
 * @date   2002-2003
 */
class LWSceneGraph : public LWSceneGraphSingleton
{
 public:
  /**
   * Item types in scene.
   */
   enum ItemType
   {
      Object=LWI_OBJECT,      ///< Object Item Type 
      Light=LWI_LIGHT,        ///< Light Item Type 
      Camera=LWI_CAMERA,      ///< Camera Item Type
      Bone=LWI_BONE           ///< Bone Item Type
   };

  /**
   * Searches for the last Item of the specified Type
   * in the Scene Graph.
   * @param TypeToSearch Type of Item to search the last entry for.
   * @return Returns NULL if no item of the specified Type found in Scene.
   */
   LWItemID FindLastOfType(ItemType TypeToSearch);
   std::string GetItemName(LWItemID nameFor);
   LWItemID AddNull(std::string & ItemName);
};


#endif /* LWCOMMON_LWSCENEGRAPH_H_HEADER_INCLUDED_C136776D */
