#include "entity.h"
#include <string>
#include <list>
#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>


EntityAnn::EntityAnn(string hdt_file_dir, string log_file_dir) {
    hdt = HDTManager::mapIndexedHDT(hdt_file_dir.c_str());
    m_logger = new EasyLogger(log_file_dir);
}

std::list<string>* EntityAnn::annotate_column(std::list<std::list<string>*>* data, unsigned idx) {
    //string cell_value;
    for(auto it=data->cbegin(); it!=data->cend(); it++) {
        auto cell_ptr = (*it)->cbegin();
        std::advance(cell_ptr, idx);
        this->compute_intermediate_coverage(*cell_ptr);
        //cell_value = *cell_ptr;
        //this->get_entities_of_value(*cell_ptr);
    }
    hdt->search("", "https://www.w3.org/2000/01/rdf-schema#label", "");
    return NULL;
}

// Get entities of a given cell value or name using the rdfs:label property
std::list<string>* EntityAnn::get_entities_of_value(string value) {
    IteratorTripleString* itt;
    TripleString* triple;
    std::list<string>* entities = new std::list<string>;
    itt = hdt->search("", rdfs_label.c_str(), value.c_str());
    while(itt->hasNext()) {
        triple = itt->next();
        entities->push_back(triple->getSubject());
        delete triple;
    }
    delete itt;
    return entities;
}

/* It returns types of the given entities and would ignore all its parents.
It does not discard types of the entities if it not a leaf (has child nodes).
For example:
A
| \
B  C
|  | \
D  F  G

if the types of en entity e are [C,A], it would return only C.
*/
std::list<string>* EntityAnn::get_leaf_classes(string entity_uri) {
    IteratorTripleString* itt;
    TripleString* triple = nullptr;
    std::list<string>* leaves = new std::list<string>;
    std::unordered_map<string, bool>* candidates = new std::unordered_map<string, bool>;
    std::unordered_map<string, bool>* to_be_deleted = new std::unordered_map<string, bool>;
    string prev_class="", curr_class="";
    itt = hdt->search(entity_uri.c_str(), rdf_type.c_str(), "");
    m_logger->log("in function get leaf classes");
    m_logger->log("entity uri: <"+entity_uri+">");
    m_logger->log("rdf_type: <"+rdf_type+">");
    while(itt->hasNext()) {
        triple = itt->next();
        curr_class = triple->getObject();
        this->add_class_to_ancestor_lookup(curr_class);
        m_logger->log("curr class uri: <"+curr_class+">");
        candidates->insert({curr_class,true});
        for(auto it=candidates->cbegin();it!=candidates->cend();it++){
            if(this->is_ancestor_of(it->first,curr_class)){
                to_be_deleted->insert({it->first,true});
            }
            else if(this->is_ancestor_of(curr_class,it->first)){
                // if curr class is not in the to_be_deleted then add it
                if(to_be_deleted->find(curr_class)==to_be_deleted->cend()){
                    to_be_deleted->insert({curr_class,true});
                }
            }
        }
        for(auto it=to_be_deleted->cbegin();it!=to_be_deleted->cend();it++){
            m_logger->log("to erase: <"+it->first+">");
            candidates->erase(it->first);
        }
        to_be_deleted->clear();
    }// while loop

    for(auto it=candidates->cbegin();it!=candidates->cend();it++){
        leaves->push_back(it->first);
    }
    delete itt;
    return leaves;
}

bool EntityAnn::is_ancestor_of(string a, string b) {
    std::unordered_map<string, bool>* a_ancestors = m_ancestor_lookup.at(a);
    return a_ancestors->find(b)!=a_ancestors->cend();
}

// return the ancestors for the given tclass (whether it is already in the lookup or not)
std::unordered_map<string, bool>* EntityAnn::add_class_to_ancestor_lookup(string tclass) {
    IteratorTripleString* itt;
    TripleString* triple;
    string parent;
    std::unordered_map<string, bool>* pancestors; // ancestors of a parent
    std::unordered_map<string, bool>* ancestors;
    // the tclass is already added to the lookup table
    if(m_ancestor_lookup.find(tclass)!=m_ancestor_lookup.cend()) {
        return m_ancestor_lookup.at(tclass);
    }
    ancestors = new std::unordered_map<string, bool>;
    m_ancestor_lookup.insert({tclass, ancestors});
    itt = hdt->search("", rdfs_subclassof.c_str(), tclass.c_str());
    while(itt->hasNext()) {
        triple = itt->next();
        parent = triple->getSubject();
        ancestors->insert({parent, true});
        pancestors = this->add_class_to_ancestor_lookup(parent);
        // for each parent
        for(auto it = pancestors->cbegin(); it!=pancestors->cend(); it++) {
            // add parent ancestors
            ancestors->insert({it->first, true});
        }
//        delete triple;
    }
    delete itt;
    return ancestors;
}


void EntityAnn::compute_intermediate_coverage(string cell_value) {
    std::list<string>* classes;
    std::list<string>* entities = this->get_entities_of_value(cell_value);
    for(auto it=entities->cbegin(); it!=entities->cend(); it++) {
        classes = this->get_leaf_classes(*it);
    }
}

