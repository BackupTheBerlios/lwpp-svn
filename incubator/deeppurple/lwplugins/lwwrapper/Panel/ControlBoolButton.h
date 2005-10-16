// ControlBoolButton.h: interface for the ControlBoolButton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLBOOLBUTTON_INCLUDED
#define CONTROLBOOLBUTTON_INCLUDED

#include "Control.h"

class ControlBoolButton : 
public Control     
{
public:
	ControlBoolButton();
	virtual ~ControlBoolButton();
   bool RegisterWithPanel(LWPanelID pan);
   int GetValue();
   void SetValue(int Val);
};

#endif // CONTROLBOOLBUTTON_INCLUDED
