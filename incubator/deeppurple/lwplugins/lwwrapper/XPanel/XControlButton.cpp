// Copyright (C) 1999 - 2002 David Forstenlechner
#include "StdAfx.h"
#include "XControlButton.h"
#include "XPanel.h"

XControlButton::XControlButton()
{
      Type="vButton";
      DataType="";
}

void XControlButton::OnButtonPressed(LWXPanelID owner, unsigned long cid)
{
static_cast<XPanel *>(Lightwave3D::xpanf->getData(owner,0))->OnSetValue(cid,NULL);
}

void XControlButton::Initialize(const char* name, XPanel* owner)
{
   XControl::Initialize(name,owner);

  Owner->XPanelHints.pop_back();
  Owner->XPanelHints.push_back( XPTAG_BUTNOTIFY );
  Owner->XPanelHints.push_back( XpH(ID) );
  Owner->XPanelHints.push_back( XpH(OnButtonPressed) );
  Owner->XPanelHints.push_back( XpEND );
}

