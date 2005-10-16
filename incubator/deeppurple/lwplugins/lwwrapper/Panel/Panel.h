#pragma once
#include "../LWcommon/Lightwave3D.h"
#include "lwdisplay.h"
#include "Observer.h"

class Control;

/** 
 * Panel base class.
 * @author David Forstenlechner
 * @date   2002-2003
 */
class Panel : public Observable, public Observer
{
public:
  /** 
   * Constructor.
   * @param name     Name to be shown in title bar.
   * @param INFlags  -Flags
   */
   Panel( const char *name="Default", int INFlags=0 );

   /**
    * Flags for Panel Creation.
    */
   /*
    * Values:
    *        - Blocking
    *        - Cancel
    *        - Frame
    *        - MouseTrap
    *        - NoButton
    */
    enum Flag { 
        Blocking=PANF_BLOCKING,    /**< Open() return delayed until Panel is closed. */
        Cancel=PANF_CANCEL,        /**< Show Continue button. */
        Frame=PANF_FRAME,          /**< Windowing system title bar. */
        MouseTrap=PANF_MOUSETRAP,  /**< Tracking mouse input */
        NoButton=PANF_NOBUTT       /**< No Continue or Continue/Cancel buttons. */
    };

    virtual ~Panel(void);
    LWPanelID pan;
    bool CreatePanel(std::string &PanelName);

    virtual int Open();
    virtual void Close();

    int ShowPanel();
    void Redraw();

    virtual void Update( Observable* theChangedSubject );

    virtual void OnDraw(int drawmode);
    virtual void OnMouseEvent(int event, int x, int y);
    virtual void OnKeyDown(unsigned short key);
    virtual void OnPanelClosed(void);

    static void PanelClosed_Callback( LWPanelID panel, void *UserData );
    static void PanelDraw_Callback( LWPanelID panel, void *userdata, int drawmode );
    static void PanelMouse_Callback( LWPanelID panel, void *userdata, int qualifiers, int x, int y ); 
    static void PanelMouseMove_Callback( LWPanelID panel, void *userdata, int qualifiers, int x, int y );
    static void PanelKeyDown_Callback( LWPanelID panel, void *userdata, unsigned short key );

    void SetPanWidth(int widht);
    void SetPanHeight(int height);
    void GetPanelPosition( int &x, int &y);

    int Flags;
    HWND ThisWindow;
    std::string PanelName;

    bool IsDragging;
    int LastMouseX;
    int LastMouseY;

    bool IsClosing(void);
    LWPanelID GetPanelID();


    bool Closing;
    static string PanelClassName;
};