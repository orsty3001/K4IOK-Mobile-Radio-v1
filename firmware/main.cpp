#include <iostream>
#include "audio_pipeline.h"
#include "i2s_hal_stub.h"

int main() {
    std::cout << "Dual-IQ Ham Radio Firmware Booting..." << std::endl;
    i2s_init();
    audio_pipeline_init();
    while (true) {
        audio_pipeline_process();
    }
    return 0;
}
