// ControlChoices.cpp: implementation of the ControlChoices class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlChoices.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlChoices::ControlChoices() :
Choices(NULL)
{

}

ControlChoices::~ControlChoices()
{

}

int ControlChoices::GetValue(void)
{
   LWValue  LocVal= { LWT_INTEGER };
   ThisControl->get(ThisControl, CTL_VALUE, &LocVal);
   return LocVal.intv.value;  
}

void ControlChoices::SetValue(int Val)
{
   LWValue locival = { LWT_INTEGER };
   locival.intv.value=Val;
   ThisControl->set(ThisControl,CTL_VALUE,&locival);
}

void ControlChoices::Initialize(const char *name, Panel *SetOwner, char **ChoiceStrings)
{
   Choices=ChoiceStrings;
   Control::Initialize(name,SetOwner);
}
