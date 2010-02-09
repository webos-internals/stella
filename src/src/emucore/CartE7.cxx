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
// $Id: CartE7.cxx 1862 2009-08-27 22:59:14Z stephena $
//============================================================================

#include <cassert>
#include <cstring>

#include "Random.hxx"
#include "System.hxx"
#include "CartE7.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeE7::CartridgeE7(const uInt8* image)
{
  // Copy the ROM image into my buffer
  memcpy(myImage, image, 16384);

  // This cart can address a 1024 byte bank of RAM @ 0x1000
  // and 256 bytes @ 0x1800
  // However, it may not be addressable all the time (it may be swapped out)
  // so probably most of the time, the area will point to ROM instead
  registerRamArea(0x1000, 1024, 0x400, 0x00);  // 1024 bytes RAM @ 0x1000
  registerRamArea(0x1800, 256, 0x100, 0x00);   // 256 bytes RAM @ 0x1800
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
CartridgeE7::~CartridgeE7()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE7::reset()
{
  // Initialize RAM with random values
  class Random random;
  for(uInt32 i = 0; i < 2048; ++i)
    myRAM[i] = random.next();

  // Install some default banks for the RAM and first segment
  bankRAM(0);
  bank(0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE7::install(System& system)
{
  mySystem = &system;
  uInt16 shift = mySystem->pageShift();
  uInt16 mask = mySystem->pageMask();

  // Make sure the system we're being installed in has a page size that'll work
  assert(((0x1400 & mask) == 0) && ((0x1800 & mask) == 0) &&
      ((0x1900 & mask) == 0) && ((0x1A00 & mask) == 0));

  // Set the page accessing methods for the hot spots
  System::PageAccess access;
  for(uInt32 i = (0x1FE0 & ~mask); i < 0x2000; i += (1 << shift))
  {
    access.directPeekBase = 0;
    access.directPokeBase = 0;
    access.device = this;
    mySystem->setPageAccess(i >> shift, access);
  }

  // Setup the second segment to always point to the last ROM slice
  for(uInt32 j = 0x1A00; j < (0x1FE0U & ~mask); j += (1 << shift))
  {
    access.device = this;
    access.directPeekBase = &myImage[7 * 2048 + (j & 0x07FF)];
    access.directPokeBase = 0;
    mySystem->setPageAccess(j >> shift, access);
  }
  myCurrentSlice[1] = 7;

  // Install some default banks for the RAM and first segment
  bankRAM(0);
  bank(0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8 CartridgeE7::peek(uInt16 address)
{
  address &= 0x0FFF;

  // Switch banks if necessary
  if((address >= 0x0FE0) && (address <= 0x0FE7))
  {
    bank(address & 0x0007);
  }
  else if((address >= 0x0FE8) && (address <= 0x0FEB))
  {
    bankRAM(address & 0x0003);
  }

  // NOTE: The following does not handle reading from RAM, however,
  // this function should never be called for RAM because of the
  // way page accessing has been setup
  // TODO - determine what really happens when you read from the write port
  return myImage[(myCurrentSlice[address >> 11] << 11) + (address & 0x07FF)];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE7::poke(uInt16 address, uInt8)
{
  address &= 0x0FFF;

  // Switch banks if necessary
  if((address >= 0x0FE0) && (address <= 0x0FE7))
  {
    bank(address & 0x0007);
  }
  else if((address >= 0x0FE8) && (address <= 0x0FEB))
  {
    bankRAM(address & 0x0003);
  }

  // NOTE: This does not handle writing to RAM, however, this 
  // function should never be called for RAM because of the
  // way page accessing has been setup
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE7::bankRAM(uInt16 bank)
{ 
  // Remember what bank we're in
  myCurrentRAM = bank;
  uInt16 offset = bank << 8;
  uInt16 shift = mySystem->pageShift();

  // Setup the page access methods for the current bank
  System::PageAccess access;

  // Set the page accessing method for the 256 bytes of RAM writing pages
  for(uInt32 j = 0x1800; j < 0x1900; j += (1 << shift))
  {
    access.device = this;
    access.directPeekBase = 0;
    access.directPokeBase = &myRAM[1024 + offset + (j & 0x00FF)];
    mySystem->setPageAccess(j >> shift, access);
  }

  // Set the page accessing method for the 256 bytes of RAM reading pages
  access.directPeekBase = 0;
  for(uInt32 k = 0x1900; k < 0x1A00; k += (1 << shift))
  {
    access.device = this;
    access.directPeekBase = &myRAM[1024 + offset + (k & 0x00FF)];
    access.directPokeBase = 0;
    mySystem->setPageAccess(k >> shift, access);
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void CartridgeE7::bank(uInt16 slice)
{ 
  if(myBankLocked) return;

  // Remember what bank we're in
  myCurrentSlice[0] = slice;
  uInt16 offset = slice << 11;
  uInt16 shift = mySystem->pageShift();

  System::PageAccess access;

  // Setup the page access methods for the current bank
  if(slice != 7)
  {
    // Map ROM image into first segment
    for(uInt32 address = 0x1000; address < 0x1800; address += (1 << shift))
    {
      access.device = this;
      access.directPeekBase = &myImage[offset + (address & 0x07FF)];
      access.directPokeBase = 0;
      mySystem->setPageAccess(address >> shift, access);
    }
  }
  else
  {
    // Set the page accessing method for the 1K slice of RAM writing pages
    for(uInt32 j = 0x1000; j < 0x1400; j += (1 << shift))
    {
      access.device = this;
      access.directPeekBase = 0;
      access.directPokeBase = &myRAM[j & 0x03FF];
      mySystem->setPageAccess(j >> shift, access);
    }

    // Set the page accessing method for the 1K slice of RAM reading pages
    for(uInt32 k = 0x1400; k < 0x1800; k += (1 << shift))
    {
      access.device = this;
      access.directPeekBase = &myRAM[k & 0x03FF];
      access.directPokeBase = 0;
      mySystem->setPageAccess(k >> shift, access);
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int CartridgeE7::bank()
{
  return myCurrentSlice[0];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int CartridgeE7::bankCount()
{
  return 8;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeE7::patch(uInt16 address, uInt8 value)
{
  address = address & 0x0FFF;
  myImage[(myCurrentSlice[address >> 11] << 11) + (address & 0x07FF)] = value;
  return true;
} 

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
uInt8* CartridgeE7::getImage(int& size)
{
  size = 16384;
  return &myImage[0];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeE7::save(Serializer& out) const
{
  const string& cart = name();

  try
  {
    uInt32 i;

    out.putString(cart);

    out.putInt(2);
    for(i = 0; i < 2; ++i)
      out.putInt(myCurrentSlice[i]);

    out.putInt(myCurrentRAM);

    // The 2048 bytes of RAM
    out.putInt(2048);
    for(i = 0; i < 2048; ++i)
      out.putByte((char)myRAM[i]);
  }
  catch(const char* msg)
  {
    cerr << "ERROR: CartridgeE7::save" << endl << "  " << msg << endl;
    return false;
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool CartridgeE7::load(Serializer& in)
{
  const string& cart = name();

  try
  {
    if(in.getString() != cart)
      return false;

    uInt32 i, limit;

    limit = (uInt32) in.getInt();
    for(i = 0; i < limit; ++i)
      myCurrentSlice[i] = (uInt16) in.getInt();

    myCurrentRAM = (uInt16) in.getInt();

    // The 2048 bytes of RAM
    limit = (uInt32) in.getInt();
    for(i = 0; i < limit; ++i)
      myRAM[i] = (uInt8) in.getByte();
  }
  catch(const char* msg)
  {
    cerr << "ERROR: CartridgeE7::load" << endl << "  " << msg << endl;
    return false;
  }

  // Set up the previously used banks for the RAM and segment
  bankRAM(myCurrentRAM);
  bank(myCurrentSlice[0]);

  return true;
}
