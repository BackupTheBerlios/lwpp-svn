#ifndef LWPP_PANEL_H
#define LWPP_PANEL_H

#include "lwpp_handler.h"

namespace lwpp
{

//! Base class for a Lightwave UI
class PanelInterfaceHandler
: public InterfaceHandler
{
	public:
		virtual LWError Panel() {return NULL;}
};

//! Wrapper for PanelInterfaceHandler
/*!
 * @ingroup Adaptor
 */
template <class T>
class PanelInterfaceAdaptor
: private InterfaceAdaptor<T>
{
  public:
	//! Set static callbacks for the LightWave item handler
	static int ActivateUI (long version, GlobalFunc *global, void *u, void *serverData) {
	  LWInterface *ui = (LWInterface *) u;
	  ui->options = PanelInterfaceAdaptor::Options;
	  ui->panel = NULL;
	  return pInterfaceAdaptor<T>::ActivateUI(ui);
	}
  private:
	static LWError Options (LWInstance instance)
	{
		T *plugin = (T *) instance;
		return plugin->Panel();
	}
};

}

#endif // LWPP_PANEL_H