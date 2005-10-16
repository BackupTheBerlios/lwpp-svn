#include "StdAfx.h"
#include "controlpopup.h"

ControlPopUp::ControlPopUp(void):
Width(100)
{
}

ControlPopUp::~ControlPopUp(void)
{
}

bool ControlPopUp::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_CHOICE;
   locdesc.choice.vertical=Width;
   locdesc.choice.items=const_cast<const char **>(Choices);
   ThisControl=Lightwave3D::panf->addControl (pan, "PopupControl", &locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

void ControlPopUp::Initialize(const char *name, Panel* SetOwner, char *ChoiceStrings[], int width)
{
   Choices=ChoiceStrings;
   Width=width;
   Control::Initialize(name,SetOwner);
}

int ControlPopUp::GetValue()
{
   LWValue locival = { LWT_INTEGER };

   this->ThisControl->get( ThisControl, CTL_VALUE, &locival );
   return locival.intv.value;
}

void ControlPopUp::SetValue(int Val)
{
   LWValue locival = { LWT_INTEGER };
   locival.intv.value=Val;
   ThisControl->set(ThisControl,CTL_VALUE,&locival);
}
