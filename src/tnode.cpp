#include "tnode.h"



//string uri;
//double ic, lc, fc; // coverage related variables
//double tc; // the variable used to compute the score fractions before the
//double is, ls, fs; // specificity related variables
//std::unordered_map<string, TNode*>* parents; // sometimes there are multiple parents
//std::unordered_map<string, TNode*>* children; // childs

TNode::TNode() {
    ic=lc=fc=tc=is=ls=fs=f=0;
    parents = new unordered_map<string, TNode*>;
    children = new unordered_map<string, TNode*>;
    uri = "Not set";
}

TNode::TNode(string auri) {
    ic=lc=fc=tc=is=ls=fs=f=0;
    parents = new unordered_map<string, TNode*>;
    children = new unordered_map<string, TNode*>;
    uri = auri;
}


std::ostream& operator<<(std::ostream& out, const TNode& tnode) {
    //   cout << "In the << operator"<<endl;
    //   return out << tnode.uri << " { ic:"<<tnode.ic<<", Lc:"<<tnode.lc<<", fc:"<<tnode.fc<< ", tc: "<< tnode.tc <<" }" ;
    //return out;
    //    out << "abc";
    //    return out;
    out << tnode.uri << " { ic:"<<tnode.ic<<", Lc:"<<tnode.lc<<", fc:"<<tnode.fc<< ", tc: "<< tnode.tc << \
        ", Is: " << tnode.is << ", Ls: " << tnode.ls << ", fs: " << tnode.fs <<" }" ;
    return out;
}


//bool operator<(const TNode& l, const TNode& r) {
//    return l.f < r.f; // keep the same order
//}
