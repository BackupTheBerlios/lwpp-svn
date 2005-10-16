#pragma once
#include "control.h"

//##ModelId=3E16FCD6034E
class ControlCheckBox : 
public Control  
{
public:
	//##ModelId=3E16FCD60362
   ControlCheckBox(void);
	//##ModelId=3E16FCD60364
   virtual ~ControlCheckBox(void);
	//##ModelId=3E16FCD6036E
   virtual bool RegisterWithPanel(LWPanelID pan);
	//##ModelId=3E16FCD60378
   bool SetValue(bool onoff);
	//##ModelId=3E16FCD60381
   bool GetValue(void);
	//##ModelId=3E16FCD6038B
   int Width;
};
