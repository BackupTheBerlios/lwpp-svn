#ifndef ACTIVATOR_INCLUDED
#define ACTIVATOR_INCLUDED

#include "../LWcommon/lightwave3d.h"

/**
 *  @short Base template for the plugin interface wrapper.
 *
 *  @author David Forstenlechner
 */
template<class T> class Activator  
{
public:
    /// The contructor relays to RegisterPlugin of the Lightwave3D namespace.
    Activator(const char *classname, const char *name, ActivateFunc *Starter, ServerTagInfo *srvinfo) 
	{
        Lightwave3D::RegisterPlugin(classname, name, Starter, srvinfo); 
	}

    virtual ~Activator() { Lightwave3D::ReleaseGlobals(); }

    /// Initialization common to all plugin classes
    static int Initialize( GlobalFunc *global )
    {
        // Save Global Function in static Lightwave3D class variable and populate Globals list
        Lightwave3D::GlobalFunction=global;

        if(!Lightwave3D::GetGlobals())
            return AFUNC_BADGLOBAL;
        
        return AFUNC_OK;
    }    
};

#endif // ACTIVATOR_INCLUDED
