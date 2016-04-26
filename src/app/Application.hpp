#pragma once

#ifndef _OSCOPE_APPLICATION_H_
#define _OSCOPE_APPLICATION_H_

#include <nanogui/screen.h>

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

namespace oscope {
    class Application : public nanogui::Screen {
    public:
        Application(int width, int height);
        
        virtual void draw(NVGcontext *ctx);
        virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
        
    };
}

#endif // _OSCOPE_APPLICATION_H_