#include "StdAfx.h"
#include "XPanel.h"
#include "XChoice.h"

XChoice::XChoice()
{
      Type="iPopChoice";
      DataType="integer";

}

void XChoice::Initialize(const char* name, XPanel* owner,const char *choices[])
{
	XControl::Initialize(name,owner);

	this->Owner->XPanelHints.pop_back();
	this->Owner->XPanelHints.push_back(XPTAG_STRLIST);
	this->Owner->XPanelHints.push_back(XpH(this->ID));
	this->Owner->XPanelHints.push_back(XpH(choices));
	this->Owner->XPanelHints.push_back(XPTAG_NULL);
	this->Owner->XPanelHints.push_back( XpEND );	
}

