#ifndef ENTITY_H
#define ENTITY_H

#include "graph.h"

#include <list>
#include <string>
#include <unordered_map>

#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>


using namespace hdt;

const string rdf_type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
const string rdfs_subclassof = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
const string rdfs_label = "http://www.w3.org/2000/01/rdf-schema#label";


class EntityAnn {

    public:
        EntityAnn(HDT*, string);
        EntityAnn(string, string);
        EntityAnn(string, string, double alpha);
        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx);
        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx, double alpha);
        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx, bool, bool);
        //        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx, double alpha, bool);
        std::list<string>* annotate_semi_scored_column(unsigned long m, double alpha);
        std::list<string>* annotate_semi_scored_column(unsigned long m);
        std::list<string>* get_entities_of_value(string value);
        std::list<string>* get_entities_of_value(string value, std::list<string>*, bool);
        std::list<string>* get_leaf_classes(string entity_uri);
        bool is_ancestor_of(string a, string b);
        TNode* get_tnode(string uri);
        bool compute_intermediate_coverage(string cell_value);
        bool compute_intermediate_coverage(string cell_value, std::list<string>*, bool);
        void compute_Ic_for_all();
        void compute_Ic_for_node(TNode* tnode);
        //        void propagate_Is_all();
        //        void propagate_Is_tnode(TNode *);
        double compute_Lc_for_node(TNode*);
        void compute_Lc_for_all();
        void compute_Is_for_all();
        void compute_Is_for_node(TNode* tnode);
        void compute_Ls_for_all();
        double compute_Ls_for_node(TNode* tnode);
        Graph* get_graph();
        TNode* update_graph(string class_uri);
        void update_graph(std::list<string>* class_uris);
        void compute_classes_entities_counts();
        unsigned long propagate_counts(TNode* tnode);
        void compute_fs();
        void compute_fc(unsigned long m);
        void compute_f();
        std::list<string>* get_candidates();
        void pick_root();
        void set_alpha(double);
        double get_alpha();
        string get_quoted(string);
//        EasyLogger* m_logger;
    private:
        EasyLogger* m_logger;
        HDT* hdt;
        Graph* m_graph;
        std::unordered_map<string, std::unordered_map<string, bool>*> m_ancestor_lookup;
        std::unordered_map<string, bool>* add_class_to_ancestor_lookup(string tclass);
        std::unordered_map<string, unsigned long> m_classes_entities_count;
        //        bool m_propagate_Is=true; // should the parents also include the Is of their childred (true=yes)
        double m_alpha;
        double m_ambiguitity_penalty=2;
        void init(string hdt_file_dir, string log_file_dir, double alpha);
        void init(HDT* hdt_ptr, string log_file_dir, double alpha);
};
#endif

