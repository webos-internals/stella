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
// Copyright (c) 1995-2009 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: TogglePixelWidget.hxx 1724 2009-05-13 13:55:40Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef TOGGLE_PIXEL_WIDGET_HXX
#define TOGGLE_PIXEL_WIDGET_HXX

#include "FrameBuffer.hxx"
#include "ToggleWidget.hxx"

/* TogglePixelWidget */
class TogglePixelWidget : public ToggleWidget
{
  public:
    TogglePixelWidget(GuiObject* boss, const GUI::Font& font,
                      int x, int y, int cols, int rows);
    virtual ~TogglePixelWidget();

    void setColor(int color) { _pixelColor = color; }
    void setState(const BoolArray& state);

    void setIntState(int value, bool swap);
    int  getIntState();

  protected:
    void drawWidget(bool hilite);

  private:
    int          _pixelColor;
    unsigned int _numBits;
    bool         _swapBits;
};

#endif
