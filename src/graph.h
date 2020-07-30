#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <string>
#include <unordered_map>
#include "tnode.h"
#include <easy_logger/easy_logger.h>


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
        Graph(string logger_fname);
        Graph(EasyLogger*);
        bool add_edge(TNode *from_node, TNode *to_node);
        bool add_edge(string from_uri, string to_uri);
        TNode* get_node(string uri);
        bool add_node(string uri);
        bool add_node(TNode* tnode);
        std::list<TNode*>* get_candidate_roots();
        std::list<TNode*>* get_leaves();
        void pick_root();
        void print_nodes();
        TNode* get_root();
        void set_root(TNode*);
        unsigned int get_max_depth(TNode*);
        std::unordered_map<string,TNode*>* m_graph;
    private:
        EasyLogger* m_logger;
        TNode* m_root;

};
#endif

