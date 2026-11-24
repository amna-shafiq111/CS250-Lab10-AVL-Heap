#include <iostream>
using namespace std;

// Node for storing microservice details
struct Node {
    int serviceID;
    int cost;
    Node* left;
    Node* right;

    Node(int id, int c) {
        serviceID = id;
        cost = c;
        left = NULL;
        right = NULL;
    }
};

// Binary Search Tree
class BST {
public:
    Node* root;

    BST() {
        root = NULL;
    }

    // Insert services according to their monthly cost
    Node* insert(Node* node, int id, int cost) {
        if (node == NULL) {
            return new Node(id, cost);
        }

        if (cost < node->cost) {
            node->left = insert(node->left, id, cost);
        }
        else {
            node->right = insert(node->right, id, cost);
        }

        return node;
    }

    void addService(int id, int cost) {
        root = insert(root, id, cost);
    }

    // Delete all nodes when the program finishes
    void deleteTree(Node* node) {
        if (node == NULL) {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    ~BST() {
        deleteTree(root);
    }
};

// ---------------- PART A ----------------

// Prints a matching downward path
void printPath(Node* path[], int size) {
    for (int i = 0; i < size; i++) {
        cout << path[i]->serviceID;

        if (i < size - 1) {
            cout << " -> ";
        }
    }

    cout << endl;
}

// Finds downward paths starting from a particular node
void findDownwardPaths(Node* current, long long target,
                       Node* path[], int depth,
                       long long sum, bool& found) {

    if (current == NULL) {
        return;
    }

    // Add current node to the path
    path[depth] = current;
    sum += current->cost;

    // Check whether the path sum matches the target
    if (sum == target) {
        printPath(path, depth + 1);
        found = true;
    }

    // Continue searching down both sides
    findDownwardPaths(current->left, target, path,
                      depth + 1, sum, found);

    findDownwardPaths(current->right, target, path,
                      depth + 1, sum, found);
}

// Starts a new downward path from every node
void checkAllStartingNodes(Node* root, long long target,
                           Node* path[], bool& found) {

    if (root == NULL) {
        return;
    }

    findDownwardPaths(root, target, path, 0, 0, found);

    checkAllStartingNodes(root->left, target, path, found);
    checkAllStartingNodes(root->right, target, path, found);
}

// ---------------- PART B ----------------

// Stores BST nodes in sorted order using inorder traversal
void storeInorder(Node* root, Node* nodes[], int& index) {

    if (root == NULL) {
        return;
    }

    storeInorder(root->left, nodes, index);

    nodes[index] = root;
    index++;

    storeInorder(root->right, nodes, index);
}

// Prints a matching subset
void printSubset(Node* selected[], int count) {

    cout << "{ ";

    for (int i = 0; i < count; i++) {
        cout << "Service " << selected[i]->serviceID
             << " (" << selected[i]->cost << ") ";

    }

    cout << "}" << endl;
}

// Finds all subsets whose costs equal the target
void findSubsets(Node* nodes[], int n, int index,
                 long long target, long long currentSum,
                 Node* selected[], int count, bool& found) {

    // Target budget reached
    if (currentSum == target) {
        printSubset(selected, count);
        found = true;
        return;
    }

    // All nodes have been checked
    if (index >= n) {
        return;
    }

    // Prune if the current node cannot fit.
    // All following nodes have equal or greater costs.
    if (currentSum + nodes[index]->cost > target) {
        return;
    }

    // Include the current node
    selected[count] = nodes[index];

    findSubsets(nodes, n, index + 1, target,
                currentSum + nodes[index]->cost,
                selected, count + 1, found);

    // Backtrack: exclude the current node
    findSubsets(nodes, n, index + 1, target,
                currentSum, selected, count, found);
}

// ---------------- MAIN FUNCTION ----------------

int main() {

    BST tree;

    int n;

    cout << "Enter number of microservices: ";
    cin >> n;

    if (n < 0 || n > 1000) {
        cout << "Invalid number of microservices." << endl;
        return 0;
    }

    // Insert microservices into the BST
    for (int i = 0; i < n; i++) {

        int id, cost;

        cout << "Enter Service ID and monthly cost: ";
        cin >> id >> cost;

        if (cost <= 0) {
            cout << "Monthly cost must be positive." << endl;
            return 0;
        }

        tree.addService(id, cost);
    }

    long long target;

    cout << "\nEnter target budget: ";
    cin >> target;

    if (target <= 0) {
        cout << "Target budget must be positive." << endl;
        return 0;
    }

    // Arrays store the current paths and selected subsets
    Node* path[1000];
    Node* selected[1000];
    Node* sortedNodes[1000];

    // -------- PART A --------

    cout << "\n========== PART A ==========\n";
    cout << "Downward paths with target sum "
         << target << ":\n";

    bool pathFound = false;

    checkAllStartingNodes(tree.root, target, path, pathFound);

    if (!pathFound) {
        cout << "No matching downward paths found." << endl;
    }

    // -------- PART B --------

    cout << "\n========== PART B ==========\n";
    cout << "Subsets with target sum "
         << target << ":\n";

    // Arrange BST nodes in sorted order
    int index = 0;

    storeInorder(tree.root, sortedNodes, index);

    bool subsetFound = false;

    findSubsets(sortedNodes, n, 0, target, 0,
                selected, 0, subsetFound);

    if (!subsetFound) {
        cout << "No matching subsets found." << endl;
    }

    return 0;
}
