#pragma once
#include "control.h"

class ControlInteger : 
public Control  
{
public:
   ControlInteger(void);
   virtual ~ControlInteger(void);
   virtual bool RegisterWithPanel(LWPanelID pan);
   virtual void Initialize(const char *name, Panel *SetOwner, int min=0, int max=100);

   int GetValue();
   void SetValue(int Val);
   int Width;
   int Min;
   int Max;
};
