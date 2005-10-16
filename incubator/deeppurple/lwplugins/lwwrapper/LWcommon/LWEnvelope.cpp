/// EXPERIMENTAL

#include "StdAfx.h"
#include "LWEnvelope.h"

bool LWEnvelope::UpdateKeyTable()
{
NumKeys=GetNumKeys();

Keys.clear();

if (NumKeys<=0)
{
	StartTime=0.0;
	EndTime=0.0;
	TimeSpan=0.0;
	return false;
}

LWEnvKeyframeID curKey=Lightwave3D::envfunc->nextKey(Envelope, NULL);
	
for (int i=0;i<NumKeys;i++)
{
LWKey tempKey;

		Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_TIME, &tempKey.Time );
		Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_VALUE, &tempKey.value );
		Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_SHAPE, &tempKey.shape );


   switch( tempKey.shape ) {
     case 0:        // TCB
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_TENSION, &tempKey.tension );
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_CONTINUITY, &tempKey.continuity );
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_BIAS, &tempKey.bias );

       break;

     case 1:        // Hermite
     case 2:        // Bezier
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_PARAM_0, &tempKey.par0 );
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_PARAM_1, &tempKey.par1 );
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_PARAM_2, &tempKey.par2 );
       Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_PARAM_3, &tempKey.par3 );

       break;

     case 3:        // Linear
       break;

     case 4:        // Stepped
       break;

	 case 5:
		 break;     // 2DBezier   FIX IT!!
	 }
   Keys.push_back(tempKey);
		curKey=Lightwave3D::envfunc->nextKey(Envelope,curKey);
}

StartTime=Keys.front().Time;
EndTime=Keys.back().Time;
TimeSpan=EndTime-StartTime;
index=0;
Lightwave3D::envfunc->egGet( Envelope, Lightwave3D::chaninfo->channelParent(EnvChannel),LWENVTAG_PREBEHAVE, &PrePostBehavior[0]);
Lightwave3D::envfunc->egGet( Envelope, Lightwave3D::chaninfo->channelParent(EnvChannel),LWENVTAG_POSTBEHAVE, &PrePostBehavior[1]);

return true;
}

LWEnvelope::LWEnvelope(const LWEnvelope&)
{
	Envelope=NULL;
	KeyCursor=NULL;
	index=0;
	NumKeys=0;
	StartTime=0.0;
	EndTime=0.0;
	TimeSpan=0.0;
	InternalCalculation=false;
	LeftBlindEnd=1.0;
	RightBlindEnd=1.0;
	IsDeleted=true;
}

LWEnvelope::~LWEnvelope()
{
}

bool LWEnvelope::MoveFirst()
{
	KeyCursor=Lightwave3D::envfunc->nextKey(Envelope,NULL);
	index=0;
	return true;
}

double LWEnvelope::GetValue()
{
	double value;
	Lightwave3D::envfunc->keyGet(Envelope,KeyCursor,LWKEY_VALUE,&value);
	return value;
}

double LWEnvelope::GetTime()
{
	double value;
	Lightwave3D::envfunc->keyGet(Envelope,KeyCursor,LWKEY_TIME,&value);
	return value;
}



bool LWEnvelope::MoveNext()
{
	KeyCursor=Lightwave3D::envfunc->nextKey(Envelope,KeyCursor);
	index++;
	return true;
}

//----------------------------------------------------------------------

bool LWEnvelope::Save( const LWSaveState *ss)
{
LWEnvKeyframeID curKey;
long	lB[1];
float	dB[7];

lB[0]=NumKeys;
LWSAVE_I4( ss, lB, 1 );

curKey=Lightwave3D::envfunc->nextKey(Envelope, NULL);	

std::list<LWKey>::iterator KeyItor=Keys.begin();

for (int i=0; i<NumKeys;i++)
{

	dB[0]=(float)KeyItor->Time;
	dB[1]=(float)KeyItor->value;
	LWSAVE_FP( ss, dB, 2 );

	lB[0]=KeyItor->shape;
	LWSAVE_I4( ss, lB, 1 );

int counter=0;
   switch( KeyItor->shape ) {
     case 0:        // TCB
	dB[counter++]=(float)KeyItor->tension;
	dB[counter++]=(float)KeyItor->continuity;
	dB[counter++]=(float)KeyItor->bias;
       break;

     case 1:        // Hermite
     case 2:        // Bezier
	dB[counter++]=(float)KeyItor->par0;
	dB[counter++]=(float)KeyItor->par1;
	dB[counter++]=(float)KeyItor->par2;
	dB[counter++]=(float)KeyItor->par3;
       break;

     case 3:        // Linear
       break;

     case 4:        // Stepped
       break;

	 case 5:
		 break;     // 2DBezier   FIX IT!!
	 }

	LWSAVE_FP( ss, dB, counter );
KeyItor++;
	curKey=Lightwave3D::envfunc->nextKey(Envelope, curKey);	
}

return true;
}

//-----------------------------------------------------------------------

bool LWEnvelope::Load( const LWLoadState *ls)
{
	long lB[1];
	float dB[7];

	LWLOAD_I4( ls, lB, 1 ) ;
	NumKeys=lB[0];

	Keys.clear();

	for (int i=0; i<NumKeys;i++)
	{
		LWKey locKey;

		LWLOAD_FP( ls, dB, 2 );
		locKey.Time=dB[0];
		locKey.value=dB[1];

		LWLOAD_I4(ls,lB,1);
		locKey.shape=lB[0];

		switch (locKey.shape)
		{
			 case 0:        // TCB
				LWLOAD_FP( ls, dB, 3 );
				locKey.tension=dB[0];
				locKey.continuity=dB[1];
				locKey.bias=dB[2];
			 break;

			 case 1:        // Hermite
			 case 2:        // Bezier
				LWLOAD_FP( ls, dB, 4 );
				locKey.par0=dB[0];
				locKey.par1=dB[1];
				locKey.par2=dB[2];
				locKey.par3=dB[3];
			 break;
			 case 3:        // Linear
			   break;

			 case 4:        // Stepped
			   break;

			 case 5:
				 break;     // 2DBezier   FIX IT!!
		}
		Keys.push_back(locKey);
	}

	return true;
}

LWEnvelopeID LWEnvelope::CopyToGroup(LWEnvelopeID result)
{
LWEnvKeyframeID curKey=0;
LWEnvKeyframeID createdKey=0;
double dValue;
double dTime;
int iValue;

NumKeys=GetNumKeys();

for ( int i=0; i<NumKeys; i++)
{
	curKey=Lightwave3D::envfunc->nextKey(Envelope,curKey);
	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_TIME,&dTime);
	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_VALUE,&dValue);

	createdKey=Lightwave3D::envfunc->createKey(result,dTime,dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_SHAPE,&iValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_SHAPE,&iValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_TENSION,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_TENSION,&dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_BIAS,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_BIAS,&dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_CONTINUITY,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_CONTINUITY,&dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_PARAM_0,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_PARAM_0,&dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_PARAM_1,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_PARAM_1,&dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_PARAM_2,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_PARAM_2,&dValue);

	Lightwave3D::envfunc->keyGet(Envelope, curKey,LWKEY_PARAM_3,&dValue);
	Lightwave3D::envfunc->keySet(result, createdKey,LWKEY_PARAM_3,&dValue);

}

	return result;
}

bool LWEnvelope::Init(LWChannelID Channel)
{
EnvChannel=Channel;
Envelope=Lightwave3D::chaninfo->channelEnvelope(Channel);
IsDeleted=false;
UpdateKeyTable();
return true;
}

bool LWEnvelope::RestoreEnvelope()
{
	if (Envelope)
	{
		RemoveAllKeys();

		LWEnvKeyframeID curKey;

		std::list<LWKey>::iterator KeyItor=Keys.begin();

		for (int n=0;n<NumKeys;n++)
		{
			curKey=Lightwave3D::envfunc->createKey(Envelope,KeyItor->Time,KeyItor->value);
			Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_SHAPE,&KeyItor->shape);

		    switch( KeyItor->shape ) 
			{
			 case 0:        // TCB
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_TENSION,&KeyItor->tension);
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_CONTINUITY,&KeyItor->continuity);
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_BIAS,&KeyItor->bias);
			   break;

			 case 1:        // Hermite
			 case 2:        // Bezier
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_PARAM_0,&KeyItor->par0);
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_PARAM_1,&KeyItor->par1);
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_PARAM_2,&KeyItor->par2);
				Lightwave3D::envfunc->keySet(Envelope,curKey,LWKEY_PARAM_3,&KeyItor->par3);
			   break;

			 case 3:        // Linear
			   break;

			 case 4:        // Stepped
			   break;

			 case 5:
				 break;     // 2DBezier   FIX IT!!
			}
		KeyItor++;
		}
	}
		return true;
}

bool LWEnvelope::RemoveAllKeys()
{
LWEnvKeyframeID curKey=Lightwave3D::envfunc->nextKey(Envelope,NULL);
while (curKey)
{
	Lightwave3D::envfunc->destroyKey(Envelope,curKey);
	curKey=Lightwave3D::envfunc->nextKey(Envelope,NULL);
}

	return true;
}

int LWEnvelope::GetNumKeys()
{
	int counter=0;
/*	LWEnvKeyframeID curKey;
	curKey=Lightwave3D::envfunc->nextKey(Envelope, NULL);
	while ( curKey )
	{
		curKey=Lightwave3D::envfunc->nextKey(Envelope, curKey);
		counter++;	
	}
*/
	Lightwave3D::envfunc->egGet(Envelope,Lightwave3D::chaninfo->channelParent(EnvChannel),LWENVTAG_KEYCOUNT,&counter);
	return counter;
}

bool LWEnvelope::GetTimeSpan()
{
StartTime=0.0;
EndTime=0.0;

LWEnvKeyframeID curKey=Lightwave3D::envfunc->nextKey(Envelope, NULL);
	if ( curKey )
	{
		Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_TIME, &StartTime );
		curKey=Lightwave3D::envfunc->nextKey(Envelope, curKey);
	}

	while (curKey)
	{  
		Lightwave3D::envfunc->keyGet( Envelope, curKey, LWKEY_TIME, &EndTime );
		curKey=Lightwave3D::envfunc->nextKey(Envelope, curKey);
	}

TimeSpan=EndTime-StartTime;

	return true;
}

bool LWEnvelope::AddKey(LWKey NewKey)
{
LWEnvKeyframeID NewKeyID;
	NewKeyID=Lightwave3D::envfunc->createKey(Envelope,NewKey.Time,NewKey.value);

	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_SHAPE,&NewKey.shape);

  switch( NewKey.shape ) {
     case 0:        // TCB
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_TENSION,&NewKey.tension);
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_CONTINUITY,&NewKey.continuity);
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_BIAS,&NewKey.bias);
       break;

     case 1:        // Hermite
     case 2:        // Bezier
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_PARAM_0,&NewKey.par0);
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_PARAM_1,&NewKey.par1);
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_PARAM_2,&NewKey.par2);
	Lightwave3D::envfunc->keySet(Envelope,NewKeyID,LWKEY_PARAM_3,&NewKey.par3);
       break;

     case 3:        // Linear
       break;

     case 4:        // Stepped
       break;

	 case 5:
		 break;     // 2DBezier   FIX IT!!
	 }

	return true;
}

double LWEnvelope::Evaluate(double Time)
{

if ( NumKeys == 0 )
	return 0.0;

if ( NumKeys == 1 )
    return Keys.begin()->value;

if(InternalCalculation)
{
   float offset=0.0;

	if (Time>EndTime)
		Time=EndTime;
	if (Time<StartTime)
		Time=StartTime;

   /* get the endpoints of the interval being evaluated */

	std::list<LWKey>::iterator key0(Keys.begin());
	std::list<LWKey>::iterator key1(Keys.begin());

   while ( Time > (*(++key1)).Time )   // Hope this construction works!
   {
      key0 = key1;
   }

   /* check for singularities first */

   if ( Time == (*key0).Time )
      return (*key0).value + offset;
   else if ( Time == (*key1).Time )
      return (*key1).value + offset;

   /* get interval length, time in [0, 1] */

   float t = ( Time - (*key0).Time ) / ( (*key1).Time - (*key0).Time );

float out=0.0;
float in=0.0;

float h1,h2,h3,h4;

   switch ( (*key1).shape )
   {
      case SHAPE_TCB:
      case SHAPE_BEZI:
      case SHAPE_HERM:
         out = outgoing( key0, key1 );
         in = incoming( key0, key1 );
         hermite( t, &h1, &h2, &h3, &h4 );
         return h1 * (*key0).value + h2 * (*key1).value + h3 * out + h4 * in + offset;

      case SHAPE_BEZ2:
         return 0.0;//bez2( key0, key1, time ) + offset;

      case SHAPE_LINE:
         return (*key0).value + t * ( (*key1).value - (*key0).value ) + offset;

      case SHAPE_STEP:
         return (*key0).value + offset;

      default:
         return offset;
   }



	double TheResult=1.0;

		return TheResult;//Lightwave3D::chaninfo->channelEvaluate(EnvChannel,Time);
}
else
	return Lightwave3D::chaninfo->channelEvaluate(EnvChannel,Time);
}


float LWEnvelope::outgoing( std::list<LWKey>::iterator key0, std::list<LWKey>::iterator key1 )
{
   float a, b, d, t, out;

   switch ( (*key0).shape )
   {
      case SHAPE_TCB:
         a = ( 1.0f - (*key0).tension )
           * ( 1.0f + (*key0).continuity )
           * ( 1.0f + (*key0).bias );
         b = ( 1.0f - (*key0).tension )
           * ( 1.0f - (*key0).continuity )
           * ( 1.0f - (*key0).bias );
         d = (*key1).value - (*key0).value;

         if ( key0!=Keys.begin() ) {
			 key0--;
			 LWKey Prev=*key0;
			 key0++;
            t = ( (*key1).Time - (*key0).Time ) / ( (*key1).Time - Prev.Time );
            out = t * ( a * ( (*key0).value - Prev.value ) + b * d );
         }
         else
            out = b * d;
         break;

      case SHAPE_LINE:
         d = (*key1).value - (*key0).value;
         if ( key0!=Keys.begin() ) {
			 key0--;
			 LWKey Prev=*key0;
			 key0++;
            t = ( (*key1).Time - (*key0).Time ) / ( (*key1).Time - Prev.Time );
            out = t * ( (*key0).value - Prev.value + d );
         }
         else
            out = d;
         break;

      case SHAPE_BEZI:
      case SHAPE_HERM:
         out = (*key0).par1;
         if ( key0!=Keys.begin() )
		 {
			 key0--;
			 LWKey Prev=*key0;
			 key0++;
            out *= ( (*key1).Time - (*key0).Time ) / ( (*key1).Time - Prev.Time );
		 }
         break;

      case SHAPE_BEZ2:
         out = (*key0).par3 * ( (*key1).Time - (*key0).Time );
         if ( fabs( (*key0).par2 ) > 1e-5f )
            out /= (*key0).par2;
         else
            out *= 1e5f;
         break;

      case SHAPE_STEP:
      default:
         out = 0.0f;
         break;
   }

   return out;

}

float LWEnvelope::incoming( std::list<LWKey>::iterator key0, std::list<LWKey>::iterator key1 )
{

   float a, b, d, t, in;

   switch ( (*key1).shape )
   {
      case SHAPE_LINE:
         d = (*key1).value - (*key0).value;
         if ( (++key1)!=Keys.end() ) {
			 LWKey Foll=*key1;
			 key1--;
            t = ( (*key1).Time - (*key0).Time ) / ( Foll.Time - (*key0).Time );
            in = t * ( Foll.value - (*key1).value + d );
         }
         else
            in = d;
         break;

      case SHAPE_TCB:
         a = ( 1.0f - (*key1).tension )
           * ( 1.0f - (*key1).continuity )
           * ( 1.0f + (*key1).bias );
         b = ( 1.0f - (*key1).tension )
           * ( 1.0f + (*key1).continuity )
           * ( 1.0f - (*key1).bias );
         d = (*key1).value - (*key0).value;

         if ( (++key1)!=Keys.end() ) {
			 LWKey Foll=*key1;
			 key1--;
            t = ( (*key1).Time - (*key0).Time ) / ( Foll.Time - (*key0).Time );
            in = t * ( b * ( Foll.value - (*key1).value ) + a * d );
         }
         else
            in = a * d;
         break;

      case SHAPE_BEZI:
      case SHAPE_HERM:
         in = (*key1).par0;
         if ( (++key1)!=Keys.end() )
		 {
			 LWKey Foll=*key1;
			 key1--;
            in *= ( (*key1).Time - (*key0).Time ) / ( Foll.Time - (*key0).Time );
		 }
         break;
         return in;

      case SHAPE_BEZ2:
         in = (*key1).par1 * ( (*key1).Time - (*key0).Time );
         if ( fabs( (*key1).par0 ) > 1e-5f )
            in /= (*key1).par0;
         else
            in *= 1e5f;
         break;

      case SHAPE_STEP:
      default:
         in = 0.0f;
         break;
   }

   return in;
}

void LWEnvelope::hermite( float t, float *h1, float *h2, float *h3, float *h4 )
{
   float t2, t3;

   t2 = t * t;
   t3 = t * t2;

   *h2 = 3.0f * t2 - t3 - t3;
   *h1 = 1.0f - *h2;
   *h4 = t3 - t2;
   *h3 = *h4 - t2 + t;

}



LWEnvelope::LWEnvelope( LWChannelID parentChannel ) :
EnvChannel(parentChannel),
IsDeleted(true)
{
   this->Envelope=Lightwave3D::chaninfo->channelEnvelope(EnvChannel);
   if (this->Envelope)
      IsDeleted=false;
}

LWKeyFrameID LWEnvelope::getFirst()
{
   return Lightwave3D::envfunc->nextKey(this->Envelope,NULL);
}

LWKeyFrameID LWEnvelope::getNext(LWKeyFrameID prev)
{
   return Lightwave3D::envfunc->nextKey(this->Envelope,prev);
}



void LWEnvelope::destroyKey(LWKeyFrameID toDestroy)
{
   Lightwave3D::envfunc->destroyKey(this->Envelope, toDestroy);
}



int LWEnvelope::getNumKeys()
{
   int numkeys=0;
   LWKeyFrameID runner=Lightwave3D::envfunc->nextKey(this->Envelope,NULL);
   while (runner)
   {
      numkeys++;
      runner=Lightwave3D::envfunc->nextKey(this->Envelope,runner);
   }
   return numkeys;
}

LWKeyFrameID LWEnvelope::addKey(Real newValue, Real atTime)
{
   return Lightwave3D::envfunc->createKey( this->Envelope, atTime, newValue );
}

void LWEnvelope::setKeyValue(LWKeyFrameID key, Real value)
{
   double pingpong=static_cast<double>(value);
   Lightwave3D::envfunc->keySet( this->Envelope, key, LWKEY_VALUE, &pingpong);
}

