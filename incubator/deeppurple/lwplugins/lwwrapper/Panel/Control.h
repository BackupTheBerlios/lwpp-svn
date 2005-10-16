#ifndef CONTROL_INCLUDED
#define CONTROL_INCLUDED

#include "Panel.h"


/// @todo redesign Controls to fix this warning!
//#pragma warning (disable:1125)


class Panel;


/** 
 *  @short Controls base class for Lightwave Panels.
 *
 *  @author David Forstenlechner
 *
 *  @date   2002-2003
 *
 *  @see Panel
 */
class Control : public Observable
{
public:

    /** Standard Constructor */
    Control();

    /** Standard Destructor  */
    virtual ~Control(void);

    /** 
     *  @param Sets initial values and creates the Control. 
     *  @param name     Control Name to display.
     *  @param SetOwner Panel to display Control in.
     *  @warning Call once only.
     */
    virtual void Initialize(const char *name, Panel *SetOwner);

    /** Shows the Control. */
	void UnHide();

    /** Hides the Control  */
	void Hide();

    virtual void MoveControl(int x, int y);

    /** @short Sets the Position of the Control, not taking the Caption text into consideration. */
    virtual void SetPositionWithoutCaption(int x, int y);

    void ChangeSizeWithoutCaption(int x, int y);

    void GetControlPosition(int &x, int &y);

    void GetControlSize(int &w, int &h);

    void GetActiveControlPosition(int &x, int &y);

    void GetActiveControlSize(int &w, int &h);

    bool IsInside(int x, int y);

    bool ChangeSize(int x, int y);

    /** Shows Control Ghosted */
    void GhostControl();

    /** Shows the Control normally */
    void UnGhostControl();

    /** Redraws Control in curent state */
    void Refresh();

    int  GetLabelWidth();

    void SetRange(int min,int max);

protected:
    std::string Name;

    Panel *Owner;

    virtual bool RegisterWithPanel(LWPanelID pan) = 0;

    LWControl *ThisControl;

    int mXPos;
    int mYPos;


private:
   static void NotifyCallback(LWControl *cnt, void *UserData);
};

#endif  // CONTROL_INCLUDED