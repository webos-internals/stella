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
// $Id: RomAuditDialog.cxx 1724 2009-05-13 13:55:40Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#include "bspf.hxx"

#include "LauncherFilterDialog.hxx"
#include "BrowserDialog.hxx"
#include "DialogContainer.hxx"
#include "EditTextWidget.hxx"
#include "ProgressDialog.hxx"
#include "FSNode.hxx"
#include "Props.hxx"
#include "PropsSet.hxx"
#include "Settings.hxx"
#include "RomAuditDialog.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RomAuditDialog::RomAuditDialog(OSystem* osystem, DialogContainer* parent,
                               const GUI::Font& font)
  : Dialog(osystem, parent, 0, 0, 0, 0),
    myBrowser(NULL)
{
  const int vBorder = 8;

  const int lineHeight   = font.getLineHeight(),
            fontWidth    = font.getMaxCharWidth(),
            fontHeight   = font.getFontHeight(),
            buttonWidth  = font.getStringWidth("Audit path:") + 20,
            buttonHeight = font.getLineHeight() + 4,
            lwidth = font.getStringWidth("ROMs with properties (renamed): ");
  int xpos = vBorder, ypos = vBorder;
  WidgetArray wid;

  // Set real dimensions
  _w = 44 * fontWidth + 10;
  _h = 7 * (lineHeight + 4) + 10;

  // Audit path
  ButtonWidget* romButton = 
    new ButtonWidget(this, font, xpos, ypos, buttonWidth, buttonHeight,
                     "Audit path:", kChooseAuditDirCmd);
  wid.push_back(romButton);
  xpos += buttonWidth + 10;
  myRomPath = new EditTextWidget(this, font, xpos, ypos + 2,
                                 _w - xpos - 10, lineHeight, "");
  wid.push_back(myRomPath);

  // Show results of ROM audit
  xpos = vBorder + 10;  ypos += buttonHeight + 10;
  new StaticTextWidget(this, font, xpos, ypos, lwidth, fontHeight,
                       "ROMs with properties (renamed): ", kTextAlignLeft);
  myResults1 = new StaticTextWidget(this, font, xpos + lwidth, ypos,
                                    _w - lwidth - 20, fontHeight, "",
                                    kTextAlignLeft);
  myResults1->setFlags(WIDGET_CLEARBG);
  ypos += buttonHeight;
  new StaticTextWidget(this, font, xpos, ypos, lwidth, fontHeight,
                       "ROMs without properties: ", kTextAlignLeft);
  myResults2 = new StaticTextWidget(this, font, xpos + lwidth, ypos,
                                    _w - lwidth - 20, fontHeight, "",
                                    kTextAlignLeft);
  myResults2->setFlags(WIDGET_CLEARBG);

  ypos += buttonHeight + 8;
  new StaticTextWidget(this, font, xpos, ypos, _w - 20, fontHeight,
                       "(*) WARNING: operation cannot be undone",
                       kTextAlignLeft);

  // Add OK and Cancel buttons
  wid.clear();
  addOKCancelBGroup(wid, font, "Audit", "Done");
  addBGroupToFocusList(wid);

  // Create file browser dialog
  myBrowser = new BrowserDialog(this, font);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RomAuditDialog::~RomAuditDialog()
{
  delete myBrowser;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RomAuditDialog::loadConfig()
{
  myRomPath->setEditString(instance().settings().getString("romdir"));
  myResults1->setLabel("");
  myResults2->setLabel("");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RomAuditDialog::auditRoms()
{
  const string& auditPath = myRomPath->getEditString();
  myResults1->setLabel("");
  myResults2->setLabel("");

  FilesystemNode node(auditPath);
  FSList files;
  node.getChildren(files, FilesystemNode::kListFilesOnly);

  // Create a progress dialog box to show the progress of processing
  // the ROMs, since this is usually a time-consuming operation
  ProgressDialog progress(this, instance().font(),
                          "Auditing ROM files ...");
  progress.setRange(0, files.size() - 1, 5);

  // Create a entry for the GameList for each file
  Properties props;
  int renamed = 0, notfound = 0;
  for(unsigned int idx = 0; idx < files.size(); idx++)
  {
    string extension;
    if(!files[idx].isDirectory() &&
       LauncherFilterDialog::isValidRomName(files[idx].getPath(), extension))
    {
      // Calculate the MD5 so we can get the rest of the info
      // from the PropertiesSet (stella.pro)
      const string& md5 = instance().MD5FromFile(files[idx].getPath());
      instance().propSet().getMD5(md5, props);
      const string& name = props.get(Cartridge_Name);

      // Only rename the file if we found a valid properties entry
      if(name != "" && name != files[idx].getDisplayName())
      {
        // Check for terminating separator
        string newfile = auditPath;
        if(newfile.find_last_of(BSPF_PATH_SEPARATOR) != newfile.length()-1)
          newfile += BSPF_PATH_SEPARATOR;
        newfile += name + "." + extension;

        if(files[idx].getPath() != newfile)
          if(AbstractFilesystemNode::renameFile(files[idx].getPath(), newfile))
            renamed++;
      }
      else
        notfound++;
    }

    // Update the progress bar, indicating one more ROM has been processed
    progress.setProgress(idx);
  }
  progress.close();

  myResults1->setValue(renamed);
  myResults2->setValue(notfound);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RomAuditDialog::handleCommand(CommandSender* sender, int cmd,
                                   int data, int id)
{
  switch (cmd)
  {
    case kOKCmd:
      auditRoms();
      break;

    case kChooseAuditDirCmd:
      myBrowser->show("Select ROM directory to audit:", myRomPath->getEditString(),
                      FilesystemNode::kListDirectoriesOnly, kAuditDirChosenCmd);
      break;

    case kAuditDirChosenCmd:
    {
      FilesystemNode dir(myBrowser->getResult());
      myRomPath->setEditString(dir.getPath());
      myResults1->setLabel("");
      myResults2->setLabel("");
      break;
    }

    default:
      Dialog::handleCommand(sender, cmd, data, 0);
      break;
  }
}
