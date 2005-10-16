#pragma once
#include "ControlDouble.h"

class ControlDistance : 
public ControlDouble
{
public:
   ControlDistance();
   virtual ~ControlDistance(void);
   bool RegisterWithPanel(LWPanelID pan);
};
