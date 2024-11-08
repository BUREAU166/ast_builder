#include "../include/visitorData.h"

bool visitorData::operator==(const visitorData& rhs) const
{
  if(id == rhs.id) return true;
  return false;
}

bool operator==(const visitorData& lhs, const visitorData& rhs)
{
  if(lhs.id == rhs.id) return true;
  return false;
}

int findTargetParent(std::vector<std::vector<visitorData>> projGraphs, visitorData *vd) {
  int elementIndex = -1;
  int parentIndex = -1;
  //printf("\n\n size %d | first id %d third id %d vd id %d \n", graph.size(), graph[0].id, graph[3].id, vd->id);
  
  for(auto graph: projGraphs) {
    for(auto el: graph) {
      if(el.spellName == vd->spellName) {
        parentIndex = el.id;
        break;
      }
    }
  }

  // //printf("elementIndex = %d for %s\n", elementIndex, vd->spellName.c_str());
  // for(int i = vd->id - 1; i >= 0; i--) {
  //   if(!graph[i].spellName.size() || graph[i].kindName == "FunctionDecl" || graph[i].kindName == "ClassDecl" || graph[i].kindName == "StructDecl") {
  //     //printf("found base %d %s\n", i, graph[i].spellName.c_str());
  //     parentIndex = graph[i].id;
  //     break;
  //   }
  // }

  return parentIndex;
}

int findControlParent(std::vector<visitorData> graph, visitorData *vd) {
  int elementIndex = -1;
  int parentIndex = -1;
  //printf("\n\n size %d | first id %d third id %d vd id %d \n", graph.size(), graph[0].id, graph[3].id, vd->id);
  
  //printf("elementIndex = %d for %s\n", elementIndex, vd->spellName.c_str());
  for(int i = vd->id - 1; i >= 0; i--) {
    if(!graph[i].spellName.size() || graph[i].kindName == "FunctionDecl" || graph[i].kindName == "ClassDecl" || graph[i].kindName == "StructDecl") {
      //printf("found base %d %s\n", i, graph[i].spellName.c_str());
      parentIndex = graph[i].id;
      break;
    }
  }

  return parentIndex;
}

int find_data_parent(std::vector<visitorData> graph, visitorData* vd) {
  int dparent_id = -1;
  //printf("  ## searchinf parent for %s\n", vd.spellName.c_str());
  std::string sn = vd->spellName;
  std::string tn = vd->typeName;
  for(visitorData el: graph) {
    // data parent is function parameter or var decl
    // @TODO check if not a function
    //printf(" ------> %s %s ???? %s %s", sn.c_str(), tn.c_str(), el.spellName.c_str(), el.typeName.c_str());
    if((el.kindName == "ParmDecl" || el.kindName == "VarDecl") 
        && (el.scope == vd->scope || el.scope == 0) 
        && (el.spellName == sn)
        && (el.typeName == tn)) 
    {
      dparent_id = el.id;
      std::string fn = vd->spellName;
      //printf("\n#=== parent for %s is %d ===#\n", fn.c_str(), dparent_id);
    }
    
    // data parent is function decl
    if(el.kindName == "FunctionDecl" && (el.scope < vd->scope) && (el.spellName == sn)) {
      dparent_id = el.id;
      std::string fn = vd->spellName;
      //printf("\n#F=== parent FUNC for %s is %d ===#\n", fn.c_str(), dparent_id);
    }

    if(el.kindName == "TypedefDecl" && (el.spellName == sn)) {
      dparent_id = el.id;
      std::string fn = vd->spellName;
    }

    if(el.kindName == "FieldDecl" && (el.spellName == sn)) {
      dparent_id = el.id;
      std::string fn = vd->spellName;
    }

    size_t pos = sn.find(" ");
    //printf(" pos in %s is %d\n", sn.c_str(), pos);
    if(pos != std::string::npos) {
      std::string structSpellName = sn.substr(pos, std::string::npos);
      structSpellName.erase(structSpellName.begin());
      //printf("  struct spellname: %s\n", structSpellName.c_str());
      if((el.kindName == "StructDecl" || el.kindName == "UnionDecl") && (structSpellName == el.spellName)) {
        dparent_id = el.id;
        std::string fn = vd->spellName;
      }
    }
    
    //printf(" maybe macro? %s %s %s\n", el.kindName.c_str(), el.spellName.c_str(), sn.c_str());
    if((el.kindName == "macro definition") && (el.spellName == sn)) {
      dparent_id = el.id;
    }
  }

  return dparent_id;
}

int find_parent(std::vector<visitorData> graph, int level) {
  if(!level) {
    return 0;
  }

  int parent_id = 0;
  //printf("find parent entr %d \n", graph.size());
  for(visitorData el: graph) {

    //printf(" ? el %s | %d == %d ?", el.spellName.c_str(), el.treeLevel, level - 1);
    if(el.treeLevel == (level - 1)) {
      parent_id = el.id;
    }
  }
    
  return parent_id;
}
