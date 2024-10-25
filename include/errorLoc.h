#ifndef ERROR_LOC
#define ERROR_LOC

#include <string>
#include <vector>

class errorLoc {
public:
  std::pair<int, int> loc;      // line and column of a found error in file 
  std::string         spelling; // error description 

  errorLoc(std::pair<int, int> _loc, std::string _spell): loc(_loc), spelling(_spell) {}
};

std::string getErrorSpelling(std::pair<int, int> curr_loc, std::vector<errorLoc> errors);

#endif
