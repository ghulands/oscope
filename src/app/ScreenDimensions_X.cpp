//
//  ScreenDimensions.cpp
//  oscope_app
//
//  Created by Greg Hulands on 4/25/16.
//
//

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h> 
#include <stdio.h>
#include <stdlib.h>

bool getScreenDimensions(int *width, int *height) {
    Display *display = XOpenDisplay(NULL);
	
	if (display == NULL) {
		fprintf(stderr, "Failed to XOpenDisplay\n");
		exit(-1);
	}

	Screen *screen = ScreenOfDisplay(display, 0);
	if (screen == NULL) {
		fprintf(stderr, "Failed to ScreenOfDisplay\n");
		exit(-1);
	}

    *width = (int)WidthOfScreen(screen);
    *height = (int)HeightOfScreen(screen);
    
    return true;
}
