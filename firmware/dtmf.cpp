#include "dtmf.h"
#include <cmath>
#include <map>
void dtmf_generate(const std::string &digits,float fs,std::vector<float>&out){
 std::map<char,std::pair<float,float>> tbl={
  {'1',{697,1209}}, {'2',{697,1336}}, {'3',{697,1477}},
  {'4',{770,1209}}, {'5',{770,1336}}, {'6',{770,1477}},
  {'7',{852,1209}}, {'8',{852,1336}}, {'9',{852,1477}},
  {'0',{941,1336}}, {'*',{941,1209}}, {'#',{941,1477}} };
 for(char d:digits){auto f=tbl[d];for(int n=0;n<(int)(fs*0.1);++n)
  out.push_back(0.5*(sinf(2*M_PI*f.first*n/fs)+sinf(2*M_PI*f.second*n/fs)));}
}
