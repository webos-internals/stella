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
// $Id: RiotWidget.hxx 1724 2009-05-13 13:55:40Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef RIOT_WIDGET_HXX
#define RIOT_WIDGET_HXX

class GuiObject;
class ButtonWidget;
class DataGridWidget;
class PopUpWidget;
class ToggleBitWidget;

#include "Array.hxx"
#include "Widget.hxx"
#include "Command.hxx"

class RiotWidget : public Widget, public CommandSender
{
  public:
    RiotWidget(GuiObject* boss, const GUI::Font& font,
               int x, int y, int w, int h);
    virtual ~RiotWidget();

    void handleCommand(CommandSender* sender, int cmd, int data, int id);
    void loadConfig();

  private:

  private:
    ToggleBitWidget* mySWCHAReadBits;
    ToggleBitWidget* mySWCHAWriteBits;
    ToggleBitWidget* mySWACNTBits;
    ToggleBitWidget* mySWCHBBits;

    DataGridWidget* myTimWrite;
    DataGridWidget* myTimRead;

    CheckboxWidget* myP0Pins[5], *myP1Pins[5];

    PopUpWidget *myP0Diff, *myP1Diff;
    PopUpWidget *myTVType;
    CheckboxWidget* mySelect;
    CheckboxWidget* myReset;

    // ID's for the various widgets
    // We need ID's, since there are more than one of several types of widgets
    enum {
      kTim1TID, kTim8TID, kTim64TID, kTim1024TID,
      kSWCHABitsID, kSWACNTBitsID, kSWCHBBitsID, kTimWriteID,
      kP0PinsID, kP1PinsID,
      kP0DiffChanged, kP1DiffChanged, kTVTypeChanged, kSelectID, kResetID
    };
};

#endif
