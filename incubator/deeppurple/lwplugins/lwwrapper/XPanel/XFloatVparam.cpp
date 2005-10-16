#include "StdAfx.h"
#include "XPanel.h"
#include "XFloatVparam.h"

XFloatVparam::XFloatVparam()
{
      Type="float-env";
      DataType="float-env";
}

XFloatVparam::~XFloatVparam()
{
}

void XFloatVparam::Initialize(const char* name, XPanel* owner)
{
   XControl::Initialize(name,owner);
}

