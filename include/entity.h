#ifndef ENTITY_H
#define ENTITY_H

#include "graph.h"

#include <list>
#include <string>
#include <unordered_map>

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>


using namespace std;
//using namespace hdt;

//const string rdf_type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
//const string rdfs_subclassof = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
//const string rdfs_label = "http://www.w3.org/2000/01/rdf-schema#label";


class EntityAnn {

 public:
  EntityAnn();
  EntityAnn(string, string);
  EntityAnn(hdt::HDT *, string);
  EntityAnn(hdt::HDT *, string, double);
  EntityAnn(string, string, double alpha);
  ~EntityAnn();      // destructor
  void setHDT(string);
  hdt::HDT *getHDT();
  void setLogger(string);
  /**
   * Annotate the entity column.
   *
   *
   * To annotate the entity column (using the given idx).
   *
   *
   * @param data the tabular data read using the parser.
   * @param idx the index of the entity column to be parsed
   *
   */
  std::list<string> *annotate_column(std::list<std::list<string>*> *data, unsigned idx);
  /**
   * Annotate the entity column.
   *
   *
   * To annotate the entity column (using the given idx).
   *
   *
   * @param data the tabular data read using the parser.
   * @param idx the index of the entity column to be parsed
   * @param alpha the alpha value which balances coverage and specificity
   *
   */
  std::list<string> *annotate_column(std::list<std::list<string>*> *data, unsigned idx, double alpha);
  /**
   * Annotate the entity column.
   *
   *
   * To annotate the entity column (using the given idx).
   *
   *
   * @param data the tabular data read using the parser.
   * @param idx the index of the entity column to be parsed.
   * @param user_context whether to use the context (the other properties) for the entity linking phase.
   * For example Messi and Barcelona. It will look for the entity that has the label Messi and the value Barcelona.
   * @param double_levels is user_context is true, it will also include the case: entity --(relation)--> entity.
   * Using the same example, it will look for the entity with the label Messi that has a relation with another entity
   * that has the label Barcelona.
   */
  std::list<string> *annotate_column(std::list<std::list<string>*> *data, unsigned idx, bool use_context,
                                     bool double_levels);
  //        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx, double alpha, bool);
  std::list<string> *annotate_semi_scored_column(unsigned long m, double alpha);
  std::list<string> *annotate_semi_scored_column(unsigned long m);
  std::list<string> *get_entities_of_value(string value);
  std::list<string> *get_entities_of_value(string value, std::list<string> *, bool);
  std::list<string> *get_leaf_classes(string entity_uri);
  bool is_ancestor_of(string a, string b);
  TNode *get_tnode(string uri);
  bool compute_intermediate_coverage(string cell_value);
  bool compute_intermediate_coverage(string cell_value, std::list<string> *, bool);
  void compute_Ic_for_all();
  void compute_Ic_for_node(TNode *tnode);
  //        void propagate_Is_all();
  //        void propagate_Is_tnode(TNode *);
  //        double compute_Lc_for_node(TNode*);
  std::unordered_map<string, bool> *compute_Lc_for_node(TNode *);
  void compute_Lc_for_all();
  void compute_Is_for_all();
  void compute_Is_for_node(TNode *tnode);
  void compute_Ls_for_all();
  double compute_Ls_for_node(TNode *tnode);
  Graph *get_graph();
  TNode *update_graph(string class_uri);
  void update_graph(std::list<string> *class_uris);
  void compute_classes_entities_counts();
  void propagate_counts(TNode *tnode);
  void compute_fs();
  void compute_fc(unsigned long m);
  void compute_f();
  std::list<string> *get_candidates();
  void pick_root();
  void set_alpha(double);
  double get_alpha();
  string strip_quotes(string);
  string get_quoted(string);
  string get_taged(string);
  std::list<string> *recompute_f(double);
  string get_title_case(string);
  void set_title_case(bool);
  bool get_title_case();
  void set_language_tag(string tag);
  unsigned long get_m();
  /**
   * Annotate a property column.
   *
   * To annotate the property column.
   *
   *
   * @param data the tabular data read using the parser.
   * @param subject_idx the index of the subject column.
   * @param property_idx the index of the property column to be indexed.
   *
   */
  std::list<string> *annotate_entity_property_column(std::list<std::list<string>*> *data, long subject_idx,
      long property_idx);
  bool annotate_entity_property_pair(string, string);
  bool annotate_text_property_pair(string, string);
  /**
   * Annotate a property column of entities using an extensive bruteforce.
   *
   * It attempts to annotate the property column using all the subjects from the provided class with the entities from
   * the property column.
   *
   *
   * @param data the tabular data read using the parser.
   * @param class_uri the class of the subject column.
   * @param property_idx the index of the property column to be indexed.
   *
   */
  std::list<string> *annotate_entity_property_heuristic(std::list<std::list<string>*> *data, string class_uri,
      long property_idx);
  /**
   * Annotate a textual property column (not an entity column) using an extensive bruteforce.
   *
   * It attempts to annotate the property column using all the subjects from the provided class with the texts from
   * the property column.
   *
   *
   * @param data the tabular data read using the parser.
   * @param class_uri the class of the subject column.
   * @param property_idx the index of the property column to be indexed.
   *
   */
  std::list<string> *annotate_text_property_heuristic(std::list<std::list<string>*> *data, string class_uri,
      long property_idx);
  /**
   * Extract relations between subjects and entities using bruteforce.
   *
   * Given a list of strings, subjects and entities, it will search for all possible relations
   * using all combinations.
   *
   *
   * @param subjects a list of subject uris.
   * @param entities list of entity uris.
   *
   */
  void search_and_append_relations_with_entities(std::list<string> *subjects, std::list<string> *entities);
  /**
   * Extract relations between subjects and objects using bruteforce.
   *
   * Given a list of strings, subjects and objects, it will search for all possible relations
   * using all combinations.
   *
   *
   * @param subjects a list of subject uris.
   * @param objects list of objects uris.
   *
   */
  void search_and_append_relations_with_objects(std::list<string> *subjects, std::list<string> *entities);
  bool add_property_count(string property_uri);
  std::list<string> *get_entities_of_class(string);
  std::list<string> *get_properties_from_map();
  unsigned long get_counts_of_class(string);
  //        const string rdf_type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
  //        const string rdfs_subclassof = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
  //        const string rdfs_label = "http://www.w3.org/2000/01/rdf-schema#label";
  string type_uri = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
  string subclassof_uri = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
//  string label_uri = "http://www.w3.org/2000/01/rdf-schema#label";
  bool delete_hdt_in_destructor = false;
  //double m_ambiguitity_penalty=2;
  double m_ambiguitity_penalty = 1; // no penalty

  void set_sample_size(long sample_size);
  long get_sample_size();

  std::list<string> get_labels_uris();
  bool append_label_uri(string);
  bool clear_label_uri();
  // source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exists-using-standard-c-c11-14-17-c
  static inline bool file_exists(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
  }

 private:
  EasyLogger *m_logger;
  hdt::HDT *m_hdt;
  //HDT:: HDT::HDT* hdt;
  Graph *m_graph;
  std::unordered_map<string, std::unordered_map<string, bool>*> m_ancestor_lookup;
  std::unordered_map<string, std::unordered_map<string, bool>*>
  m_descendents_lookup; // to store all childs and their descendents
  std::unordered_map<string, bool> *add_class_to_ancestor_lookup(string tclass);
  std::unordered_map<string, unsigned long> m_classes_entities_count;
  std::unordered_map<string, unsigned long> m_classes_propagated_count;
  std::unordered_map<string, unsigned long> *m_properties_counts;
  std::list<string> m_labels_uris;
  //        bool m_propagate_Is=true; // should the parents also include the Is of their childred (true=yes)
  double m_alpha;
  unsigned long m_m;
  bool m_retry_with_title_case = false;
  string m_lang_tag;
  long m_sample_size = 0;
  void init(string hdt_file_dir, string log_file_dir, double alpha);
  void init(hdt::HDT *hdt_ptr, string log_file_dir, double alpha);
};


//bool sort_pair_property(const pair<string,unsigned long> &a,
//               const pair<string,unsigned long> &b){
//       return a.second>b.second;
//}



#endif

