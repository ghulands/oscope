#include "NonMovableWindow.hpp"

namespace oscope {
    NonMovableWindow::NonMovableWindow(Widget *parent, const std::string &title) : nanogui::Window(parent, title) {
        
    }
    
    bool NonMovableWindow::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
        if (Widget::mouseButtonEvent(p, button, down, modifiers))
            return true;
        return false;
    }
    
    bool NonMovableWindow::mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) {
        return false;
    }
}
