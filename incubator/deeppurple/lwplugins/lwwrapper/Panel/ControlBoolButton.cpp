// ControlBoolButton.cpp: implementation of the ControlBoolButton class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ControlBoolButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlBoolButton::ControlBoolButton()
{

}

ControlBoolButton::~ControlBoolButton()
{

}

bool ControlBoolButton::RegisterWithPanel(LWPanelID pan)
{

   LWPanControlDesc locDesc;
   locDesc.type=LWT_BOOLEAN;

   this->ThisControl=Lightwave3D::panf->addControl(pan,"BoolButtonControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}

int ControlBoolButton::GetValue()
{
   LWValue locival = { LWT_INTEGER };

   this->ThisControl->get( ThisControl, CTL_VALUE, &locival );
   return locival.intv.value;
}

void ControlBoolButton::SetValue(int Val)
{
   LWValue locival = { LWT_INTEGER };
   locival.intv.value=Val;
   ThisControl->set(ThisControl,CTL_VALUE,&locival);
}
