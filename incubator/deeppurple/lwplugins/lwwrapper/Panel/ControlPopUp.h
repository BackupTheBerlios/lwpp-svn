#pragma once

#include "controlchoices.h"

class ControlPopUp : 
public ControlChoices  
{
public:
   ControlPopUp(void);
   virtual ~ControlPopUp(void);
   virtual bool RegisterWithPanel(LWPanelID pan);
   virtual void Initialize(const char *name, Panel* SetOwner, char **ChoiceStrings, int width=100);

   int GetValue();
   void SetValue(int Val);

protected:
   int Width;
};
