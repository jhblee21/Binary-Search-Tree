//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : James Lee
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : CS-300 Project Two: Course Planner Binary Search Tree
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string name;
    string prereq1;
    string prereq2;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root
    inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // Special Case: if root equal to null
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    else {
        addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseID
    removeNode(root, courseId);
    
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* curNode = root;

    // keep looping downwards until bottom reached or matching courseId found
    while (curNode != nullptr) {
        // if match found, return current course
        if (curNode->course.courseId == courseId) {
            return curNode->course;
        }
        // if course is smaller than current node then traverse left
        else if (curNode->course.courseId > courseId) {
            curNode = curNode->left;
        }
        // else larger so traverse right
        else {
            curNode = curNode->right;
        }
    }
    Course course;
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node - Current node in tree
 * @param course - Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course courseToInsert) {
    // if node in tree is larger then new course, add to left
    if (courseToInsert.courseId < node->course.courseId) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(courseToInsert);
        }
        // else recurse down the left node
        else {
            addNode(node->left, courseToInsert);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(courseToInsert);
        }
        //else
        else {
            // recurse down the left node
            addNode(node->right, courseToInsert);
        }
    }
}

void BinarySearchTree::removeNode(Node* curNode, string courseId) {
    Node* parNode = nullptr;
    // search for node while curNode is not null
    while (curNode != nullptr) {
        // if node is found:
        if (curNode->course.courseId == courseId) {
            // if there are no children nodes (i.e. it is leaf node)
            if ((curNode->left == nullptr) && (curNode->right == nullptr)) {
                // if node is root:
                if (parNode == nullptr) {
                    root = nullptr;
                }
                else if (parNode->left == curNode) {
                    parNode->left = nullptr;
                }
                else {
                    parNode->right = nullptr;
                }
            }
            // else if node has only left child node
            else if (curNode->right == nullptr) {
                // if that node is root
                if (parNode == nullptr) {
                    root = curNode->left;
                }
                else if (parNode->left == curNode) {
                    parNode->left = curNode->left;
                }
                else {
                    parNode->right = curNode->left;
                }
            }
            // else if node has only right child node
            else if (curNode->left == nullptr) {
                if (parNode == nullptr) {
                    root = curNode->right;
                }
                else if (parNode->left == curNode) {
                    parNode->left = curNode->right;
                }
                else {
                    parNode->right = curNode->right;
                }
            }
            // case where there are two children nodes
            else {
                // next node equal to leftmost child node of right subtree
                Node* nextNode = curNode->right;
                while (nextNode->left != nullptr) {
                    nextNode = nextNode->left;
                }
                Node* temp   = new Node;
                temp->course = nextNode->course;
                temp->left   = nextNode->left;
                temp->right  = nextNode->right;
                // recursively remove the next node
                Remove(nextNode->course.courseId);
                // assign curNode data with temp node data
                curNode->course = temp->course;
                curNode->left   = temp->left;
                curNode->right  = temp->right;
            }
            return;
        }
        // else if node not found and courseId key greater than node key, search right child node
        else if (curNode->course.courseId < courseId) {
            parNode = curNode;
            curNode = curNode->right;
        }
        // else, search left child node
        else {
            parNode = curNode;
            curNode = curNode->left;
        }
    }
    return;
}

void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder left
        inOrder(node->left);
        //output courseID, name, and any prerequisites (prereq1, prereq2)
        cout << node->course.courseId << ", ";
        cout << node->course.name << endl;
        //InOrder right
        inOrder(node->right);
    }
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.name     = file[i][1];
            course.prereq1  = file[i][2];
            course.prereq2  = file[i][3];

            // push this course to the end
            bst->Insert(course);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    // Print courseID and course name
    cout << course.courseId << ", " << course.name << endl;
    // If course has any prerequisites, print them as well
    if (!course.prereq1.empty()) {
        cout << "Prerequisites: " << course.prereq1;
        if (!course.prereq2.empty()) {
            cout << ", " << course.prereq2 << endl;
        }
        else {
            cout << endl;
        }
    }
    return;
}

/**
 * Print Menu
 */
void printMenu() {
    cout << "Menu:" << endl;
    cout << "  1. Load Binary Tree Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl << endl;
    cout << "What would you like to do? ";
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath   = argv[1];
        courseKey = "98129";
        break;
    case 3:
        csvPath   = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath   = "course_information.csv";
        courseKey = "98129";
    }


    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << endl;
        printMenu();
        cin >> choice;

        // If user choice is not 1, 2, 3, or 9, display error message then menu.
        while ((choice != 1) && (choice != 2) && (choice != 3) && (choice != 9)) {
            cout << choice << " is not a valid option." << endl << endl;
            printMenu();
            cin >> choice;
        }

        switch (choice) {

        case 1:
            cout << endl;

            // Method call to load the courses
            loadCourses(csvPath, bst);
            dataLoaded = true;   // Data has been loaded into the tree

            break;

        case 2:
            // If data has not been loaded into the tree, display error message.
            if (!dataLoaded) {
                cout << "Error. Please load the file data into the data structure first (Menu option 1)." << endl;
            }
            // If data has been loaded into the tree, proceed.
            else {
                cout << endl;
                cout << "Here is the list of the courses in the Computer Science program in alphanumeric order: " << endl;
                bst->InOrder();
            }
            break;

        case 3:
            // If data has not been loaded into the tree, display error message.
            if (!dataLoaded) {
                cout << "Error. Please load the file data into the data structure first (Menu option 1)." << endl;
            }
            // If data has been loaded into the tree, proceed.
            else {
                cout << endl << "What course do you want to know about? (Enter course ID) ";

                cin >> courseKey;
                // convert user input course key to uppercase string
                for (unsigned int i = 0; i < courseKey.length(); i++) {
                    courseKey[i] = toupper(courseKey[i]);
                }

                // Find course by searching bst with courseKey
                course = bst->Search(courseKey);

                // If course is found, display course information
                if (!course.courseId.empty()) {
                    cout << endl;
                    displayCourse(course);
                }
                // If course is not found, display not found message
                else {
                    cout << courseKey << " not found." << endl;
                }
            }
            break;
        }
    }

    // Case when user enters 9, display message and exit program
    cout << "Thank you for using the course planner!" << endl;
	return 0;
}
