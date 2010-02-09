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
// $Id: FileSnapDialog.hxx 1724 2009-05-13 13:55:40Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef FILE_SNAP_DIALOG_HXX
#define FILE_SNAP_DIALOG_HXX

class OSystem;
class GuiObject;
class DialogContainer;
class BrowserDialog;
class CheckboxWidget;
class PopUpWidget;
class EditTextWidget;

#include "Dialog.hxx"
#include "Command.hxx"
#include "FSNode.hxx"

class FileSnapDialog : public Dialog, public CommandSender
{
  public:
    FileSnapDialog(OSystem* osystem, DialogContainer* parent,
                   const GUI::Font& font, GuiObject* boss);
    ~FileSnapDialog();

    void handleCommand(CommandSender* sender, int cmd, int data, int id);

  private:
    void loadConfig();
    void saveConfig();
    void setDefaults();

  private:
    enum {
      kChooseRomDirCmd      = 'LOrm', // rom select
      kChooseStateDirCmd    = 'LOsd', // state dir
      kChooseCheatFileCmd   = 'LOcf', // cheatfile (stella.cht)
      kChoosePaletteFileCmd = 'LOpf', // palette file (stella.pal)
      kChoosePropsFileCmd   = 'LOpr', // properties file (stella.pro)
      kChooseSnapDirCmd     = 'LOsn', // snapshot dir
      kChooseEEPROMDirCmd   = 'LOee', // eeprom dir
      kStateDirChosenCmd    = 'LOsc', // state dir changed
      kCheatFileChosenCmd   = 'LOcc', // cheatfile changed
      kPaletteFileChosenCmd = 'LOpc', // palette file changed
      kPropsFileChosenCmd   = 'LOrc', // properties file changed
      kEEPROMDirChosenCmd   = 'LOec'  // eeprom dir changed
    };

    BrowserDialog* myBrowser;

    // Config paths
    EditTextWidget* myRomPath;
    EditTextWidget* myStatePath;
    EditTextWidget* myEEPROMPath;
    EditTextWidget* myCheatFile;
    EditTextWidget* myPaletteFile;
    EditTextWidget* myPropsFile;
    EditTextWidget* mySnapPath;
    CheckboxWidget* mySnapSingle;
    CheckboxWidget* mySnap1x;

    // Indicates if this dialog is used for global (vs. in-game) settings
    bool myIsGlobal;
};

#endif
