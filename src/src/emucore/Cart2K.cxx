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
// $Id: Cart2K.cxx 1862 2009-08-27 22:59:14Z stephena $
//============================================================================

#include <cassert>
#include <cstring>

#include "System.hxx"
#include "Cart2K.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Cartridge2K::Cartridge2K(const uInt8* image, uInt32 size)
{
  // Size can be a maximum of 2K
  if(size > 2048) size = 2048;

  // Set image size to closest power-of-two for the given size
  mySize = 1;
  while(mySize < size)
    mySize <<= 1;

  // The smallest addressable area by Stella is 64 bytes
  // This should really be read from the System, but for now I'm going
  // to cheat a little and hard-code it to 64 (aka 2^6)
  if(mySize < 64)
    mySize = 64;

  // Initialize ROM with illegal 6502 opcode that causes a real 6502 to jam
  myImage = new uInt8[mySize];
  memset(myImage, 0x02, mySize);

  // Copy the ROM image into my buffer
  memcpy(myImage, image, size);

  // Set mask for accessing the image buffer
  // This is guaranteed to work, as mySize is a power of two
  myMask = mySize - 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
Cartridge2K::~Cartridge2K()
{
  delete[] myImage;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Cartridge2K::reset()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Cartridge2K::install(System& system)
{
  mySystem = &system;
  uInt16 shift = mySystem->pageShift();
  uInt16 mask = mySystem->pageMask();

  // Make sure the system we're being installed in has a page size that'll work
  assert((0x1000 & mask) == 0);

  // Map ROM image into the system
  System::PageAccess access;
  for(uInt32 address = 0x1000; address < 0x2000; address += (1 << shift))
  {
    access.device = this;
    access.directPeekBase = &myImage[address & myMask];
    access.directPokeBase = 0;
    mySystem->setPageAccess(address >> shift, access);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 Cartridge2K::peek(uInt16 address)
{
  return myImage[address & myMask];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Cartridge2K::poke(uInt16, uInt8)
{
  // This is ROM so poking has no effect :-)
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void Cartridge2K::bank(uInt16 bank)
{
  // Doesn't support bankswitching
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Cartridge2K::bank()
{
  // Doesn't support bankswitching
  return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int Cartridge2K::bankCount()
{
  return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Cartridge2K::patch(uInt16 address, uInt8 value)
{
  myImage[address & myMask] = value;
  return true;
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8* Cartridge2K::getImage(int& size)
{
  size = mySize;
  return &myImage[0];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Cartridge2K::save(Serializer& out) const
{
  const string& cart = name();

  try
  {
    out.putString(cart);
  }
  catch(const char* msg)
  {
    cerr << "ERROR: Cartridge2K::save" << endl << "  " << msg << endl;
    return false;
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool Cartridge2K::load(Serializer& in)
{
  const string& cart = name();

  try
  {
    if(in.getString() != cart)
      return false;
  }
  catch(const char* msg)
  {
    cerr << "ERROR: Cartridge2K::load" << endl << "  " << msg << endl;
    return false;
  }

  return true;
}
