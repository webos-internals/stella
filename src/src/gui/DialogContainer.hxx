//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2008 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: DialogContainer.hxx,v 1.24 2008/02/06 13:45:23 stephena Exp $
//============================================================================

#ifndef DIALOG_CONTAINER_HXX
#define DIALOG_CONTAINER_HXX

class Dialog;
class OSystem;

#include "EventHandler.hxx"
#include "Stack.hxx"
#include "bspf.hxx"


/**
  The base class for groups of dialog boxes.  Each dialog box has a
  parent.  In most cases, the parent is itself a dialog box, but in the
  case of the lower-most dialog box, this class is its parent.
  
  This class keeps track of its children (dialog boxes), organizes them into
  a stack, and handles their events.

  @author  Stephen Anthony
  @version $Id: DialogContainer.hxx,v 1.24 2008/02/06 13:45:23 stephena Exp $
*/
class DialogContainer
{
  friend class EventHandler;

  public:
    /**
      Create a new DialogContainer stack
    */
    DialogContainer(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~DialogContainer();

  public:
    /**
      Update the dialog container with the current time.
      This is useful if we want to trigger events at some specified time.

      @param time  The current time in microseconds
    */
    void updateTime(uInt32 time);

    /**
      Handle a keyboard event.

      @param ascii    ASCII translation
      @param key      Actual key symbol
      @param mod      Modifiers
      @param state    Pressed or released
    */
    void handleKeyEvent(int ascii, int key, int mod, uInt8 state);

    /**
      Handle a mouse motion event.

      @param x      The x location
      @param y      The y location
      @param button The currently pressed button
    */
    void handleMouseMotionEvent(int x, int y, int button);

    /**
      Handle a mouse button event.

      @param b     The mouse button
      @param x     The x location
      @param y     The y location
      @param state The state (pressed or released)
    */
    void handleMouseButtonEvent(MouseButton b, int x, int y, uInt8 state);

    /**
      Handle a joystick button event.

      @param stick   The joystick number
      @param button  The joystick button
      @param state   The state (pressed or released)
    */
    void handleJoyEvent(int stick, int button, uInt8 state);

    /**
      Handle a joystick axis event.

      @param stick  The joystick number
      @param axis   The joystick axis
      @param value  Value associated with given axis
    */
    void handleJoyAxisEvent(int stick, int axis, int value);

    /**
      Handle a joystick hat event.

      @param stick  The joystick number
      @param axis   The joystick hat
      @param value  Value associated with given hat
    */
    void handleJoyHatEvent(int stick, int hat, int value);

    /**
      Handle a resize event.
    */
    void handleResizeEvent();

    /**
      Draw the stack of menus.
    */
    void draw();

    /**
      Add a dialog box to the stack.
    */
    void addDialog(Dialog* d);

    /**
      Remove the topmost dialog box from the stack.
    */
    void removeDialog();

    /**
      Reset dialog stack to the main configuration menu.
    */
    void reStack();

    /**
      Redraw all dialogs on the stack.
    */
    void refresh() { myRefreshFlag = true; }

    /**
      Return the bottom-most dialog of this container.
    */
    const Dialog* baseDialog() const { return myBaseDialog; }

  private:
    void reset();

  protected:
    OSystem* myOSystem;
    Dialog*  myBaseDialog;
    FixedStack<Dialog *> myDialogStack;

  private:
    enum {
      kDoubleClickDelay   = 500,
      kRepeatInitialDelay = 400,
      kRepeatSustainDelay = 50
    };

    // Indicates the most current time (in milliseconds) as set by updateTime()
    int myTime;

    // Indicates a full refresh of all dialogs is required
    bool myRefreshFlag;

    // For continuous 'key down' events
    struct {
      int ascii;
      int keycode;
      int flags;
    } myCurrentKeyDown;
    int myKeyRepeatTime;

    // For continuous 'mouse down' events
    struct {
      int x;
      int y;
      int button;
    } myCurrentMouseDown;
    int myClickRepeatTime;
	
    // For continuous 'joy button down' events
    struct {
      int stick;
      int button;
    } myCurrentButtonDown;
    int myButtonRepeatTime;

    // For continuous 'joy axis down' events
    struct {
      int stick;
      int axis;
      int value;
    } myCurrentAxisDown;
    int myAxisRepeatTime;

    // For continuous 'joy hat' events
    struct {
      int stick;
      int hat;
      int value;
    } myCurrentHatDown;
    int myHatRepeatTime;

    // Position and time of last mouse click (used to detect double clicks)
    struct {
      int x, y;   // Position of mouse when the click occured
      int time;   // Time
      int count;  // How often was it already pressed?
    } myLastClick;
};

#endif
