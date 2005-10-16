#pragma once
//#include "Lightwave3D.h"

class LWChannel;

class SceneItem : public Observable, public Observer
{
public:

	LWChanGroupID GetChannelGroup();

	bool GetWorldToObjectMatrix(Matrix3 & WtoO, LWTime time);

   SceneItem(LWItemID Item);

   virtual ~SceneItem(void);


   int      numEnvelopes;

   LWEnvelopeID Envelopes[9];

   Vector3 GetRotation();

   bool SetRotation(Vector3 &loc);

   Vector3 GetPosition();


   Vector3 GetScale(double time);

   bool SetPosition(Vector3 &loc);

   virtual void addObserver(Observer* toAdd);
   virtual void deleteObserver(Observer* toDelete);
   virtual void Update(Observable* theChangedSubject);
   Vector3 GetAxis(int axis);
   Real &getTime();
   void setTime(Real newTime);
   LWChannel *getChannel(string & channame);
   void GetObjectToWorldMatrix(Matrix3 &OtoW, LWTime time);





 private:
   LWItemID ThisItem;
   Real Time;

   LWChannel *theLWChannels[9];
};

inline Real &SceneItem::getTime()
{
   return this->Time;
}

inline void SceneItem::setTime(Real newTime)
{
   this->Time=newTime;
}


