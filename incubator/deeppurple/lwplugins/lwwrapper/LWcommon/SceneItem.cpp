#include "StdAfx.h"
#include "LWChannel.h"
#include "sceneitem.h"
#include "LWlib.h"

SceneItem::SceneItem(LWItemID Item) :
numEnvelopes(0),
ThisItem(Item),
Time(0.0)
{
   for (int i=0; i<9; i++)
   {
      theLWChannels[i]=NULL;
   }

   if (ThisItem==NULL)
      return;

   // TODO  Replace Envelopes by LWEnvelope Classes
   if (Lightwave3D::iteminfo->type(ThisItem)==LWI_CAMERA)
      numEnvelopes=6;
   else
      numEnvelopes=9;

   LWChanGroupID grp=Lightwave3D::iteminfo->chanGroup(ThisItem);
   for (i=0; i<numEnvelopes; i++)
   {
      this->Envelopes[i]=LWLib::GetEnvelopeByName(grp,(LWLib::StdEnvNames[i]).c_str());
      this->theLWChannels[i]=new LWChannel(LWLib::GetChannelByName(grp,(LWLib::StdEnvNames[i]).c_str()));
   }
}

SceneItem::~SceneItem(void)
{
   for (int i=0; i<9; i++)
   {
      if (theLWChannels[i])
         delete theLWChannels[i];
   }
}

Vector3 SceneItem::GetRotation()
{
   double value;
   Vector3 loc(0.0,0.0,0.0);
   LWEnvKeyframeID loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[3], NULL);
   Lightwave3D::envfunc->keyGet( Envelopes[3], loc_Key, LWKEY_VALUE , &value ) ;
   loc.x=(Real)value;

   loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[4], NULL);
   Lightwave3D::envfunc->keyGet( Envelopes[4], loc_Key, LWKEY_VALUE , &value ) ;
   loc.y=(Real)value;

   loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[5], NULL);
   Lightwave3D::envfunc->keyGet( Envelopes[5], loc_Key, LWKEY_VALUE , &value ) ;
   loc.z=(Real)value;

   return loc;
}

bool SceneItem::SetRotation(Vector3 &loc)
{
   double value=loc.x;

   LWEnvKeyframeID loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[3], NULL);
   Lightwave3D::envfunc->keySet( Envelopes[3], loc_Key, LWKEY_VALUE , &value ) ;

   value=loc.y;
   loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[4], NULL);
   Lightwave3D::envfunc->keySet( Envelopes[4], loc_Key, LWKEY_VALUE , &value ) ;

   value=loc.z;
   loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[5], NULL);
   Lightwave3D::envfunc->keySet( Envelopes[5], loc_Key, LWKEY_VALUE , &value ) ;

   return true;
}

bool SceneItem::SetPosition(Vector3 &loc)
{
   double value=loc.x;

   LWEnvKeyframeID loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[0], NULL);
   Lightwave3D::envfunc->keySet( Envelopes[0], loc_Key, LWKEY_VALUE , &value ) ;

   value=loc.y;
   loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[1], NULL);
   Lightwave3D::envfunc->keySet( Envelopes[1], loc_Key, LWKEY_VALUE , &value ) ;

   value=loc.z;
   loc_Key=Lightwave3D::envfunc->nextKey(Envelopes[2], NULL);
   Lightwave3D::envfunc->keySet( Envelopes[2], loc_Key, LWKEY_VALUE , &value ) ;

   return true;
}


Vector3 SceneItem::GetPosition()
{
   double Value[3];
   Lightwave3D::iteminfo->param(ThisItem,LWIP_W_POSITION,Time,Value );
   return Vector3((Real)Value[0],(Real)Value[1],(Real)Value[2]);
}

bool SceneItem::GetWorldToObjectMatrix(Matrix3 & WtoO, LWTime time)
{
   double value[3];
   Lightwave3D::iteminfo->param( this->ThisItem, LWIP_W_RIGHT, time, value );
   WtoO[0][0]=(Real)value[0];
   WtoO[1][0]=(Real)value[1];
   WtoO[2][0]=(Real)value[2];
   Lightwave3D::iteminfo->param( this->ThisItem, LWIP_W_UP, time, value );
   WtoO[0][1]=(Real)value[0];
   WtoO[1][1]=(Real)value[1];
   WtoO[2][1]=(Real)value[2];
   Lightwave3D::iteminfo->param( this->ThisItem, LWIP_W_FORWARD, time, value );
   WtoO[0][2]=(Real)value[0];
   WtoO[1][2]=(Real)value[1];
   WtoO[2][2]=(Real)value[2];
	return (bool)0;
}


LWChanGroupID SceneItem::GetChannelGroup()
{
	return Lightwave3D::iteminfo->chanGroup(this->ThisItem);
}

Vector3 SceneItem::GetScale(double time)
{
   double Value[3];
   Lightwave3D::iteminfo->param(ThisItem,LWIP_SCALING,time,Value );
   return Vector3((Real)Value[0],(Real)Value[1],(Real)Value[2]);
}

void SceneItem::addObserver(Observer* toAdd)
{
   if (!(this->numObservers()))
   {
      for (int i=0; i<numEnvelopes; i++)
      {
         if (this->theLWChannels[i])
            this->theLWChannels[i]->addObserver(this);
      }
   }
   Observable::addObserver(toAdd);
}

void SceneItem::deleteObserver(Observer* toDelete)
{
   Observable::deleteObserver(toDelete);
   if (!(this->numObservers()))
   {
      for (int i=0; i<numEnvelopes; i++)
      {
         if(this->theLWChannels[i])
            this->theLWChannels[i]->deleteObserver(this);
      }
   }
}

void SceneItem::Update(Observable* theChangedSubject)
{
   if (static_cast<LWChannel *>(theChangedSubject)->IsDeleted())
   {
      for (int i=0; i<numEnvelopes; i++)
      {
         if (this->theLWChannels[i]==theChangedSubject)
         {
            delete this->theLWChannels[i];
            this->theLWChannels[i]=NULL;
         }
      }
   }
   else
      this->notifyObservers();
}

Vector3 SceneItem::GetAxis(int axis)
{
   assert( axis<3 && axis>=0 );
   Vector3 curAxis;

   double dummy[3];

   switch(axis)
   {
   case 0:
      Lightwave3D::iteminfo->param(this->ThisItem, LWIP_RIGHT, Time, dummy);
      break;
   case 1:
      Lightwave3D::iteminfo->param(this->ThisItem, LWIP_UP, Time, dummy);
      break;
   case 2:
      Lightwave3D::iteminfo->param(this->ThisItem, LWIP_FORWARD, Time, dummy);
      break;
   default:
      Lightwave3D::iteminfo->param(this->ThisItem, LWIP_RIGHT, Time, dummy);
      break;
   }
   
   curAxis.x=dummy[0];
   curAxis.y=dummy[1];
   curAxis.z=dummy[2];

   return curAxis;
}

LWChannel *SceneItem::getChannel(string & channame)
{
   int chosen=0;
   while( LWLib::StdEnvNames[chosen]!=channame && chosen<9 )
   {
      chosen++;
   }

   if (chosen<9)
   {
      return this->theLWChannels[chosen];
   }

   return NULL;
}



void SceneItem::GetObjectToWorldMatrix(Matrix3 &OtoW, LWTime time)
{
   double value[3];
   Lightwave3D::iteminfo->param( this->ThisItem, LWIP_RIGHT, time, value );
   OtoW[0][0]=(Real)value[0];
   OtoW[1][0]=(Real)value[1];
   OtoW[2][0]=(Real)value[2];
   Lightwave3D::iteminfo->param( this->ThisItem, LWIP_UP, time, value );
   OtoW[0][1]=(Real)value[0];
   OtoW[1][1]=(Real)value[1];
   OtoW[2][1]=(Real)value[2];
   Lightwave3D::iteminfo->param( this->ThisItem, LWIP_FORWARD, time, value );
   OtoW[0][2]=(Real)value[0];
   OtoW[1][2]=(Real)value[1];
   OtoW[2][2]=(Real)value[2];
}

