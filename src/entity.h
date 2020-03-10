#ifndef ENTITY_H
#define ENTITY_H

#include <list>
#include <string>

#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>


using namespace hdt;

const string rdf_type = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
const string rdfs_subclassof = "http://www.w3.org/2000/01/rdf-schema#subClassOf";
const string rdfs_label = "https://www.w3.org/2000/01/rdf-schema#label";

class EntityAnn {

    public:
        EntityAnn(string, string);
        std::list<string>* annotate_column(std::list<std::list<string>*>* data, unsigned idx);
        std::list<string>* get_entities_of_value(string value);
    private:
        EasyLogger* m_logger;
        HDT* hdt;
};
#endif

