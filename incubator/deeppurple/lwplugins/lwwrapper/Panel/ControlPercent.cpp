// ControlPercent.cpp: implementation of the ControlPercent class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlPercent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlPercent::ControlPercent() :
Min(0),
Max(100)
{

}

ControlPercent::~ControlPercent()
{

}

bool ControlPercent::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_RANGE;
   locDesc.range.min=Min;
   locDesc.range.max=Max;
   
   this->ThisControl=Lightwave3D::panf->addControl(pan,"PercentControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}

void ControlPercent::Initialize(const char *name, Panel* SetOwner, int min, int max)
{
   Min=min;
   Max=max;
   Control::Initialize(name,SetOwner);
}