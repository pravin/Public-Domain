#include "btree.h"
#include <string.h>

BTree::BTree() {
    for(int c = 0; c < CHILD_SIZE; c++) {
        m_children[c] = NULL;
    }
    m_replacement = NULL;
}

BTree::~BTree() {
    // Free memory
    for (int c = 0; c < CHILD_SIZE; c++) {
        if (m_children[c] != NULL) {
            delete(m_children[c]);
            m_children[c] = NULL;
        }
    }
    if (m_replacement != NULL) {
        delete(m_replacement);
    }
}

void BTree::addPhrase(char *text, char *replacement) {
    char c = text[0];

    if (m_children[c] == NULL) {
        m_children[c] = new BTree();
    }

    if (strlen(text) > 0) {
        m_children[c]->addPhrase(text+1); // Recursive call on child
    }
    else { // This is the last character, save replacement string
        int len = strlen(replacement) + 1;
        m_replacement = new char[len];
        strncpy(m_replacement, replacement, len);
    }
}

char * BTree::match(char *text) {
    char c = text[0];
    char *retval = NULL;

    if (strlen(text) == 0) { // We are on the last character
        retval = replacement;
    }
    else if (m_children[c] != NULL) { // Child with the next character exists
        retval = m_children[c]->match(text+1);
    }
    return retval;
}
