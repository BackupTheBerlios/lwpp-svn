#include "StdAfx.h"
#include "XInfo.h"

XInfo::XInfo()
{
   Type="sInfo";
   DataType="string";
}

void XInfo::Initialize(const char * name, XPanel * owner)
{
	XControl::Initialize(name,owner);
}