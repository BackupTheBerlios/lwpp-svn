#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include "HandlerImpl.h"
#include "../../lwinclude/lwvolume.h"

/**
 * Base class for Volumetric Plugins.
 * @author David Forstenlechner
 * @date   2002-2003
 * @see HandlerImpl
 */
class VolumeHandler : 
public HandlerImpl  
{
public:
  /**
   * Gets Samples along a ray.
   * @param va Volume Access Struct to provide data and functions for volumetric rendering.
   */
   virtual double Evaluate( LWVolumeAccess *va);

  /**
   * Sets range of ray types to be affected by this Volume Plugin.
   */
   virtual int    Flags   ();
};

#endif
