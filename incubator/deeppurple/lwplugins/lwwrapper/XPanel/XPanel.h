#ifndef XPANEL_H
#define XPANEL_H

#include "../LWcommon/Lightwave3D.h"

class XControl;

/** 
 * Base class wrapping  Lightwave3D-XPanels.
 * @author  David Forstenlechner
 * @date    2002-2003
 */
class XPanel
{
public:
   XPanel();
   virtual ~XPanel();

   /// Overloaded by plugin interface classes to get the instance data.
	virtual void SetInstance(void * inst);

   vector<LWXPanelHint> XPanelHints;
    static string PanelClassName;

  /**
   * Handle user input.
   * @param XControl which signaled a change.
   * @param New value set by the XControl.
   * @return Error code, return 1 for no error, 0 if you encoutered problems or if the ControlID is unknown.
   */
	virtual int OnSetValue(unsigned long ChangedControlID, void *Value);

  /**
   * Return current value.
   * @param XControl requesting the value.
   * @return Value to set.
   */
	virtual void * OnGetValue(unsigned long XControlID);

  /**
   * Called when XPanel was closed.
   */
   virtual void OnPanelClosed(void);   

	void RegisterControl(XControl * ToRegister);

   LWXPanelID GetPanelID();
   
   bool IsClosing(void);



   protected:
	   bool SetMin(unsigned long id, int value);
	   bool SetMax(unsigned long id, int value);

      static void Destroy_Notify(void * inst);
      static void *UI_Get( void *inst, unsigned long vid );
      static en_LWXPRefreshCodes UI_Set( void *inst, unsigned long vid, void *value );

      LWXPanelID ThisPanel;
      vector<LWXPanelControl> XControls;
      vector<LWXPanelDataDesc> XControlsDesc;
      unsigned long ControlInstances;
      static LWXPanelControl  ZeroControl;
      static LWXPanelDataDesc ZeroDesc;

      bool Closing;
  private:


public:
    void Open(void);
};

#endif
