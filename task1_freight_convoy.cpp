#include <iostream>
#include <queue>
#include <string>
using namespace std;

// Node for storing container information
struct Node {
    string id;
    int weight;
    int truckNo;
    Node* next;

    Node(string i, int w) {
        id = i;
        weight = w;
        truckNo = -1;
        next = NULL;
    }
};

// Singly Linked List for containers
class ContainerList {
public:
    Node* head;

    ContainerList() {
        head = NULL;
    }

    void insert(string id, int weight) {
        Node* newNode = new Node(id, weight);

        if (head == NULL) {
            head = newNode;
            return;
        }

        Node* temp = head;

        while (temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = newNode;
    }

    // Displays containers assigned to each truck
    void display(int truckCount) {
        for (int i = 0; i < truckCount; i++) {
            cout << "Truck " << i + 1 << ": ";

            Node* temp = head;
            bool found = false;

            while (temp != NULL) {
                if (temp->truckNo == i) {
                    if (found) {
                        cout << ", ";
                    }

                    cout << temp->id;
                    found = true;
                }

                temp = temp->next;
            }

            if (!found) {
                cout << "Empty";
            }

            cout << endl;
        }
    }
};

// Backtracking function to assign containers to trucks
bool assignContainers(Node* current, int capacities[],
                      int truckIndex, int remainingCapacity,
                      int truckCount, long long remainingWeight) {

    // All containers have been assigned
    if (current == NULL) {
        return true;
    }

    // No trucks are left
    if (truckIndex >= truckCount) {
        return false;
    }

    // Calculate total remaining truck capacity
    long long totalCapacity = remainingCapacity;

    for (int i = truckIndex + 1; i < truckCount; i++) {
        totalCapacity += capacities[i];
    }

    // Prune if remaining containers cannot fit
    if (remainingWeight > totalCapacity) {
        return false;
    }

    // Try loading the current container into this truck
    if (current->weight <= remainingCapacity) {

        current->truckNo = truckIndex;

        // Move to the next container
        if (assignContainers(current->next, capacities,
                             truckIndex, remainingCapacity - current->weight,
                             truckCount, remainingWeight - current->weight)) {
            return true;
        }

        // Undo the assignment if this choice fails
        current->truckNo = -1;
    }

    // Try the next truck
    if (truckIndex + 1 < truckCount) {
        return assignContainers(current, capacities,
                                truckIndex + 1,
                                capacities[truckIndex + 1],
                                truckCount, remainingWeight);
    }

    return false;
}

int main() {
    ContainerList containers;
    queue<int> trucks;

    int n, m;

    cout << "Enter number of containers: ";
    cin >> n;

    // Take container details
    for (int i = 0; i < n; i++) {
        string id;
        int weight;

        cout << "Enter container ID and weight: ";
        cin >> id >> weight;

        containers.insert(id, weight);
    }

    cout << "\nEnter number of trucks: ";
    cin >> m;

    // Store truck capacities in a queue
    for (int i = 0; i < m; i++) {
        int capacity;

        cout << "Enter capacity of truck " << i + 1 << ": ";
        cin >> capacity;

        trucks.push(capacity);
    }

    // Copy capacities into an array for backtracking
    int* capacities = new int[m];

    for (int i = 0; i < m; i++) {
        capacities[i] = trucks.front();
        trucks.pop();
    }

    // Calculate total weight of all containers
    long long totalWeight = 0;

    Node* temp = containers.head;

    while (temp != NULL) {
        totalWeight += temp->weight;
        temp = temp->next;
    }

    // Start backtracking
    if (m > 0 && assignContainers(containers.head, capacities,
                                  0, capacities[0], m, totalWeight)) {

        cout << "\nContainers loaded successfully!\n";

        containers.display(m);
    }
    else {
        cout << "\nInsufficient truck capacity." << endl;
    }

    delete[] capacities;

    return 0;
}
