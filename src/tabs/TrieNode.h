#ifndef TRIE_NODE_H
#define TRIE_NODE_H

#include <list>
#include <string>
#include <iostream>

#include <giomm.h>
#include <glibmm.h>

class TrieNode {
public:
    inline TrieNode();
    std::string data;
    std::list<TrieNode>* deeper;
    Gtk::TreeRow row;
};

TrieNode::TrieNode() {}
#endif
