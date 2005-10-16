#include "StdAfx.h"
#include "ControlBorder.h"

void ControlBorder::Draw()
{
    Lightwave3D::panf->drawFuncs->drawBorder(Owner, 0, mXPos, mYPos, Width, Height);
}