
#ifndef _LWPIPANEL_H
#define _LWPIPANEL_H

#include "LWPPanel.h"


//// high-level wraper for LWPanels [DOCUMENTME] ////

class LWPIPanel : public LWPPanel
{
private:
	static XCALL_(void) Close(LWPanelID panel, void * data)
	{
		LWPIPanel * ipanel = 0;
		LWPPlugin::panelFuncs->get(panel, PAN_USERDATA, &ipanel);
		ipanel->close();
	}
	static XCALL_(void) Keys(LWPanelID panel, void * data, LWDualKey key)
	{
		LWPIPanel * ipanel = 0;
		LWPPlugin::panelFuncs->get(panel, PAN_USERDATA, &ipanel);
		ipanel->keys(key);
	}
	static XCALL_(void) Draw(LWPanelID panel, DrMode mode)
	{
		LWPIPanel * ipanel = 0;
		LWPPlugin::panelFuncs->get(panel, PAN_USERDATA, &ipanel);
		ipanel->drawSub(mode);
	}
public:
	LWPIPanel() {}
	virtual ~LWPIPanel()
	{
		if (panel)
		{
		    close();
			destroy();
			cleanUp();
		}
	}
	virtual int init(char const * title, void * data)
	{
		if (panel)
		{
			close();
			destroy();
			cleanUp();
		}
		if (!LWPPanel::init(title, data)) return 0;
		// associate the panel with this ipanel
		// [FIXME] does this mean the "data" parameter is really unused?
		setData(this);
		setClose(Close);
		setKeys(Keys);
		setDraw(Draw);
		return 1;
	}
	virtual void cleanUp() {}
	virtual void close() {}
	virtual void keys(LWDualKey) {}
	virtual void drawSub(DrMode) {}
};

#endif
