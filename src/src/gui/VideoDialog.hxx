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
// $Id: VideoDialog.hxx,v 1.23 2008/02/06 13:45:24 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef VIDEO_DIALOG_HXX
#define VIDEO_DIALOG_HXX

class CommandSender;
class DialogContainer;
class PopUpWidget;
class SliderWidget;
class StaticTextWidget;
class CheckboxWidget;

#include "OSystem.hxx"
#include "Dialog.hxx"
#include "bspf.hxx"

class VideoDialog : public Dialog
{
  public:
    VideoDialog(OSystem* osystem, DialogContainer* parent,
                const GUI::Font& font, int x, int y, int w, int h);
    ~VideoDialog();

  private:
    void loadConfig();
    void saveConfig();
    void setDefaults();

    void handleRendererChange(int item);
    void handleFullscreenChange(bool enable);
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);

  private:
    PopUpWidget*      myRendererPopup;
    PopUpWidget*      myFilterPopup;
    PopUpWidget*      myFSStretchPopup;
    SliderWidget*     myAspectRatioSlider;
    StaticTextWidget* myAspectRatioLabel;
    PopUpWidget*      myPalettePopup;
    PopUpWidget*      myFSResPopup;
    SliderWidget*     myUIZoomSlider;
    StaticTextWidget* myUIZoomLabel;
    SliderWidget*     myTIAZoomSlider;
    StaticTextWidget* myTIAZoomLabel;

    SliderWidget*     myFrameRateSlider;
    StaticTextWidget* myFrameRateLabel;
    CheckboxWidget*   myFullscreenCheckbox;
    CheckboxWidget*   myColorLossCheckbox;
    CheckboxWidget*   myUseVSyncCheckbox;
    CheckboxWidget*   myCenterCheckbox;

    enum {
      kRendererChanged    = 'VDrd',
      kAspectRatioChanged = 'VDar',
      kUIZoomChanged      = 'VDui',
      kTIAZoomChanged     = 'VDti',
      kFrameRateChanged   = 'VDfr',
      kFullScrChanged     = 'VDfs'
    };
};

#endif
