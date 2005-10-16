#include "StdAfx.h"
#include "controlfilebutton.h"

ControlFileButton::ControlFileButton(void) :
Width(50)
{
}

ControlFileButton::~ControlFileButton(void)
{
}

bool ControlFileButton::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_STRING;
   locDesc.area.width=Width;
   locDesc.area.height=0;   

   this->ThisControl=Lightwave3D::panf->addControl(pan,"FileButtonControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}

void ControlFileButton::GetValue(std::string &FileName)
{
   char sBuffer[255];

   LWValue LocSval = { LWT_STRING };
   LocSval.str.buf=sBuffer;
   LocSval.str.bufLen=255;
   ThisControl->get(ThisControl, CTL_VALUE, &LocSval);

   FileName=sBuffer;
}
