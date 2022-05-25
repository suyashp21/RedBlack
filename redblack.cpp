#include <iostream>
#include <cstring>
#include <fstream>

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
void remove(node* root, node* n);
node* sibling(node* root, node* n);
node* uncle(node* root, node* n);
void deletefix(node* root, node* n);

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
	root = n;
	n->color = 1;
      }
      else {
	add(root, n);
	fix(root, n);
      }
      while (root->parent != NULL) {
	root = root->parent;
      }
    }
    else if (strcmp(action, "FILE") == 0 || strcmp(action, "file") == 0) {
      // read in from file
      ifstream myfile ("Numbers.txt");
      for (int i=1; i<=11; i++) {
	int k;
	myfile >> k;
	cout << k << endl;
	node* n = new node();
	n->data = k;
	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	if (root == NULL) {
	  root = n;
	  n->color = 1;
	}
	else {
	  add(root, n);
	  fix(root, n);
	}
	while (root->parent != NULL) {
	  root = root->parent;
	}
	// print(root, 0);
      }
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
      cout << "Enter the number to delete: "; cin >> input;
      node* n = search(root, input);
      if (n == NULL) {
	cout << "That number isn't in the tree" << endl;
      }
      else {
	// maybe add confirmation message?
	remove(root, n);
      }
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
    // cout << "A" << endl;
    n->parent->color = 1;
    n->parent->parent->color = 0;
    uncle(root, n)->color = 1;
    fix(root, n->parent->parent);
  }
  else if (n->parent->parent->left != NULL && n->parent->parent->left->right == n) {
    // cout << "B" << endl;
    node* p = n->parent;
    node* g = n->parent->parent;
    g->left = n;
    n->parent = g;
    p->right = n->left;
    if (n->left != NULL) {n->left->parent = p;}
    n->left = p;
    p->parent = n;
    // cout << p->data << ", " << p->parent->data << endl;
    fix(root, p);
  }
  else if (n->parent->parent->right != NULL && n->parent->parent->right->left == n) {
    // cout << "C" << endl;
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
  else if ((uncle(root, n) == NULL || uncle(root, n)->color == 1) && n->parent->parent->left != NULL && n->parent->parent->left->left == n) {
    // cout << "D" << endl;
    if (n->parent->parent == root) {
      node* p = n->parent;
      node* g = p->parent;
      p->parent = g->parent;
      g->left = p->right;
      g->parent = p;
      p->right = g;
      if (g->left != NULL) {g->left->parent = g;}
      p->color = 1;
      g->color = 0;
      // root = p;
    }
    else {
      node* p = n->parent;
      node* g = p->parent;
      p->parent = g->parent;
      if (g->parent->right == g) {g->parent->right = p;}
      else if (g->parent->left == g) {g->parent->left = p;}
      g->left = p->right;
      g->parent = p;
      p->right = g;
      if (g->left != NULL) {g->left->parent = g;}
      p->color = 1;
      g->color = 0;
    }
  }
  else if ((uncle(root, n) == NULL || uncle(root, n)->color == 1) && n->parent->parent->right != NULL && n->parent->parent->right->right == n) {
    // cout << "E" << endl;
    if (n->parent->parent == root) {
      node* p = n->parent;
      node* g = p->parent;
      p->parent = g->parent;
      g->right = p->left;
      g->parent = p;
      p->left = g;
      if (g->right != NULL) {g->right->parent = g;}
      p->color = 1;
      g->color = 0;
      // root = p;
    }
    else {
      node* p = n->parent;
      node* g = p->parent;
      p->parent = g->parent;
      if (g->parent->right == g) {g->parent->right = p;}
      else if (g->parent->left == g) {g->parent->left = p;}
      g->right = p->left;
      g->parent = p;
      p->left = g;
      if (g->right != NULL) {g->right->parent = g;}
      p->color = 1;
      g->color = 0;
    }

  }
}

void remove(node* root, node* n) {
  cout << n->data << " " << n->parent->data << endl; // debugging
  if (n->left != NULL && n->right != NULL) {
    node* n1 = n->left;
    while (n1->right != NULL) {
      n1 = n1->right;
    }
    n->data = n1->data;
    remove(root, n1);
  }
  else if (n->left != NULL && n->right == NULL) {
    // cout << "This case." << endl;
    if (n == root) {
    }
    else {
      if (n->color == 0) {
	if (n->parent->left == n) {n->parent->left = n->left; n->left->parent = n->parent;}
	else if (n->parent->right == n) {n->parent->right = n->left; n->right->parent = n->parent;}
	delete n;
      }
      else if (n->color == 1 && n->left->color == 0) {
	// cout << "This subcase." << endl;
        if (n->parent->left == n) {n->parent->left = n->left; n->left->parent = n->parent;}
        else if (n->parent->right == n) {n->parent->right = n->left; n->left->parent = n->parent;}
	n->left->color = 1;
        delete n;
      }
      else {
	if (n->parent->left == n) {n->parent->left = n->left; n->left->parent = n->parent;}
	else if (n->parent->right == n) {n->parent->right = n->left; n->left->parent = n->parent;}
	deletefix(root, n->left);
	delete n;
      }
    }
  }
  else if (n->left == NULL && n->right != NULL) {
    if (n == root) {
    }
    else {
      if (n->color == 0) {
	if (n->parent->left == n) {n->parent->left = n->right; n->right->parent = n->parent;}
	else if (n->parent->right == n) {n->parent->right = n->right; n->right->parent = n->parent;}
	delete n;
      }
      else if (n->color == 1 && n->right->color == 0) {
        if (n->parent->left == n) {n->parent->left = n->right; n->right->parent = n->parent;}
        else if (n->parent->right == n) {n->parent->right = n->right; n->right->parent = n->parent;}
	n->right->color = 1;
        delete n;
      }
      else {
	if (n->parent->left == n) {n->parent->left = n->right; n->right->parent = n->parent;}
	else if (n->parent->right == n) {n->parent->right = n->right; n->right->parent = n->parent;}
	deletefix(root, n->right);
	delete n;
      }
    }
  }
  else if (n->left == NULL && n->right == NULL) {
    if (n == root) {
    }
    else {
      if (n->color == 0) {
	if (n->parent->left == n) {n->parent->left = NULL;}
	else if (n->parent->right == n) {n->parent->right = NULL;}
	delete n;
      }
      else if (n->color == 1) {
      }
    }
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

void deletefix(node* root, node* n) {
  if (n != NULL && n->color == 0) {
    // do nothing
  }
  // Case 1
  else if (n == root) {
    // n is root, do nothing
  }
  // Case 2
  else if (sibling(root, n) != NULL && n->parent->color == 1 && sibling(root, n)->color == 0) {
    node* p = n->parent;
    node* s = sibling(root, n);
    if (p->left == n) {
      s->parent = p->parent;
      if (p->parent != NULL) {
	if (p->parent->left == p) {p->parent->left = s;}
	else if (p->parent->right == p) {p->parent->right = s;}
      }
      p->parent = s;
      s->left = p;
      p->right = s->left;
      if (p->right != NULL) {p->right->parent = p;}
      p->color = 0;
      s->color = 1;
      // fix(root, n)
    }
    else if (p->right == n) {
      s->parent = p->parent;
      if (p->parent != NULL) {
        if (p->parent->left == p) {p->parent->left = s;}
        else if (p->parent->right == p) {p->parent->right = s;}
      }
      p->parent = s;
      s->right = p;
      p->left = s->right;
      if (p->left != NULL) {p->left->parent = p;}
      p->color = 0;
      s->color = 1;
      // fix(root, n)
    }
  }
  // Case 3
  else if (sibling(root, n) != NULL && n->parent->color == 1 && sibling(root, n)->color == 1 && (sibling(root,n)->left == NULL || sibling(root,n)->left->color == 1) && (sibling(root,n)->right == NULL || sibling(root,n)->right->color==1)) {
    sibling(root, n)->color = 0;
    fix(root, n->parent);
  }
  // Case 4
  else if (sibling(root, n) != NULL && n->parent->color == 0 && sibling(root, n)->color == 1 && (sibling(root,n)->left == NULL || sibling(root,n)->left->color == 1) && (sibling(root,n)->right == NULL || sibling(root,n)->right->color==1)) {
    sibling(root, n)->color = 0;
    n->parent->color = 1;
  }

}
