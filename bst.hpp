// Yusuf Pisan pisan@uw.edu
// 15 Jan 2018

// BST class
// Creates a BST to store values
// Uses Node which holds the Data
// Uses templates to store any type of Data
// binarysearchtreee.cpp file is included at the bottom of the .h file
// binarysearchtreee.cpp is part of the template, cannot be compiled separately

#ifndef BST_HPP
#define BST_HPP

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;

template<class T>
class BST {
  // display BST tree in a human-readable format
  friend ostream &operator<<(ostream &Out, const BST &Bst) {
    Bst.printSideways(Out, Bst.Root);
    Out << endl;
    Bst.printVertical(Out, Bst.Root);
    return Out;
  }

private:
  // Node for BST
  // silencing clang-tidy since
  // the Data field cannot be initialized
  // NOLINTNEXTLINE
  struct Node {
    T Data; // NOLINTNEXTLINE
    struct Node *Left{nullptr};
    struct Node *Right{nullptr};
  };

  // refer to data type "struct Node" as Node
  using Node = struct Node;

  // root of the tree
  Node *Root{nullptr};

  // number of nodes in the tree
  int Size;

  // height of a Node, nullptr is 0, Root is 1, static, no access to 'this'
  static int getHeight(const Node *N) {
    if (N == nullptr) {
      return 0;
    }
    return 1 + max(getHeight(N->Left),
                   getHeight(N->Right));
  }

  /**
   * print tree sideways with root on left
                6
            2
                5
        0
                4
            1
                3
   */
  static ostream &printSideways(ostream &Out, const Node *Curr, int Level = 0) {
    const static char SP = ' ';
    const static int ReadabilitySpaces = 4;
    if (!Curr)
      return Out;
    printSideways(Out, Curr->Right, ++Level);
    Out << setfill(SP) << setw(Level * ReadabilitySpaces) << SP;
    Out << Curr->Data << endl;
    printSideways(Out, Curr->Left, Level);
    return Out;
  }

  static ostream &centeredPrint(ostream &Out, int Space,
                                const string &Str, char FillChar = ' ') {
    auto StrL = static_cast<int>(Str.length());
    int Extra = (Space - StrL) / 2;
    if (Extra > 0) {
      Out << setfill(FillChar) << setw(Extra + StrL) << Str;
      Out << setfill(FillChar) << setw(Space - Extra - StrL) << FillChar;
    } else {
      Out << setfill(FillChar) << setw(Space) << Str;
    }
    return Out;
  }

  /**
   * print tree with the root at top
   *
      _____0______
   __1___      __2___
  3     4     5     6
   *
  **/
  static ostream &printTreeLevel(ostream &Out, queue<const Node *> &Q,
                                 int Width, int Depth, int MaxDepth) {
    const static char SP = ' ';
    const static char UND = '_';
    int Nodes = 0;
    // added static cast to silence cland tidy
    int MaxN = static_cast<int>(pow(2, Depth - 1));
    int SpaceForEachItem = Width * pow(2, MaxDepth - 1) / MaxN; // NOLINT
    string
        Bigspace = string(static_cast<uint64_t>(SpaceForEachItem / 4), SP);
    while (Nodes++ < MaxN) {
      const Node *Tp = Q.front();
      Node *TpL = nullptr;
      Node *TpR = nullptr;
      Q.pop();
      string Label = "N";
      if (Tp) {
        stringstream Ss;
        Ss << Tp->Data;
        Label = Ss.str();
        TpL = Tp->Left;
        TpR = Tp->Right;
      }
      char Filler = Depth == MaxDepth ? SP : UND;
      if (Depth == MaxDepth) {
        centeredPrint(Out, SpaceForEachItem, Label, Filler);
      } else {
        Out << Bigspace;
        centeredPrint(Out, SpaceForEachItem / 2, Label, Filler);
        Out << Bigspace;
        Q.push(TpL);
        Q.push(TpR);
      }
    }
    Out << endl;
    return Out;
  }

  // helper function for displaying tree sideways, works recursively
  static ostream &printVertical(ostream &Out, Node *Curr) {
    const static int WIDTH = 6;  // must be even
    if (!Curr)
      return Out << "[__]";
    // figure out the maximum depth which determines how wide the tree is
    int MaxDepth = getHeight(Curr);
    queue<const Node *> Q;
    Q.push(Curr);
    for (int Depth = 1; Depth <= MaxDepth; ++Depth) {
      printTreeLevel(Out, Q, WIDTH, Depth, MaxDepth);
    }
    return Out;
  }

  // copy constructor helper
  // recursively hard copies the other BST
  // to this BST
  void copyHelper(const Node* Other, Node* &N) {
    if (Other != nullptr) {
      N = new Node;
      N->Data = Other->Data;
      N->Right = nullptr;
      N->Left = nullptr;
      copyHelper(Other->Left, N->Left);
      copyHelper(Other->Right, N->Right);
    }
  }

  // helper function to recursively add an item to the tree
  bool addHelper(Node* &AddNode, const T &Item) {
    if (!AddNode) {
      AddNode = new Node;
      AddNode->Data = Item;
      AddNode->Left = nullptr;
      AddNode->Right = nullptr;
      Size++;
      return true;
    }
    if (Item < AddNode->Data) {
      return addHelper(AddNode->Left, Item);
    }
    if (Item > AddNode->Data) {
      return addHelper(AddNode->Right, Item);
    }
    return false;
  }

  // a helper function to find the minimum element
  // of a subtree
  Node* findMin(Node* N) const{
    while (N->Left != nullptr) {
      N = N->Left;
    }
    return N;
  }

  // a helper function to help remove an Item
  // from BST recursively
  bool removeHelper(const T &Item, Node* &N) {
    if (N == nullptr) {
      return false;
    }
    if (Item < N->Data) {
      return removeHelper(Item, N->Left);
    }
    if (Item > N->Data) {
      return removeHelper(Item, N->Right);
    }
    // node with Item found
    // leaf node
    if (N->Left == nullptr && N->Right == nullptr) {
      Size--;
      delete N;
      N = nullptr;
      return true;
    }
    // one right child
    if (N->Left == nullptr) {
      Node* Temp = N->Right;
      Size--;
      delete N;
      N = Temp;
      return true;
    }
    // one left child
    if (N->Right == nullptr) {
      Node* Temp = N->Left;
      Size--;
      delete N;
      N = Temp;
      return true;
    }
    // two children
    Node* Temp = findMin(N->Right);
    N->Data = Temp->Data;
    return removeHelper(Temp->Data, N->Right);
  }

  // a helper function to see if the BST
  // contains a value
  bool containsHelper(const T &Item, Node* Node) const{
    if (Node == nullptr) {
      return false;
    }
    if (Item == Node->Data) {
      return true;
    }
    if (Item < Node->Data) {
      return containsHelper(Item, Node->Left);
    }
    return containsHelper(Item, Node->Right);
  }

  // recursive in order traversal helper function
  // takes a function that takes a single parameter of type T
  void inOrderHelper(void Visit(const T &Item), Node* Node) const {
    if (Node == nullptr) {
      return;
    }
    inOrderHelper(Visit, Node->Left);
    Visit(Node->Data);
    inOrderHelper(Visit, Node->Right);
  }

  // recursive pre order traversal helper function
  // takes a function that takes a single parameter of type T
  void preOrderHelper(void Visit(const T &Item), Node* Node) const {
    if (Node == nullptr) {
      return;
    }
    Visit(Node->Data);
    preOrderHelper(Visit, Node->Left);
    preOrderHelper(Visit, Node->Right);
  }

  // recursive post order traversal helper function
  // takes a function that takes a single parameter of type T
  void postOrderHelper(void Visit(const T &Item), Node* Node) const {
    if (Node == nullptr) {
      return;
    }
    postOrderHelper(Visit, Node->Left);
    postOrderHelper(Visit, Node->Right);
    Visit(Node->Data);
  }

  // helper function to recursively delete all nodes
  // using post-order so that we don't lose pointer
  // to an existing node
  void clearHelper(Node* N) {
    if (N != nullptr) {
      clearHelper(N->Left);
      clearHelper(N->Right);
      if (N->Left) {
        N->Left = nullptr;
      }
      if (N->Right) {
        N->Right = nullptr;
      }
      delete N;
      N = nullptr;
    }
  }

  // a helper function to convert the tree to an array
  // fills the input array with elements of current tree
  // uses in order traversal to get a sorted array
  // Suppressing clang-tidy for the function signature
  // because of use of &I on a static int
  void toSortedArray(T* &Array, Node* Node, int &I) const { // NOLINTNEXTLINE
    if (Node == nullptr) {
      return;
    }
    toSortedArray(Array, Node->Left, I);
    Array[I++] = Node->Data;
    toSortedArray(Array, Node->Right, I);
  }

  // a helper function to read an array and store
  // it in this BST
  Node* toTree(T* &Array, int Start, int End) {
    if (Start > End) {
      return nullptr;
    }
    int MidPoint = (Start + End) / 2;
    Node* N = new Node;
    N->Data = Array[MidPoint];
    if (Start == End) {
      N->Left = nullptr;
      N->Right = nullptr;
      return N;
    }
    N->Left = toTree(Array, Start, MidPoint - 1);
    N->Right = toTree(Array, MidPoint + 1, End);
    return N;
  }

  // a helper function to help determine tree equality
  // recursively
  bool equalityHelper(const Node* First, const Node* Second) const {
    if (First == nullptr && Second == nullptr ) {
      return true;
    }
    if (First == nullptr || Second == nullptr || First->Data != Second->Data) {
      return false;
    }
    return equalityHelper(First->Left, Second->Left)
        && equalityHelper(First->Right, Second->Right);
  }
public:
  // constructor, empty tree
  BST() {
    Size = 0;
  }

  // constructor, tree with root
  explicit BST(const T &RootItem) {
    Root = new Node;
    Root->Data = RootItem;
    Root->Left = nullptr;
    Root->Right = nullptr;
    Size = 1;
  }

  // given an array of length n
  // create a tree to have all items in that array
  // with the minimum height (i.e. rebalance)
  // Assignment specification
  // NOLINTNEXTLINE
  BST(const T Arr[], int N) {
    if (N == 0) {
      Root = nullptr;
      Size = 0;
    }
    if (N >= 1) {
      Size = 0;
      for (int I = 0; I < N; I++) {
        add(Arr[I]);
      }
    }
    rebalance();
  }

  // copy constructor
  BST(const BST<T> &Bst) {
    if (Bst.Root == nullptr) {
      Size = 0; // Silencing the clang-tidy here
      return;
    }
    this->Size = Bst.Size;
    copyHelper(Bst.Root, this->Root);
  }

  // destructor
  virtual ~BST() {
    clear();
  }

  // true if no nodes in BST
  bool isEmpty() const {
    return (Size == 0);
  }

  // 0 if empty, 1 if only root, otherwise
  // height of root is max height of subtrees + 1
  int getHeight() const {
    return getHeight(Root);
  }

  // Number of nodes in BST
  int numberOfNodes() const {
    return Size;
  }

  // add a new item, return true if successful
  bool add(const T &Item) {
    return addHelper(Root, Item);
  }

  // remove item, return true if successful
  bool remove(const T &Item) {
    if (Size == 0) {
      return false;
    }
    if (Size == 1) {
      delete Root;
      Root = nullptr;
      Size = 0;
      return true;
    }
    return removeHelper(Item, Root);
  }

  // true if item is in BST
  bool contains(const T &Item) const {
    return containsHelper(Item, Root);
  }

  // inorder traversal: left-root-right
  // takes a function that takes a single parameter of type T
  void inOrderTraverse(void Visit(const T &Item)) const {
    inOrderHelper(Visit, Root);
  }

  // preorder traversal: root-left-right
  void preOrderTraverse(void Visit(const T &Item)) const {
    preOrderHelper(Visit, Root);
  }

  // postorder traversal: left-right-root
  void postOrderTraverse(void Visit(const T &Item)) const {
    postOrderHelper(Visit, Root);
  }

  // create dynamic array, copy all the items to the array
  // and then read the array to re-create this tree from scratch
  // so that resulting tree is balanced
  void rebalance() {
    static int Index = 0;
    T* TempArray = new T[Size];
    toSortedArray(TempArray, Root, Index);
    int SizeTemp = Size;  // Saving the size for later,
                          // as clear resets the size
    if (Root) {
      clear();
    }
    Size = SizeTemp;      // resetting the size
    Root = toTree(TempArray, 0, Size - 1);
    delete[] TempArray;
    Index = 0;  // resetting the static value
                // for next rebalance, if any
  }

  // delete all nodes in tree
  void clear() {
    if (Root) {
      clearHelper(Root);
      Size = 0;
      Root = nullptr;
    }
  }

  // trees are equal if they have the same structure
  // AND the same item values at all the nodes
  // == operator overload
  bool operator==(const BST<T> &Other) const {
    if (&Other == this) {
      return true;
    }
    if (Other.Size != this->Size) {
      return false;
    }
    return equalityHelper(this->Root, Other.Root);
  }

  // not == to each other
  // checks inequality by converting to array and then
  // comparing the two arrays
  bool operator!=(const BST<T> &Other) const {
    return !(*this == Other);
  }
};

#endif  // BST_HPP
