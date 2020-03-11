#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <string>
#include <unordered_map>

#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>


using namespace hdt;

const string rdf_type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
const string rdfs_subclassof = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
const string rdfs_label = "https://www.w3.org/2000/01/rdf-schema#label";

//// This is used to store and permits a quick
//typedef struct AnsNode{
//    string uri;
//    struct AnsNode* next;
//} AnsNode;

class EntityAnn {

    public:
        EntityAnn(string, string);
        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx);
        std::list<string>* get_entities_of_value(string value);
        std::list<string>* get_leaf_classes(string entity_uri);

    private:
        EasyLogger* m_logger;
        HDT* hdt;
        std::unordered_map<string,std::unordered_map<string,bool>*> m_ancestor_lookup;
        std::unordered_map<string,bool>* add_class_to_ancestor_lookup(string tclass);
        void compute_intermediate_coverage(string cell_value);

};
#endif

