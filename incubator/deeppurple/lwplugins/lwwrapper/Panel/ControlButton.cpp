#include "StdAfx.h"
#include "controlbutton.h"

ControlButton::ControlButton(void) :
Width(200)
{
}

ControlButton::~ControlButton(void)
{
}

bool ControlButton::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_AREA;
   locDesc.area.width=Width;
   locDesc.area.height=0;

   this->ThisControl=Lightwave3D::panf->addControl(pan,"ButtonControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}

void ControlButton::Initialize(const char *name, Panel* SetOwner, int width)
{
   this->Width=width;
   Control::Initialize(name,SetOwner);
}