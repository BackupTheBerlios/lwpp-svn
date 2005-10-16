// ControlXColor.cpp: implementation of the ControlXColor class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlXColor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlXColor::ControlXColor()
{
color[0]=0.0;
color[1]=0.0;
color[2]=0.0;
//Lightwave3D::vparmf->create  (int envType, LWVPDT_NOTXTR);

}

ControlXColor::~ControlXColor()
{

}
enum { ID_COLOR = 0x8001 };

 static LWXPanelControl ColorControl[] = {
      { ID_COLOR,    "Color",    "color",  },
      { 0 }
   };

 static LWXPanelDataDesc ColorControlDD[] = {
      { ID_COLOR,   "Color",   "color" },
      { 0 }
   };

LWXPanelHint ControlXColor::ColorHint[] = {
      XpCHGNOTIFY( ChangeNotify_Callback ),
      XpEND
   };


bool ControlXColor::RegisterWithPanel(LWPanelID pan)
{
   ThisXPanel=Lightwave3D::xpanf->create(LWXP_FORM, ColorControl);
   Lightwave3D::xpanf->describe(ThisXPanel, ColorControlDD, NULL, NULL);
   Lightwave3D::xpanf->formSet(ThisXPanel,ID_COLOR,color);
   Lightwave3D::xpanf->hint(ThisXPanel,ID_COLOR,ColorHint);
   Lightwave3D::xpanf->setData(ThisXPanel,0,this);

   LWPanControlDesc Locdesc;
   Locdesc.type=LWT_XPANEL;
   Locdesc.xpanel.xpan=ThisXPanel;
   ThisControl=Lightwave3D::panf->addControl (pan, "XPanelControl", &Locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

void ControlXColor::ChangeNotify_Callback (LWXPanelID pan, unsigned long cid, unsigned long vid, int event_type)
{
int debug=1;
}

void ControlXColor::DoNotifyOwner(bool NotifyOwner)
{
}

/*

void *ControlXColor::Get_Callback (void *inst, unsigned long vid)
{
  if (vid==ID_COLOR)
     return color;

  return NULL;

}

LWXPRefreshCode   ControlXColor::Set_Callback (void *inst, unsigned long vid,void *value)
{
   if (vid==ID_COLOR)
   {
      double *dValue=(double *)value;
      color[0]=dValue[0];
      color[1]=dValue[1];
      color[2]=dValue[2];

      return LWXPRC_DFLT;
   }
   
   return LWXPRC_NONE;
}
*/