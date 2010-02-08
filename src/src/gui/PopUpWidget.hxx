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
// $Id: PopUpWidget.hxx,v 1.20 2008/02/06 13:45:24 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef POPUP_WIDGET_HXX
#define POPUP_WIDGET_HXX

class GUIObject;
class PopUpDialog;

#include "bspf.hxx"

#include "Array.hxx"
#include "Command.hxx"
#include "Dialog.hxx"
#include "Widget.hxx"


/**
 * Popup or dropdown widget which, when clicked, "pop up" a list of items and
 * lets the user pick on of them.
 *
 * Implementation wise, when the user selects an item, then a kPopUpItemSelectedCmd 
 * is broadcast, with data being equal to the tag value of the selected entry.
 */
class PopUpWidget : public Widget, public CommandSender
{
  friend class PopUpDialog;

  struct Entry {
    string name;
    int tag;
  };

  typedef Common::Array<Entry> EntryList;

  protected:
    EntryList _entries;
    int       _selectedItem;
    string    _label;
    int       _labelWidth;

  public:
    PopUpWidget(GuiObject* boss, const GUI::Font& font,
                int x, int y, int w, int h,
                const string& label, int labelWidth = 0, int cmd = 0);
    ~PopUpWidget();

    bool wantsFocus()  { return true; }

    void appendEntry(const string& entry, int tag = (int)-1);
    void clearEntries();

    /** Select the entry at the given index. */
    void setSelected(int item);
	
    /** Select the first entry matching the given name. */
    void setSelectedName(const string& name);

    /** Select the first entry matching the given tag. */
    void setSelectedTag(int tag);

    /** Select the highest/last entry in the internal list. */
    void setSelectedMax();

    int getSelected() const
      { return _selectedItem; }
    int getSelectedTag() const
      { return (_selectedItem >= 0) ? _entries[_selectedItem].tag : (int)-1; }
    const string& getSelectedString() const
      { return (_selectedItem >= 0) ? _entries[_selectedItem].name : EmptyString; }

  protected:
    void handleMouseDown(int x, int y, int button, int clickCount);
    bool handleEvent(Event::Type e);
    void drawWidget(bool hilite);

  protected:
    int	_cmd;

  private:
    PopUpDialog* myPopUpDialog;
    int myArrowsY;
    int myTextY;
};

//
// PopUpDialog
//
class PopUpDialog : public Dialog
{
  friend class PopUpWidget;

  public:
    PopUpDialog(PopUpWidget* boss, int clickX, int clickY);
	
    void drawDialog();
    void center() { recalc(); }

  protected:
    void handleMouseDown(int x, int y, int button, int clickCount);
    void handleMouseWheel(int x, int y, int direction);
    void handleMouseMoved(int x, int y, int button);
    void handleKeyDown(int ascii, int keycode, int modifiers);  // Scroll through entries with arrow keys etc
    void handleJoyDown(int stick, int button);
    void handleJoyAxis(int stick, int axis, int value);
    bool handleJoyHat(int stick, int hat, int value);
    void handleEvent(Event::Type e);

    void drawMenuEntry(int entry, bool hilite);

    void recalc();
    int findItem(int x, int y) const;
    void setSelection(int item);
    bool isMouseDown();
	
    void moveUp();
    void moveDown();

  private:
    void sendSelection();
    void cancelSelection();

  protected:
    PopUpWidget* _popUpBoss;
    int          _clickX, _clickY;
    uInt8*       _buffer;
    int          _selection;
    int          _oldSelection;
    int          _openTime;
    bool         _twoColumns;
    int          _entriesPerColumn;
};

#endif
