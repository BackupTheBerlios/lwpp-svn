#ifndef LWPP_HANDLER_H
#define LWPP_HANDLER_H

#include <lwhost.h>
#include <lwserver.h>
#include <lwfilter.h>
#include <lwxpanel.h>

#include "lwpp_debug.h"

//! LightWrap++
namespace lwpp
{

//! Base class for any LW plugin
class InstanceHandler
{
  private:
	const char *m_description;
	const char *m_pluginName;
  public:
	GlobalFunc *global;
	InstanceHandler(GlobalFunc *g, void *context, LWError *err)
	{
		global = g;
		return;
	}
	//! Load Settings
	virtual LWError Load(const LWLoadState *ls) {return NULL;}
	//! Save Settings
	virtual LWError Save(const LWSaveState *ls) {return NULL;}	
	virtual const char*DescLn() {return NULL;}
	virtual void SetName(const char *n) {m_description = n;}
};

/*! @defgroup Adaptor Static Adaptor Classes */

//! Wrapper for InstanceHandler
/*!
 * @ingroup Adaptor
 */
template <class T>
class InstanceAdaptor
{
  protected:
	//! Set static callbacks for the LightWave instance handler
	static void Activate(LWInstanceFuncs *inst, GlobalFunc *global) {
	  inst->create	= InstanceAdaptor::Create;
	  inst->destroy	= InstanceAdaptor::Destroy;
	  inst->copy	= InstanceAdaptor::Copy;
	  inst->load	= InstanceAdaptor::Load;
	  inst->save	= InstanceAdaptor::Save;
	  inst->descln	= InstanceAdaptor::DescLn;
	  inst->priv	= global; // pass on the global
	}
  private:
	static LWInstance Create (void *priv, void *context, LWError *err)
	{
	  T *plugin = new T((GlobalFunc *)priv, context, err);
	  dostream d;
	  d << "Created new plugin at and passing it to LW: " << plugin << "\n";
	  d << "GlobalFunc: " << priv << "\n";

	  return (LWInstance) plugin;
	  //return new T((GlobalFunc *)priv, context, err);
	}

	static void Destroy (LWInstance instance)
	{
	  T *plugin = (T *) instance;
	  delete plugin;
	}

	static LWError Copy (LWInstance instance, LWInstance instance2)
	{
	  T *to = (T *) instance;
	  T *from = (T *) instance2;
	  to = from;
	  return NULL;
	}

	static LWError Load (LWInstance instance, const LWLoadState *ls)
	{
	  T *plugin = (T *) instance;
	  return plugin->Load(ls);
	}

	static LWError Save (LWInstance instance, const LWSaveState *ss)
	{
	  T *plugin = (T *) instance;
	  return plugin->Save(ss);
	}

	static const char *DescLn (LWInstance instance)
	{
		T *plugin = (T *) instance;
		return plugin->DescLn();
	}
};

//! LW ItemFuncs class extension
class ItemHandler
{
	public:
		virtual const LWItemID *UseItems()
		{
			static LWItemID id = 0;
			return &id;
		}
		virtual void ChangeID(const LWItemID *) {;}
};

//! Wrapper for ItemHandler
/*!
 * @ingroup Adaptor
 */
template <class T>
class ItemAdaptor
{
  protected:
	//! Set static callbacks for the LightWave item handler
	static void Activate(LWItemFuncs *inst) {
		inst->useItems = ItemAdaptor::UseItems;
		inst->changeID = ItemAdaptor::ChangeID;
	}
  private:
	static const LWItemID *UseItems (LWInstance instance)
	{
		T *plugin = (T *) instance;
		return plugin->UseItems();
	}

	static void ChangeID (LWInstance instance, const LWItemID *ids)
	{
		T *plugin = (T *) instance;
		plugin->ChangeID(ids);
	}
};

//! Base class for a Lightwave UI
class InterfaceHandler
{
	public:
		virtual LWError Command(const char *command) {return NULL;}
};

//! Wrapper for InterfaceHandler
/*!
 * @ingroup Adaptor
 */
template <class T>
class InterfaceAdaptor
{
  protected:
	//! Set static callbacks for the LightWave item handler
	static int ActivateUI(LWInterface *ui) {
	  	dostream d;
    	d << "InterfaceAdaptor inst = " << ui->inst << "\n";
		ui->command = InterfaceAdaptor::Command;
		return AFUNC_OK;
	}
  private:
	static LWError Command (LWInstance instance, const char* command)
	{
		T *plugin = (T *) instance;
		return plugin->Command(command);
	}
};

}
#endif // LWPP_HANDLER_H