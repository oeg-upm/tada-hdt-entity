#ifndef TNODE_H
#define TNODE_H

#include<string>
#include<list>
#include <unordered_map>

using namespace std;

class TNode {
    public:
        TNode(string uri);

        string uri;
        double ic, lc, fc; // coverage related variables
        double tc; // the variable used to compute the score fractions before the
        double is, ls, fs; // specificity related variables
        std::unordered_map<string,TNode*> *parents; // sometimes there are multiple parents
        std::unordered_map<string,TNode*> *children; // childs
    private:

};

#endif // TNODE_H
