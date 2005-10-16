#include "StdAfx.h"
#include "ControlSlider.h"

ControlSlider::ControlSlider() :
Min(0),
Max(100),
Width(50),
EntryField(false)
{
}

int ControlSlider::GetValue()
{
   LWValue locival = { LWT_INTEGER };

   this->ThisControl->get( ThisControl, CTL_VALUE, &locival );
   return locival.intv.value;
}

void ControlSlider::SetValue(int Val)
{
   LWValue locival = { LWT_INTEGER };

   locival.intv.value=Val;

   this->ThisControl->set( ThisControl, CTL_VALUE, &locival );
}

bool ControlSlider::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_RANGE;
   locdesc.range.width=Width;
   locdesc.range.min=Min;
   locdesc.range.max=Max;
   if (EntryField)
      ThisControl=Lightwave3D::panf->addControl(pan, "SliderControl", &locdesc, const_cast<char *>(Name.c_str()));
   else
      ThisControl=Lightwave3D::panf->addControl(pan, "HSlideControl", &locdesc, const_cast<char *>(Name.c_str()));

	return true;
}

void ControlSlider::Initialize(const char *name, Panel *SetOwner, int min, int max, int width, bool entryfield)
{
   Min=min;
   Max=max;
   Width=width;
   EntryField=entryfield;
   Control::Initialize(name, SetOwner);
}