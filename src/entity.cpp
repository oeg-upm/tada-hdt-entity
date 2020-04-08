#include "entity.h"
#include "tnode.h"
#include <string>
#include <list>
#include <ctype.h>
#include <easy_logger/easy_logger.h>
#include <HDTManager.hpp>


void EntityAnn::init(string hdt_file_dir, string log_file_dir, double alpha){
    hdt = HDTManager::mapIndexedHDT(hdt_file_dir.c_str());
    m_logger = new EasyLogger(log_file_dir);
    m_graph = new Graph(m_logger);
    m_alpha = alpha;
}

void EntityAnn::init(HDT* hdt_ptr, string log_file_dir, double alpha){
    hdt =hdt_ptr;
    m_logger = new EasyLogger(log_file_dir);
    m_graph = new Graph(m_logger);
    m_alpha = alpha;
}



EntityAnn::EntityAnn(HDT* hdt_ptr, string log_file_dir) {
    init(hdt_ptr,log_file_dir,1.0);
}


EntityAnn::EntityAnn(HDT* hdt_ptr, string log_file_dir, double alpha) {
    init(hdt_ptr,log_file_dir,alpha);
}

EntityAnn::EntityAnn(string hdt_file_dir, string log_file_dir) {
    init(hdt_file_dir,log_file_dir,1.0);
}


EntityAnn::EntityAnn(string hdt_file_dir, string log_file_dir, double alpha) {
    init(hdt_file_dir,log_file_dir,alpha);
}


std::list<string>* EntityAnn::annotate_column(std::list<std::list<string>*>* data, unsigned idx, double alpha) {
    m_alpha = alpha;
    return this->annotate_column(data, idx);
}

std::list<string>* EntityAnn::annotate_column(std::list<std::list<string>*>* data, unsigned idx, bool use_context, bool double_levels) {
    unsigned col_id  = 0;
    unsigned long m=0;
    string entity;
    std::list<string>* prop;
    if(use_context) {
        for(auto it=data->cbegin(); it!=data->cend(); it++) {
            if(it==data->cbegin()) { // to skip the header
                continue;
            }
            col_id = 0;
            entity = "-----#####";
            prop = new std::list<string>;
            for(auto jt=(*it)->cbegin(); jt!=(*it)->cend(); jt++, col_id++) {
                if(col_id != idx) {
                    prop->push_back(*jt);
                }
                else {
                    entity= *jt;
                }
            }
            entity = strip_quotes(entity);
            m_logger->log("annotate_column> compute intermediate coverage --- >");
            if(this->compute_intermediate_coverage(entity, prop, double_levels)) {
                m++;
            }
            delete prop;
        }
        m_m = m;
        m_logger->log("annotate_column> m: "+to_string(m));
        return this->annotate_semi_scored_column(m);
    }
    else {
        return this->annotate_column(data, idx);
    }
}



std::list<string>* EntityAnn::annotate_column(std::list<std::list<string>*>* data, unsigned idx) {
    unsigned long m=0;
    string l;
    std::list<std::list<string>*>::iterator ito = data->begin();
    std::advance(ito, idx);
    (*ito)->cbegin();
    for(auto it =(*ito)->cbegin(); it!=(*ito)->cend(); it++ ) {
        if(it==(*ito)->cbegin()) { // to skip the header
            continue;
        }
        m_logger->log("annotate_column> cell value: "+(*it));
        l = *it;
        if(this->compute_intermediate_coverage(l)) {
            m++;
        }
    }
    return this->annotate_semi_scored_column(m);
}


std::list<string>* EntityAnn::annotate_semi_scored_column(unsigned long m, double alpha) {
    this->compute_Ic_for_all();
    this->compute_Lc_for_all();
    this->pick_root();
    this->compute_classes_entities_counts();
    this->compute_Is_for_all();
    this->compute_Ls_for_all();
    this->compute_fc(m);
    this->compute_fs();
    this->compute_f();
    return this->get_candidates();
}


std::list<string>* EntityAnn::recompute_f(double alpha) {
    m_alpha = alpha;
    m_logger->log("recompute_f> with alpha: "+to_string(alpha));
    this->compute_f();
    return this->get_candidates();
}


std::list<string>* EntityAnn::annotate_semi_scored_column(unsigned long m) {
    return this->annotate_semi_scored_column(m, m_alpha);
}




// Get entities of a given cell value or name using the rdfs:label property
std::list<string>* EntityAnn::get_entities_of_value(string value) {
    string qvalue;
    IteratorTripleString* itt;
    TripleString* triple;
    string tcased;
    std::list<string>* entities = new std::list<string>;
    qvalue = get_quoted(value);
    qvalue = get_taged(qvalue);
    itt = hdt->search("", rdfs_label.c_str(), qvalue.c_str());
    m_logger->log("get_entities_of_value: cell value  <"+value+">");
    while(itt->hasNext()) {
        triple = itt->next();
        entities->push_back(triple->getSubject());
        m_logger->log("get_entities_of_value: "+triple->getSubject());
    }
    if(entities->size()==0){
        m_logger->log("no values for qvalue<"+qvalue+"> "+"  <"+value+">");
        if(m_retry_with_title_case){
            tcased = get_title_case(value);
            if(tcased!=value){
                m_logger->log("get_entities_of_value> tcased: "+tcased);
                return get_entities_of_value(tcased);
            }
            else{
                m_logger->log("get_entities_of_value> debug, tcased: <"+tcased+">");
            }
        }
    }
    delete itt;
    return entities;
}


// Get entities of a given cell value or name using the rdfs:label property
std::list<string>* EntityAnn::get_entities_of_value(string value, std::list<string>* properties, bool double_level) {
    string qprop,qvalue;
    IteratorTripleString* itt;
    TripleString* triple;
    std::list<string>* entities;// = this->get_entities_of_value(value);
    std::list<string>* strict_entities = new std::list<string>;
    std::list<string>* prop_entities;
    bool to_break;
    qvalue = get_quoted(value);
    qvalue = get_taged(qvalue);
    entities = this->get_entities_of_value(value);
    for(auto it = entities->cbegin(); it!=entities->cend(); it++) {
        for(auto it2=properties->cbegin(); it2!=properties->cend(); it2++) {
            qprop = get_quoted(*it2);
            itt = hdt->search((*it).c_str(), "", qprop.c_str());
            //            m_logger->log("get_entities_of_value: cell value  <"+value+">");
            if(itt->hasNext()) {
                strict_entities->push_back(*it);
                m_logger->log("get_entities_of_value: "+(*it)+" and property"+(*it2));
                delete itt;
                break;
            }
            delete itt;
        }
        if(double_level) {
            for(auto it2=properties->cbegin(); it2!=properties->cend(); it2++) {
                prop_entities = this->get_entities_of_value(*it2);
                to_break = false;
                for(auto it3=prop_entities->cbegin(); it3!=prop_entities->cend(); it3++) {
                    itt = hdt->search((*it).c_str(), "", (*it3).c_str());
                    if(itt->hasNext()) {
                        strict_entities->push_back(*it);
                        m_logger->log("get_entities_of_value> "+(*it)+" and property (double level) "+(*it3));
                        delete itt;
                        to_break = true;
                        break;
                    }
                    delete itt;
                }
                delete prop_entities;
                if(to_break) {
                    break;
                }
            }// for loop it2
        }// double
    }// for loop it
    delete entities;
    return strict_entities;
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
        candidates->insert({curr_class, true});
        for(auto it=candidates->cbegin(); it!=candidates->cend(); it++) {
            if(this->is_ancestor_of(it->first, curr_class)) {
                m_logger->log("case1: <"+it->first+"> is ancestor of <"+curr_class+">");
                to_be_deleted->insert({it->first, true});
            }
            else if(this->is_ancestor_of(curr_class, it->first)) {
                // if curr class is not in the to_be_deleted then add it
                if(to_be_deleted->find(curr_class)==to_be_deleted->cend()) {
                    m_logger->log("case2: <"+curr_class+"> is ancestor of <"+it->first+">");
                    to_be_deleted->insert({curr_class, true});
                }
            }
        }
        for(auto it=to_be_deleted->cbegin(); it!=to_be_deleted->cend(); it++) {
            m_logger->log("to erase: <"+it->first+">");
            candidates->erase(it->first);
        }
        to_be_deleted->clear();
    }// while loop
    for(auto it=candidates->cbegin(); it!=candidates->cend(); it++) {
        leaves->push_back(it->first);
    }
    delete itt;
    return leaves;
}


bool EntityAnn::is_ancestor_of(string b, string a) {
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
        //        m_logger->log("add_class_to_ancestor_lookup> the class already exists: <"+tclass+">");
        return m_ancestor_lookup.at(tclass);
    }
    m_logger->log("add_class_to_ancestor_lookup> the class is being added to the lookup: <"+tclass+">");
    ancestors = new std::unordered_map<string, bool>;
    m_ancestor_lookup.insert({tclass, ancestors});
    //    itt = hdt->search("", rdfs_subclassof.c_str(), tclass.c_str());
    itt = hdt->search(tclass.c_str(), rdfs_subclassof.c_str(), "");
    while(itt->hasNext()) {
        triple = itt->next();
        parent = triple->getObject();
        //        parent = triple->getSubject();
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

//void EntityAnn::score_cell(string cell_value){

//}

// it can be sped up more
bool EntityAnn::compute_intermediate_coverage(string cell_value) {
    std::list<string>* classes;
    std::list<string>* entities;
    size_t Q_size, Z_size;
    TNode* tnode;
    entities = this->get_entities_of_value(cell_value); // Z(v):
    Z_size = entities->size();
    m_logger->log("compute_intermediate_coverage> number of entities: "+to_string(entities->size()));
    for(auto it=entities->cbegin(); it!=entities->cend(); it++) {
        classes = this->get_leaf_classes(*it); // Q(e): *it = e
        this->update_graph(classes);
        Q_size = classes->size();
        m_logger->log("compute_intermediate_coverage> got the QSIZE "+to_string(Q_size));
        for(auto it2=classes->cbegin(); it2!=classes->cend(); it2++) {
            m_logger->log("compute_intermediate_coverage> leaf: "+(*it2));
            // add the class to the lookup if not added yet
            this->add_class_to_ancestor_lookup(*it2);
            m_logger->log("compute_intermediate_coverage> to get: "+(*it2));
            tnode = m_graph->get_node(*it2);
            if(tnode == nullptr) {
                m_logger->log("Error: in compute_intermediate_coverage, tnode <"+cell_value+"> is null\n\n");
                cout<< "Error: in compute_intermediate_coverage, tnode <"<<cell_value<<"> is null\n\n";
            }
            tnode->tc += 1.0 / (Q_size * Z_size);
        }
    }
    return entities->size()>0;
}


bool EntityAnn::compute_intermediate_coverage(string cell_value, std::list<string>* properties, bool double_level) {
//    string qcell_value;
    std::list<string>* classes;
    std::list<string>* entities;
    size_t Q_size, Z_size;
    TNode* tnode;
    m_logger->log("compute_intermediate_coverage> cell value: "+cell_value);
//    qcell_value= get_quoted(cell_value);
    entities = this->get_entities_of_value(cell_value, properties, double_level); // Z(v):
    Z_size = entities->size();
    m_logger->log("compute_intermediate_coverage> Z_size 1: "+to_string(Z_size)+" for cell value: "+cell_value);
    if(Z_size == 0) {
        delete entities;
        entities = this->get_entities_of_value(cell_value);
        Z_size = entities->size();
        m_logger->log("compute_intermediate_coverage> Z_size 2: "+to_string(Z_size)+" for cell value: "+cell_value);
        Z_size *= m_ambiguitity_penalty; // penalty (to add more weight to entities matched with a property
    }
    m_logger->log("compute_intermediate_coverage> number of entities: "+to_string(entities->size()));
    for(auto it=entities->cbegin(); it!=entities->cend(); it++) {
        classes = this->get_leaf_classes(*it); // Q(e): *it = e
        this->update_graph(classes);
        Q_size = classes->size();
        m_logger->log("compute_intermediate_coverage> got the QSIZE ");
        for(auto it2=classes->cbegin(); it2!=classes->cend(); it2++) {
            // add the class to the lookup if not added yet
            this->add_class_to_ancestor_lookup(*it2);
            tnode = m_graph->get_node(*it2);
            if(tnode == nullptr) {
                m_logger->log("Error: in compute_intermediate_coverage, tnode <"+cell_value+"> is null\n\n");
                cout<< "Error: in compute_intermediate_coverage, tnode <"<<cell_value<<"> is null\n\n";
            }
            tnode->tc += 1.0 / (Q_size * Z_size);
        }
    }
    return entities->size()>0;
}

TNode* EntityAnn::get_tnode(string uri) {
    //    m_graph->print_nodes();
    return m_graph->get_node(uri);
}


Graph* EntityAnn::get_graph() {
    return m_graph;
}

void EntityAnn::update_graph(std::list<string>* classes) {
    for(auto it=classes->cbegin(); it!=classes->cend(); it++) {
        this->update_graph(*it);
    }
}

TNode* EntityAnn::update_graph(string class_uri) {
    IteratorTripleString* itt;
    TripleString* triple = nullptr;
    string parent;
    TNode* pnode;
    TNode* tnode = m_graph->get_node(class_uri);
    bool orphan=true; // has not parents
    if(tnode==nullptr) {
        m_logger->log("update_graph> to add class: "+class_uri);
        tnode = new TNode(class_uri);
        itt = hdt->search(class_uri.c_str(), rdfs_subclassof.c_str(), "");
        while(itt->hasNext()) {
            orphan = false;
            triple = itt->next();
            parent = triple->getObject();
            m_logger->log("update_graph> parent: "+parent);
            pnode = this->update_graph(parent);
            m_logger->log("update_graph> adding a link "+parent+" -------- "+class_uri);
            m_graph->add_edge(pnode, tnode);
        }
        delete itt;
        if(orphan){
            m_graph->add_node(tnode);
            m_logger->log("update_graph> "+class_uri+" is an orphan");
//            cout<<class_uri <<"  is an orphan\n\n";
        }
    }
    return tnode;
}

double EntityAnn::compute_Lc_for_node(TNode* tnode) {
    double d;
    m_logger->log("compute_Lc_for_node> "+tnode->uri);
    if(tnode->lc==0.0) {
        m_logger->log("compute_Lc_for_node> in if "+tnode->uri);
        for(auto it=tnode->children->cbegin(); it!=tnode->children->cend(); it++) {
            d = this->compute_Lc_for_node(it->second);
            tnode->lc += d;
            m_logger->log("compute_Lc_for_node> "+tnode->uri+ " append "+it->second->uri+" with value "+to_string(d));
            //            tnode->lc = tnode->lc + this->compute_Lc_for_node(it->second);
        }
        // if the tnode is a leave node
        if(tnode->lc == 0.0) {
            m_logger->log("compute_Lc_for_node> is a child "+tnode->uri);
            tnode->lc = tnode->ic;
        }
    }
    return tnode->lc;
}

void EntityAnn::compute_Lc_for_all() {
    std::list<TNode*>* roots = m_graph->get_candidate_roots();
    for(auto it=roots->cbegin(); it!=roots->cend(); it++) {
        this->compute_Lc_for_node(*it);
    }
    //    std::list<TNode*> *leaves = m_graph->get_leaves();
    //    for(auto it=leaves->cbegin();it!=leaves->cend();it++){
    //        this->compute_Lc_for_node(*it);
    //    }
}

void EntityAnn::compute_Ic_for_all() {
    std::list<TNode*>* roots = m_graph->get_candidate_roots();
    for(auto it=roots->cbegin(); it!=roots->cend(); it++) {
        this->compute_Ic_for_node(*it);
    }
}


void EntityAnn::compute_Ic_for_node(TNode* tnode) {
    if(tnode->ic==0.0) {
        tnode->ic = tnode->tc;
        for(auto it=tnode->children->cbegin(); it!=tnode->children->cend(); it++) {
            this->compute_Ic_for_node(it->second);
        }
    }
}




void EntityAnn::compute_classes_entities_counts() {
    IteratorTripleString* itt;
    unsigned long num_of_entities;
    TNode* r;
    for(auto it=m_graph->m_graph->cbegin(); it!=m_graph->m_graph->cend(); it++) {
        itt = hdt->search("", rdf_type.c_str(), it->first.c_str());
        num_of_entities = static_cast<unsigned long>(itt->estimatedNumResults());
        //        m_logger->log("entities of "+it->first+" is: "+to_string(num_of_entities));
        //		if(num_of_entities==0) {
        //            num_of_entities++;
        //        }
        m_classes_entities_count.insert({it->first, num_of_entities});
        delete itt;
    }
    r = this->m_graph->get_root();
    if(r!=nullptr) {
        propagate_counts(r);
    }
    else {
        m_logger->log("compute_classes_entities_counts> root is null");
    }
}

// include the counts of the childs because HDT does not perform reasoning
unsigned long EntityAnn::propagate_counts(TNode* tnode) {
    unsigned long count;
    count = m_classes_entities_count.at(tnode->uri);
    for(auto it=tnode->children->cbegin(); it!=tnode->children->cend(); it++) {
        count += this->propagate_counts(it->second);
    }
    m_classes_entities_count.at(tnode->uri) = count;
    m_logger->log("propagate_counts> "+tnode->uri+" #"+to_string(count));
    return count;
}

void EntityAnn::compute_Is_for_all() {
    TNode* r;
    r = m_graph->get_root();
    if(r!=nullptr) {
        r->is = 1;
        this->compute_Is_for_node(r);
    }
    else {
        m_logger->log("compute_Is_for_all> root is null");
    }
}

void EntityAnn::compute_Is_for_node(TNode* tnode) {
    TNode* ch = nullptr;
    double ch_count;
    unsigned long p_count = m_classes_entities_count.at(tnode->uri);
    if(p_count==0) {
        p_count++;
    }
    for(auto it=tnode->children->cbegin(); it!=tnode->children->cend(); it++) {
        ch = it->second;
        ch_count = static_cast<double>(m_classes_entities_count.at(ch->uri));
        if(ch_count == 0.0) {
            ch_count=1.0;
        }
        ch->is = ch_count / p_count;
        m_logger->log("compute_Is_for_node> Is for node: "+tnode->uri+", "+to_string(ch_count)+" / "+to_string(p_count)+" = "+to_string(ch->is));
        this->compute_Is_for_node(ch);
    }
}

void EntityAnn::compute_Ls_for_all() {
    std::list<TNode*>* leaves = m_graph->get_leaves();
    for(auto it=leaves->cbegin(); it!=leaves->cend(); it++) {
        this->compute_Ls_for_node(*it);
    }
}

double EntityAnn::compute_Ls_for_node(TNode* tnode) {
    double ls = tnode->ls;
    double ls_2, ls_max;
    TNode* p;
    if(ls == 0.0) {
        if(tnode->parents->size() == 0) { // root
            tnode->ls = 1;
            m_logger->log("Ls> a root: "+tnode->uri);
        }
        else {
            p = tnode->parents->cbegin()->second;
            ls_max = this->compute_Ls_for_node(p);
            for(auto it=tnode->parents->cbegin(); it!=tnode->parents->cend(); it++) {
                ls_2 = this->compute_Ls_for_node(it->second);
                if(ls_2>ls_max) {
                    p = it->second;
                    ls_max = ls_2;
                }
            }
            ls = tnode->is * p->ls;
            tnode->ls = ls;
            m_logger->log("Ls> is: "+to_string(tnode->is)+" parent Ls: "+to_string(p->ls)+" parent: "+p->uri);
        }
    }
    return ls;
}

void EntityAnn::compute_fs() {
    TNode* tnode;
    for(auto it=this->m_graph->m_graph->cbegin(); it!=this->m_graph->m_graph->cend(); it++) {
        tnode = it->second;
        tnode->fs = -1 * tnode->ls + 1;
        m_logger->log("compute_fs> "+tnode->uri+", -1 * "+to_string(tnode->ls)+" + 1 = "+to_string(tnode->fs));
    }
}

void EntityAnn::compute_fc(unsigned long m) {
    TNode* tnode;
    double m_d = static_cast<unsigned long>(m);
    for(auto it=this->m_graph->m_graph->cbegin(); it!=this->m_graph->m_graph->cend(); it++) {
        tnode = it->second;
        tnode->fc = tnode->lc/m_d;
        m_logger->log("compute_fc> "+tnode->uri+", "+to_string(tnode->lc)+" / "+to_string(m_d)+" = "+to_string(tnode->fc));
    }
}

void EntityAnn::compute_f() {
    TNode* tnode;
    for(auto it=this->m_graph->m_graph->cbegin(); it!=this->m_graph->m_graph->cend(); it++) {
        tnode = it->second;
        tnode->f = tnode->fc * m_alpha + tnode->fs * (1.0 - m_alpha);
        m_logger->log("compute_f> "+tnode->uri+" : "+to_string(m_alpha)+" * "+to_string(tnode->fc)+" + "+to_string((1.0 - m_alpha))+" * "+to_string(tnode->fs));
    }
}

bool compare_tnodes (const  TNode* const& first, const  TNode* const& second) {
    return first->f < second->f;
}

std::list<string>* EntityAnn::get_candidates() {
    std::list<string>* candidates = new std::list<string>;
    std::list<TNode*>* tnodes = new std::list<TNode*>;
    for(auto it=m_graph->m_graph->begin(); it!=m_graph->m_graph->cend(); it++) {
        tnodes->push_back(it->second);
    }
    tnodes->sort(compare_tnodes);
    for(auto it=tnodes->cbegin(); it!=tnodes->cend(); it++) {
        m_logger->log("get_candidates> "+(*it)->uri+" ----- ("+to_string((*it)->f)+")");
        candidates->push_front((*it)->uri);
    }
    return candidates;
}

void EntityAnn::pick_root() {
    m_graph->pick_root();
}

string EntityAnn::get_quoted(string v){
    string qcell_value = v;
    if(qcell_value[0] != '\"') {
        qcell_value = "\""+qcell_value+"\"";
    }
//    return qcell_value+m_lang_tag;
    return qcell_value;
}

string EntityAnn::get_taged(string qv){
    return qv+m_lang_tag;
}

void EntityAnn::set_alpha(double alpha){
    m_alpha = alpha;
}

double EntityAnn::get_alpha(){
    return m_alpha;
}

void EntityAnn::set_language_tag(string tag){
    m_lang_tag = tag;
}

string EntityAnn::get_title_case(string s){
    string tcased = "";
    char ch;
    bool start_new_word = true; // will be on once a space of found
    for(size_t i=0;i<s.size();i++){
        if(s[i]==' '){
            start_new_word = true;
            tcased += " ";
        }
        else if(start_new_word){
            ch = static_cast<char>(toupper(s[i]));
            tcased.push_back(ch);
            start_new_word = false;
        }
        else{
            ch = static_cast<char>(tolower(s[i]));
            tcased.push_back(ch);
        }
    }
    return tcased;
}

void EntityAnn::set_title_case(bool t){
    m_retry_with_title_case = t;
}


bool EntityAnn::get_title_case(){
    return m_retry_with_title_case;
}

string EntityAnn::strip_quotes(string s){
    string t="";
    for(size_t i =0;i<s.length();i++){
        if(s[i]!='"'){
            t.push_back(s[i]);
        }
    }
    return t;
}


