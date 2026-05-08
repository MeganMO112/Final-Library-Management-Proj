#include <iostream>
#include <ctime>
#include <time.h>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;
using namespace std::chrono;

const int MAX_TRANSACTIONS = 10000;

// ---------------- LIBRARY FUNCTIONS ----------------
//book creation
struct Book {
    int bookID;
    string author;
    string title;
    int quantity;
};

//transaction creation
struct Transaction {
    int studentID;
    int bookID;
    string bookTitle;
    string action;
    string date;
};

//time keeping for transaction
string getCurrentDate() {
    time_t now = time(0);
    string dt = ctime(&now);

    if (!dt.empty() && dt.back() == '\n') {
        dt.pop_back();
    }

    return dt;
}

void addBooks(Book library[], int &count) {
    cout << "Enter Book ID: ";
    cin >> library[count].bookID;
    cin.ignore();

    cout << "Enter Book Title: ";
    getline(cin, library[count].title);

    cout << "Enter Author Name: ";
    getline(cin, library[count].author);

    cout << "Enter Quantity: ";
    cin >> library[count].quantity;

    cout << "Book Added Successfully!\n";
    count++;
}

void displayBooks(Book library[], int count) {
    if (count == 0) {
        cout << "No books available.\n";
        return;
    }
    cout << "\nAvailable Books:\n";
    for (int i = 0; i < count; i++) {
        cout << "Book ID   : " << library[i].bookID << endl;
        cout << "Title     : " << library[i].title << endl;
        cout << "Author    : " << library[i].author << endl;
        cout << "Quantity    : " << library[i].quantity << endl;
    }
}

void deleteBook(Book library[], int &count) {
    int id;
    cout << "Enter Book ID to delete: ";
    cin >> id;

    for (int i = 0; i < count; i++) {
        if (library[i].bookID == id) {
            for (int j = i; j < count - 1; j++) {
                library[j] = library[j + 1];
            }

            count--;
            cout << "Book Deleted Successfully!\n";
            return;
        }
    }

    cout << "Book not found.\n";
}

// ---------------- BOOK ID FILE FUNCTIONS ----------------

//uses BUBBLE SORT (O(n^2))
void GenerateBookIds(int arr[], int size) { 
for (int i = 0; i < size; i++) {
    arr[i] = rand() % 900000 + 100000; 
} 
ofstream MyFile("BookIDs.txt"); 
auto start = high_resolution_clock::now(); 
// Bubble sort 
for (int i = 0; i < size - 1; i++) 
{ 
    for (int j = 0; j < size - i - 1; j++) { 
        if (arr[j] > arr[j + 1]) { 
            swap(arr[j], arr[j + 1]); 
        } 
    } 
} // add sorted nums to file 
for (int i = 0; i < size; i++) { 
    MyFile << arr[i] << "\n"; 
}


MyFile.close(); 
auto stop = high_resolution_clock::now(); 
auto duration = duration_cast<microseconds>(stop - start); 
    
cout << "Book IDs generated and saved to BookIDs.txt.\n"; 
cout << "Time taken by function: " << duration.count() << " microseconds" << endl; 

}


// uses binary search (O(log n)) as it is already sorted from bubble sort above ^
void SearchFromFile() {
    ifstream inFile("BookIDs.txt");

    const int MAX_SIZE = 10000;
    int arr[MAX_SIZE];
    int size = 0;
    int num;

    while (inFile >> num && size < MAX_SIZE) {
        arr[size++] = num;
    }

    inFile.close();

    if (size == 0) {
        cout << "File is empty.\n";
        return;
    }

    int target;
    cout << "Enter ID to search: ";
    cin >> target;

    auto start = high_resolution_clock::now();

    //binary search 

    int left = 0;
    int right = size - 1;
    bool found = false;

    
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == target) {
            cout << "Found at index: " << mid << endl;
            found = true;
            break;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    if (!found) {
        cout << "Not found\n";
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Search time: " << duration.count() << " microseconds\n";
}


//Binary search tree setup (generating student IDs) > uses O(N) time complexity 
struct Node {
    int data;
    Node* left;
    Node* right;
};

Node* createNode(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

/*void inorderTraversal(Node* root) {
    if (root != nullptr) {
        inorderTraversal(root->left);
        cout << root->data << " ";
        inorderTraversal(root->right);
    }
}*/

Node* insertNode(Node* root, int id) {
    if (root == nullptr) {
        return createNode(id);
    }

    if (id < root->data) {
        root->left = insertNode(root->left, id);
    } else if (id > root->data) {
        root->right = insertNode(root->right, id);
    }

    return root;
}

Node* searchNode(Node* root, int target) {
    if (root == nullptr || root->data == target) {
        return root;
    }

    if (root->data < target) {
        return searchNode(root->right, target);
    }

    return searchNode(root->left, target);
}

Node* minValueNode(Node* node) {
    Node* current = node;

    while (current && current->left != nullptr) {
        current = current->left;
    }

    return current;
}

Node* GenerateStudentIds(int size) {
    ofstream outFile("StudentIDs.txt");

    if (!outFile) {
        cout << "Error opening file!\n";
        return nullptr;
    }

    Node* root = nullptr;

     auto start = high_resolution_clock::now();

    for (int i = 0; i < size; i++) {
        int id = rand() % 900000 + 100000;

        outFile << id << "\n";
        root = insertNode(root, id);
    }


    outFile.close();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Student IDs generated and saved to StudentIDs.txt.\n";
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;


    return root;
}

void SearchBST(Node* root) {
    int target;
    cout << "\nEnter ID to search: ";
    cin >> target;

    auto start = high_resolution_clock::now();

    Node* result = searchNode(root, target);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    if (result != nullptr) {
        cout << "SID found: " << result->data << endl;
    } else {
        cout << "SID not found.\n";
    }

    cout << "Search time: " << duration.count() << " microseconds\n";
}



//generated (experiment)
void addTransaction(Transaction transactions[], int &transactionCount,
                    int studentID, int bookID, string bookTitle, string action) {

    if (transactionCount >= MAX_TRANSACTIONS) {
        cout << "Transaction storage full.\n";
        return;
    }

    Transaction& t = transactions[transactionCount];

    t.studentID = studentID;
    t.bookID = bookID;
    t.bookTitle = bookTitle;
    t.action = action;
    t.date = getCurrentDate();

    // Save directly to file
    ofstream outFile("Transactions.txt", ios::app);

    if (!outFile) {
        cout << "Error opening Transactions.txt.\n";
        return;
    }

    outFile << "SID:" << t.studentID << ","
            << "ISBN:" << t.bookID << ","
            << t.bookTitle << ","
            << t.action << ","
            << t.date << "\n";

    transactionCount++;
}

//manual - student 
void addManualTransaction(Transaction transactions[], int &transactionCount,
                          int studentID, int bookID, string bookTitle, string action) {

    if (transactionCount >= MAX_TRANSACTIONS) {
        cout << "Transaction storage full.\n";
        return;
    }

    Transaction& t = transactions[transactionCount];

    t.studentID = studentID;
    t.bookID = bookID;
    t.bookTitle = bookTitle;
    t.action = action;
    t.date = getCurrentDate();

    // Save directly to manual file
    ofstream outFile("ManualTransactions.txt", ios::app);

    if (!outFile) {
        cout << "Error opening ManualTransactions.txt.\n";
        return;
    }

    outFile << "SID:" << t.studentID << ","
            << "ISBN:" << t.bookID << ","
            << t.bookTitle << ","
            << t.action << ","
            << t.date << "\n";

    transactionCount++;
}

void generateTransactionsFromFiles(Transaction transactions[],
                                   int &transactionCount,
                                   int amount) {
    ifstream studentFile("StudentIDs.txt");
    ifstream bookFile("BookIDs.txt");

    if (!studentFile) {
        cout << "StudentIDs.txt not found. Generate Student IDs first.\n";
        return;
    }

    if (!bookFile) {
        cout << "BookIDs.txt not found. Generate Book IDs first.\n";
        return;
    }

    const int MAX_SIZE = 10000;
    int studentIDs[MAX_SIZE];
    int bookIDs[MAX_SIZE];

    int studentCount = 0;
    int bookCount = 0;
    int id;

    while (studentFile >> id && studentCount < MAX_SIZE) {
        studentIDs[studentCount++] = id;
    }

    while (bookFile >> id && bookCount < MAX_SIZE) {
        bookIDs[bookCount++] = id;
    }

    studentFile.close();
    bookFile.close();

    if (studentCount == 0 || bookCount == 0) {
        cout << "StudentIDs.txt or BookIDs.txt is empty.\n";
        return;
    }

    string actions[] = {"Borrow", "Return"};

    auto start = high_resolution_clock::now();

    int generated = 0;

    for (int i = 0; i < amount && transactionCount < MAX_TRANSACTIONS; i++) {
        int randomStudentIndex = rand() % studentCount;
        int randomBookIndex = rand() % bookCount;
        int randomActionIndex = rand() % 2;

        int studentID = studentIDs[randomStudentIndex];
        int bookID = bookIDs[randomBookIndex];
        string bookTitle = "Book_" + to_string(bookID);
        string action = actions[randomActionIndex];

        addTransaction(
            transactions,
            transactionCount,
            studentID,
            bookID,
            bookTitle,
            action
        );

        generated++;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << generated << " transactions generated from StudentIDs.txt and BookIDs.txt.\n";
    cout << "Generation time: " << duration.count() << " microseconds\n";
}

void printTransactionHeader() {
    cout << "\n================================================================================\n";
    cout << left << setw(14) << "Student ID"
         << setw(14) << "ISBN"
         << setw(22) << "Title"
         << setw(12) << "Action"
         << "Date\n";
    cout << "================================================================================\n";
}

void printTransactionRow(string line) {
    stringstream ss(line);

    string sid, isbn, title, action, date;

    getline(ss, sid, ',');
    getline(ss, isbn, ',');
    getline(ss, title, ',');
    getline(ss, action, ',');
    getline(ss, date);

    if (sid.find("SID:") != string::npos) {
        sid = sid.substr(sid.find(":") + 1);
    }

    if (isbn.find("ISBN:") != string::npos) {
        isbn = isbn.substr(isbn.find(":") + 1);
    }

    cout << left << setw(14) << sid
         << setw(14) << isbn
         << setw(22) << title.substr(0, 21)
         << setw(12) << action
         << date.substr(0, 24) << endl;
}

void displayFirst100Transactions(string filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cout << filename << " not found. Generate or create transactions first.\n";
        return;
    }

    string line;
    int count = 0;

    auto start = high_resolution_clock::now();

    printTransactionHeader();

    while (getline(inFile, line) && count < 100) {
        printTransactionRow(line);
        count++;
    }

    cout << "================================================================================\n";

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Display time: " << duration.count() << " microseconds\n";

    inFile.close();
}

void displayAllTransactions(string filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cout << filename << " not found. Generate or create transactions first.\n";
        return;
    }

    string line;

    auto start = high_resolution_clock::now();

    printTransactionHeader();

    while (getline(inFile, line)) {
        printTransactionRow(line);
    }

    cout << "================================================================================\n";

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Display time: " << duration.count() << " microseconds\n";

    inFile.close();
}

//Linear search - just searching from randomly generated transactions from bookid and studentid files 
void searchTransactionsFromFile(string filename) {
    ifstream inFile(filename);

    if (!inFile) {
        cout << filename << " not found. Generate or create transactions first.\n";
        return;
    }

    int targetID;
    cout << "Enter Student ID to search transactions: ";
    cin >> targetID;

    string line;
    string studentIDText;
    bool found = false;

    auto start = high_resolution_clock::now();

    printTransactionHeader();

    while (getline(inFile, line)) {
        stringstream ss(line);

        getline(ss, studentIDText, ',');

        if (studentIDText.find("SID:") != string::npos) {
            studentIDText = studentIDText.substr(studentIDText.find(":") + 1);
        }

        if (!studentIDText.empty() && stoi(studentIDText) == targetID) {
            printTransactionRow(line);
            found = true;
        }
    }

    cout << "================================================================================\n";

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    if (!found) {
        cout << "No transactions found for that student.\n";
    }

    cout << "Search time: " << duration.count() << " microseconds\n";

    inFile.close();
}

string findBookTitle(Book library[], int count, int bookID) {
    for (int i = 0; i < count; i++) {
        if (library[i].bookID == bookID) {
            return library[i].title;
        }
    }

    return "Book_" + to_string(bookID);
}

int findBookIndex(Book library[], int count, int bookID) {
    for (int i = 0; i < count; i++) {
        if (library[i].bookID == bookID) {
            return i;
        }
    }

    return -1;
}

void manualTransaction(Book library[], int bookCount,
                       Transaction transactions[], int &transactionCount) {
    int studentID;
    int bookID;
    int actionChoice;
    string action;

    cout << "Enter Student ID: ";
    cin >> studentID;

    cout << "Enter Book ID: ";
    cin >> bookID;

    int bookIndex = findBookIndex(library, bookCount, bookID);

    if (bookIndex == -1) {
        cout << "Book not found in library inventory.\n";
        return;
    }

    cout << "1. Borrow\n";
    cout << "2. Return\n";
    cout << "Enter action: ";
    cin >> actionChoice;

    if (actionChoice == 1) {
        action = "Borrow";

        if (library[bookIndex].quantity <= 0) {
            cout << "Book is currently out of stock.\n";
            return;
        }

        library[bookIndex].quantity--;
    } else if (actionChoice == 2) {
        action = "Return";
        library[bookIndex].quantity++;
    } else {
        cout << "Invalid action.\n";
        return;
    }

    string bookTitle = library[bookIndex].title;

    addManualTransaction(
        transactions,
        transactionCount,
        studentID,
        bookID,
        bookTitle,
        action
    );

    cout << "Manual transaction saved to ManualTransactions.txt.\n";
}

// ---------------- EXPERIMENT MENU ----------------

void experimentMenu(Transaction transactions[],
                    int &transactionCount,
                    Node* &studentRoot) {

    int choice;

    do {
        cout << "\n--- EXPERIMENT MENU ---\n";
        cout << "1. Generate Student IDs \n";
        cout << "2. Search Student ID \n";
        cout << "3. Generate Book IDs \n";
        cout << "4. Search Book ID \n";
        cout << "5. Generate Transactions\n";
        cout << "6. Display First 100 Generated Transactions\n";
        cout << "7. Search Generated Transactions\n";
        cout << "8. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                int size;
                cout << "Enter number of student IDs: ";
                cin >> size;
                studentRoot = GenerateStudentIds(size);
                break;

            case 2:
                if (studentRoot == nullptr)
                    cout << "Generate Student IDs first.\n";
                else
                    SearchBST(studentRoot);
                break;

            case 3: {
                int size;
                cout << "Enter number of book IDs: ";
                cin >> size;

                int* arr = new int[size];
                GenerateBookIds(arr, size);
                delete[] arr;
                break;
            }

            case 4:
                SearchFromFile();
                break;

            case 5: {
                int amount;
                cout << "Enter number of transactions: ";
                cin >> amount;

                generateTransactionsFromFiles(
                    transactions,
                    transactionCount,
                    amount
                );
                break;
            }

            case 6:
                displayFirst100Transactions("Transactions.txt");
                break;

            case 7:
                searchTransactionsFromFile("Transactions.txt");
                break;

            case 8:
                cout << "Returning to Admin Menu...\n";
                break; 
        }
    } while (choice != 8);
}

// ---------------- MENUS ----------------

void adminMenu(Book library[], int &bookCount,
               Transaction transactions[], int &transactionCount,
               Node* &studentRoot) {
    int choice;

    do {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. Add Book\n";
        cout << "2. Display Books\n";
        cout << "3. Delete Book\n";
        cout << "4. Display All Student Transactions\n";
        cout << "5. Search Student Transactions\n";
        cout << "6. Experiment\n";
        cout << "7. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addBooks(library, bookCount);
                break;

            case 2:
                displayBooks(library, bookCount);
                break;

            case 3:
                deleteBook(library, bookCount);
                break;

            case 4:
                displayAllTransactions("ManualTransactions.txt");
                break;

            case 5:
                searchTransactionsFromFile("ManualTransactions.txt");
                break;

            case 6:
                experimentMenu(transactions, transactionCount, studentRoot);
                break;

            case 7:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 7);
}

void studentMenu(Book library[], int &bookCount,
                 Transaction transactions[], int &transactionCount) {
    int choice;

    do {
        cout << "\n--- STUDENT MENU ---\n";
        cout << "1. Enter Borrow/Return Transaction\n";
        cout << "2. Display Books\n";
        cout << "3. Display Student Transactions\n";
        cout << "4. Search Student Transactions\n";
        cout << "5. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manualTransaction(library, bookCount, transactions, transactionCount);
                break;

            case 2:
                displayBooks(library, bookCount);
                break;

            case 3:
                displayFirst100Transactions("ManualTransactions.txt");
                break;

            case 4:
                searchTransactionsFromFile("ManualTransactions.txt");
                break;

            case 5:
                cout << "Returning to main menu.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 5);
}

// ---------------- MAIN ----------------

int main() {
    srand(time(0));

    const int MAX = 1000;
    Book* library = new Book[MAX];
    int bookCount = 0;

    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount = 0;

    Node* studentRoot = nullptr;

    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Admin\n";
        cout << "2. Student\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                adminMenu(library, bookCount, transactions, transactionCount, studentRoot);
                break;

            case 2:
                studentMenu(library, bookCount, transactions, transactionCount);
                break;

            case 3:
                cout << "Exiting program...\n";
                delete[] library;
                return 0;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 3);

    delete[] library;
    return 0;
}
