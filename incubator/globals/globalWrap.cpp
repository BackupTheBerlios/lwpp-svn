//=============================================================================
///
/// \File globalWrap.cpp
///   Storage space for statics. Some inlines may be moved here later.
///
/// CONTENTS:
/// ~~~~~~~~
///	  LWPPGlobal::GLOBAL      - static storage.
///	  LWPPGlobal::SystemID    - static storage.
///	  LWPPGlobal::ProductInfo - static storage.
///	  LWPPGlobal::LocaleInfo  - static storage.
///
///	@Author	CWCunningham
///
//=============================================================================

#include "globalWrap.h" ///< LW GlobalFunc wrapper declarations & inlines.
#include <string.h>     ///< string functions
#include <stdlib.h>     ///< ultoa

namespace lwpp
{

/// Wont link unless we set aside storage space for statics.
/*\@{*/
GlobalFunc   *LWPPGlobal::GLOBAL      = NULL;
unsigned long LWPPGlobal::SystemID    = 0;
unsigned long LWPPGlobal::ProductInfo = 0;
unsigned long LWPPGlobal::LocaleInfo  = 0;
/*\@}*/

/// utility function for revision() member function.
/// Tested OK
const char * ulAsString(unsigned long input)
{
	static char returnString[34];
	ultoa(input, returnString, 10);
	return returnString;
}

//=========================================================================
//
/// LWPPGlobal Constructor, where all the magic happens.
/// Tested OK
//
LWPPGlobal::LWPPGlobal(GlobalFunc* global, long version)
{
	/// Only initialize once
	//
	if (GLOBAL == NULL && global != NULL)
	{
		GLOBAL = global;
		SystemID    = (unsigned long)
			GLOBAL(LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT);
		ProductInfo = (unsigned long)
			GLOBAL(LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT);

		/// Because the locale uses only two bytes of this 4 byte value,
		/// I'm going to store the version number in the upper 2 bytes.
		//
		LocaleInfo  = (unsigned long)
			GLOBAL(LWLOCALEINFO_GLOBAL, GFUSE_TRANSIENT);
		LocaleInfo &= 0x0000ffff;
		LocaleInfo |= (version << 16);
	}
}

//=========================================================================
//
/// LWPPGlobal revision - returns a user friendly string.
/// Tested OK
//
const char * LWPPGlobal::revision()
{
	/// Tried using std::string, but got compiler errors when including stdlib.h.
	/// Because I needed ultoa more than I needed a string class, stdlib won out.
	//
	static char returnValue[128] = "";
	if (! strlen(returnValue))
	{
		const char *dot           = ".";
		const char *prodLwav      = "Lightwave ";
		const char *prodInsp      = "Inspire3d ";
		const char *prodOthr      = "Newtek Other? ";
		const char *appLayout     = "Layout ";
		const char *appModeler    = "Modeler ";
		const char *appScreamer   = "ScreamerNet ";
		const char *unknown       = "Unknown ";
		const char *utilityString = NULL;
		switch (product())
		{
			// Currently, the product can be LWINF_PRODLWAV (LightWave),
			// LWINF_PRODINSP3D (Inspire),
			// or LWINF_PRODOTHER.
			case LWINF_PRODLWAV:
				utilityString = prodLwav;
				break;
			case LWINF_PRODINSP3D:
				utilityString = prodInsp;
				break;
			case LWINF_PRODOTHER:
				utilityString = prodOthr;
				break;
			default:
				utilityString = unknown;
				break;
		}

		strcpy(returnValue, utilityString);
		strcat(returnValue, "rev [");
		strcat(returnValue, ulAsString(revisionMajor()));
		strcat(returnValue, dot);
		strcat(returnValue, ulAsString(revisionMinor()));
		strcat(returnValue, dot);
		strcat(returnValue, ulAsString(version()));
		strcat(returnValue, " Build ");
		strcat(returnValue, ulAsString(build()));
		strcat(returnValue, "] ");
		strcat(returnValue, "App: ");

		switch(application())
		{
			// The context can be LWSYS_LAYOUT,
			// LWSYS_MODELER, or LWSYS_SCREAMERNET.
			case LWSYS_LAYOUT:
				utilityString = appLayout;
				break;
			case LWSYS_MODELER:
				utilityString = appModeler;
				break;
			case LWSYS_SCREAMERNET:
				utilityString = appScreamer;
				break;
			default:
				utilityString = unknown;
				break;
		}
		strcat(returnValue, utilityString);
		strcat(returnValue, dot);
		strcat(returnValue, " Dongle ID ");
		strcat(returnValue, ulAsString(dongleID()));
		strcat(returnValue, dot);
	}
	return returnValue;
}

} // End - namespace lwpp
