#include <tabular_parser/parser.h>
#include<iostream>
#include "entity.h"
#include <HDTManager.hpp>

using namespace hdt;
int main(){
    cout << "Hello!" <<endl;
    string file_dir1="/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/5873256_0_7795190905731964989.csv";
    string file_dir2 = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/86747932_0_7532457067740920052.csv";
    unsigned int col_idx= 1;
    string m_hdt = "/Users/aalobaid/workspaces/Cworkspace/tada-hdt/dbpedia_all.hdt";
    EntityAnn* ea = new EntityAnn(m_hdt, "entity.log");
    //ea->set_language_tag("@en");
    ea->set_title_case(true);
    ea->set_language_tag("@en");
    std::list<string>* candidates;
    Parser p(file_dir1);
    cout << "file_dir: "<<file_dir1<<endl;
    cout << "col_idx: "<<col_idx<<endl;
//        cout << "pre parse:\n";
//        p.parse_vertical();
//        cout <<" parse vertical\n";
    candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);

    Parser p2(file_dir2);
    cout << "file_dir2: "<<file_dir2<<endl;
    cout << "col_idx: "<<col_idx<<endl;
//        cout << "pre parse:\n";
//        p.parse_vertical();
//        cout <<" parse vertical\n";
    candidates = ea->annotate_column(p2.parse_vertical(), col_idx, true, true);


    return 0;
}

