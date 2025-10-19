# pragma once
#include "vfo.h"
enum class Action{ROTATE,PRESS,LONG};
void input_handle(VFOState&,Action,int=0);