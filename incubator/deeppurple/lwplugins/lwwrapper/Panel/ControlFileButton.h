#pragma once
#include "control.h"

class ControlFileButton : 
public Control  
{
public:
   ControlFileButton(void);
   virtual ~ControlFileButton(void);
   bool RegisterWithPanel(LWPanelID pan);
   void GetValue(std::string &FileName);
   int Width;
};
