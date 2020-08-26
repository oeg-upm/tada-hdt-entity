#include <tabular_parser/parser.h>
#include<iostream>
#include "entity.h"
#include <HDTManager.hpp>

using namespace hdt;
int main(){
//    std::list<string>* candidates;
//    string hdt_dir = "/home/ahmad/datasets/wikidata20200309.hdt";
//    EntityAnn *ea = new EntityAnn(hdt_dir.c_str(), "sample.log", 0.1);
//    ea->set_language_tag("@en");
//    ea->subclassof_uri = "http://www.wikidata.org/prop/direct/P279";
//    ea->type_uri = "http://www.wikidata.org/prop/direct/P31";
//    Parser p("/home/ahmad/tada-hdt-semtab-2020/data/tables/0DGO26EF.csv");
////    candidates = ea.annotate_column(p.parse_vertical(), 2, true, true);
//    candidates = ea->annotate_entity_property_column(p.parse_vertical(),0,2);
//    cout << "Candidats: ";
//    for(auto it=candidates->cbegin();it!=candidates->cend();it++){
//        cout <<*it<<endl;
//    }
//    delete ea;

      cout << "Hello!" <<endl;
//      EntityAnn ea("/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt","entity.log");
//      std::list<std::list<string>*>* data;
//          Parser p("test.csv");
//          data = p.parse();
//          for(auto it=data->cbegin();it!=data->cend();it++){
//              for(auto jt=(*it)->cbegin();jt!=(*it)->cend();jt++){
//                  cout << (*jt) << " ";
//              }
//              cout << endl<<"----------"<<endl;
//          }
//      string hdt_file_dir = "test.hdt";
//      HDT* hdt = HDTManager::mapIndexedHDT(hdt_file_dir.c_str());
//      IteratorTripleString* itt;
//      TripleString* triple;
//      itt = hdt->search("","","\"boxer1\"");
//      cout<< "\n==================\nprinting list\n";
//      while(itt->hasNext()) {
//          triple = itt->next();
//          cout << triple->getSubject() << endl;
//      }
//      delete itt;

      return 0;
}

