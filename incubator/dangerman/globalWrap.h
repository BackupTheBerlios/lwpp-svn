#ifndef LWPP_GLOBALWRAP_H
#define LWPP_GLOBALWRAP_H
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

#include <lwserver.h> // LW GlobalFunc
#include <lwhost.h>   // LW ProductInfo, SystemID, and LocaleInfo
#include <lwtypes.h>  // NULL

/// The namespace for all LightWrap++ code.
//
namespace lwpp
{
//*****************************************************************************
///
/// \class LWPPGlobal - This class must be constructed in your activate function
///	in order for any globals derived from this class to work. It can be
/// constructed as an automatic variable, or on the heap. Because the
/// working values are static, they survive destruction
///
//*****************************************************************************
class LWPPGlobal
{
  protected:
	//=========================================================================
	/// I made storage for these values static because it seems
	/// wasteful and useless for each global to have its own
	/// identical copy of these values that don't change during
	/// a session. Once the constructor is called, these values
	/// will live on, even after destruction.
	//
	/* @{ */
  	static GlobalFunc   *GLOBAL;
	static unsigned long SystemID;
	static unsigned long ProductInfo;
	static unsigned long LocaleInfo;
	/* @} */
	/// Other variables we capture
	/* @{ */
	long                 pluginVersion;
	/// Should we capture this here, or does it rightly
	/// belong in the plugin wrapper?
	//
	// void				*local;
	/* @} */

  public:

	//=========================================================================
	/// Constructor, where all the magic happens.
	//
	LWPPGlobal(GlobalFunc* global, long version);

	//=========================================================================
	/// Do nothing default constructor. It's required for constructing derived
	/// objects, but if you haven't called the other constructor, your derived
	/// objects are dead in the water.
	//
	LWPPGlobal(){}

	//=========================================================================
	/// Destructor - does nothing.
	//
	virtual ~LWPPGlobal() {/* Nothing to do here */}

	//=========================================================================
	/// Copy constructor - Prevent explicit copies.
	/// You will get a linker error if you call this.
	//
	LWPPGlobal(LWPPGlobal&);

	//=========================================================================
	/// Assignment operator - Prevent explicit copies
	/// You will get a linker error if you call this.
	//
	LWPPGlobal & operator =(LWPPGlobal&);

	//=========================================================================
	/// isValid - Lets you know if a global call will work or fail.
	///
	/// \todo - Check to see if any of the "info" values could
	///         be 0 under valid circumstances.
	///
	virtual bool isValid()
	{
		return (GLOBAL != NULL &&
				SystemID       &&
				ProductInfo    &&
				LocaleInfo);
	}

	//=========================================================================
	/// operator ! - A synonym for checking isValid()
	///   like this:
	/// LWPPGlobal myGlobal(global);
	/// if (!myGlobal)
	/// {
	///   return AFUNC_BADGLOBAL;
	/// }
	///
	/// Be aware that if you construct one of these on the heap
	/// you'll have to change the syntax accordingly. I'm thinking
	/// "if (! *myGlobal)", but I haven't tested this. Otherwise
	/// you're just checking for a non-NULL address, and
	/// not actually calling isValid().
	///
	/// Another alternative for heap based construction would be:
	/// if (myGlobal->operator !())
	/// but from that, you see the following is cleaner syntax:
	/// if (! myGlobal->isValid())
	/// Tested OK
	//
	bool operator !()
	{
		// Because isValid() is a virtual call, you can use
		// "if (!anyGlobal) {}" with any derived type.
		//
		return !isValid();
	}

	//=========================================================================
	/// application - possible return values are
	///   - LWSYS_LAYOUT
	///   - LWSYS_MODELER
	///   - LWSYS_SCREAMERNET
	/// Tested OK
	//
	unsigned long application()
	{
		// return (SystemID & LWSYS_TYPEBITS) >> 28;
		//
		/// Because LWSYS_ constants are before the value is shifted right
		/// I'm returning the value unshifted. User friendly functions
		/// should be added.
		//
		return SystemID & LWSYS_TYPEBITS;
	}

	//=========================================================================
	/// dongleID - returns the dongle number. If the application
	/// is screamernet, returns the node ID.
	/// Tested OK
	//
	unsigned long dongleID()
	{
		return SystemID & LWSYS_SERIALBITS;
	}

	//=========================================================================
	/// product - possible return values are
	///   - LWINF_PRODLWAV  (1)
	///   - LWINF_INSP3D    (2)
	///   - LWINF_PRODOTHER	(4)
	/// Tested OK
	//
	unsigned long product()
	{
		return ProductInfo & LWINF_PRODUCT;
	}

	//=========================================================================
	/// build - returns the Build number of the product.
	///
	/// Known build numbers - Special Thanks to Lynx3D for collecting these
	///		Windows & Mac
	///
	///		6.0:   429
	///		6.0b:  446
	///
	///		6.5:   471, 484 (?)
	///		6.5b:  508
	///
	///		7.0:   537
	///		7.0b:  543
	///
	///		7.5:   572
	///		7.5b:  582
	///		7.5c:  584
	///		7.5d:  598
	///
	///		8.0:   690
	///		8.0.1: 734
	///
	/// If you know any build numbers for any platform which are not
	/// listed here, please report them to LightWrap++
	/// Tested OK
	///
	unsigned long build()
	{
		/// Using the macro defined in "lwhost.h"
		//
		return LWINF_GETBUILD(ProductInfo);
	}

	//=========================================================================
	/// revisionMinor - returns the minor revision number.
	/// Tested OK
	//
	unsigned long revisionMinor()
	{
		/// Using the macro defined in "lwhost.h"
		//
		return LWINF_GETMINOR(ProductInfo);
	}

	//=========================================================================
	/// revisionMajor - returns the major revision number.
	/// Tested OK
	//
	unsigned long revisionMajor()
	{
		/// Using the macro defined in "lwhost.h"
		//
		return LWINF_GETMAJOR(ProductInfo);
	}

	//=========================================================================
	/// version - returns the version number passed to the constructor.
	///
	/// I would like to convert this to an unsigned long, but existing code
	/// may depend on it's original type.
	///
	/// Tested OK
	//
	long version()
	{
		/// The version variable passed to the activate function.
		//
		return pluginVersion;
	}

	//=========================================================================
	/// revision - returns a user friendly string.
	//
	const char *revision();

	//=========================================================================
	/// locale - returns the language ID.
	//
	unsigned long locale()
	{
		return LocaleInfo & LWLOC_LANGID;
	}
}; // End - class LWPPGlobal

//*****************************************************
//
/// \class LWPPFileRequester -
///   encapsulates the following globals:
///   LWFileReqFunc
///   LWFileActivateFunc
///   LWFileTypeFunc
///   LWDirInfoFunc
///
/// \todo - Create easy to use calls like
///   "loadAnimation(...)", "saveObject(...)"
//
//*****************************************************
class LWPPFileRequester : public LWPPGlobal
{
	/// Pointers to the functions returned from GLOBAL
	//
	/* @{ */
	LWFileReqFunc      *fileRequestFunc;
	LWFileActivateFunc *fileActivateFunc;
	LWFileTypeFunc     *fileTypeFunc;
    LWDirInfoFunc	   *dirInfoFunc;
	/* @} */
  public:

	//=========================================================================
	/// Constructor, make sure the GLOBAL func is good,
	/// then acquire each of the fileRequest functions.
	//
	LWPPFileRequester()
	{
		fileRequestFunc  = NULL;
		fileActivateFunc = NULL;
		fileTypeFunc     = NULL;
		dirInfoFunc      = NULL;
		if (GLOBAL != NULL)
		{
			fileRequestFunc =
				(LWFileReqFunc*)GLOBAL(LWFILEREQFUNC_GLOBAL, GFUSE_ACQUIRE);
			fileActivateFunc =
				(LWFileActivateFunc*)GLOBAL(LWFILEACTIVATEFUNC_GLOBAL, GFUSE_ACQUIRE);
			fileTypeFunc =
				(LWFileTypeFunc*)GLOBAL(LWFILETYPEFUNC_GLOBAL, GFUSE_ACQUIRE);
			dirInfoFunc =
				(LWDirInfoFunc*)GLOBAL(LWDIRINFOFUNC_GLOBAL, GFUSE_ACQUIRE);
		}
	}

	//=========================================================================
	/// Destructor, if any of the fileRequest functions is non-NULL,
	/// then we know that GLOBAL must be good, so just release the
	/// non-NULL functions.
	//
	virtual ~LWPPFileRequester()
	{
		if (fileRequestFunc)
			GLOBAL(LWFILEREQFUNC_GLOBAL, GFUSE_RELEASE);
		if (fileActivateFunc)
			(LWFILEACTIVATEFUNC_GLOBAL, GFUSE_RELEASE);
		if (fileTypeFunc)
			GLOBAL(LWFILETYPEFUNC_GLOBAL, GFUSE_RELEASE);
		if (dirInfoFunc)
			GLOBAL(LWDIRINFOFUNC_GLOBAL, GFUSE_RELEASE);
	}

	//=========================================================================
	/// isValid - verifies that all fileRequest functions
	/// were properly initialized. It seems a little extreme that if
	/// one fails, they're all invalid, but they're typically used
	/// together.
	///
	/// They could be broken up into separate globalTypes.
	//
	bool isValid()
	{
		return (fileRequestFunc &&
		        fileActivateFunc &&
		        fileTypeFunc &&
		        dirInfoFunc);
	}

	//=========================================================================
	/// LW's basic "File Request" global.
	/// friendly overrides will be added later.
	//
	int fileRequest(const char *title, char *filter, char *path,
					char *returnedFileName, int buflen)
	{
		// typedef int LWFileReqFunc (const char *hail, char *name,
		//                      char *path, char *fullName, int buflen);
		if (fileRequestFunc)
			return fileRequestFunc(title, filter, path,
									returnedFileName, buflen);
    	return AFUNC_BADGLOBAL;
	}

	//=========================================================================
	/// LW's basic "File Request 2" global.
	/// user friendly overrides will be added later.
	//
	int fileRequest2(int version, LWFileReqLocal *requestLocal)
	{
		// int LWFileActivateFunc (int version, LWFileReqLocal *requestLocal);
    	if (fileActivateFunc)
    		return fileActivateFunc(version, requestLocal);
    	return AFUNC_BADGLOBAL;
	}

	//=========================================================================
	/// LW's basic dirInfoFunc, seems user friendly enough.
	//
	const char *directoryInfo(const char *directoryType)
	{
		// const char * LWDirInfoFunc (const char *directoryType);
		if (dirInfoFunc)
			return dirInfoFunc(directoryType);
		return NULL;
	}

	//=========================================================================
	/// LW's basic fileTypeFunc, returns a filter for use in fileRequests.
	//
	const char *fileType(const char *type)
	{
		// const char * LWFileTypeFunc (const char *type);
		if (fileTypeFunc)
			return fileTypeFunc(type);
		return NULL;
	}
	/// \todo Add user friendly file requesters
}; // End - class LWPPFileRequester.

//*****************************************************
//
/// \class - LWPPColorActivate, encapsulates
/// Lightwave's LWColorActivateFunc.
///
/// \todo Couldn't there be a user friendly version?
//
//*****************************************************
class LWPPColorActivate : public LWPPGlobal
{
	LWColorActivateFunc *colorActivateFunc;
  public:

	//=========================================================================
	/// Acquire on construction
	//
	LWPPColorActivate() : colorActivateFunc(NULL)
	{
		if (GLOBAL)
		{
			colorActivateFunc = (LWColorActivateFunc*)
				GLOBAL(LWCOLORACTIVATEFUNC_GLOBAL, GFUSE_ACQUIRE);
		}
	}

	//=========================================================================
	/// Release on destruction
	//
	virtual ~LWPPColorActivate()
	{
		if (colorActivateFunc)
			GLOBAL(LWCOLORACTIVATEFUNC_GLOBAL, GFUSE_RELEASE );
	}

	//=========================================================================
	/// isValid() - Basic test for non-dangling pointer
	//
	bool isValid()
	{
		return (colorActivateFunc != NULL);
	}

	//=========================================================================
	/// Since there is only one function associated with this global
	/// it seemed a nicety to simplify the syntax to:
	///   "LWPPColorActivate colorActivate;"
	///   "colorActivate(...);"
	int operator ()(int version, LWColorPickLocal *colorPickLocal)
	{
		if (isValid())
		{
			return colorActivateFunc(version, colorPickLocal);
		}
		else
		{
			return AFUNC_BADGLOBAL;
		}
	}
};

//*****************************************************
//
/// \class - LWPPMessageFuncs
///
///	 Encapsulates the LWMessageFuncs global type
///    The basic syntax is preserved, but one return
///    value has been added to the interactive
///    functions: -1 is returned in case of a bad
///    global since AFUNC_BADGLOBAL would duplicate
///    a normal return value.
//
//*****************************************************
class LWPPMessageFuncs : public LWPPGlobal
{
	/// The globalType pointer.
	//
	LWMessageFuncs *messageFuncs;
  public:

	//=========================================================================
	/// Acquire on construction.
	/// Tested OK
	//
  	LWPPMessageFuncs() : messageFuncs(NULL)
	{
		if (GLOBAL)
		{
			messageFuncs = (LWMessageFuncs*)
				GLOBAL(LWMESSAGEFUNCS_GLOBAL, GFUSE_ACQUIRE);
		}
	}

	//=========================================================================
	/// Release on destruction.
	//
  	virtual ~LWPPMessageFuncs()
	{
		if (messageFuncs)
			GLOBAL(LWMESSAGEFUNCS_GLOBAL, GFUSE_RELEASE);

	}

	//=========================================================================
	/// isValid() - Basic test for non-dangling pointer
	/// Tested OK
	//
	bool isValid()
	{
		return (messageFuncs != NULL);
	}

	//=========================================================================
	/// Basic call to info(...)
	/// Tested OK
	//
	void info(const char *firstLine, const char *secondLine = NULL)
	{
		if (isValid())
			messageFuncs->info(firstLine, secondLine);
	}

	//=========================================================================
	/// Basic call to error(...)
	/// Tested OK
	//
	void error(const char *firstLine, const char *secondLine = NULL)
	{
		if (isValid())
			messageFuncs->error(firstLine, secondLine);
	}

	//=========================================================================
	/// Basic call to warning(...)
	/// Tested OK
	//
	void warning(const char *firstLine, const char *secondLine = NULL)
	{
		if (isValid())
			messageFuncs->warning(firstLine, secondLine);
	}

	//=========================================================================
	/// okCancel() - Returning AFUNC_BADGLOBAL would appear to be
	/// a user response, so in case of a bad global, this function returns -1
	//
	int okCancel(const char *title, const char *firstLine,
					const char *secondLine = NULL)
	{
		if (isValid())
		{
			return messageFuncs->okCancel(title != NULL ? title : "OK ?",
											firstLine, secondLine);
		}
		else
			return -1;
	}

	//=========================================================================
	/// yesNo() - Returning AFUNC_BADGLOBAL would appear to be
	/// a user response, so in case of a bad global, this function returns -1
	/// Tested OK
	//
	int yesNo(const char *title, const char *firstLine,
				const char *secondLine = NULL)
	{
		if (isValid())
		{
			messageFuncs->yesNo(title != NULL ? title : "OK ?",
											firstLine, secondLine);
		}
		else
			return -1;
	}

	//=========================================================================
	/// yesNoCan() - Returning AFUNC_BADGLOBAL would appear to be
	/// a user response, so in case of a bad global, this function returns -1
	//
	int yesNoCan(const char *title, const char *firstLine,
					const char *secondLine = NULL)
	{
		if (isValid())
		{
			messageFuncs->yesNoCan(title != NULL ? title : "OK ?",
											firstLine, secondLine);
		}
		else
			return -1;
	}

	//=========================================================================
	/// yesNoAll() - Returning AFUNC_BADGLOBAL would appear to be
	/// a user response, so in case of a bad global, this function returns -1
	//
	int yesNoAll(const char *title, const char *firstLine,
					const char *secondLine = NULL)
	{
		if (isValid())
		{
			messageFuncs->yesNoAll(title != NULL ? title : "OK ?",
											firstLine, secondLine);
		}
		else
			return -1;
	}

};

} // End - namespace lwpp

#endif // LWPP_GLOBALWRAP
