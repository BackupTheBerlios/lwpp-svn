// HandlerImpl.h: interface for the HandlerImpl class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HANDLERIMPL_INCLUDED
#define HANDLERIMPL_INCLUDED
/**
 * Base class for all Handler type Lightwave Plugins.
 * @author David Forstenlechner
 * @date   2002-2003
 */
class HandlerImpl  
{
public:
  /**
   * Loads instance data.
   * @param lState Stream to load parameters.
   */
	virtual void Load ( const LWLoadState *lState ) {}

  /**
   * Loads instance data.
   * @param lState Stream to save parameters.
   */
   virtual void Save ( const LWSaveState *sState ) {}

  /**
   * Returns a small description of the Plugin instance.
   */
   virtual const char * Describe ();

  /**
   * Returns a Null terminated array of Item IDs.
   * ChangeID() is called if any of the specified IDs change.
   */
   virtual const LWItemID *UseItems();

  /**
   * Callback for changed Item IDs specified in UseItems().
   */
   virtual void ChangeID(const LWItemID *itemID) {}

  /**
   * Called on start rendering.
   * @param Mode Specifies if preview or final rendering is performed.
   */
   virtual void Init(int Mode) {}

  /**
   * Called on stop of rendering.
   */
   virtual void Cleanup() {}

  /**
   * Called once for each frame or antialias pass to be rendered.
   */
   virtual void NewTime( LWFrame Frame, LWTime time ) {}
};

#endif // HANDLERIMPL_INCLUDED
