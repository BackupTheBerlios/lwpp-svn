#include "StdAfx.h"
#include "ProceduralTexture.h"

ProceduralTexture::ProceduralTexture(void *context)
{
    ThisLayer=reinterpret_cast<LWTLayerID>(context);
}

double ProceduralTexture::Evaluate( LWTextureAccess *va) { return 1.0; }

int    ProceduralTexture::Flags() { return 0; }
