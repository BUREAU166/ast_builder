#ifndef VISITOR_DATA
#define VISITOR_DATA

#include <string>
#include <vector>
class visitorData {
public:
  int         parent_id;  // Id of a parent Node
  int         id;
  int         treeLevel;
  std::string kindName;   // Name of a token (e.g. FuncDecl, VarDecl etc.)
  std::string spellName;  // The exact name of a token (name of a function, variable)
  std::string typeName;   // Type name for typed tokens 
  int         dataParent; // Declaration id of a var 
  int         scope;      // Scope at which this token is available
  bool        isError;
  std::string errorSpelling;
};

int find_data_parent(std::vector<visitorData> graph, visitorData* vd);

int find_parent(std::vector<visitorData> graph, int level);

#endif


