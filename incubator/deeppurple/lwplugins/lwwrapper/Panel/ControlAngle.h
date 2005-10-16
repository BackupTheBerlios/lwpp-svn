#pragma once
#include "ControlDouble.h"

class ControlAngle : 
public ControlDouble  
{
public:
   ControlAngle();
   virtual ~ControlAngle(void);
   virtual bool RegisterWithPanel(LWPanelID pan);
};
