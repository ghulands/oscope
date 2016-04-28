#include <oscope/oscope.h>

//#define __NO_STD_VECTOR // Use cl::vector instead of STL version
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <stdio.h>

#include "Application.hpp"

using namespace oscope;
using namespace cl;

bool getScreenDimensions(int *width, int *height);

#include <math.h>

void generate_sine_wave(long num_samples, int16_t *samples) {
    double interval = (M_PI * 2.0) / (num_samples * 1.0);
    double cur = 0.0;
    for (long i = 0; i < num_samples; i++) {
        cur = i * interval;
        cur = sin(cur);
        samples[i] = cur * INT16_MAX;
    }
}

int main(int argc, char *argv[]) {
	Version v = Version::instance();
	fprintf(stdout, "%d.%d\n", v.major, v.minor);
    
    vector<Platform> platforms;
    Platform::get(&platforms);
    
    // Select the default platform and create a context using this platform and the GPU
    cl_context_properties cps[3] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)(platforms[0])(),
        0
    };
    Context context( CL_DEVICE_TYPE_GPU, cps);
    
    // Get a list of devices on this platform
    vector<Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    std::cout<< "Using device: "<<devices[0].getInfo<CL_DEVICE_VENDOR>()<<"\n";
    
    time_t start = time(NULL);
    uint64_t count = 2600000000;
    int16_t *samples = (int16_t *)malloc(sizeof(int16_t) * count);
    time_t end = time(NULL);
    fprintf(stdout, "malloc took %ld\n", end - start);
    
    start = time(NULL);
    generate_sine_wave(count, samples);
    end = time(NULL);
    fprintf(stdout, "generate_sine_wave took %ld\n", end - start);
    
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
