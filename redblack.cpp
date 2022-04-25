#include <iostream>
#include <cstring>

using namespace std;

struct node {
  int data;
  int color; // 0 for red, 1 for black
  node* parent;
  node* left;
  node* right;
}

node* search(node* n, int k);
  
int main() {
  node* root = NULL;
  char* action[10];
  int input;
  while (true) {
    cout << "What would you like to do? (ADD, FILE, SEARCH, DELETE, PRINT, QUIT) "; cin >> action;
    if (strcmp(action, "ADD") == 0 || strcmp(action, "add") == 0) {
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
    }
    else if (strcmp(action, "QUIT") == 0 || strcmp(action, "quit") == 0) {
      break;
    }
  }
}

node* search(node* n, int k) {
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
