// Copyright (C) 1999 - 2002 David Forstenlechner

#include "Referencable.h"


int Referencable::AddRef()
{
   return ++ReferenceCounter;
}

int Referencable::ReleaseRef()
{
   return --ReferenceCounter;
}


int Referencable::GetReferenceCount()
{
   return ReferenceCounter;
}

Referencable::Referencable() : 
   ReferenceCounter(1)
{}

