#include "tnode.h"



string uri;
double ic, lc, fc; // coverage related variables
double tc; // the variable used to compute the score fractions before the
double is, ls, fs; // specificity related variables
std::unordered_map<string,TNode*> *parents; // sometimes there are multiple parents
std::unordered_map<string,TNode*> *children; // childs


TNode::TNode(string auri){
    ic=lc=fc=tc=is=ls=fs=0;
    parents = new unordered_map<string,TNode*>;
    children = new unordered_map<string,TNode*>;
    uri = auri;
}



