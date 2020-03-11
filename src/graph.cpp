#include"graph.h"
#include "tnode.h"

#include <unordered_map>


Graph::Graph() {
    m_graph = new std::unordered_map<string, TNode*>;
}

void Graph::add_edge(TNode* from_node, TNode* to_node) {
    // add nodes if they do not exists
    if(m_graph->find(from_node->uri) == m_graph->cend()) {
        m_graph->insert({from_node->uri, from_node});
    }
    if(m_graph->find(to_node->uri) == m_graph->cend()) {
        m_graph->insert({to_node->uri, to_node});
    }
    // if the edge does not exist
    if(from_node->children->find(to_node->uri) == from_node->children->cend()) {
        from_node->children->insert({to_node->uri, to_node});
    }
    if(to_node->parents->find(from_node->uri) == to_node->parents->cend()) {
        to_node->parents->insert({from_node->uri, from_node});
    }
}




TNode* Graph::get_node(string uri) {
    return NULL;
}
