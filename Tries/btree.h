#ifndef BTREE_H
#define BTREE_H

// You can reduce this to include only alphabets A-Z, a-z to reduce memory 
// consumption. Or increase this to include UTF-8 characters. If you plan to 
// use UTF-8, remember to change char * to w_char *
const int CHILD_SIZE=128; 
const int MAX_STRING_SIZE=256;

class BTree
{
protected:
    BTree *m_children[CHILD_SIZE];
    char *m_replacement;

public:
    BTree();
    ~BTree();
    void addPhrase(char *text, char *replacement);
    char *match(char *text);
};

#endif // BTREE_H
