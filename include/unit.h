#ifndef UNIT_H
#define UNIT_H

#include <clang-c/Index.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cstring>
#include "../include/visitorData.h"
#include "../include/errorLoc.h"

struct macroDef {
  std::pair<int, int> loc;
  int id;
  unsigned int isFunction;
};

class parseUnit {
public:
  int id;
  int scope;
  CXTranslationUnit tu;
  int diags;
  std::vector<errorLoc> errors;
  std::vector<visitorData> graph;
  char* filename;
  char* path;
  char* targetName;
  unsigned int curLevel;
  int legacyScope;
  std::vector<visitorData> legacyGraph;
  CXCursor currParent;
  std::vector<visitorData> currentScopeChildren;
  std::vector<macroDef> macrosLoc;

  parseUnit(char* filename, char* target) {
    this->id = 0; 
    this->scope = 0; 
    this->diags = 0;
    this->curLevel = 0;
    this->filename = filename;
    path = (char*)malloc(200 * sizeof(char));
    this->path = realpath(filename, path);
    this->targetName = target;
    this->legacyScope = -1;
  }

  // visualization to graphviz ext
  
  std::string to_dot();

  std::string getCursorKindName( CXCursorKind cursorKind );
 
  std::string getCursorSpelling( CXCursor cursor );

  std::string getBinaryOperator(CXCursor cursor);

  std::string getUnaryOperator(CXCursor cursor);

  std::string getTypeDef(CXCursor cursor);

  std::string getIntegerLiteral(CXCursor cursor);

  std::string getMacrosText(CXCursor cursor);

  std::string getTypeSpell(CXType cxType);

  std::string getCurrentFilePath(CXSourceLocation location);

  std::pair<int, int> getCurrentLocation(CXSourceLocation location);

  CXChildVisitResult visitor( CXCursor cursor, CXCursor /* parent */);

  std::vector<errorLoc> diagnoseForErrors();
  
  static CXChildVisitResult visitorHelper(CXCursor cursor, CXCursor parent, CXClientData client_data);

  std::vector<visitorData> parse();
};

#endif
