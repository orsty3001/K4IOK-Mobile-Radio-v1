# include <stdio.h>
# include "vfo.h"
# include "ui.h"
int main(){VFOState A{146.520,146.520,12.5,false},B{446.000,446.000,12.5,false};ui_render(A,B);return 0;}