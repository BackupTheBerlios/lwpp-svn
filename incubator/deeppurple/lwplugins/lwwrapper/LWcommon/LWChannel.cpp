#include "StdAfx.h"
#include "LWChannel.h"
#include "LWEnvelope.h"

LWChannel::LWChannel(LWChannelID ChanID) :
deleted(false),
theLWEnvelope(NULL)
{
   this->thisChannel=ChanID;
}

int LWChannel::Channel_Callback(void *data, LWEnvelopeID env, LWEnvEvent event, void *eventData)
{
   static_cast<LWChannel *>(data)->OnChannelEvent(event);
   return 1;
}

void LWChannel::OnChannelEvent( int ChanEvent )
{
   if ( ChanEvent==LWEEVNT_DESTROY )
      this->deleted=true;
   this->notifyObservers();
}

void LWChannel::addObserver( Observer* toAdd )
{
   if ( !(this->numObservers()) )
   {
      Lightwave3D::envfunc->setEnvEvent( Lightwave3D::chaninfo->channelEnvelope(this->thisChannel), Channel_Callback, this );
   }

   Observable::addObserver( toAdd );
}

void LWChannel::deleteObserver( Observer* toDelete )
{
   Observable::deleteObserver( toDelete );

   if (!(this->numObservers()))
   {
      Lightwave3D::envfunc->setEnvEvent( Lightwave3D::chaninfo->channelEnvelope(this->thisChannel), NULL, this );
   }
}

bool LWChannel::IsDeleted()
{
   return this->deleted;
}



LWEnvelope *LWChannel::getEnvelope()
{
   if ( !(this->theLWEnvelope) )
   {
      this->theLWEnvelope=new LWEnvelope(this->thisChannel);
   }
   return this->theLWEnvelope;
}

LWChannel::~LWChannel()
{
   if ( (!this->deleted) && (this->numObservers()) )
      Lightwave3D::envfunc->setEnvEvent( Lightwave3D::chaninfo->channelEnvelope(this->thisChannel), NULL, this );
   if(this->theLWEnvelope)
      delete this->theLWEnvelope;
}

