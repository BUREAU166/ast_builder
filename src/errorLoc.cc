#include "../include/errorLoc.h"

std::string getErrorSpelling(std::pair<int, int> curr_loc, std::vector<errorLoc> errors) {
  printf("  ?-- loc: %d %d\n", curr_loc.first, curr_loc.second);
  for(errorLoc el: errors) {
    printf("  ?-- err: %d %d\n", el.loc.first, el.loc.second);
    if(el.loc == curr_loc) {
      return el.spelling;
    }
  } 

  return "";
}
