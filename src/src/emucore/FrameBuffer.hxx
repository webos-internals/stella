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
// $Id: FrameBuffer.hxx,v 1.94 2008/03/13 22:58:06 stephena Exp $
//============================================================================

#ifndef FRAMEBUFFER_HXX
#define FRAMEBUFFER_HXX

#include <SDL.h>

class OSystem;
class Console;

namespace GUI {
  class Font;
  class Surface;
}

#include "EventHandler.hxx"
#include "VideoModeList.hxx"
#include "bspf.hxx"


// Text alignment modes for drawString()
enum TextAlignment {
  kTextAlignLeft,
  kTextAlignCenter,
  kTextAlignRight
};

// Line types for drawing rectangular frames
enum FrameStyle {
  kSolidLine,
  kDashLine
};

// Different types of framebuffer derived objects
enum BufferType {
  kSoftBuffer,
  kGLBuffer
};

// Positions for onscreen/overlaid messages
enum MessagePosition {
  kTopLeft,
  kTopCenter,
  kTopRight,
  kMiddleLeft,
  kMiddleCenter,
  kMiddleRight,
  kBottomLeft,
  kBottomCenter,
  kBottomRight
};

// Colors indices to use for the various GUI elements
enum {
  kColor = 256,
  kBGColor,
  kShadowColor,
  kTextColor,
  kTextColorHi,
  kTextColorEm,
  kDlgColor,
  kWidColor,
  kWidFrameColor,
  kBtnColor,
  kBtnColorHi,
  kBtnTextColor,
  kBtnTextColorHi,
  kCheckColor,
  kScrollColor,
  kScrollColorHi,
  kSliderColor,
  kSliderColorHi,
  kDbgChangedColor,
  kDbgChangedTextColor,
  kDbgColorHi,
  kNumColors
};


/**
  This class encapsulates the MediaSource and is the basis for the video
  display in Stella.  All graphics ports should derive from this class for
  platform-specific video stuff.

  All GUI elements (ala ScummVM) are drawn here as well.

  @author  Stephen Anthony
  @version $Id: FrameBuffer.hxx,v 1.94 2008/03/13 22:58:06 stephena Exp $
*/
class FrameBuffer
{
  friend class TiaOutputWidget;

  public:
    /**
      Creates a new Frame Buffer
    */
    FrameBuffer(OSystem* osystem);

    /**
      Destructor
    */
    virtual ~FrameBuffer();

    /**
      (Re)initializes the framebuffer display.  This must be called before any
      calls are made to derived methods.

      @param title   The title of the window
      @param width   The width of the framebuffer
      @param height  The height of the framebuffer
    */
    void initialize(const string& title, uInt32 width, uInt32 height);

    /**
      Updates the display, which depending on the current mode could mean
      drawing the mediasource, any pending menus, etc.
    */
    void update();

    /**
      Shows a message onscreen.

      @param message  The message to be shown
      @param position Onscreen position for the message
      @param color    Color of text in the message
    */
    void showMessage(const string& message,
                     MessagePosition position = kBottomCenter,
                     int color = kTextColorHi);

    /**
      Hides any onscreen messages.
    */
    void hideMessage();

    /**
      Returns the current width of the framebuffer *before* any scaling.

      @return  The current unscaled width
    */
    inline const uInt32 baseWidth()  { return myBaseDim.w; }

    /**
      Returns the current height of the framebuffer *before* any scaling.

      @return  The current unscaled height
    */
    inline const uInt32 baseHeight() { return myBaseDim.h; }

    /**
      Returns the current width of the framebuffer image.
      Note that this will take into account the current scaling (if any).

      @return  The current width
    */
    inline const uInt32 imageWidth()  { return myImageDim.w; }

    /**
      Returns the current height of the framebuffer image.
      Note that this will take into account the current scaling (if any).

      @return  The current height
    */
    inline const uInt32 imageHeight() { return myImageDim.h; }

    /**
      Indicates that the TIA area is dirty, and certain areas need
      to be redrawn.
    */
    void refresh();

    /**
      Toggles between fullscreen and window mode.
      Grabmouse activated when in fullscreen mode.  
    */
    void toggleFullscreen();

    /**
      Enables/disables fullscreen mode.
      Grabmouse activated when in fullscreen mode.  

      @param enable  Set the fullscreen mode to this value
    */
    void setFullscreen(bool enable);

    /**
      This method is called when the user wants to switch to the next available
      video mode (functionality depends on fullscreen or windowed mode).
      direction = -1 means go to the next lower video mode
      direction = +1 means go to the next higher video mode

      @param direction  Described above
    */
    bool changeVidMode(int direction);

    /**
      Sets the state of the cursor (hidden or grabbed) based on the
      current mode.
    */
    void setCursorState();

    /**
      Shows or hides the cursor based on the given boolean value.
    */
    virtual void showCursor(bool show);

    /**
      Grabs or ungrabs the mouse based on the given boolean value.
    */
    void grabMouse(bool grab);

    /**
      Answers if the display is currently in fullscreen mode.
    */
    bool fullScreen() const;

    /**
      Set the title for the main SDL window.
    */
    void setWindowTitle(const string& title);

    /**
      Set up the TIA/emulation palette for a screen of any depth > 8.

      @param palette  The array of colors
    */
    virtual void setTIAPalette(const uInt32* palette);

    /**
      Set up the user interface palette for a screen of any depth > 8.

      @param palette  The array of colors
    */
    virtual void setUIPalette(const uInt32* palette);

    /**
      This method should be called to draw a rectangular box with sides
      at the specified coordinates.

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the box
      @param h      The height of the box
      @param colorA Lighter color for outside line.
      @param colorB Darker color for inside line.
    */
    void box(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
             int colorA, int colorB);

    /**
      This method should be called to draw a framed rectangle.
      I'm not exactly sure what it is, so I can't explain it :)

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the area
      @param h      The height of the area
      @param color  The color of the surrounding frame
    */
    void frameRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
                   int color, FrameStyle style = kSolidLine);

    /**
      This method should be called to draw the specified string.

      @param font   The font to draw the string with
      @param str    The string to draw
      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the string area
      @param h      The height of the string area
      @param color  The color of the text
      @param align  The alignment of the text in the string width area
      @param deltax 
      @param useEllipsis  Whether to use '...' when the string is too long
    */
    void drawString(const GUI::Font* font, const string& str, int x, int y, int w,
                    int color, TextAlignment align = kTextAlignLeft,
                    int deltax = 0, bool useEllipsis = true);

    /**
      Informs the Framebuffer of a change in EventHandler state.
    */
    virtual void stateChanged(EventHandler::State state) { }

  //////////////////////////////////////////////////////////////////////
  // The following methods are system-specific and must be implemented
  // in derived classes.
  //////////////////////////////////////////////////////////////////////
  public:
    /**
      This method is called to get the specified scanline data.

      @param row  The row we are looking for
      @param data The actual pixel data (in bytes)
    */
    virtual void scanline(uInt32 row, uInt8* data) const = 0;

    /**
      This method should be called to draw a horizontal line.

      @param x     The first x coordinate
      @param y     The y coordinate
      @param x2    The second x coordinate
      @param color The color of the line
    */
    virtual void hLine(uInt32 x, uInt32 y, uInt32 x2, int color) = 0;

    /**
      This method should be called to draw a vertical line.

      @param x     The x coordinate
      @param y     The first y coordinate
      @param y2    The second y coordinate
      @param color The color of the line
    */
    virtual void vLine(uInt32 x, uInt32 y, uInt32 y2, int color) = 0;

    /**
      This method should be called to draw a filled rectangle.

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the area
      @param h      The height of the area
      @param color  The color of the area
    */
    virtual void fillRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h,
                          int color) = 0;

    /**
      This method should be called to draw the specified character.

      @param font   The font to use to draw the character
      @param c      The character to draw
      @param x      The x coordinate
      @param y      The y coordinate
      @param color  The color of the character
    */
    virtual void drawChar(const GUI::Font* font, uInt8 c, uInt32 x, uInt32 y,
                          int color) = 0;

    /**
      This method should be called to draw the bitmap image.

      @param bitmap The data to draw
      @param x      The x coordinate
      @param y      The y coordinate
      @param color  The color of the character
      @param h      The height of the data image
    */
    virtual void drawBitmap(uInt32* bitmap, Int32 x, Int32 y, int color,
                            Int32 h = 8) = 0;

    /**
      This method should be called to draw an SDL surface.

      @param surface The data to draw
      @param x       The x coordinate
      @param y       The y coordinate
    */
    virtual void drawSurface(const GUI::Surface* surface, Int32 x, Int32 y) = 0;

    /**
      This method should be called to convert and copy a given row of RGB
      data into an SDL surface.

      @param surface  The data to draw
      @param row      The row of the surface the data should be placed in
      @param data     The data in uInt8 R/G/B format
      @param rowbytes The number of bytes in row of 'data'
    */
    virtual void bytesToSurface(GUI::Surface* surface, int row,
                                uInt8* data, int rowbytes) const = 0;

    /**
      This method should be called to translate the given coordinates
      to their unzoomed/unscaled equivalents.

      @param x  X coordinate to translate
      @param y  Y coordinate to translate
    */
    virtual void translateCoords(Int32& x, Int32& y) const = 0;

    /**
      This method should be called to add a dirty rectangle
      (ie, an area of the screen that has changed)

      @param x      The x coordinate
      @param y      The y coordinate
      @param w      The width of the area
      @param h      The height of the area
    */
    virtual void addDirtyRect(uInt32 x, uInt32 y, uInt32 w, uInt32 h) = 0;

    /**
      Enable/disable phosphor effect.
    */
    virtual void enablePhosphor(bool enable, int blend) = 0;

    /**
      This method is called to map a given r,g,b triple to the screen palette.

      @param r  The red component of the color.
      @param g  The green component of the color.
      @param b  The blue component of the color.
    */
    virtual Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b) const = 0;

    /**
      This method is called to create a surface compatible with the one
      currently in use, but having the given dimensions.

      @param width   The requested width of the new surface.
      @param height  The requested height of the new surface.
    */
    virtual GUI::Surface* createSurface(int width, int height) const = 0;

    /**
      This method is called to query the type of the FrameBuffer.
    */
    virtual BufferType type() const = 0;

  protected:
    /**
      This method is called to initialize the video subsystem
      with the given video mode.  Normally, it will also call setVidMode().
    */
    virtual bool initSubsystem(VideoMode mode) = 0;

    /**
      This method is called to change to the given video mode.

      @param mode  The video mode to use for rendering the mediasource
    */
    virtual bool setVidMode(VideoMode mode) = 0;

    /**
      Switches between the filtering options in the video subsystem.
    */
    virtual void toggleFilter() = 0;

    /**
      This method should be called anytime the MediaSource needs to be redrawn
      to the screen.
    */
    virtual void drawMediaSource() = 0;

    /**
      This method is called before any drawing is done (per-frame).
    */
    virtual void preFrameUpdate() = 0;

    /**
      This method is called after any drawing is done (per-frame).
    */
    virtual void postFrameUpdate() = 0;

    /**
      This method is called to provide information about the FrameBuffer.
    */
    virtual string about() const = 0;

  protected:
    // The parent system for the framebuffer
    OSystem* myOSystem;

    // Dimensions of the base image, before scaling.
    // All external GUI items should refer to these dimensions,
    //  since this is the *real* size of the image.
    // The other sizes are simply scaled versions of these dimensions.
    SDL_Rect myBaseDim;

    // Dimensions of the actual image, after zooming
    SDL_Rect myImageDim;

    // Dimensions of the SDL window (not always the same as the image)
    SDL_Rect myScreenDim;

    // The SDL video buffer
    SDL_Surface* myScreen;

    // SDL initialization flags
    uInt32 mySDLFlags;

    // TIA palettes for normal and phosphor modes
    Uint32 myDefPalette[256+kNumColors];
    Uint32 myAvgPalette[256][256];

    // Indicates if the TIA area should be redrawn
    bool theRedrawTIAIndicator;

    // Use phosphor effect (aka no flicker on 30Hz screens)
    bool myUsePhosphor;

    // Amount to blend when using phosphor effect
    int myPhosphorBlend;

  private:
    /**
      Set the icon for the main SDL window.
    */
    void setWindowIcon();

    /**
      Set the icon for the main SDL window.
    */
    void drawMessage();

    /**
      Used to calculate an averaged color for the 'phosphor' effect.

      @param c1  Color 1
      @param c2  Color 2

      @return  Averaged value of the two colors
    */
    uInt8 getPhosphor(uInt8 c1, uInt8 c2);

    /**
      Calculate the maximum level by which the base window can be zoomed and
      still fit in the given screen dimensions.
    */
    uInt32 maxWindowSizeForScreen(uInt32 screenWidth, uInt32 screenHeight);

    /**
      Set all possible video modes (both windowed and fullscreen) available for
      this framebuffer based on current emulation state and maximum window size.
    */
    void setAvailableVidModes();

    /**
      Returns an appropriate video mode based on the current eventhandler
      state, taking into account the maximum size of the window.

      @return  A valid VideoMode for this framebuffer
    */
    VideoMode getSavedVidMode();

  private:
    // Indicates the number of times the framebuffer was initialized
    uInt32 myInitializedCount;

    // Used to set intervals between messages while in pause mode
    uInt32 myPausedCount;

    // Used for onscreen messages
    struct Message {
      string text;
      int counter;
      int x, y, w, h;
      uInt32 color;
    };
    Message myMessage;

    // The list of all available video modes for this framebuffer
    VideoModeList myWindowedModeList;
    VideoModeList myFullscreenModeList;
    VideoModeList* myCurrentModeList;
};

#endif
