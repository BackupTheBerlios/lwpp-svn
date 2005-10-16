#ifndef PROCEDURALTEXTURE_ADAPTOR_INCLUDED
#define PROCEDURALTEXTURE_ADAPTOR_INCLUDED

#include "HandlerItemRend.h"

#include "../../lwinclude/lwtexture.h"

struct st_LWTextureAccess;

template <class T> class ProceduralTexture_Adaptor 
: public HandlerItemRend<T>
{
public:
	ProceduralTexture_Adaptor(const char *name);
	virtual ~ProceduralTexture_Adaptor();

   static double Evaluate_Callback(LWInstance inst, st_LWTextureAccess *ta);

   static unsigned int Flags_Callback( LWInstance inst );

   static int ProceduralTexture_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData );
};

template <class T>
ProceduralTexture_Adaptor<T>::ProceduralTexture_Adaptor(const char *name) 	: HandlerItemRend<T>(LWTEXTURE_HCLASS, name, ProceduralTexture_Callback ) {}

template <class T>
ProceduralTexture_Adaptor<T>::~ProceduralTexture_Adaptor() {}

template <class T>
double ProceduralTexture_Adaptor<T>::Evaluate_Callback(LWInstance inst, st_LWTextureAccess *ta)
{ 
	return ((T*)inst)->Evaluate(ta); 
}

template <class T>
unsigned int ProceduralTexture_Adaptor<T>::Flags_Callback( LWInstance inst )
{ 
	return ((T*)inst)->Flags(); 
}

template <class T>
int ProceduralTexture_Adaptor<T>::ProceduralTexture_Callback( long version, GlobalFunc *global, void *INlocal, void *serverData ) 
{
	if (version != LWTEXTURE_VERSION)
	   return (AFUNC_BADVERSION);

   LWTextureHandler *local=(LWTextureHandler *)INlocal;      

   local->evaluate = Evaluate_Callback;

   local->flags    = Flags_Callback;

   return HandlerItemRend_Callback( global, local->inst, local->item, local->rend );
}


#endif // PROCEDURALTEXTURE_ADAPTOR_INCLUDED
