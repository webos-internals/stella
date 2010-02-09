/*   SDLMain.h - main entry point for our Cocoa-ized SDL app
       Initial Version: Darrell Walisser <dwaliss1@purdue.edu>
       Non-NIB-Code & other changes: Max Horn <max@quendi.de>

    Feel free to customize this file to suit your needs
*/
/* $Id: SDLMain.h 1724 2009-05-13 13:55:40Z stephena $ */

#import <Cocoa/Cocoa.h>

@interface SDLMain : NSObject
{
}
+ (SDLMain *)sharedInstance;

@end
