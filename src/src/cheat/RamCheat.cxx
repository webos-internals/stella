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
// $Id: RamCheat.cxx 1724 2009-05-13 13:55:40Z stephena $
//============================================================================

#include "Console.hxx"
#include "System.hxx"
#include "CheatManager.hxx"

#include "RamCheat.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RamCheat::RamCheat(OSystem* os, const string& name, const string& code)
  : Cheat(os, name, code)
{
  address = (uInt16) unhex(myCode.substr(0, 2));
  value   = (uInt8) unhex(myCode.substr(2, 2));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
RamCheat::~RamCheat()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RamCheat::enable()
{
  if(!myEnabled)
  {
    myEnabled = true;
    myOSystem->cheat().addPerFrame(this, myEnabled);
  }
  return myEnabled;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool RamCheat::disable()
{
  if(myEnabled)
  {
    myEnabled = false;
    myOSystem->cheat().addPerFrame(this, myEnabled);
  }
  return myEnabled;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void RamCheat::evaluate()
{
  myOSystem->console().system().poke(address, value);
}
