#include "SuffixTree.h"
#include "Node.h"
#include "Suffix.h" 
#include <iostream>

SuffixTree::SuffixTree(std::string s) {
    length = s.length();
    tree_string = ' ' + s;
    internal_node_ID = 0;
	current_end = new int(0);
    root = new Node(NULL, 1, new int (0), internal_node_ID);
	root->suffix_link = root;
}

void SuffixTree::construct() {
	(*current_end)++;
	current_full_string = new Node(root, 1, current_end, 1);
    root->add_child(current_full_string);
    //print_tree();

    for (int i = 1; i < length; i++) {
        SPA(i);
		print_tree();
		if (i % 100 == 0) std::cerr << "Phase: " << i << std::endl;
    }
}

void SuffixTree::print_tree() {
    print_node(root);
	std::cout << "END OF TREE" << std::endl;
}

void SuffixTree::print_node(Node* parent) {
    int parent_ID = parent->ID;
    Node* child = parent->child;
    while (child != NULL) {
        std::cout << parent_ID 
                  << " " << get_substr(child->begin_index, *child->end_index) 
                  << " " << child->ID << std::endl;
        print_node(child);
        child = child->sibling;
    } 
}

//SPA: Single Phase Algorithm (Gusfield, 1997)
void SuffixTree::SPA(int i) { 
	Suffix previous_suffix(current_full_string, *current_end);
	(*current_end)++;
	//the first extension is always done implicitly by incrementing the current_end counter
	
    for (int j = 2; j <= (i + 1); j++) {
        SEA(previous_suffix, j, i);
		//print_tree();
    }
}

//SEA: Single Extension Algorithm (Gusfield, 1997)
void SuffixTree::SEA(Suffix& previous_suffix, int j, int i) {

	Suffix suffix;
	Node* origin = previous_suffix.walk_up();

	if (origin == root)
		suffix = get_suffix(root, j, i);
	else if (origin == previous_suffix.node)
		suffix = get_suffix(origin->suffix_link, *origin->end_index, *origin->end_index - 1);
	else
		suffix = get_suffix(origin->suffix_link, previous_suffix.node->begin_index, previous_suffix.char_index);

		
//		suffix = (origin == root? get_suffix(root, j, i)
	//				: get_suffix(origin->suffix_link, 
		//			previous_suffix.node->begin_index, 
			//		previous_suffix.char_index));

	//Suffix suffix = get_suffix(root, j, i);							
    //RULE1 (path ends at a leaf) - do nothing!
	//RULE2 (path doesn't end at a leaf and no path continues with char [i + 1]):
    if (!suffix.ends_at_leaf() && !suffix.continues_with_char(*this, tree_string[i + 1]))
        RULE2(suffix, i + 1, j);
    //else RULE3 (some ongoing path starts with char [i + 1]) - do nothing!
	if (previous_suffix.new_internal_node)
		previous_suffix.node->suffix_link = suffix.node;
	previous_suffix = suffix;
}

//The 'skip/count' trick for suffix tree traversal (Gusfield, 1997)
Suffix SuffixTree::get_suffix(Node* origin, int begin_index, int end_index) {
	Node* original_origin = origin;
	int original_begin_index = begin_index;
	int original_end_index = end_index;
	int char_index = *origin->end_index;
	char ch = tree_string[begin_index];
	while (begin_index <= end_index) {
		origin = origin->get_child(*this, tree_string[begin_index]);
		if (origin->edge_length() < end_index - begin_index + 1)
			char_index = *origin->end_index;
		else char_index = origin->begin_index + (end_index - begin_index);
		begin_index+=origin->edge_length();
	}
	return Suffix(origin, char_index);
}

std::string SuffixTree::get_substr(int start_pos, int end_pos) {
    if (start_pos > end_pos) return std::string();
    return tree_string.substr(start_pos, end_pos - start_pos + 1);
}

void SuffixTree::RULE2(Suffix& suffix, int char_index, int new_leaf_ID) {
    if (!suffix.ends_at_node()) {  //eg. case 2 (path ends inside an edge)
		suffix.node->split_edge(suffix.char_index, --internal_node_ID);
		suffix.node = suffix.node->parent;
		suffix.new_internal_node = true;
	}
	Node* new_leaf = new Node(suffix.node, char_index, current_end, new_leaf_ID);  
    suffix.node->add_child(new_leaf);
}

std::string SuffixTree::walk_up(Node*& v, Suffix suffix) {
	std::string to_return;
	int test = 0;
	v = suffix.node;
	while (v->suffix_link == NULL && v != root) {
		v = v->parent;
		std::cout << "Looped: " << ++test << std::endl;
	}	
	return std::string();
}

