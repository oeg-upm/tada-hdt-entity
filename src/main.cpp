#include <tabular_parser/parser.h>
#include <getoptpp/getopt_pp.h>
#include<iostream>
#include "entity.h"
#include <HDTManager.hpp>

using namespace GetOpt;
using namespace hdt;


void print_options() {
  cout << "Options: \n";
  cout << "\t-h --hdt <path to the hdt file>\n";
  cout << "\t-f --file <the path to the data CSV file>\n";
  cout << "\t-c --subject-col <e.g., the id of the subject column>\n";
  cout << "\t-g --lang-tag <e.g., en for english>\n";
  cout << "\t-t --title <yes or no. If yes, it would use a title case if the original case was not matched\n";
  cout << "\t-s --sample <the sample size (# cells in the subject column). 0 means use all the data, and not just a sample>\n";
  cout << "\t-l --log <the path to the logfile>\n";
  cout << "\t--label-uris <the uris for the labelling properties>\n";
}

std::vector<string> *get_list_args_long(GetOpt_pp *ops_ptr, string arg_label) {
  std::vector<string> *vec = new std::vector<string>();

  for (GetOpt_pp::long_iterator it = ops_ptr->begin(); it != ops_ptr->end(); ++it) {
    if (*it != arg_label) {
      continue;
    }

    it >> (*vec);
  }

  return vec;
}

int main(int argc, char **argv) {
  GetOpt_pp ops(argc, argv);

  print_options();

  std::vector<string> *label_uris = nullptr;
  string file_dir, hdt_path, lang_tag, col_id_str, title_case, log_path, sample_size_str;
  ops >> Option('h', "hdt", hdt_path, "");
  ops >> Option('f', "file", file_dir, "");
  ops >> Option('c', "subject-col", col_id_str, "0");
  ops >> Option('g', "lang-tag", lang_tag, "");
  ops >> Option('t', "title", title_case, "no");
  ops >> Option('s', "sample", sample_size_str, "0");
  ops >> Option('l', "log", log_path, "entity.log");
  label_uris = get_list_args_long(&ops, "label-uris");


  std::list<string> *candidates;
  int col_idx, sample_size;

  col_idx = stoi(col_id_str);
  sample_size = stoi(sample_size_str);

  EntityAnn *ea = new EntityAnn(hdt_path, log_path);
  ea->set_title_case(title_case == "yes");

  if (lang_tag != "") {
    ea->set_language_tag("@" + lang_tag);
  }

  Parser p(file_dir);

  if (label_uris != nullptr && label_uris->size() > 0) {
    ea->clear_label_uri();

    for (auto it = label_uris->cbegin(); it != label_uris->cend(); it++) {
      ea->append_label_uri(*it);
    }
  }

  candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);

  cout << "Number of candidates: " << candidates->size() << "\n";

  for (auto it = candidates->cbegin(); it != candidates->cend(); it++) {
    cout << "Candidate: " << *it << endl;
  }

//  ea->set_language_tag("@en");
  //    ea->set_title_case(true);
  //    ea->set_language_tag("@en");

  //    Parser p(file_dir1);
  //    cout << "file_dir: "<<file_dir1<<endl;
  //    cout << "col_idx: "<<col_idx<<endl;
  ////        cout << "pre parse:\n";
  ////        p.parse_vertical();
  ////        cout <<" parse vertical\n";
  //    candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);

  //    Parser p2(file_dir2);
  //    cout << "file_dir2: "<<file_dir2<<endl;
  //    cout << "col_idx: "<<col_idx<<endl;
  ////        cout << "pre parse:\n";
  ////        p.parse_vertical();
  ////        cout <<" parse vertical\n";
  //    candidates = ea->annotate_column(p2.parse_vertical(), col_idx, true, true);

  return 0;
}


//int main() {

//  std::list<string> *candidates;
//  cout << "Hello!" << endl;
////    string file_dir1="/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/5873256_0_7795190905731964989.csv";
////    string file_dir2 = "/Users/aalobaid/workspaces/Pyworkspace/tada-gam/local_data/t2dv2/86747932_0_7532457067740920052.csv";
//  int col_idx = 0;
//  string m_hdt = "/Users/aalobaid/workspaces/Datasets/Bigg/biggv1.hdt";
//  string fdir = "/Users/aalobaid/Downloads/a.csv";
//  EntityAnn *ea = new EntityAnn(m_hdt, "entity.log");
//  ea->set_title_case(true);
//  Parser p(fdir);
//  ea->clear_label_uri();
//  ea->append_label_uri("http://www.w3.org/2004/02/skos/core#prefLabel");
//  candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);

//  for (auto it = candidates->cbegin(); it != candidates->cend(); it++) {
//    cout << "Candidate: " << *it << endl;
//  }

//  //ea->set_language_tag("@en");
////    ea->set_title_case(true);
////    ea->set_language_tag("@en");

////    Parser p(file_dir1);
////    cout << "file_dir: "<<file_dir1<<endl;
////    cout << "col_idx: "<<col_idx<<endl;
//////        cout << "pre parse:\n";
//////        p.parse_vertical();
//////        cout <<" parse vertical\n";
////    candidates = ea->annotate_column(p.parse_vertical(), col_idx, true, true);

////    Parser p2(file_dir2);
////    cout << "file_dir2: "<<file_dir2<<endl;
////    cout << "col_idx: "<<col_idx<<endl;
//////        cout << "pre parse:\n";
//////        p.parse_vertical();
//////        cout <<" parse vertical\n";
////    candidates = ea->annotate_column(p2.parse_vertical(), col_idx, true, true);


//  return 0;
//}

