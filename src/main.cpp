#include <tabular_parser/parser.h>
#include<iostream>
#include "entity.h"
using namespace std;

int main(){
      cout << "Hello!" <<endl;
      EntityAnn ea("/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt","entity.log");


      std::list<std::list<string>*>* data;
          Parser p("test.csv");
          data = p.parse();
          for(auto it=data->cbegin();it!=data->cend();it++){
              for(auto jt=(*it)->cbegin();jt!=(*it)->cend();jt++){
                  cout << (*jt) << " ";
              }
              cout << endl<<"----------"<<endl;
          }



return 0;
}

