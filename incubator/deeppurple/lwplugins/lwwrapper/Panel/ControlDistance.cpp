#include "StdAfx.h"
#include "controldistance.h"

ControlDistance::ControlDistance()
{
}

ControlDistance::~ControlDistance(void)
{
}

bool ControlDistance::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_FLOAT;
   
   this->ThisControl=Lightwave3D::panf->addControl(pan,"FloatControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}
