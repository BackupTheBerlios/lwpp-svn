
#ifndef _LWPUTILS_H
#define _LWPUTILS_H

//// Useful LW-specific constants ////

#define LWPINFINITY 2.0e+014		// distance from the Camera to the Backdrop in Layout in L[6]
									// [NOTE] infinity in L5.6 is only 1300000014336.0
#define LWPNUDGE 0.0000004			// approximate thickness of a polygon in Layout (not *that* useful)

//// Useful additions to the LWPanels macros ////

#define CON_SETX(ctl,x) 	  ( (ival.intv.value=x,(*ctl->set)(ctl, CTL_X, &ival)) )	   
#define CON_SETUSERDATA(ctl,d) ( (ival.intv.value=(int)d,(*ctl->set)(ctl,CTL_USERDATA,&ival)) )
#define CON_SETUSEREVENT(ctl,f) ( (ival.intv.value=(int)f,(*ctl->set)(ctl,CTL_USEREVENT,&ival)) )
#define CON_SETUSERDRAW(ctl,f) ( (ival.intv.value=(int)f,(*ctl->set)(ctl,CTL_USERDRAW,&ival)) )

#endif
