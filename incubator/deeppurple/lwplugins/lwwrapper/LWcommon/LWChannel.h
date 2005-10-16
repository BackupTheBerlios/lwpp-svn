#ifndef LWCOMMON_LWCHANNEL_H_HEADER_INCLUDED_C13192E5
#define LWCOMMON_LWCHANNEL_H_HEADER_INCLUDED_C13192E5

class LWEnvelope;

class LWChannel : public Observable
{
 public:
   static int Channel_Callback(void *data, LWEnvelopeID env, LWEnvEvent event, void *eventData);
   virtual void OnChannelEvent(int ChanEvent);
   LWChannel(LWChannelID ChanID);
   virtual void addObserver(Observer* toAdd);
   virtual void deleteObserver(Observer* toDelete);
   bool IsDeleted();
  /**
   * Returns the Envelope to the Channel.
   * @return Pointer to Envelope. Do not delete it!
   */
   LWEnvelope *getEnvelope();
   ~LWChannel();










 private:
   LWChannelID thisChannel;
   bool deleted;
   LWEnvelope *theLWEnvelope;




};



#endif /* LWCOMMON_LWCHANNEL_H_HEADER_INCLUDED_C13192E5 */
