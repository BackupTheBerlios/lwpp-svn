#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include "lwpp_xpanel.h"
#include "lwpp_panel.h"

namespace lwpp
{

//! Class for Imagefilters
/*!
 * @note LWFilterAccess is still the original struct and not replaced by the wrapper with a class
 */
class ImageFilterHandler :
	public InstanceHandler,
	public ItemHandler
{
	public:
	  ImageFilterHandler(GlobalFunc *g, void *context, LWError *err) 
		: InstanceHandler(g, context, err)
		{;}
	  void Process(const LWFilterAccess *fa);
	  virtual unsigned int Flags() {return 0;}
};

//! Class for Imagefilters with a LWPanel
/*!
 */
class ImageFilterPanelHandler :
	public ImageFilterHandler,
	public PanelInterfaceHandler
{
	public:
	  ImageFilterPanelHandler(GlobalFunc *g, void *context, LWError *err) 
		: ImageFilterHandler(g, context, err)
		{;}
};

//! Class for Imagefilters with an XPanel
/*!
 */
class ImageFilterXPanelHandler :
	public ImageFilterHandler,
	public XPanelInterfaceHandler
{
	public:
	  ImageFilterXPanelHandler(GlobalFunc *g, void *context, LWError *err) 
		: ImageFilterHandler(g, context, err)
		{;}
};


/*! @addtogroup Adaptor Static Adaptor */
//! Wrapper for an ImageFilterHandler
/*!
 * @ingroup Adaptor
 */
template <class T> 
class ImageFilterAdaptor :
private InstanceAdaptor <T>,
private ItemAdaptor <T>
{
  public:
	//! Set static callbacks for a LightWave ImageFilter
	static int Activate (long version, GlobalFunc *global, void *l, void *serverData)
	{
	  if ( version != LWIMAGEFILTER_VERSION ) return AFUNC_BADVERSION;
	  
	  LWImageFilterHandler *local = (LWImageFilterHandler *)l;

	  InstanceAdaptor<T>::Activate(local->inst, global);
	  ItemAdaptor <T>::Activate(local->item);
	  local->process = ImageFilterAdaptor::Process;
	  local->flags   = ImageFilterAdaptor::Flags;

	  return AFUNC_OK;
	}
  private:
	static void Process (LWInstance instance, const LWFilterAccess *fa)
	{
	  T *plugin = (T *) instance;
	  plugin->Process(fa);
	}

	static unsigned int Flags (LWInstance instance)
	{
	  T *plugin = (T *) instance;
	  return plugin->Flags();
	}
};

//! Wrapper for an ImageFilterPanelHandler
/*!
 * @ingroup Adaptor
 */
template <class T> 
class PanelImageFilterAdaptor :
public ImageFilterAdaptor <T>,
public PanelInterfaceAdaptor <T>
  {};

//! Wrapper for an ImageFilterXPanelHandler
/*!
 * @ingroup Adaptor
 */
template <class T> 
class XPanelImageFilterAdaptor :
public ImageFilterAdaptor <T>,
public XPanelInterfaceAdaptor <T>
{
	static int ActivateUI (long version, GlobalFunc *global, LWInstance u, void *serverData) 
	{
	  LWInterface *ui = (LWInterface *) u;
	  dostream d;
  	  d << "Activate UI for: " << ui->inst << "\n";
	  return XPanelInterfaceAdaptor<T>::ActivateUI(version, global, u, serverData);
	}
	static int Activate (long version, GlobalFunc *global, LWInstance l, void *serverData) 
	{
	  dostream d;
  	  d << "Activate ImageFilterHandler at: " << l << "\n";
	  return ImageFilterAdaptor<T>::Activate
		(version, global, l, serverData);
	}
};

}

#endif // IMAGEFILTER_H