#ifndef LWPP_RENDER_H
#define LWPP_RENDER_H
//=============================================================================
///
/// \File globalWrap.h
///   Declaration and definition of LWPPGlobal - the base class for
///   LightWrap's globalTypes.
///
///   Definition of a globalType: Any value returned by a call to
///   LightWave's global function.
///
///   Each globalType is derived from LWPPGlobal.
///
///   Because SystemID, ProductInfo and LocaleInfo are captured in
///   LWPPGlobal, there is no need to create these as separate globalTypes.
///
/// CONTENTS:
/// ~~~~~~~~
///	  LWPPGlobal - the mother of all globalTypes
///
///   LWPPFileRequester - contains all the functionality of LW's fileRequest
///     globals and the utility functions that they use.
///
///	  LWPPColorActivate - an encapsulation of LWColorActivate.
///
///   LWPPMessageFuncs - an encapsulation of LWMessageFuncs.
///
///	\Author	CWCunningham
///
//=============================================================================

#include "globalWrap.h" ///< LWPPGlobal
#include <lwrender.h>   ///< LW InterfaceInfo

namespace lwpp
{
//*****************************************************************************
///
/// \class LWPPInterfaceInfo - This class must be constructed in your activate function
///	in order for any globals derived from this class to work. It can be
/// constructed as an automatic variable, or on the heap. Because the
/// working values are static, they survive destruction
///
//*****************************************************************************
class LWPPInterfaceInfo : public LWPPGlobal
{
	LWInterfaceInfo *interfaceInfo;

  public:

	//=========================================================================
	/// Constructor, acquire if possible.
	//
	LWPPInterfaceInfo() : interfaceInfo(NULL)
	{
		if (GLOBAL != NULL)
		{
			interfaceInfo = (LWInterfaceInfo *)
				GLOBAL(LWINTERFACEINFO_GLOBAL, GFUSE_ACQUIRE);
		}
	}

	//=========================================================================
	/// Destructor - release acquisition.
	//
	virtual ~LWPPInterfaceInfo()
	{
		if (interfaceInfo != NULL)
		{
			GLOBAL(LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE);
		}
	}

	//=========================================================================
	/// Copy constructor - Prevent explicit copies.
	/// You will get a linker error if you call this.
	//
	LWPPInterfaceInfo(LWPPInterfaceInfo&);

	//=========================================================================
	/// Assignment operator - Prevent explicit copies
	/// You will get a linker error if you call this.
	//
	LWPPInterfaceInfo & operator =(LWPPInterfaceInfo&);

	//=========================================================================
	/// isValid - Lets you know if a global call will work or fail.
	//
	virtual bool isValid()
	{
		return (interfaceInfo != NULL);
	}

	//=========================================================================
	/// curTime - returns the current time in the LW interface.
	//
	LWTime curTime()
	{
		if (isValid())
		{
			return interfaceInfo->curTime;
		}
		else
			return (LWTime)0;
	}

	//=========================================================================
	/// selItems - returns NULL terminated list of selected items.
	//
    const LWItemID   *selItems()
	{
		if (isValid())
		{
			return interfaceInfo->selItems;
		}
		else
			return (LWItemID)NULL;
	}

	//=========================================================================
	/// itemFlags - returns itemFlags associated with item.
	///    LWITEMF_SELECTED        (1<<0)
	///    LWITEMF_SHOWCHILDREN    (1<<1)
	///    LWITEMF_SHOWCHANNELS    (1<<2)
	///    LWITEMF_LOCKED          (1<<3)
	/// In case of BAD_GLOBAL returns 0xdeadbeef
	//
	unsigned int itemFlags(LWItemID item)
	{
		if (isValid())
		{
			return interfaceInfo->itemFlags(item);
		}
		else
			return 0xdeadbeef;
	}

	//=========================================================================
	/// previewStart - returns preview start frame.
	//
	LWFrame previewStart()
	{
		if (isValid())
		{
			return interfaceInfo->previewStart;
		}
		else
			return (LWFrame)0;
	}

	//=========================================================================
	/// previewEnd - returns preview end frame.
	//
	LWFrame previewEnd()
	{
		if (isValid())
		{
			return interfaceInfo->previewEnd;
		}
		else
			return (LWFrame)0;
	}

	//=========================================================================
	/// previewStep - returns frames per frame in the preview.
	//
	LWFrame previewStep()
	{
		if (isValid())
		{
			return interfaceInfo->previewStep;
		}
		else
			return (LWFrame)0;
	}

	//=========================================================================
	/// dynaUpdate - returns one of:
	///   LWDYNUP_OFF
	///   LWDYNUP_DELAYED
	///   LWDYNUP_INTERACTIVE
	///
	/// returns -1 for BAD_GLOBAL
	//
	int dynaUpdate()
	{
		if (isValid())
		{
			return interfaceInfo->dynaUpdate;
		}
		else
			return -1;
	}

	//=========================================================================
	/// schemaPos - read the schema position of item.
	//
	void schemaPos(LWItemID item, double *x, double *y)
	{
		if (isValid())
			interfaceInfo->schemaPos(item, x, y);
	}

	//=========================================================================
	/// itemVis - returns visibility flag for item.
	///		LWOVIS_HIDDEN           0
	///     LWOVIS_BOUNDINGBOX      1
	///     LWOVIS_VERTICES         2
	///     LWOVIS_WIREFRAME        3
	///     LWOVIS_FFWIREFRAME      4
	///     LWOVIS_SHADED           5
	///     LWOVIS_TEXTURED         6
	///	some item types return
	///		LWIVIS_HIDDEN           0
	///     LWIVIS_VISIBLE          1
	/// Returns -1 in case of BAD_GLOBAL
	//
    int itemVis(LWItemID item)
	{
		if (isValid())
		{
			return interfaceInfo->itemVis(item);
		}
		else
			return -1;
	}

	//=========================================================================
	/// displayFlags - returns 0 or more of:
	///		LWDISPF_MOTIONPATHS     (1<<0)
	///     LWDISPF_HANDLES         (1<<1)
	///     LWDISPF_IKCHAINS        (1<<2)
	///     LWDISPF_CAGES           (1<<3)
	///     LWDISPF_SAFEAREAS       (1<<4)
	///     LWDISPF_FIELDCHART      (1<<5)
	///
	/// In case of BAD_GLOBAL returns 0xdeadbeef
	//
	unsigned int displayFlags()
	{
		if (isValid())
			return interfaceInfo->displayFlags;
		else
			return 0xdeadbeef;
	}
	//=========================================================================
	///	generalFlags - returns 0 or more of the following:
	///		LWGENF_HIDETOOLBAR      (1<<0)
	///     LWGENF_RIGHTTOOLBAR     (1<<1)
	///     LWGENF_PARENTINPLACE    (1<<2)
	///     LWGENF_FRACTIONALFRAME  (1<<3)
	///     LWGENF_KEYSINSLIDER     (1<<4)
	///     LWGENF_PLAYEXACTRATE    (1<<5)
	///     LWGENF_AUTOKEY          (1<<6)
	/// In case of BAD_GLOBAL returns 0xdeadbeef
	//
    unsigned int generalFlags()
	{
		if (isValid())
			return interfaceInfo->generalFlags;
		else
			return 0xdeadbeef;
	}

	//=========================================================================
	/// boxThreshold - returns the number of points above which, an item will
	///  be displayed as a bounding box.
	///	Added in version 7
	/// Returns -1 in case of a BAD_GLOBAL
	//
    int boxThreshold()
	{
		if (isValid() && revisionMajor() > 6)
			return interfaceInfo->boxThreshold;
		else
			return -1;
	}
	//=========================================================================
	/// itemColor - returns the color Layout uses to draw the chosen item.
	/// According to the SDK, there are 15 possible return values which
	/// are indexes into the array of colors that can be set in the spreadSheet
	///	Added in version 7
	/// Returns -1 in case of a BAD_GLOBAL
	//
    int itemColor(LWItemID item)
	{
		if (isValid() && revisionMajor() > 6)
			return interfaceInfo->itemColor(item);
		else
			return -1;
	}
	//=========================================================================
	/// alertLevel - returns one of the following:
	///		LWALERT_BEGINNER        0
	///     LWALERT_INTERMEDIATE    1
	///     LWALERT_EXPERT          2
	/// Added in version 7
	/// Returns -1 in case of a BAD_GLOBAL
	/// Tested OK
	//
	int alertLevel()
	{
		if (isValid() && revisionMajor() > 6)
			return interfaceInfo->alertLevel;
		else
			return -1;
	}
	//=========================================================================
	/// autoKeyCreate - returns one of the following:
	///		LWAKC_OFF               0
	///     LWAKC_MODIFIED          1
	///     LWAKC_ALL               2
	/// Added in version 7.5 ???
	/// Returns -1 in case of a BAD_GLOBAL
	///
	/// NOTICE the use of the build() function to prevent crashes
	/// when working in 7.5b or less. Support for this member may have
	/// existed before 7.5c ... if you know when it was added, please
	/// report that to LightWrap++
	//
	int autoKeyCreate()
	{
		if (isValid() && (build() >= 572)) // 7.5 or better.
			return interfaceInfo->autoKeyCreate;
		else
			return -1;
	}
};

} // End - namespace lwpp

#endif // LWPP_RENDER_H
