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
// $Id: CommandDialog.hxx,v 1.8 2008/02/06 13:45:23 stephena Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef COMMAND_DIALOG_HXX
#define COMMAND_DIALOG_HXX

class Properties;
class CommandSender;
class DialogContainer;

#include "OSystem.hxx"
#include "Dialog.hxx"

class CommandDialog : public Dialog
{
  public:
    CommandDialog(OSystem* osystem, DialogContainer* parent);
    ~CommandDialog();

  protected:
    virtual void handleCommand(CommandSender* sender, int cmd, int data, int id);

  private:
    int mySelectedItem;

    enum {
      kSelectCmd     = 'Csel',
      kResetCmd      = 'Cres',
      kColorCmd      = 'Ccol',
      kBWCmd         = 'Cbwt',
      kLeftDiffACmd  = 'Clda',
      kLeftDiffBCmd  = 'Cldb',
      kRightDiffACmd = 'Crda',
      kRightDiffBCmd = 'Crdb',
      kSaveStateCmd  = 'Csst',
      kStateSlotCmd  = 'Ccst',
      kLoadStateCmd  = 'Clst',
      kSnapshotCmd   = 'Csnp',
      kFormatCmd     = 'Cfmt',
      kPaletteCmd    = 'Cpal',
      kReloadRomCmd  = 'Crom',
      kExitCmd       = 'Clex'
    };

    enum {
      kNumRows = 4,
      kNumCols = 4
    };
};

#endif
