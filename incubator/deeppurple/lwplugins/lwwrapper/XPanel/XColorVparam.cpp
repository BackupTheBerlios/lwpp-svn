// XColorVparam.cpp: implementation of the XColorVparam class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "XColorVparam.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XColorVparam::XColorVparam()
{
      Type="color-env";
      DataType="color-env";
}

XColorVparam::~XColorVparam()
{

}

void XColorVparam::Initialize(const char* name, XPanel* owner)
{
   XControl::Initialize(name,owner);
}

