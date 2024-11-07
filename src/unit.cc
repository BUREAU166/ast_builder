#include "../include/unit.h"
#include <fstream>
#include <string>

std::string parseUnit::to_sub_dot() {
  std::vector<scopedBlock> cut = cutUnusedByTarget();
  this->scopedGraph = cut;
  //printf("scopedGraph length: %d\n", scopedGraph.size());
  std::string graphTitle = "digraph G {\ncompound=true;\n";
  for(int i = 0; i < cut.size(); i++) {
    std::string graphName = "cluster_" + std::to_string(i);
    //printf("%s\n\n", to_dot(el.graph, el.graph[0].spellName, true).c_str());
    graphTitle.append(to_dot(cut[i].graph, graphName, true));
    graphTitle.append("\n");
  }
  graphTitle.append("}");

  return graphTitle;
}

std::string parseUnit::to_dot(std::vector<visitorData> _graph, std::string name, bool withSub) {
  std::string graphClass = withSub ? "subgraph" : "digraph";
  std::string dot  = graphClass + " " + name  +  " {\nnode [shape=record];\n";
  std::vector<std::pair<int, int>> pairs;
  std::vector<std::pair<int, int>> dpairs;
  std::vector<int> declars;
  int from, to;
  // state all nodes
  for(visitorData el: _graph) {
    // node name
    bool found_duplicate = false;
    for(int eid: declars) {
      if(el.id == eid) found_duplicate = true;
    }
    declars.push_back(el.id);
    if(!found_duplicate) {
      dot.append(name + std::to_string(el.id));
      std::string label = " [label=\"";
      if(el.spellName.size()) {
        label.append("{");
        label.append(el.kindName); 
        label.append(" | ");
        label.append(el.spellName);
        if(el.typeName.size()) {
          label.append(" | ");
          label.append(el.typeName);
        }
        if(el.errorSpelling.size()) {
          label.append(" | ");
          label.append(el.errorSpelling);
        }
        label.append("}");
        if (el.spellName == this->targetName) {
          label.append(" \"color=\"red\" style=\"filled\" fillcolor=\"white");
        } 
      }
      else {
        label.append(el.kindName);
        label.append("\"][shape=\"oval");
      } 
      if(el.isError) {
        label.append("\"][color=\"red");
      }
      label.append("\"];\n");
      dot.append(label);
    }
  }

  // draw connections
  for(int i = 1; i < _graph.size(); i++) {
    if(_graph[i].parent_id >= 0) {
      from = _graph[i].controlParent;
      to = _graph[i].id;
      bool found_duplicate = false;
      for(auto p: pairs) {
        if(p.first == from && p.second == to) {
          found_duplicate = true;
        }
      }

      pairs.push_back(std::pair<int, int>(from ,to));
      if(!found_duplicate) {
        if(!_graph[i].spellName.size() || graph[i].kindName == "ReturnStmt") {
          std::string conn = name + std::to_string(_graph[i].controlParent);
          conn.append(" -> ");
          conn.append(name + std::to_string(_graph[i].id));
          conn.append(";\n");
          dot.append(conn);
        }
        else {
          std::string conn = name + std::to_string(_graph[i-1].id);
          conn.append(" -> ");
          conn.append(name + std::to_string(_graph[i].id));
          conn.append(";\n");
          dot.append(conn);
        } 
      }
    }
  }

  // draw data flow connections
  /*
  for(int i = 0; i < _graph.size(); i++) {
    if(_graph[i].dataParent != -1) {
      from = _graph[i].id; 
      to = _graph[i].dataParent;
      bool found_duplicate = false;
      for(auto p: dpairs) {
        if(p.first == from && p.second == to) {
          found_duplicate = true;
        }
      }
      dpairs.push_back(std::pair<int, int>(from, to));
      if(!found_duplicate) {
        std::string conn = _graph[0].spellName + std::to_string(_graph[i].id);
        conn.append(" -> ");
        conn.append(_graph[0].spellName + std::to_string(_graph[i].dataParent));
        conn.append("[style=dashed];");
        conn.append("\n");
        dot.append(conn);
      }
    }
  }
*/
  dot.append("\n}");
  return dot;
}

std::string parseUnit::getCursorKindName( CXCursorKind cursorKind )
{
  CXString kindName  = clang_getCursorKindSpelling( cursorKind );
  std::string result = clang_getCString( kindName );

  clang_disposeString( kindName );
  return result;
}

std::string parseUnit::getCursorSpelling( CXCursor cursor )
{
  CXString cursorSpelling = clang_getCursorSpelling( cursor );
  std::string result      = clang_getCString( cursorSpelling );

  clang_disposeString( cursorSpelling );
  return result;
}

std::string parseUnit::getBinaryOperator(CXCursor cursor) {
  CXToken *tokens;
  unsigned numTokens;
  std::string tok = "";
  CXSourceRange range = clang_getCursorExtent(cursor);
  clang_tokenize(this->tu, range, &tokens, &numTokens);

  // @TODO FIX 
  /*
  CXString s = clang_getTokenSpelling(tu, tokens[1]);
  const char* str = clang_getCString(s);
  tok.append(std::string(str));
  clang_disposeString(s);
  */
  for(unsigned i=0; i<numTokens; i++) {
    CXString s = clang_getTokenSpelling(tu, tokens[i]);
    const char* str = clang_getCString(s);
    //printf("   got %s\n", str);
    tok += std::string(str);
    clang_disposeString(s);
  }

  std::string rr = "";
  for(char& c: tok) {
    if((c == '<') || (c == '>')) rr.append("\\");
    rr += c;
  }

  clang_disposeTokens(tu, tokens, numTokens);

  return rr;
}

std::string parseUnit::getUnaryOperator(CXCursor cursor) {
  CXToken *tokens;
  unsigned numTokens;
  std::string tok = "";
  CXSourceRange range = clang_getCursorExtent(cursor);
  clang_tokenize(this->tu, range, &tokens, &numTokens);

  // @TODO FIX 
  /*
  CXString s = clang_getTokenSpelling(tu, tokens[1]);
  const char* str = clang_getCString(s);
  //printf("   got UNARY %s\n", str);
  tok.append(std::string(str));
  clang_disposeString(s);
  */
  for(unsigned i=0; i<numTokens; i++) {
    CXString s = clang_getTokenSpelling(tu, tokens[i]);
    const char* str = clang_getCString(s);
    if(!strcmp(str, "<") || !strcmp(str, ">")) tok.append("\\");
    tok.append(std::string(str));
    clang_disposeString(s);
  }
  /*
  for(unsigned i=0; i<numTokens; i++) {
    CXString s = clang_getTokenSpelling(tu, tokens[i]);
    const char* str = clang_getCString(s);
    //printf("   got %s\n", str);
    tok = std::string(str);
    clang_disposeString(s);
  }
  */
  clang_disposeTokens(tu, tokens, numTokens);

  return tok;
}

std::string parseUnit::getTypeDef(CXCursor cursor) {
  CXType realType = clang_getTypedefDeclUnderlyingType(cursor);
  CXString realTypeName = clang_getTypeSpelling(realType);

  std::string result      = clang_getCString( realTypeName );

  clang_disposeString(realTypeName);

  return result;
}

std::string parseUnit::getIntegerLiteral(CXCursor cursor) {
  CXSourceRange range = clang_getCursorExtent(cursor);
  std::string tok;
  CXToken *tokens = 0;
  unsigned int nTokens = 0;
  clang_tokenize(this->tu, range, &tokens, &nTokens);
  for (unsigned int i = 0; i < nTokens; i++)
  {
      CXString spelling = clang_getTokenSpelling(tu, tokens[i]);
      tok = std::string(clang_getCString(spelling));
      clang_disposeString(spelling);
  }

  clang_disposeTokens(tu, tokens, nTokens); 
 
  return tok;
}

std::string parseUnit::getMacrosText(CXCursor cursor) {
  CXSourceRange range = clang_getCursorExtent(cursor);
  std::string tok = "";
  CXToken *tokens = 0;
  unsigned int nTokens = 0;
  clang_tokenize(this->tu, range, &tokens, &nTokens);
  for (unsigned int i = 0; i < nTokens; i++)
  {
      CXString spelling = clang_getTokenSpelling(tu, tokens[i]);
      //printf("token = %s\n", clang_getCString(spelling));
      tok.append(clang_getCString(spelling));
      tok.append(" ");
      clang_disposeString(spelling);
  }

  clang_disposeTokens(tu, tokens, nTokens); 
 
  return tok;
}

std::string parseUnit::getTypeSpell(CXType cxType) {
  CXString typeName  = clang_getTypeSpelling(cxType);
  std::string result = clang_getCString( typeName );
  std::string rr = "";
  for(char& c: result) {
    if((c == '<') || (c == '>')) rr.append("\\");
    rr += c;
  }

  clang_disposeString( typeName );
  return rr;
}

std::pair<int, int> parseUnit::getCurrentLocation(CXSourceLocation location) {
  CXFile file;
  std::string filename;
  unsigned int line, column, offset;
  clang_getSpellingLocation(location, &file, &line, &column, &offset);

  if(file) {
    CXString cxfilename = clang_File_tryGetRealPathName(file);
    std::string filename = clang_getCString(cxfilename);
    clang_disposeString(cxfilename);
    //printf("\n#===== CURRENT LOCATION file: %s line: %d column: %d offset: %d =====#\n", filename.c_str(), line, column, offset);
  }
  return std::pair<int, int>(line, column);
}

std::string parseUnit::getCurrentFilePath(CXSourceLocation location) {
  CXFile file;
  std::string filename;
  unsigned int line, column, offset;
  clang_getSpellingLocation(location, &file, &line, &column, &offset);

  //if(!file) printf(" file not found?? \n");

  if(file) {
    CXString cxfilename = clang_File_tryGetRealPathName(file);
    filename = clang_getCString(cxfilename);
    clang_disposeString(cxfilename);
    //printf("\n#===== CURRENT LOCATION file: %s line: %d column: %d offset: %d =====#\n", filename.c_str(), line, column, offset);
    //printf(" is from main file? => %d\n", (clang_Location_isFromMainFile( location )));
  }

  return filename;
}

// get the names of the functions that our TARGET function calls/uses
std::vector<scopedBlock> parseUnit::cutUnusedByTarget() {
  std::vector<scopedBlock> scopes;
  std::vector<visitorData> currGraph;
  scopedBlock sb;
  bool skipFunc = false;

  std::vector<std::string> deps;

  bool isTarget = false;
  for(auto el: graph) {
    if(el.kindName == "FunctionDecl") {
      //printf(" !-- WATCHING FUNC %s\n --! ", el.spellName.c_str());
      if(el.spellName == targetName) {
        isTarget = true;
        //printf("found target %s\n", el.spellName.c_str());
      }
      else {
        //printf(" now false %s\n", el.spellName.c_str());
        isTarget = false;
      }
    }
    if(el.kindName == "CallExpr") {
      //printf("call to %s| %d\n", el.spellName.c_str(), isTarget);
    }
    if(el.kindName == "CallExpr" && isTarget) {
      //printf("depend : %s\n", el.spellName.c_str());
      deps.push_back(el.spellName); 
    }
    if(isTarget) {
      currGraph.push_back(el);
    }
  }

  sb.name = targetName;
  sb.graph = currGraph;
  scopes.push_back(sb);

  //printf("pushed %s\n", currGraph[0].spellName.c_str());

  currGraph.clear();

  bool found = false;
  for(auto el: graph) {
    if(el.kindName == "FunctionDecl") {
      //printf("new scope %s | found %d\n", el.spellName.c_str(), found);
      if(currGraph.size() && found) 
      {
        sb.name = currGraph[0].spellName;
        sb.graph = currGraph; 
        //printf(" pushed %s \n", currGraph[0].spellName.c_str());
        scopes.push_back(sb);
        found = false;
        currGraph.clear();
      }
      if(!found) {
        currGraph.clear();
      }
      if(el.spellName == targetName) {
        found = true;
      }
      for(auto name: deps) {
        if(el.spellName == name)  {
          found = true;
        }
      }
      

    }
    currGraph.push_back(el);
  }  
  
  /*
  if(currGraph.size()) {
    sb.name = currGraph[0].spellName;
    sb.graph = currGraph; 
    scopes.push_back(sb);
    currGraph.clear();
  }
  */
  return scopes;
}

void parseUnit::checkScopedGraph() {
  std::vector<scopedBlock> cut = cutUnusedByTarget();
  for(scopedBlock sb: cut) {
    //printf(" scope %s | first el %s\n", sb.name.c_str(), sb.graph[0].spellName.c_str());
  }
}

CXChildVisitResult parseUnit::visitorHelper(CXCursor cursor, CXCursor parent, void* client_data) {
  return static_cast<parseUnit *>(client_data)->visitor(cursor, parent);
}

CXChildVisitResult parseUnit::visitor( CXCursor cursor, CXCursor /* parent */)
{
  CXSourceLocation location = clang_getCursorLocation( cursor );
  
  std::pair<int, int> loc = getCurrentLocation(location);
  std::string currFile = getCurrentFilePath(location);
  //if(clang_Location_isFromMainFile( location )) {
  //  printf(" encountered header dependend tokens\n");
  //}
  if( clang_Location_isFromMainFile( location ) == 0 && (strcmp(currFile.c_str(), this->path) != 0) && (clang_getCursorKind(cursor) != CXCursor_DeclStmt)) {
    //printf(" NOT IN MAIN FILE \n");
    //std::string currFile = getCurrentFilePath(location);
    //printf(" ! NOT FROM MAIN FILE => %s vs og %s | %d \n", currFile.c_str(), this->path, );
    /*
    CXCursorKind cursorKind = clang_getCursorKind( cursor );
    CXType cursorType = clang_getCursorType(cursor);
    
    std::string kindName = getCursorKindName(cursorKind);
    std::string spellName = getCursorSpelling(cursor);  
    //printf("  -- cursorType: %d\n", cursorType.kind); 
    if(kindName.size()) printf("  -- cursorKind: %d | kindName: %s\n", cursorKind, kindName.c_str());
    if(spellName.size()) printf("  -- spellName: %s\n", spellName.c_str());
    */
    return CXChildVisit_Continue;
  }

  //printf("%s\n",to_dot().c_str());

  CXCursorKind cursorKind = clang_getCursorKind( cursor );
  CXType cursorType = clang_getCursorType(cursor);
  
  std::string lit;
  std::string op;
  std::string kindName = getCursorKindName(cursorKind);
  std::string spellName = getCursorSpelling(cursor); 
  std::string errorSpell = getErrorSpelling(loc, errors);
   
  bool foundFunc = false;
  bool isTargetFunc = spellName == this->targetName ? true : false;

  unsigned int curLevel = this->curLevel;
  unsigned int nextLevel = curLevel + 1;
  int parent_id = -1;
  bool skipChild = false;
  visitorData* vd = new visitorData();
  vd->dataParent = -1;
  vd->kindName = kindName;
  vd->spellName = spellName;
  if(curLevel) parent_id = find_parent(graph, curLevel);
  //printf("after parent_id\n");
  vd->treeLevel = curLevel;
  vd->id = id;
  vd->parent_id = parent_id;
  vd -> scope = scope;
  
  switch(cursorKind) {
    case CXCursor_ParenExpr:  {
      return CXChildVisit_Continue;
    } break;
    case CXCursor_UnexposedExpr: {
      if(currentScopeChildren.size() && (currentScopeChildren[currentScopeChildren.size() - 1].kindName == "ReturnStmt")) {
        currentScopeChildren[currentScopeChildren.size() - 1].spellName += spellName;
        graph[graph.size() - 1].spellName += spellName;
        //legacyGraph[legacyGraph.size() - 1].spellName += spellName;

      }
      return CXChildVisit_Continue;
    } break;
    case CXCursor_MacroExpansion: {
      macroDef md;
      md.loc = loc;
      md.id = id;
      md.isFunction = clang_Cursor_isMacroFunctionLike(cursor);
      int dparent_id = find_data_parent(legacyGraph, vd);
      vd->dataParent = dparent_id;
      this->macrosLoc.push_back(md);
    } break;
    case CXCursor_MacroDefinition: {
      std::string macroText = getMacrosText(cursor);
      vd->typeName = macroText;
    } break;
    case CXCursor_ParmDecl: case CXCursor_VarDecl: {
      std::string typeName = getTypeSpell(cursorType);
      vd->typeName = typeName;
      /*
      if(currentScopeChildren.size() && (!(currentScopeChildren[currentScopeChildren.size() - 1].spellName.size()))) {
        //printf("!-- MERGER RETURN %s %s\n", currentScopeChildren[currentScopeChildren.size() - 1].kindName.c_str(), vd->spellName.c_str());
        currentScopeChildren[currentScopeChildren.size() - 1].spellName = vd->spellName;
        currentScopeChildren[currentScopeChildren.size() - 1].typeName = vd->typeName;

        skipChild = true;
      }
      */
    } break;
    case CXCursor_IntegerLiteral: case CXCursor_FloatingLiteral: {
      if(currentScopeChildren.size() && (currentScopeChildren[currentScopeChildren.size() - 1].kindName == "VarDecl")) {
        lit = getIntegerLiteral(cursor);
        if(lit.size()) vd->spellName = lit;
        std::string opb = " = ";
        opb.append(lit);
        //printf("INTDECL %s %s\n", opb.c_str(), lit.c_str());
        currentScopeChildren[currentScopeChildren.size() - 1].spellName += opb;
        graph[graph.size() - 1].spellName += opb;
        //legacyGraph[legacyGraph.size() - 1].spellName += opb;
      }
      return CXChildVisit_Continue;
    } break;
    case CXCursor_BinaryOperator: case CXCursor_CompoundAssignOperator: {
      op = getBinaryOperator(cursor);
      if(op.size()) vd->spellName = op;
    } break;
    case CXCursor_UnaryOperator: case CXCursor_UnaryExpr: {
      op = getUnaryOperator(cursor);
      if(op.size()) vd->spellName = op;
    } break;
    case CXCursor_FunctionDecl: {
      std::string typeName = getTypeSpell(cursorType); 
      vd->typeName = typeName;
      if(spellName == targetName) {
        foundFunc = true;
      }
      scope++;
    } break;
    case CXCursor_StringLiteral: {
      spellName.erase(std::remove(spellName.begin(), spellName.end(), '\"'), spellName.end()); 
      vd->spellName = spellName;
    } break;
    case CXCursor_DeclRefExpr: {
      return CXChildVisit_Continue;
    } break;
    /*
    case CXCursor_DeclRefExpr: {
      //printf(";;;;;;;;;;;;;;;;;;;;;DeclRefExpr; %s %s\n", kindName.c_str(), vd->spellName.c_str());
      std::string typeName = getTypeSpell(cursorType);
      vd->typeName = typeName;
      int dparent_id = find_data_parent(legacyGraph, vd);
      vd->dataParent = dparent_id;
      if(currentScopeChildren.size()) {
        printf("!-- MERGER RETURN %s %s\n", currentScopeChildren[currentScopeChildren.size() - 1].kindName.c_str(), vd->spellName.c_str());
        vd->parent_id = parent_id; 
        currentScopeChildren[currentScopeChildren.size() - 1] = *vd;
        printf("last child %s dparent %d\n", currentScopeChildren[currentScopeChildren.size() - 1].kindName.c_str(), currentScopeChildren[currentScopeChildren.size() - 1].dataParent);
        //currentScopeChildren[currentScopeChildren.size() - 1].spellName = vd->spellName;
        //currentScopeChildren[currentScopeChildren.size() - 1].typeName = vd->typeName;
        //currentScopeChildren[currentScopeChildren.size() - 1].dataParent = dparent_id;
        skipChild = true;
      }
    } break;
    */
    case CXCursor_TypedefDecl: {
      std::string realType = getTypeDef(cursor);
      vd->typeName = realType;
    } break;
    case CXCursor_TypeRef: {
      int dparent_id = find_data_parent(legacyGraph, vd);
      //printf("  TYPEREF: : %s %d\n", realType.c_str(), dparent_id);
      vd->dataParent = dparent_id;
    } break;
    case CXCursor_CallExpr: {
      //printf("!-- COMPARE %s to %s\n", spellName.c_str(), targetName);
      if(spellName == targetName) {
        foundFunc = true;
      }  
    } break;
    /*
    case CXCursor_UnexposedExpr: {
      if(currentScopeChildren.size() && !currentScopeChildren[currentScopeChildren.size() - 1].spellName.size()) {
        //currentScopeChildren[currentScopeChildren.size() - 1].kindName = "MERGED";
        currentScopeChildren[currentScopeChildren.size() - 1].spellName = spellName;
        legacyGraph[legacyGraph.size() - 1].spellName = spellName;
        graph[graph.size() - 1].spellName = spellName;
        

        printf("!-- MERGER RETURN %s %s\n", currentScopeChildren[currentScopeChildren.size() - 1].kindName.c_str(), vd->spellName.c_str());
        //skipChild = true;
        return CXChildVisit_Continue;

      }
    } break;
    */
  } 
  
  if(graph.size()) vd->controlParent = findControlParent(graph, vd); 
   
  // maybe its in macro
  // @TODO UGLYYYYY EWWW
  /*
  if(vd->dataParent == -1) {
    for(macroDef el: macrosLoc) {
      if(el.loc == loc) {
        if((el.isFunction && (cursorKind == CXCursor_BinaryOperator || cursorKind == CXCursor_UnaryOperator))
            ||
            (!el.isFunction) && (cursorKind != CXCursor_BinaryOperator && cursorKind != CXCursor_UnaryOperator))
        {
          vd->dataParent = el.id;
        }
      }
      if(el.loc.first == loc.first) {
        if(el.isFunction && (cursorKind == CXCursor_BinaryOperator || cursorKind == CXCursor_UnaryOperator))
        {
          vd->dataParent = el.id;
        }
      }
    }
  }
*/
  if(errorSpell.size()) {
    //printf("THIS IS ERROR %s \n", vd->kindName.c_str());
    vd->isError = true;
    vd->errorSpelling = errorSpell;
  }
  
  if(foundFunc) {
    this->legacyScope = scope;
  }
  /*
  std::cout << std::string( curLevel, '-' ) << " | "<< std::to_string(scope) <<  " | parent = "  << std::to_string(vd->parent_id) << " | data_parent = " << vd->dataParent << " | id = " << vd->id << " "<< legacyScope << " " << getCursorKindName(
  cursorKind ) << " (" << getCursorSpelling( cursor ) << ")\n";
 */
  id++;
  this->curLevel++;

  if(scope == legacyScope || isTargetFunc) {
    for(auto el: currentScopeChildren) {
      legacyGraph.push_back(el);
    }
  }

  if(!skipChild) {
    currentScopeChildren.push_back(*vd);
    graph.push_back(*vd);
  }

  clang_visitChildren( cursor,
                       parseUnit::visitorHelper,
                       this ); 

  this->curLevel--;

  return CXChildVisit_Continue;
}

std::vector<errorLoc> parseUnit::diagnoseForErrors() {
  CXDiagnosticSet err_set = clang_getDiagnosticSetFromTU(tu);
  std::vector<errorLoc> errors;
  int err_num = clang_getNumDiagnosticsInSet(err_set);
  //printf("error number %d\n", err_num);
  for(int i = 0; i < err_num; i++) {
    CXDiagnostic cxd = clang_getDiagnosticInSet(err_set, i);
    CXSourceLocation dsloc = clang_getDiagnosticLocation(cxd);
    std::pair<int, int> dloc = getCurrentLocation(dsloc);

    CXString spell = clang_getDiagnosticSpelling(cxd);
    std::string spelling = clang_getCString( spell );
    //printf(" ERROR desc %s %d %d \n", spelling.c_str(), dloc.first, dloc.second); 
    clang_disposeString( spell );
    clang_disposeDiagnostic(cxd);

    errorLoc err(dloc, spelling);
    errors.push_back(err);
  }
  clang_disposeDiagnosticSet(err_set);

  return errors;
}

std::vector<visitorData> parseUnit::parse()
{
  CXIndex index        = clang_createIndex( 0, 1 );
  visitorData* vData = (visitorData*)malloc(sizeof(visitorData));
  //CXTranslationUnit tu = clang_createTranslationUnit( index, "hello.cpp" );
 
  char* arg1 = "-x";
  char* arg2 = "c++";
  char* arg3 = "-std=c++17";
  char* arg4 = "-E";
  char* arg5 = "-I/usr/lib/gcc/x86_64-linux-gnu/12/include";
  const char** cl_args = (const char**)malloc(5 * sizeof(char*));
  cl_args[0] = arg1;
  cl_args[1] = arg2;
  cl_args[2] = arg3;
  cl_args[3] = arg4;
  cl_args[4] = arg5;

  tu = clang_parseTranslationUnit(
    index,
    filename, cl_args, 5,
    nullptr, 0,
    CXTranslationUnit_None | CXTranslationUnit_DetailedPreprocessingRecord); //Parse "file.cpp" 

  if( !tu ) {
    return std::vector<visitorData>();
  }

  errors = diagnoseForErrors();

  CXCursor rootCursor  = clang_getTranslationUnitCursor( tu );

  unsigned int treeLevel = 0;
  vData->treeLevel = 0;
  vData->id = 0;

  clang_visitChildren( rootCursor, parseUnit::visitorHelper, this );

  clang_disposeTranslationUnit( tu );
  clang_disposeIndex( index );

  std::string filename = "asg_graph.dot";

  //checkScopedGraph();

  if(targetName != "") {
    std::string dotgraph = to_sub_dot(); 
    printf("%s\n", dotgraph.c_str());
  }
  else {
    printf("%s\n", to_dot(graph, this->filename, false).c_str()); 
  } 

  /*
  std::ofstream outFile(filename);
  if (outFile.is_open()) {
    outFile << to_dot().c_str();
    outFile.close();
  } else {
    std::cerr << "Unable to open file " << filename << "\n";
  }
*/
  return graph;
}
