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
// $Id: 
//============================================================================

#ifndef CARTRIDGEX07_HXX
#define CARTRIDGEX07_HXX

class System;

#include "bspf.hxx"
#include "Cart.hxx"

/**
  Bankswitching method as defined/created by John Payson (aka Supercat)
  and Fred Quimby (aka batari).

  This bankswitching method has 16 4K banks that can be accessed at
  addresses $1000 to $1FFF. The bankswitching hotspots are all below
  $1000. X07 uses two types of hotspots:
  
  0 1xxx nnnn 1101 -- Switch to bank nnnn
  0 0xxx 0nxx xxxx -- If in bank 111x, switch to bank 111n.
                      In any other bank, do not switch.
 
  Note that the latter will hit on almost any TIA access.

  @author  Eckhard Stolberg
  @version $Id: 
*/
class CartridgeX07 : public Cartridge
{
  public:
    /**
      Create a new cartridge using the specified image

      @param image Pointer to the ROM image
    */
    CartridgeX07(const uInt8* image);
 
    /**
      Destructor
    */
    virtual ~CartridgeX07();

  public:
    /**
      Reset device to its power-on state
    */
    virtual void reset();

    /**
      Install cartridge in the specified system.  Invoked by the system
      when the cartridge is attached to it.

      @param system The system the device should install itself in
    */
    virtual void install(System& system);

    /**
      Install pages for the specified bank in the system.

      @param bank The bank that should be installed in the system
    */
    virtual void bank(uInt16 bank);

    /**
      Get the current bank.

      @return  The current bank, or -1 if bankswitching not supported
    */
    virtual int bank();

    /**
      Query the number of banks supported by the cartridge.
    */
    virtual int bankCount();

    /**
      Patch the cartridge ROM.

      @param address  The ROM address to patch
      @param value    The value to place into the address
      @return    Success or failure of the patch operation
    */
    virtual bool patch(uInt16 address, uInt8 value);

    /**
      Access the internal ROM image for this cartridge.

      @param size  Set to the size of the internal ROM image data
      @return  A pointer to the internal ROM image data
    */
    virtual uInt8* getImage(int& size);

    /**
      Save the current state of this cart to the given Serializer.

      @param out  The Serializer object to use
      @return  False on any errors, else true
    */
    virtual bool save(Serializer& out) const;

    /**
      Load the current state of this cart from the given Deserializer.

      @param in  The Deserializer object to use
      @return  False on any errors, else true
    */
    virtual bool load(Deserializer& in);

    /**
      Get a descriptor for the device name (used in error checking).

      @return The name of the object
    */
    virtual string name() const { return "CartridgeX07"; }

  public:
    /**
      Get the byte at the specified address.

      @return The byte at the specified address
    */
    virtual uInt8 peek(uInt16 address);

    /**
      Change the byte at the specified address to the given value

      @param address The address where the value should be stored
      @param value The value to be stored at the address
    */
    virtual void poke(uInt16 address, uInt8 value);

  private:
    // Indicates which bank is currently active
    uInt16 myCurrentBank;

    // The 64K ROM image of the cartridge
    uInt8 myImage[65536];
};

#endif
