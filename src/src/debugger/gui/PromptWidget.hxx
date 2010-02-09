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
// $Id: PromptWidget.hxx 1724 2009-05-13 13:55:40Z stephena $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef PROMPT_WIDGET_HXX
#define PROMPT_WIDGET_HXX

#include <stdarg.h>

#include "GuiObject.hxx"
#include "Widget.hxx"
#include "Command.hxx"
#include "ScrollBarWidget.hxx"
#include "bspf.hxx"

class PromptWidget : public Widget, public CommandSender
{
  public:
    PromptWidget(GuiObject* boss, const GUI::Font& font,
                 int x, int y, int w, int h);
    virtual ~PromptWidget();

  public:
    int printf(const char *format, ...);
    int vprintf(const char *format, va_list argptr);
#undef putchar
    void putchar(int c);
    void print(const string& str);
    void printPrompt();
    bool saveBuffer(string& filename);

  protected:
    inline int &buffer(int idx) { return _buffer[idx % kBufferSize]; }

    void drawWidget(bool hilite);
    void drawCaret();
    void putcharIntern(int c);
    void insertIntoPrompt(const char *str);
    void updateScrollBuffer();
    void scrollToCurrent();

    // Line editing
    void specialKeys(int keycode);
    void nextLine();
    void killChar(int direction);
    void killLine(int direction);
    void killLastWord();

    // History
    void addToHistory(const char *str);
    void historyScroll(int direction);

    void handleMouseDown(int x, int y, int button, int clickCount);
    void handleMouseWheel(int x, int y, int direction);
    bool handleKeyDown(int ascii, int keycode, int modifiers);
    void handleCommand(CommandSender* sender, int cmd, int data, int id);

    // Account for the extra width of embedded scrollbar
    virtual int getWidth() const { return _w + kScrollBarWidth; }

    virtual bool wantsFocus() { return true; }

    void loadConfig();

  private:
    enum {
      kBufferSize     = 32768,
      kLineBufferSize = 256,
      kHistorySize    = 20
    };

    int  _buffer[kBufferSize];
    int  _linesInBuffer;

    int  _lineWidth;
    int  _linesPerPage;

    int  _currentPos;
    int  _scrollLine;
    int  _firstLineInBuffer;
	
    int  _promptStartPos;
    int  _promptEndPos;

    ScrollBarWidget* _scrollBar;

    char _history[kHistorySize][kLineBufferSize];
    int _historySize;
    int _historyIndex;
    int _historyLine;

    int _kConsoleCharWidth, _kConsoleCharHeight, _kConsoleLineHeight;

    bool _inverse;
    bool _makeDirty;
    bool _firstTime;
    bool _exitedEarly;

    int compareHistory(const char *histLine);
};

#endif
