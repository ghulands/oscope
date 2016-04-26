//
//  ScreenDimensions.cpp
//  oscope_app
//
//  Created by Greg Hulands on 4/25/16.
//
//

#include <Cocoa/Cocoa.h>

bool getScreenDimensions(int *width, int *height) {
    NSRect rect = [[NSScreen mainScreen] frame];
    
    *width = (int)NSWidth(rect);
    *height = (int)NSHeight(rect);
    
    return true;
}
