#pragma once
#include "control.h"

class ControlButton : 
public Control  
{
public:
   ControlButton(void);
   virtual ~ControlButton(void);

   virtual void Initialize(const char *name, Panel* SetOwner, int width=200);
   bool RegisterWithPanel(LWPanelID pan);
protected:
   int Width;

};
