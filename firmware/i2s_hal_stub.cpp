#include "i2s_hal_stub.h"
#include <iostream>
void i2s_init(){ std::cout<<"I2S HAL Init"<<std::endl; }
void i2s_write(float l,float r){ (void)l; (void)r; }
