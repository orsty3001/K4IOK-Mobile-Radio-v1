# include "input.h"
void input_handle(VFOState& v,Action a,int d){ if(a==Action::ROTATE)vfo_step(v,d);}