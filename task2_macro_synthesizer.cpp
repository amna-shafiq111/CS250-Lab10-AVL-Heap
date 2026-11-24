#include <iostream>
#include <stack>
#include <string>
using namespace std;

// Tries different operation sequences using backtracking
void findSequences(int current, int target, int budget,
                   int overflowLimit, string operations[],
                   int operationCount, stack<string>& opStack,
                   bool& found) {

    // Target state reached
    if (current == target) {
        cout << "Sequence: ";

        // Copy the stack to print operations in correct order
        stack<string> temp = opStack;
        string result[100];
        int count = 0;

        while (!temp.empty()) {
            result[count++] = temp.top();
            temp.pop();
        }

        // Print the sequence from first operation to last
        for (int i = count - 1; i >= 0; i--) {
            cout << result[i] << " ";
        }

        if (count == 0) {
            cout << "No operations needed";
        }

        cout << endl;
        found = true;
        return;
    }

    // Stop if the operation budget is finished
    if (budget == 0) {
        return;
    }

    // Try each allowed operation
    for (int i = 0; i < operationCount; i++) {

        int next = current;
        bool valid = true;

        if (operations[i] == "+3") {
            next = current + 3;
        }
        else if (operations[i] == "*2") {
            next = current * 2;
        }
        else if (operations[i] == "-5") {
            next = current - 5;
        }
        else if (operations[i] == "/2") {

            // Division is allowed only for even numbers
            if (current % 2 == 0) {
                next = current / 2;
            }
            else {
                valid = false;
            }
        }
        else {
            valid = false;
        }

        // Reject values outside the allowed range
        if (!valid || next < 0 || next > overflowLimit) {
            continue;
        }

        // Push the selected operation before recursion
        opStack.push(operations[i]);

        findSequences(next, target, budget - 1,
                      overflowLimit, operations,
                      operationCount, opStack, found);

        // Undo the operation when backtracking
        opStack.pop();
    }
}

int main() {

    int initialState, targetState;
    int maxOperations, overflowLimit;
    int n;

    cout << "Enter initial state: ";
    cin >> initialState;

    cout << "Enter target state: ";
    cin >> targetState;

    cout << "Enter maximum operation budget: ";
    cin >> maxOperations;

    cout << "Enter overflow limit: ";
    cin >> overflowLimit;

    cout << "Enter number of allowed operations: ";
    cin >> n;

    // Store allowed operations in a normal array
    string* operations = new string[n];

    cout << "Enter allowed operations (+3, *2, -5, /2):\n";

    for (int i = 0; i < n; i++) {
        cin >> operations[i];
    }

    // Stack stores the current operation sequence
    stack<string> opStack;

    bool found = false;

    cout << "\nValid operation sequences:\n";

    if (initialState < 0 || initialState > overflowLimit) {
        cout << "Invalid initial state." << endl;
    }
    else if (maxOperations >= 0 && n > 0) {

        findSequences(initialState, targetState,
                      maxOperations, overflowLimit,
                      operations, n, opStack, found);

        if (!found) {
            cout << "No valid sequence found." << endl;
        }
    }
    else {
        cout << "No valid sequence found." << endl;
    }

    delete[] operations;

    return 0;
}
