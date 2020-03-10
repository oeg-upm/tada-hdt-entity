#include "entity.h"
#include <string>
#include <list>
#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>

EntityAnn::EntityAnn(string hdt_file_dir, string log_file_dir){
   hdt = HDTManager::mapIndexedHDT(hdt_file_dir.c_str());
   m_logger = new EasyLogger(log_file_dir);
}



std::list<string>* EntityAnn::annotate_column(std::list<std::list<string>*>* data, unsigned idx){
    //string cell_value;
    for(auto it=data->cbegin();it!=data->cend();it++){
        auto cell_ptr = (*it)->cbegin();
        std::advance(cell_ptr,idx);
        //cell_value = *cell_ptr;
        this->get_entities_of_value(*cell_ptr);
    }


    hdt->search("","https://www.w3.org/2000/01/rdf-schema#label","");
    return NULL;
}

// Get entities of a given cell value or name using the rdfs:label property
std::list<string>* EntityAnn::get_entities_of_value(string value){
    IteratorTripleString *itt;
    TripleString * triple;
    std::list<string>* entities = new std::list<string>;
    itt = hdt->search("",rdfs_label.c_str(),value.c_str());
    while(itt->hasNext()){
        triple = itt->next();
        entities->push_back(triple->getSubject());
        delete triple;
    }
    delete itt;
    return entities;
}

