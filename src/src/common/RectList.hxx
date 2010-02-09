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
// $Id: RectList.hxx 1724 2009-05-13 13:55:40Z stephena $
//============================================================================

#ifndef RECTLIST_HXX
#define RECTLIST_HXX

#include <SDL.h>

class RectList
{
  public:
    RectList(Uint32 size = 256);
    ~RectList();

    void add(SDL_Rect* rect);

    SDL_Rect* rects();
    Uint32 numRects();
    void start();
    void print(int boundWidth, int boundHeight);

  private:
    Uint32 currentSize, currentRect;

    SDL_Rect* rectArray;
};

#endif
