// ControlXColor.h: interface for the ControlXColor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLXCOLOR_INCLUDED
#define CONTROLXCOLOR_INCLUDED

#include "Control.h"

class ControlXColor : public Control  
{
public:
	ControlXColor();
	virtual ~ControlXColor();
    virtual bool RegisterWithPanel(LWPanelID pan);
    virtual void DoNotifyOwner(bool NotifyOwner=true);

    LWXPanelID ThisXPanel;

    double color[3];
    LWVParmID Color;

    static LWXPanelHint ColorHint[];

    static void ChangeNotify_Callback (LWXPanelID pan, unsigned long cid, unsigned long vid, int event_type);

//   static void *Get_Callback (void *inst, unsigned long vid);
//   static LWXPRefreshCode   Set_Callback (void *inst, unsigned long vid,void *value);
};

#endif // CONTROLXCOLOR_INCLUDED
