#ifndef TNODE_H
#define TNODE_H

#include <iostream>
#include<string>
#include<list>
#include <unordered_map>

using namespace std;

class TNode {

    public:
        TNode();
        TNode(string auri);

        string uri;
        double ic, lc, fc; // coverage related variables
        double tc; // the variable used to compute the score fractions before the
        double is, ls, fs; // specificity related variables
        double f; // the result of the final score
        std::unordered_map<string,TNode*> *parents; // sometimes there are multiple parents
        std::unordered_map<string,TNode*> *children; // childs
//        std::ostream& operator<<(std::ostream& out, const TNode& tnode);
//        TNode& operator=(const TNode& other){
//            if (this != &other) {
//                this->uri = other.uri;
//                this->fc = other.fc;
//                this->fs = other.fs;
//                this->children = other.children;
//                this->ic = other.ic;
//                this->is = other.is;
//                this->lc = other.lc;
//                this->ls = other.ls;
//                this->parents = other.parents;
//                this->tc = other.tc;
//            }
//            return *this;
//        }
   // private:

};
std::ostream& operator<<(std::ostream& out, const TNode& tnode);
//bool operator<(const TNode& l, const TNode& r);
//std::ostream& operator<<(std::ostream& os, const T& obj)
//std::ostream& operator<<(std::ostream& out, const TNode& tnode)
//{
////   return out << tnode.uri << " { ic:"<<tnode.is<<", Lc:"<<tnode.lc<<"fc:"<<tnode.fc<<" }" ;
//    return out;
//}

//inline bool operator==(const TNode& lhs, const TNode& rhs){
//    return lhs.uri == rhs.uri;
//}

//inline bool operator!=(const TNode& lhs, const TNode& rhs){
//    return !(lhs.uri == rhs.uri);
//}



#endif // TNODE_H

