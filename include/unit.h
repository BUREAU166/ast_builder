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

class scopedBlock {
public:
  std::string name;
  std::vector<visitorData> graph;
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
  std::vector<std::string> targets;
  unsigned int curLevel;
  int legacyScope;
  int fakeParent;
  std::vector<visitorData> legacyGraph;
  CXCursor currParent;
  std::vector<visitorData> currentScopeChildren;
  std::vector<macroDef> macrosLoc;
  std::vector<scopedBlock> scopedGraph;
  std::string scopeName;
  std::vector<std::string> headers;
  bool isReadProj;
  std::vector<std::vector<visitorData>> projGraphs;
  std::vector<std::string> fileNames;
  char* entryFile;
  std::vector<std::string> savedTargets;
  std::vector<std::pair<int, std::string>> targetNodes;

  parseUnit(char* filename, char* target) {
    this->id = 0; 
    this->scope = 0; 
    this->diags = 0;
    this->curLevel = 0;
    this->filename = filename;
    this->fileNames.push_back(std::string(filename));
    path = (char*)malloc(200 * sizeof(char));
    this->path = realpath(filename, path);
    this->targetName = target;
    this->targets.push_back(std::string(target));
    this->legacyScope = -1;
    this->entryFile = filename;
  }

  parseUnit(char* filename) {
    this->id = 0; 
    this->scope = 0; 
    this->diags = 0;
    this->curLevel = 0;
    this->filename = filename;
    path = (char*)malloc(200 * sizeof(char));
    this->path = realpath(filename, path);
    this->targetName = "";
    //this->targets.push_back(std::string(target));
    this->legacyScope = -1;
  }

  void prepare(char* filename) {
    this->id = 0; 
    this->scope = 0; 
    this->diags = 0;
    this->curLevel = 0;
    this->filename = filename;
    if (std::find(fileNames.begin(), fileNames.end(), std::string(filename)) == fileNames.end() )
      this->fileNames.push_back(std::string(filename));
    path = (char*)malloc(200 * sizeof(char));
    this->path = realpath(filename, path);
    //this->targetName = ;
    this->legacyScope = -1;

    this->graph.clear();
    this->legacyGraph.clear();
    this->currentScopeChildren.clear();
    this->scopedGraph.clear();

    this->isReadProj = true;
  }

  // visualization to graphviz ext
  std::string to_sub_dot();

  std::string to_dot(std::vector<visitorData> _graph, std::string name, bool withSub, std::string modName);

  std::string toJson(std::vector<visitorData> _graph, std::string name, bool withSub, std::string modName);

  void checkScopedGraph();

  std::vector<scopedBlock> cutUnusedByTarget();

  std::string getInclusionPath(CXCursor cursor);

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

  std::vector<visitorData> parseIncludes();

  std::vector<visitorData> parse();

  std::vector<std::string> getProjectFiles(std::string projPath);
};

#endif
