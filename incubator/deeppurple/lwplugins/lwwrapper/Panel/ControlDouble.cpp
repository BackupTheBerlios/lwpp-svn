// ControlDouble.cpp: implementation of the ControlDouble class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlDouble.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlDouble::ControlDouble()
{

}

ControlDouble::~ControlDouble()
{

}

double ControlDouble::GetValue(void)
{
   LWValue Locval    = { LWT_FLOAT };
   ThisControl->get(ThisControl, CTL_VALUE, &Locval);
   return (Real)Locval.flt.value;
}

void ControlDouble::SetValue(double Angle)
{
   LWValue Locval    = { LWT_FLOAT };
   Locval.flt.value=Angle;
   ThisControl->set(ThisControl, CTL_VALUE, &Locval);
}




bool ControlDouble::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_FLOAT;
   
   this->ThisControl=Lightwave3D::panf->addControl(pan,"FloatControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}