// Copyright (C) 1999 - 2002 David Forstenlechner
#include "StdAfx.h"
#include "XControl.h"

XControl::XControl()
{
}

XControl::~XControl()
{
}

void XControl::Initialize(const char * name, XPanel * owner)
{
   Name=name;
   Owner=owner;
   Owner->RegisterControl(this);
}

