#ifndef LWPP_XPANEL_H
#define LWPP_XPANEL_H

#include "lwpp_handler.h"

namespace lwpp
{

//! XPanel Interface
class XPanelInterfaceHandler
: public InterfaceHandler
{
	public:
		virtual LWXPanelID XPanel() {return NULL;}
		virtual void *  XGet(unsigned long vid ) {return NULL;}
		virtual LWXPRefreshCode XSet(unsigned long vid, void *value ) {return LWXPRC_DFLT;}
};

//! Wrapper for XPanelInterfaceHandler
/*!
 * @ingroup Adaptor
 */
template <class T>
class XPanelInterfaceAdaptor
: private InterfaceAdaptor<T>
{
  public:
	//! Set static callbacks for an XPanel
	static int ActivateUI (long version, GlobalFunc *global, LWInstance u, void *serverData) {
	  LWInterface *ui = (LWInterface *) u;
	  InterfaceAdaptor<T>::ActivateUI(ui); // Activate base class
	  dostream d;
	  //! @bug somehow ui->inst is not the class we create before... how, why? Templates?
  	  d << "Return ui for: " << ui->inst << "\n";
	  ui->panel = NULL;
	  //ui->panel = ((T *)ui->inst)->XPanel();
	  ui->options = NULL;
	  return AFUNC_OK;
	}
	//! Move data to XPanel
	static void *XGet( LWInstance inst, unsigned long vid )
	{
	  return ((T *)inst)->XGet(vid);
	}
	//! Return data from XPanel
	static LWXPRefreshCode XSet( LWInstance inst, unsigned long vid, void *value )
	{
	  return ((T *)inst)->XSet(vid, value);
	}
};

}

#endif // LWPP_XPANEL_H