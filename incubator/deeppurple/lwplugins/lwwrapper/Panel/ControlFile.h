#pragma once
#include "control.h"

class ControlFile : 
public Control  
{
public:
   ControlFile(void);
   virtual ~ControlFile(void);
   virtual void Initialize(const char *name, Panel *SetOwner, int width=50);
   bool RegisterWithPanel(LWPanelID pan);
   void GetValue(std::string &FileName);
   int Width;
};
