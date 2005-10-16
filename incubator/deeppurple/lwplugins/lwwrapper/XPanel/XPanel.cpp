#include "StdAfx.h"
#include "XPanel.h"
#include "XControl.h"

LWXPanelControl  XPanel::ZeroControl={0};
LWXPanelDataDesc XPanel::ZeroDesc={0};

XPanel::XPanel() : 
  ThisPanel(NULL),
  ControlInstances(0x8001),
  Closing(false)
{ 
  XControls.push_back( ZeroControl );  
  
  XPanelHints.push_back( XPTAG_DESTROYNOTIFY );
  XPanelHints.push_back( XpH(Destroy_Notify) );
  XPanelHints.push_back( XPTAG_NULL );
  XPanelHints.push_back( XpEND );


  XControlsDesc.push_back(ZeroDesc);
}

bool XPanel::IsClosing(void)
{
   return Closing;
}

XPanel::~XPanel() 
{ 
}

LWXPanelID XPanel::GetPanelID( )  
{ 
   ThisPanel=Lightwave3D::xpanf->create( LWXP_VIEW, &(*XControls.begin()) ); 
   Lightwave3D::xpanf->describe( ThisPanel, &(*(XControlsDesc.begin())), UI_Get, UI_Set );
   Lightwave3D::xpanf->hint    ( ThisPanel, 0, &(*(XPanelHints.begin())) );
   Lightwave3D::xpanf->viewInst( ThisPanel, this );
   Lightwave3D::xpanf->setData ( ThisPanel, 0, this );
   return ThisPanel; 
}

void XPanel::Destroy_Notify(void * inst)
   {
   (static_cast<XPanel *>(inst))->Closing=true;
   (static_cast<XPanel *>(inst))->OnPanelClosed();
   if (inst)
      delete (XPanel *)inst;
   inst=NULL;
   }

void *XPanel::UI_Get( void *inst, unsigned long vid )
   {
   return (static_cast<XPanel *>(inst))->OnGetValue(vid);
   }

en_LWXPRefreshCodes   XPanel::UI_Set( void *inst, unsigned long vid, void *value )
   {
   return static_cast<en_LWXPRefreshCodes>((static_cast<XPanel *>(inst))->OnSetValue(vid,value));    
   }

void XPanel::OnPanelClosed(void)
{

}

void XPanel::RegisterControl(XControl * ToRegister)
{
   ToRegister->ID=ControlInstances++;

   LWXPanelControl NewControl;
   NewControl.cid=ToRegister->ID;
   NewControl.label=ToRegister->Name.c_str();
   NewControl.ctrlclass=ToRegister->Type.c_str();
   XControls.pop_back();
   XControls.push_back( NewControl );
   XControls.push_back( ZeroControl );

   if (ToRegister->DataType=="")
		return;
   LWXPanelDataDesc newDesc;
   newDesc.vid=ToRegister->ID;
   newDesc.name=ToRegister->Name.c_str();
   newDesc.datatype=ToRegister->DataType.c_str();
   XControlsDesc.pop_back();
   XControlsDesc.push_back(newDesc);
   XControlsDesc.push_back(ZeroDesc);
}

int XPanel::OnSetValue(unsigned long ChangedControlID, void *Value)
{
	return 0;
}

void * XPanel::OnGetValue(unsigned long XControlID)
{
	return (void *)0;
}

void XPanel::SetInstance(void * inst)
{
}

bool XPanel::SetMin(unsigned long id, int value)
{
   this->XPanelHints.pop_back();
   this->XPanelHints.push_back(XPTAG_MIN);
   this->XPanelHints.push_back(XpH(id));
   this->XPanelHints.push_back(XpH(value));
   this->XPanelHints.push_back(XpEND);

return true;
}

bool XPanel::SetMax(unsigned long id, int value)
{
   this->XPanelHints.pop_back();
   this->XPanelHints.push_back(XPTAG_MAX);
   this->XPanelHints.push_back(XpH(id));
   this->XPanelHints.push_back(XpH(value));
   this->XPanelHints.push_back(XpEND);
   return true;
}


string XPanel::PanelClassName = "XPanel";

void XPanel::Open(void)
{
}
