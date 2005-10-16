#include "StdAfx.h"
#include "controlangle.h"

ControlAngle::ControlAngle()
{
}

ControlAngle::~ControlAngle(void)
{
}

bool ControlAngle::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_RANGE;
   locDesc.range.min=0;
   locDesc.range.max=360;
   
   this->ThisControl=Lightwave3D::panf->addControl(pan,"AngleControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}

