#ifndef PROCEDURALTEXTURE_INCLUDED
#define PROCEDURALTEXTURE_INCLUDED


#include "HandlerImpl.h"

#include "../../lwinclude/lwtexture.h"

class ProceduralTexture : public HandlerImpl  
{
public:

	ProceduralTexture(void *context);

	virtual ~ProceduralTexture() {}

	virtual double Evaluate( LWTextureAccess *va);

	virtual int    Flags   ();

	LWTLayerID ThisLayer;
};

#endif // PROCEDURALTEXTURE_INCLUDED