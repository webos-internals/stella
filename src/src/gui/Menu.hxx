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
// $Id: Menu.hxx,v 1.16 2008/02/06 13:45:24 stephena Exp $
//============================================================================

#ifndef MENU_HXX
#define MENU_HXX

class OSystem;

#include "DialogContainer.hxx"

/**
  The base dialog for all configuration menus in Stella.

  @author  Stephen Anthony
  @version $Id: Menu.hxx,v 1.16 2008/02/06 13:45:24 stephena Exp $
*/
class Menu : public DialogContainer
{
  public:
    /**
      Create a new menu stack
    */
    Menu(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~Menu();
};

#endif
