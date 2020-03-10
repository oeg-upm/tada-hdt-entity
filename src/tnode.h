#ifndef TNODE_H
#define TNODE_H

#include<string>

using namespace std;

class TNode {
    public:
        TNode();

        string uri;
        double ic, lc, fc; // coverage related variables
        double is, ls, fs; // specificity related variables



    private:

};

#endif // TNODE_H
