/**
 * Testing BST - Binary Search Tree functions
 *
 * This file has series of tests for BST
 * Each test is independent and uses assert statements
 * Test functions are of the form
 *
 *      test_netidXX()
 *
 * where netid is UW netid and XX is the test number starting from 01
 *
 * Test functions can only use the public functions from BST
 * testBSTAll() is called from main in main.cpp
 * testBSTAll calls all other functions
 * @author Multiple
 * @date ongoing
 */

#include "bst.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Trying to avoid global variables,
 * by creating a singleton class with our visitor functions
 * stringstream SS contains the output from visitor
 */
class TreeVisitor {
public:
  // never create an instance of TreeVisitor object
  // we'll just use the static functions
  TreeVisitor() = delete;

  // insert output to SS rather than cout, so we can test it
  static stringstream SS;

  static string getSS() { return SS.str(); }

  static void resetSS() { SS.str(string()); }
  // instead of cout, insert item into a string stream
  static void visitor(const string &Item) { SS << Item; }

  // instead of cout, insert item into a string stream
  static void visitor(const int &Item) { SS << Item; }
};

// initialize the static variable
//  warning: initialization of 'SS' with static storage duration
//  may throw an exception that cannot be caught [cert-err58-cpp]
//  Not sure how to do it without making code harder to read
//  NOLINTNEXTLINE
stringstream TreeVisitor::SS;

/**
 * Test functions by Yusuf Pisan
 */

// Testing ==
void testPisan01() {
  cout << "Starting testPisan01" << endl;
  cout << "* Testing == and !=" << endl;
  BST<string> B1;
  BST<string> B2;
  BST<string> B3;
  // == and != for empty trees
  assert(B1 == B2 && (!(B1 != B2)));
  B1.add("c");
  B2.add("c");
  B3.add("b");

  // == and !- for 1-Node trees B1, B2, B3
  assert(B1 == B2 && (!(B1 != B2)));
  assert(B1 != B3 && (!(B1 == B3)));
  cout << "Ending testPisan01" << endl;
}

// Testing == in detail
void testPisan02() {
  cout << "Starting testPisan02" << endl;
  cout << "* Testing == and != with more detail" << endl;
  BST<string> B1;
  BST<string> B2;
  BST<string> B3;
  for (auto &S : vector<string>{"c", "a", "f", "g", "x"})
    B1.add(S);

  for (auto &S : vector<string>{"c", "f", "a", "g", "x"})
    B2.add(S);

  B3.add("b");

  // == for 5-Node trees B1, B2
  assert(B1 == B2 && (!(B1 != B2)));

  BST<string> B4(B3);

  // copy constructor for 1-Node trees B3, B4
  assert(B3 == B4 && (!(B3 != B4)));

  BST<string> B5(B1);

  // copy constructor for 5-Node trees B1, B5
  assert(B1 == B5 && (!(B5 != B1)));

  BST<string> B7("b");
  // 1-param constructor for 1-Node trees B3, B7
  assert(B3 == B7 && (!(B3 != B7)));

  cout << "Ending testPisan02" << endl;
}

// Testing traversal
void testPisan03() {
  cout << "Starting testPisan03" << endl;
  cout << "* Testing traversal" << endl;
  BST<string> B1;
  BST<string> B2;
  BST<string> B3;
  for (auto &S : vector<string>{"c", "a", "f", "g", "x"})
    B1.add(S);

  for (auto &S : vector<string>{"c", "f", "a", "g", "x"})
    B2.add(S);

  B3.add("b");

  TreeVisitor::resetSS();
  B1.inOrderTraverse(TreeVisitor::visitor);
  string Result = "acfgx";
  assert(TreeVisitor::getSS() == Result);

  TreeVisitor::resetSS();
  B1.preOrderTraverse(TreeVisitor::visitor);
  Result = "cafgx";
  assert(TreeVisitor::getSS() == Result);

  TreeVisitor::resetSS();
  B1.postOrderTraverse(TreeVisitor::visitor);
  Result = "axgfc";
  assert(TreeVisitor::getSS() == Result);

  cout << "Visual check B1:" << endl;
  cout << B1 << endl;
  cout << "Ending testPisan03" << endl;
}

/**
 * Test functions by Sample Sample
 */

// Testing Array Constructor and clear
void testArshiaj01() {
  cout << "Starting testArshiaj01" << endl;
  cout << "Testing the Array constructor of tree" << endl;
  cout << "Filling the tree with an array of : ";
  cout << "{10, 1, 2, 3, 9, 5, 4, 8, 6, 7}" << endl;
  // filling an array with 10 numbers in random order
  int Arr1[] = {10, 1, 2, 3, 9, 5, 4, 8, 6, 7};
  // array constructor
  BST<int> B1(Arr1, 10);
  TreeVisitor::resetSS();
  B1.inOrderTraverse(TreeVisitor::visitor);
  string Result = "12345678910";
  assert(TreeVisitor::getSS() == Result);
  // Visual test for a balanced tree
  cout << B1 << endl;
  // testing array size
  assert(B1.numberOfNodes() == 10);
  assert(!B1.isEmpty());
  // testing height of balanced tree
  // which is floor of 1+log2Size == 4 in this case
  assert(B1.getHeight() == 4);
  // testing clear
  cout << "Clearing the tree and printing" << endl;
  B1.clear();
  cout << B1 << endl;
  assert(B1.numberOfNodes() == 0);
  cout << "Ending testArshiaj01" << endl;
}

// testing operations on an empty tree
void testArshiaj02() {
  cout << "Starting testArshiaj02" << endl;
  cout << "Removing from, getting count and height of empty tree" << endl;
  BST<int> B1;
  assert(!B1.remove(1));
  assert(B1.numberOfNodes() == 0);
  assert(B1.getHeight() == 0);
  assert(B1.isEmpty());
  cout << "printing empty tree" << endl;
  cout << B1 << endl;
  cout << "Ending testArshiaj02" << endl;
}

// testing Remove and all its possible problems
void testArshiaj03() {
  cout << "Starting testArshiaj03" << endl;
  cout << "Testing remove" << endl;

  BST<string> B1;
  for (auto &S : vector<string>{"a", "b", "c", "e", "f", "g"}) {
    B1.add(S);
  }
  cout << "Balancing" << endl;
  B1.rebalance();
  cout << B1 << endl;
  cout << "Removing Leaf Node : g" << endl;
  B1.remove("g");
  cout << B1 << endl;
  cout << "Removing a Node with one child : a" << endl;
  B1.remove("a");
  cout << B1 << endl;
  cout << "Removing a Node with two children : c" << endl;
  B1.remove("c");
  cout << B1 << endl;
  // checking contains
  assert(B1.contains("b"));
  assert(!B1.contains("a"));
  assert(B1.numberOfNodes() == 3);
  cout << "clearing and printing" << endl;
  B1.clear();
  cout << B1 << endl;
  cout << "Ending testArshiaj03" << endl;
}

// testing add, duplicates etc.
void testArshiaj04() {
  cout << "Starting testArshiaj04" << endl;
  BST<int> B1;
  for (int I = 1; I <= 20; I++) {
    B1.add(I);
  }
  cout << "Height of unbalanced tree of entering 20 ";
  cout << "numbers from 1 to 20 is : " << B1.getHeight() << endl;
  cout << "Height after rebalancing : ";
  B1.rebalance();
  cout << B1.getHeight() << endl;
  cout << B1 << endl;
  cout << "Ending testArshiaj04" << endl;
}

// Calling all test functions
void testBSTAll() {
  testPisan01();
  testPisan02();
  testPisan03();
  testArshiaj01();
  testArshiaj02();
  testArshiaj03();
  testArshiaj04();
}