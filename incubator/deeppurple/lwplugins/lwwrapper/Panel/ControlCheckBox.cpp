#include "StdAfx.h"
#include "controlcheckbox.h"

ControlCheckBox::ControlCheckBox(void) :
Width(50)
{
}

ControlCheckBox::~ControlCheckBox(void)
{
}

bool ControlCheckBox::RegisterWithPanel(LWPanelID pan)
{
LWPanControlDesc locdesc;
locdesc.type=LWT_BOOLEAN;
ThisControl=Lightwave3D::panf->addControl(pan, "BoolControl", &locdesc, const_cast<char *>(this->Name.c_str()) );
if (ThisControl)
   return true;
else
   return false;
}

bool ControlCheckBox::SetValue(bool onoff)
{
   LWValue Locval    = { LWT_INTEGER };
   Locval.intv.value=onoff;
   ThisControl->set(ThisControl, CTL_VALUE, &Locval);

   return false;
}

bool ControlCheckBox::GetValue(void)
{
   LWValue Locval    = { LWT_INTEGER };
   ThisControl->get(ThisControl, CTL_VALUE, &Locval);
   return (Locval.intv.value!=0);
}
