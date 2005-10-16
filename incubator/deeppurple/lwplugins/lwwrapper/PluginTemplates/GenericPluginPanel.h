// GlobalPluginPanel.h: interface for the GlobalPluginPanel class.
//
//////////////////////////////////////////////////////////////////////

#ifndef GLOBALPLUGINPANEL_INCLUDED
#define GLOBALPLUGINPANEL_INCLUDED


#include "../Panel/Panel.h"
#include "../../lwinclude/lwgeneric.h"


class GenericPluginPanel : 
public Panel  
{
public:
	GenericPluginPanel(const char *name="LayoutGeneric", int INFlags=0) : Panel(name,INFlags){}

	void SetGlobalLocal(LWLayoutGeneric *local)
	{
		Local=(*local);
	}

	LWLayoutGeneric Local;
};

#endif // GLOBALPLUGINPANEL_INCLUDED
