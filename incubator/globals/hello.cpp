/*
======================================================================
hello.c - A simple LayoutGeneric example.

	Ernie Wright  20 Mar 00

hello.cpp - Heavily modified to test LWPPGlobal global wraps.
			1/5/05 - More changes and cosmetics.
====================================================================== */

#include "globalWrap.h"          ///< LWPPGlobal Mother of all globals
						         ///< & LWPPMessageFuncs
#include "lwppRender.h"          ///< LWPPInterfaceInfo

#include <lwgeneric.h>           /* for the LayoutGeneric class   */
#include <lwpanel.h>             /* for "classic" panels          */

#include <stdio.h>               /* for NULL #define              */
#include <string.h>              // strlen

LWPanelFuncs *panf;              /* the panel functions           */
LWPanelID panel;                 /* the panel                     */
LWControl *ctl;                  /* a control on the panel        */
LWPanControlDesc desc;           /* used by macros in lwpanel.h   */
LWValue sval = { LWT_STRING };   /*  read lwpanel.h to see how    */
char edit[ 80 ] =                /* string for the edit control   */
	"This is an edit field.";


/*
======================================================================
DemoGeneric()

The activation function.  Layout calls this when the user selects the
generic from the generic list on the General Options panel.
====================================================================== */

XCALL_( int )
DemoGeneric( long version, GlobalFunc *global, void *Local,
				void *serverData )
{
	/// that cheesy pointer casting I hope we will get rid of
	//
	LWLayoutGeneric *local = (LWLayoutGeneric *)Local;

	using namespace lwpp;

	int returnValue = AFUNC_OK;
	/// The secret of global usage, construct an LWPPGlobal, passing it
	/// the globalFunc pointer and version. Once this is done, every
	/// aspect of version/revision/locale can be obtained from the
	/// LWPPGlobal
	//
	LWPPGlobal testGlobal(global, version);

	/// Once a proper initialization has occured, it propogates to all
	/// LWPPGlobals
	//
	if (!testGlobal)
	{
		returnValue = AFUNC_BADGLOBAL;
		return returnValue;
	}

	/// A globalType, used to read the user's alert level
	//
	LWPPInterfaceInfo interfaceInfo;

	/// OK, we've made it this far, global is good.
	//
	LWPPMessageFuncs message; // ======= Test LWPPMessageFuncs.
	/// For any guaranteed use, it's necessary to test "isValid()".
	/// These tests are built into info(), error() and warning()
	/// that doesn't mean the call will succeed, it just won't crash.
	///
	/// Notice I'm using this func without testing whether it's valid!
	/// This is not likely to be heavily trafficed code, so it has an
	/// internal validity test. It's valid. or it doesn't work,
	/// but it doesn't crash if it's not valid.
	//
	message.info("We've made it ...", "this far."); // Test info()

	// test virtual resolution of operator !() which is defined in
	// the base class (LWPPGlobal). (Operator !() is not virtual,
	// but isValid() is ... see the code.
	//
	if (!message || !interfaceInfo)
	{
		returnValue = AFUNC_BADGLOBAL;
		return returnValue;
	}

	// Capture the users alert level setting. If it's anything other
	// than beginner, set it to beginner so that the following message
	// funcs become interactive.
	//
	int alertLevel;
	char alertString[13];

	// Make sure that the alertLevel call is supported by the running
	// version of Lightwave.
	//
	alertLevel = interfaceInfo.alertLevel();
	switch (alertLevel)
	{
	case -1:
		// Unsupported
		//
		break;
	case 0:
		// Users alert level is already beginner, so there's no need to
		// alter it.
		//
		break;
	case 1: // User alert level intermediate.
	case 2: // User alert level expert.
		strcpy(alertString, "AlertLevel 0");
		local->evaluate(local->data, alertString);
		break;
	}

	int answer;
	// ============================================ Test yesNo()

	answer = message.yesNo("Reality check","Operator !() says global is",
					(returnValue == AFUNC_OK) ? "OK" : "BAD");

	///             3          2    1   0        -1
	/// ---------------------------------------------------
	/// okCancel    -          -    OK  Cancel   Bad_Global
	/// yesNo       -          -    Yes No       Bad_Global
	/// yesNoCan    -          Yes  No  Cancel   Bad_Global
	/// yesNoAll    Yes to All Yes  No  Cancel   Bad_Global

	switch (answer)
	{
		case -1:
			returnValue = AFUNC_BADGLOBAL;
			return returnValue;
			break; // Test compiler's "unreachable code warning"
		case 0:
			message.warning("You're answer was NO"); // test warning
			break;
		case 1:
			message.error("You're answer was YES");	// test error
			break;
		case 2:
		case 3:
		default:
			message.info("Hmmmm", "Unexpected returnValue from yesNo");
			break;
	}
	int ok;

	/*	Make sure Layout calls us with the right version of the
		LWLayoutGeneric structure.  Because of a version numbering bug
		in build 429, we need to take a couple of extra steps in generic
		class plug-ins to verify the version. */

	/// WHAT !
	/// Speaking of bugs, here's an extraction of the build from the
	/// systemID ... there are a couple of bugs here, not the least
	/// of which is that the build number is stored in LWProductInfo
	/// not LWSystemID. The other bug is that the build number has to
	/// be shifted left 4 bits before it could ever be an odd number.
	//
	// sysid = ( unsigned long ) global( LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT );
	// ver = ( sysid & LWINF_BUILD ) == 429 ? 2 : 4;
	// if ( version != ver ) return AFUNC_BADVERSION;
	//
	if (testGlobal.build() == 429 && testGlobal.version() != 2)
		return AFUNC_BADVERSION;

	message.info(testGlobal.revision());

	/* get the message and panels functions */

	panf = (LWPanelFuncs *)global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
	if ( !panf )
		return AFUNC_BADGLOBAL;

	/* initialize the panels functions and create a panel */

	panf->globalFun = global;

	panel = PAN_CREATE( panf, "Hello World!" );
	if ( !panel ) return AFUNC_BADGLOBAL;

	/* create an edit field on the panel and initialize its contents */

	ctl = STR_CTL( panf, panel, "Edit Me", 40 );
	SET_STR( ctl, edit, sizeof( edit ));

	/* display the panel; this waits until the user closes it */

	ok = panf->open( panel, PANF_BLOCKING | PANF_CANCEL );

	/* ok is TRUE if the user pressed OK, FALSE if he or she pressed Cancel */

	if ( ok )
		GET_STR( ctl, edit, sizeof( edit ));

	/* free the panel */

	PAN_KILL( panf, panel );

	/* display the contents of the edit string */

	message.warning( "The edit string contains:", edit );

	/* while we're here, let's issue a command */

	if (message.yesNo( "It's up to you ...",
		"Issue the \"About\" command now?") == 1)
			local->evaluate( local->data, "About" );

	// Put the alertLevel back where we found it.
	//
	if (alertLevel > 0)
	{
		alertLevel += 0x30;
		alertString[strlen(alertString) - 1] = (char)alertLevel;
		local->evaluate( local->data, alertString );
	}
	/* done! */

	return returnValue;
}


/*
======================================================================
This is the server description.  LightWave looks at this first to
determine what plug-ins the file contains.  It lists each plug-in's
class and internal name, along with a pointer to the activation
function.  You can optionally add a user name, or more than one in
different languages, if you like.
====================================================================== */

ServerRecord ServerDesc[] = {
	{ LWLAYOUTGENERIC_CLASS, "HelloWorld", DemoGeneric },
	{ NULL }
};
