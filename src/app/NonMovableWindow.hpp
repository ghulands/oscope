#pragma once

#ifndef _OSCOPE_NON_MOVABLE_WINDOW_H_
#define _OSCOPE_NON_MOVABLE_WINDOW_H_

#include <nanogui/window.h>
#include <nanogui/glutil.h>
#include <nanogui/widget.h>

using Eigen::Vector2i;

namespace oscope {
    class NonMovableWindow : public nanogui::Window {
    public:
        NonMovableWindow(Widget *parent, const std::string &title = "Untitled");
        
        virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
        virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers);
    };
};

#endif // _OSCOPE_NON_MOVABLE_WINDOW_H_
