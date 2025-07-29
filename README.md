# Expression Simplifier

This project reads algebraic expressions written in postfix notation and converts them into binary expression trees. The expressions can include numbers and variables, and support the operations `+` (addition), `-` (subtraction), and `*` (multiplication). Once the tree is built, the expression is simplified according to standard algebraic rules.

### ExpressionTree Class

The `ExpressionTree` class represents an expression tree and provides functionality to build the tree from a postfix expression and simplify it:

```cpp
class ExpressionTree {
public:
    ExpressionTree();
    ~ExpressionTree();

    bool BuildExpressionTree(const string& postfix);
    void Simplify() { _root = SimplifyTree(_root); };

    friend ostream& operator<<(ostream& os, const ExpressionTree& tree) {
        return os << ToString(tree._root, false);
    }

private:
    static TreeNode* SimplifyTree(TreeNode* tree);
    static string ToString(TreeNode* tree, bool NeedOuterParen);
    static bool IsSameTree(TreeNode* tree1, TreeNode* tree2);

    TreeNode* _root;
};
```

### TreeNode Class

The `TreeNode` class is used to represent individual nodes in the expression tree:

```cpp
enum NodeType {
    Operator,
    NumberOperand,
    VariableOperand
};

class TreeNode {
public:
    TreeNode(NodeType nodeType, string data);
    ~TreeNode();

    NodeType Type() const { return _nodeType; };
    string Data() const { return _data; };
    TreeNode* Left() const { return _left; };
    TreeNode* Right() const { return _right; };

    void SetLeft(TreeNode* left) { _left = left; };
    void SetRight(TreeNode* right) { _right = right; };

    bool IsNumber() const { return _nodeType == NumberOperand; };
    bool IsZero() const { return _nodeType == NumberOperand && _data == "0"; };
    bool IsOne() const { return _nodeType == NumberOperand && _data == "1"; };
    bool SplitNumTimesVariable(int& c, TreeNode** tree) const;

private:
    NodeType _nodeType;
    string _data;
    TreeNode* _left;
    TreeNode* _right;
};
```

### Stack

A templated `Stack` class, implemented using a `VariableArrayList`, is used to build the expression tree from postfix input.

---

## Features

### Expression Tree Construction

* Parses a postfix expression and builds a binary expression tree using a stack-based approach.
* Supports variables, integers, and the operators `+`, `-`, and `*`.

### Expression Simplification

The `SimplifyTree` method recursively simplifies expressions using a set of algebraic rules:

1. **Constant Folding**

   * `2 3 +` → `5`
   * `5 2 -` → `3`
   * `4 3 *` → `12`

2. **Multiplication by Zero**

   * `x 0 *` or `0 x *` → `0`

3. **Adding/Subtracting Zero**

   * `x 0 +` or `0 x +` → `x`
   * `x 0 -` → `x`

4. **Multiplication by One**

   * `x 1 *` or `1 x *` → `x`

5. **Subtraction of Equal Quantities**

   * `x x -` → `0`

6. **Customary Order for Multiplication**

   * `x 3 *` → `3 x *` (places constants on the left)

7. **Distributive Law**

   * `2 x * 3 x * +` → `(2+3) x *` → `5 x *`
   * `2 x * 3 x * -` → `(2-3) x *` → `-1 x *`

These rules allow expressions to be rewritten in simpler and more standard forms. The implementation makes use of utility methods like `IsNumber`, `IsZero`, `IsOne`, `IsSameTree`, and `SplitNumTimesVariable`.

---

## Usage

* Enter a postfix expression with tokens separated by spaces.
* The program displays both the original and the simplified infix expression.
* Supports input from the command line or from a file like `postfix.txt`.
