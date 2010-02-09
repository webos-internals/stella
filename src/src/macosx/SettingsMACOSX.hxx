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
// $Id: SettingsMACOSX.hxx 1724 2009-05-13 13:55:40Z stephena $
//============================================================================

#ifndef SETTINGS_MAC_OSX_HXX
#define SETTINGS_MAX_OSX_HXX

class OSystem;

/**
  This class defines Macintosh OSX system specific settings.

  @author  Mark Grebe
  @version $Id: SettingsMACOSX.hxx 1724 2009-05-13 13:55:40Z stephena $
*/
class SettingsMACOSX : public Settings
{
  public:
    /**
      Create a new UNIX settings object
    */
    SettingsMACOSX(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~SettingsMACOSX();

  public:
	
    /**
      This method should be called to load the current settings from the 
	  standard Mac preferences.
    */
    void loadConfig();

    /**
      This method should be called to save the current settings to the
	  standard Mac preferences.
    */
    void saveConfig();
};

#endif
