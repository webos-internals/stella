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
// $Id: CartEFSC.cxx 1862 2009-08-27 22:59:14Z stephena $
//============================================================================

#include <cassert>
#include <cstring>

#include "Random.hxx"
#include "System.hxx"
#include "CartEFSC.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeEFSC::CartridgeEFSC(const uInt8* image)
{
  // Copy the ROM image into my buffer
  memcpy(myImage, image, 65536);

  // This cart contains 128 bytes extended RAM @ 0x1000
  registerRamArea(0x1000, 128, 0x80, 0x00);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeEFSC::~CartridgeEFSC()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeEFSC::reset()
{
  // Initialize RAM with random values
  class Random random;
  for(uInt32 i = 0; i < 128; ++i)
    myRAM[i] = random.next();

  // Upon reset we switch to bank 1
  bank(1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeEFSC::install(System& system)
{
  mySystem = &system;
  uInt16 shift = mySystem->pageShift();
  uInt16 mask = mySystem->pageMask();

  // Make sure the system we're being installed in has a page size that'll work
  assert((0x1000 & mask) == 0);

  // Set the page accessing methods for the hot spots
  System::PageAccess access;
  for(uInt32 i = (0x1FE0 & ~mask); i < 0x2000; i += (1 << shift))
  {
    access.directPeekBase = 0;
    access.directPokeBase = 0;
    access.device = this;
    mySystem->setPageAccess(i >> shift, access);
  }

  // Set the page accessing method for the RAM writing pages
  for(uInt32 j = 0x1000; j < 0x1080; j += (1 << shift))
  {
    access.device = this;
    access.directPeekBase = 0;
    access.directPokeBase = &myRAM[j & 0x007F];
    mySystem->setPageAccess(j >> shift, access);
  }
 
  // Set the page accessing method for the RAM reading pages
  for(uInt32 k = 0x1080; k < 0x1100; k += (1 << shift))
  {
    access.device = this;
    access.directPeekBase = &myRAM[k & 0x007F];
    access.directPokeBase = 0;
    mySystem->setPageAccess(k >> shift, access);
  }

  // Install pages for bank 1
  bank(1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeEFSC::peek(uInt16 address)
{
  address &= 0x0FFF;

  // Switch banks if necessary
  if((address >= 0x0FE0) && (address <= 0x0FEF))
    bank(address - 0x0FE0);

  // Reading from the write port triggers an unwanted write
  // The value written to RAM is somewhat undefined, so we use 0
  // Thanks to Kroko of AtariAge for this advice and code idea
  if(address < 0x0080)  // Write port is at 0xF000 - 0xF080 (128 bytes)
  {
    if(myBankLocked) return 0;
    else return myRAM[address] = 0;
  }  
  else
    return myImage[(myCurrentBank << 12) + address];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeEFSC::poke(uInt16 address, uInt8)
{
  address &= 0x0FFF;

  // Switch banks if necessary
  if((address >= 0x0FE0) && (address <= 0x0FEF))
    bank(address - 0x0FE0);

  // NOTE: This does not handle accessing RAM, however, this function
  // should never be called for RAM because of the way page accessing
  // has been setup
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeEFSC::bank(uInt16 bank)
{
  if(myBankLocked) return;

  // Remember what bank we're in
  myCurrentBank = bank;
  uInt16 offset = myCurrentBank << 12;
  uInt16 shift = mySystem->pageShift();
  uInt16 mask = mySystem->pageMask();

  // Setup the page access methods for the current bank
  System::PageAccess access;
  access.device = this;
  access.directPokeBase = 0;

  // Map ROM image into the system
  for(uInt32 address = 0x1100; address < (0x1FE0U & ~mask);
      address += (1 << shift))
  {
    access.directPeekBase = &myImage[offset + (address & 0x0FFF)];
    mySystem->setPageAccess(address >> shift, access);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int CartridgeEFSC::bank()
{
  return myCurrentBank;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int CartridgeEFSC::bankCount()
{
  return 16;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeEFSC::patch(uInt16 address, uInt8 value)
{
  myImage[(myCurrentBank << 12) + (address & 0x0FFF)] = value;
  return true;
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8* CartridgeEFSC::getImage(int& size)
{
  size = 65536;
  return &myImage[0];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeEFSC::save(Serializer& out) const
{
  const string& cart = name();

  try
  {
    out.putString(cart);

    out.putInt(myCurrentBank);
  }
  catch(const char* msg)
  {
    cerr << "ERROR: CartridgeEFSC::save" << endl << "  " << msg << endl;
    return false;
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeEFSC::load(Serializer& in)
{
  const string& cart = name();

  try
  {
    if(in.getString() != cart)
      return false;

    myCurrentBank = (uInt16) in.getInt();
  }
  catch(const char* msg)
  {
    cerr << "ERROR: CartridgeEFSC::load" << endl << "  " << msg << endl;
    return false;
  }

  // Remember what bank we were in
  bank(myCurrentBank);

  return true;
}
