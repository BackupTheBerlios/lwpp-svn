#pragma once
#include "control.h"

//##ModelId=3E16FCD601D1
class ControlString : 
public Control  
{
public:
   ControlString(void);
   virtual ~ControlString(void);
   virtual void Initialize(const char *name, Panel *SetOwner, int width=50, bool isreadonly=false);
   virtual bool RegisterWithPanel(LWPanelID pan);
   bool GetValue (string &LName);
   bool SetValue (string &newString);
protected:
   int Width;
   bool IsReadOnly;
};
