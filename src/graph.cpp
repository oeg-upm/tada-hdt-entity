#include"graph.h"
#include "tnode.h"


#include <unordered_map>


Graph::Graph(EasyLogger* m) {
    m_graph = new std::unordered_map<string, TNode*>;
    m_logger = m;
}

bool Graph::add_edge(TNode* from_node, TNode* to_node) {
//    // add nodes if they do not exists
//    if(m_graph->find(from_node->uri) == m_graph->cend()) {
//        m_graph->insert({from_node->uri, from_node});
//    }
//    if(m_graph->find(to_node->uri) == m_graph->cend()) {
//        m_graph->insert({to_node->uri, to_node});
//    }
    // add the nodes if they are new
    bool added = false;
    this->add_node(from_node);
    m_logger->log("add uri tnode with: "+from_node->uri);
    this->add_node(to_node);
    m_logger->log("add uri tnode with: "+to_node->uri);
    // if the edge does not exist
    if(from_node->children->find(to_node->uri) == from_node->children->cend()) {
        from_node->children->insert({to_node->uri, to_node});
        added = true;
    }
    if(to_node->parents->find(from_node->uri) == to_node->parents->cend()) {
        to_node->parents->insert({from_node->uri, from_node});
        added = true;
    }
    return added;
}

bool Graph::add_edge(string from_uri, string to_uri){
    m_logger->log("add uri: "+from_uri);
    this->add_node(from_uri);
    m_logger->log("add uri: "+to_uri);
    this->add_node(to_uri);
    TNode* from_node = m_graph->at(from_uri);
    TNode* to_node = m_graph->at(to_uri);
    return this->add_edge(from_node, to_node);
}

bool Graph::add_node(string uri){
    TNode * tnode;
    // if this is a new uri
    if(m_graph->find(uri) == m_graph->cend()) {
        tnode = new TNode(uri);
        m_graph->insert({uri, tnode});
        return true;
    }
    return false;
}

bool Graph::add_node(TNode* tnode){
    // if this is a new uri
    if(m_graph->find(tnode->uri) == m_graph->cend()) {
        m_graph->insert({tnode->uri, tnode});
        return true;
    }
    return false;
}


TNode* Graph::get_node(string uri) {
    if(m_graph->find(uri) != m_graph->cend()) {
         m_logger->log("uri: <"+uri+">  is found");
        return m_graph->at(uri);
    }
    m_logger->log("uri: <"+uri+">  is not found");
    return nullptr;
}

void Graph::print_nodes(){
    cout<< "printing nodes: "<<endl;
    for(auto it=m_graph->cbegin();it!=m_graph->cend();it++){
        cout<<it->first<<endl;
    }
    cout << "=======END=======\n";
}
