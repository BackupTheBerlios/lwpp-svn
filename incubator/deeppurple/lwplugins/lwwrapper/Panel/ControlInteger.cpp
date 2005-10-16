#include "StdAfx.h"
#include "controlinteger.h"

ControlInteger::ControlInteger(void) :
Width(40),
Min(0),
Max(10000)
{
}

ControlInteger::~ControlInteger(void)
{
}

bool ControlInteger::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_RANGE;
   locdesc.range.width=Width;
   locdesc.range.min=Min;
   locdesc.range.max=Max;
   ThisControl=Lightwave3D::panf->addControl(pan, "MiniSliderControl", &locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

void ControlInteger::Initialize(const char *name, Panel *SetOwner, int min, int max)
{
   Min=min;
   Max=max;
   Control::Initialize(name,SetOwner);
}

int ControlInteger::GetValue()
{
   LWValue locival = { LWT_INTEGER };

   this->ThisControl->get( ThisControl, CTL_VALUE, &locival );
   return locival.intv.value;
}

void ControlInteger::SetValue(int Val)
{
   LWValue locival = { LWT_INTEGER };
   locival.intv.value=Val;
   ThisControl->set(ThisControl,CTL_VALUE,&locival);
}
