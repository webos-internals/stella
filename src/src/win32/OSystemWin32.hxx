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
// $Id: OSystemWin32.hxx 1823 2009-07-01 14:39:01Z stephena $
//============================================================================

#ifndef OSYSTEM_WIN32_HXX
#define OSYSTEM_WIN32_HXX

#include "OSystem.hxx"
#include "bspf.hxx"

/**
  This class defines Windows system specific settings.

  @author  Stephen Anthony
  @version $Id: OSystemWin32.hxx 1823 2009-07-01 14:39:01Z stephena $
*/
class OSystemWin32 : public OSystem
{
  public:
    /**
      Create a new Win32 operating system object
    */
    OSystemWin32();

    /**
      Destructor
    */
    virtual ~OSystemWin32();

  public:
    /**
      This method returns number of ticks in microseconds.

      @return Current time in microseconds.
    */
    virtual uInt64 getTicks() const;
};

#endif
