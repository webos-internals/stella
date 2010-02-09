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
// $Id: SettingsGP2X.hxx 1724 2009-05-13 13:55:40Z stephena $
// Modified by Alex Zaballa on 2006/01/04 for use on GP2X
//============================================================================

#ifndef SETTINGS_GP2X_HXX
#define SETTINGS_GP2X_HXX

class OSystem;

#include "bspf.hxx"

/**
  This class defines GP2X system specific settings.
*/
class SettingsGP2X : public Settings
{
  public:
    /**
      Create a new GP2X settings object
    */
    SettingsGP2X(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~SettingsGP2X();
};

#endif
