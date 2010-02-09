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
// $Id: OptionsDialog.hxx 1724 2009-05-13 13:55:40Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef OPTIONS_DIALOG_HXX
#define OPTIONS_DIALOG_HXX

class CommandSender;
class DialogContainer;
class GuiObject;
class VideoDialog;
class AudioDialog;
class InputDialog;
class UIDialog;
class FileSnapDialog;
class RomAuditDialog;
class GameInfoDialog;
class CheatCodeDialog;
class HelpDialog;
class AboutDialog;
class OSystem;

#include "Dialog.hxx"
#include "bspf.hxx"

class OptionsDialog : public Dialog
{
  public:
    OptionsDialog(OSystem* osystem, DialogContainer* parent, GuiObject* boss,
                  bool global);
    virtual ~OptionsDialog();

  private:
    void loadConfig();
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);
    void checkBounds(int width, int height, int* x, int* y, int* w, int* h);

  private:
    VideoDialog*     myVideoDialog;
    AudioDialog*     myAudioDialog;
    InputDialog*     myInputDialog;
    UIDialog*        myUIDialog;
    FileSnapDialog*  myFileSnapDialog;
    RomAuditDialog*  myRomAuditDialog;
    GameInfoDialog*  myGameInfoDialog;
    CheatCodeDialog* myCheatCodeDialog;
    HelpDialog*      myHelpDialog;
    AboutDialog*     myAboutDialog;

    ButtonWidget* myVideoSettingsButton;
    ButtonWidget* myAudioSettingsButton;
    ButtonWidget* myUIButton;
    ButtonWidget* myFileSnapButton;
    ButtonWidget* myRomAuditButton;
    ButtonWidget* myGameInfoButton;
    ButtonWidget* myCheatCodeButton;
    ButtonWidget* myHelpButton;
    ButtonWidget* myAboutButton;

    // Indicates if this dialog is used for global (vs. in-game) settings
    bool myIsGlobal;

    enum {
      kVidCmd      = 'VIDO',
      kAudCmd      = 'AUDO',
      kInptCmd     = 'INPT',
      kUsrIfaceCmd = 'URIF',
      kFileSnapCmd = 'FLSN',
      kAuditCmd    = 'RAUD',
      kInfoCmd     = 'INFO',
      kCheatCmd    = 'CHET',
      kHelpCmd     = 'HELP',
      kAboutCmd    = 'ABOU',
      kExitCmd     = 'EXIM'
    };
};

#endif
