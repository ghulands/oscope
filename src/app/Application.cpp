#include "Application.hpp"

#include <nanogui/glutil.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/button.h>
#include <nanogui/popupbutton.h>
#include <nanogui/checkbox.h>
#include <nanogui/widget.h>

#include "NonMovableWindow.hpp"

namespace oscope {
    Application::Application(int width, int height) : nanogui::Screen(Eigen::Vector2i(width, height), "Oscope", false, true) {
        using namespace nanogui;
        
        Window *window = new NonMovableWindow(this, "Oscope");
        window->setPosition(Vector2i(0, 0));
        window->setSize(Vector2i(width, height));
        window->setLayout(new GroupLayout());
        window->setVisible(true);
        
        new Label(window, "Push buttons", "sans-bold");
        
        Button *b = new Button(window, "Plain button");
        b->setCallback([] { cout << "pushed!" << endl; });
        b = new Button(window, "Styled");
        b->setBackgroundColor(Color(0, 0, 255, 25));
        b->setCallback([] { cout << "pushed!" << endl; });
        
        new Label(window, "Toggle buttons", "sans-bold");
        b = new Button(window, "Toggle me");
        b->setFlags(Button::ToggleButton);
        b->setChangeCallback([](bool state) { cout << "Toggle button state: " << state << endl; });
        
        new Label(window, "Radio buttons", "sans-bold");
        b = new Button(window, "Radio button 1");
        b->setFlags(Button::RadioButton);
        b = new Button(window, "Radio button 2");
        b->setFlags(Button::RadioButton);
        
        new Label(window, "A tool palette", "sans-bold");
        Widget *tools = new Widget(window);
        tools->setLayout(new BoxLayout(Orientation::Horizontal,
                                       Alignment::Middle, 0, 6));
        
        new Label(window, "Popup buttons", "sans-bold");
        PopupButton *popupBtn = new PopupButton(window, "Popup", ENTYPO_ICON_EXPORT);
        Popup *popup = popupBtn->popup();
        popup->setLayout(new GroupLayout());
        new Label(popup, "Arbitrary widgets can be placed here");
        new CheckBox(popup, "A check box");
        popupBtn = new PopupButton(popup, "Recursive popup", ENTYPO_ICON_FLASH);
        popup = popupBtn->popup();
        popup->setLayout(new GroupLayout());
        new CheckBox(popup, "Another check box");

        performLayout(mNVGContext);
    }
    
    void Application::draw(NVGcontext *ctx) {
        Screen::draw(ctx);
    }
    
    bool Application::keyboardEvent(int key, int scancode, int action, int modifiers) {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }
        return false;
    }
};
