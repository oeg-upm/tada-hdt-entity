#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <string>
#include <unordered_map>
#include "tnode.h"

using namespace std;

//typedef struct Node {
//    string uri;
//    double ic,lc,fc; // coverage related variables
//    double is,ls,fs; // specificity related variables
//    struct Node* parent; //
//} Node;

// Graph as adjacency list with pointer to the parent
class Graph {
    public:
        Graph();
        void add_edge(TNode *from_node, TNode *to_node);
        TNode* get_node(string uri);
    private:
        std::unordered_map<string,TNode*>* m_graph;
};
#endif

