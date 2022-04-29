#include <iostream>
#include <cstring>

using namespace std;

struct node {
  int data;
  int color; // 0 for red, 1 for black
  node* parent;
  node* left;
  node* right;
};

void add(node* n1, node* n);
node* search(node* n, int k);
void print(node* n, int k);
void fix(node* root, node* n);
node* sibling(node* root, node* n);
node* uncle(node* root, node* n);

int main() {
  node* root = NULL;
  char action[10];
  int input;
  while (true) {
    cout << "What would you like to do? (ADD, FILE, SEARCH, DELETE, PRINT, QUIT) "; cin >> action;
    if (strcmp(action, "ADD") == 0 || strcmp(action, "add") == 0) {
      cout << "Enter the number to add: "; cin >> input;
      node* n = new node();
      n->data = input;
      n->parent = NULL;
      n->left = NULL;
      n->right = NULL;
      n->color = 0;
      if (root == NULL) {
	n->color = 1;
	root = n;
      }
      else {
	add(root, n);
	fix(root, n);
      }
    }
    else if (strcmp(action, "FILE") == 0 || strcmp(action, "file") == 0) {
    }
    else if (strcmp(action, "SEARCH") == 0 || strcmp(action, "search") == 0) {
      cout << "What do you want to search for? "; cin >> input;
      if (search(root, input) != NULL) {
	cout << "Yes, that is in the tree." << endl;
      }
      else {
	cout << "No, that ain't in the tree." << endl;
      }
    }
    else if (strcmp(action, "DELETE") == 0 || strcmp(action, "delete") == 0) {
      // will be added in second part of red-black tree
    }
    else if (strcmp(action, "PRINT") == 0 || strcmp(action, "print") == 0) {
      print(root, 0);
    }
    else if (strcmp(action, "QUIT") == 0 || strcmp(action, "quit") == 0) {
      break;
    }
    cout << endl;
  }
}

void add(node* n1, node* n) {

  if (n1->data >= n->data) {
    if (n1->left == NULL) {
      n1->left = n;
      n->parent = n1;
    }
    else {
      add(n1->left, n);
    }
  }

  else if (n1->data < n->data) {
    if (n1->right == NULL) {
      n1->right = n;
      n->parent = n1;
    }
    else {
      add(n1->right, n);
    }
  }
}

node* search(node* n, int k) {
  // same as binary search tree
  if (n == NULL) {
    return NULL;
  }

  else if (n->data == k) {
    return n;
  }

  else if (n->data > k) {
    return search(n->left, k); // switch back to left
  }

  else {
    return search(n->right, k); // switch back to right
  }
}

void print(node* n, int k) {
  if (n == NULL) {
    // do nothing
  }
  else {
    print(n->right, k+1);

    for (int i=0; i<k; i++) {
      cout << "\t";
    }
    cout << n->data;
    if (n->color == 0) {
      cout << "(R)" << endl;
    }
    if (n->color == 1) {
      cout << "(B)" << endl;
    }
    print(n->left, k+1);
  }
}

void fix(node* root, node* n) {
  // adjust the tree according to the red-black tree rules
  if (n == root) {
    n->color = 1;
  }
  else if (n->parent->color == 1) {
    // do nothing
  }
  else if (uncle(root, n) != NULL && uncle(root, n)->color == 0) {
    n->parent->color = 1;
    n->parent->parent->color = 0;
    uncle(root, n)->color = 1;
    fix(root, n->parent->parent);
  }
  else if (n->parent->parent->left != NULL && n->parent->parent->left->right == n) {
    node* p = n->parent;
    node* g = n->parent->parent;
    g->left = n;
    n->parent = g;
    p->right = n->left;
    if (n->left != NULL) {n->left->parent = p;}
    n->left = p;
    p->parent = n;
    cout << p->data << ", " << p->parent->data << endl;
    fix(root, p);
  }
  else if (n->parent->parent->right != NULL && n->parent->parent->right->left == n) {
    node* p = n->parent;
    node* g = n->parent->parent;
    g->right = n;
    n->parent = g;
    p->left = n->right;
    if (n->right != NULL) {n->right->parent = p;}
    n->right = p;
    p->parent = n;
    fix(root, p);
  }
}

node* sibling(node* root, node* n) {
  if (n == root) {
    return NULL;
  }
  else {
    if (n->parent->left == n) {
      return n->parent->right;
    }
    else {
      return n->parent->left;
    }
  }
}

node* uncle(node* root, node* n) {
  if (n == root) {
    return NULL;
  }
  else {
    return sibling(root, n->parent);
  }
}
