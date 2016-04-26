#include <oscope/version.h>

#include <stdio.h>

#include "Application.hpp"

using namespace oscope;

bool getScreenDimensions(int *width, int *height);

int main(int argc, char *argv[]) {
	Version v = Version::instance();
	fprintf(stdout, "%d.%d\n", v.major, v.minor);
    
    int width, height;
    getScreenDimensions(&width, &height);
    width=1024;
    height=768;
    
    try {
        nanogui::init();
        
        {
            nanogui::ref<Application> app = new Application(width, height);
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }
        
        nanogui::shutdown();
    }
    catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
#if defined(_WIN32)
        MessageBoxA(nullptr, error_msg.c_str(), NULL, MB_ICONERROR | MB_OK);
#else
        std::cerr << error_msg << endl;
#endif
        return -1;
    }

    
    
    
    
    
    
	return 0;
}
