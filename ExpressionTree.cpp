//
// Implements the ExpressionTree Class
// Author: Max Benson
// Date: 10/27/2021
//

#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

#include "Stack.h"
#include "ExpressionTree.h"

// Token testing routines
bool IsNumber(string token);
bool IsVariable(string token);
bool IsOperator(string token);

/**
 * Default constructor
 * Creates an "null tree"
 */
ExpressionTree::ExpressionTree() {
    _root = nullptr;
}

/**
 * Destructor
 * Frees the dynamic memory allocated for the tree
 */
ExpressionTree::~ExpressionTree() {
    delete _root;
}

/**
 * Build an expression tree from its postfix representation
 * In case of error the stack is cleaned up.  Because it contains
 * pointers to TreeNodes, if any are left on the stack they must be
 * explicitly deleted
 * @param postfix string representation of tree
 * @return true if postfix valid and tree was built, false otherwise
 */
bool ExpressionTree::BuildExpressionTree(const string& postfix) {
    stringstream ss(postfix);
    string token;
    Stack<TreeNode*> expTree;
    TreeNode* nodeToDelete;

    while(ss >> token) {
        if (IsNumber(token) || IsVariable(token)) {
            if (IsNumber(token)) { expTree.Push(new TreeNode(::NumberOperand, token)); }
            else { expTree.Push(new TreeNode(::VariableOperand, token)); }
        }
        else if (IsOperator(token)) {
            if (expTree.Size() < 2) {
                cout << "ERROR: operator found with no operands" << endl;
                for (int i = expTree.Size(); i > 0; i--) {
                    nodeToDelete = expTree.Pop();
                    delete nodeToDelete;
                }
                return false;
            }
            TreeNode* expression = new TreeNode(Operator, token);
            expression->SetRight(expTree.Pop());
            expression->SetLeft(expTree.Pop());
            expTree.Push(expression);
        }
        else {
            cout << "ERROR: input " << token << " not valid" << endl;
            for (int i = expTree.Size(); i > 0; i--) {
                nodeToDelete = expTree.Pop();
                delete nodeToDelete;
            }
            return false;
        }
    }
    if (expTree.Size() != 1) {
        cout << "ERROR: postfix expression is not valid" << endl;
        for (int i = expTree.Size(); i > 0; i--) {
            nodeToDelete = expTree.Pop();
            delete nodeToDelete;
        }
        return false;
    }
    _root = expTree.Pop();
    return true;
}

/**
 * Recursively simplify an expression stored in an expression tree.  THe following simplications are performed
 * - Addition, multiplication, and subtraction of constants is performed reducing the subtree to a leaf containing a number
 * - 0 + exp, exp + 0, exp - 0  will be reduced to exp, in general exp will a tree
 * - 1 * exp, exp * 1  will be reduced to exp, in general exp will a tree
 * - 0 * exp, exp * 0  will be reduce to a leaf containing 0
 * - exp - exp will be reduce to a leaf containing 0
 * - exp * number will be changed to number * exp
 * - (c1 * exp) + (c2 * exp) where c1, c2 are numbers  will be changed to (c1+c2) * exp
 * - (c1 * exp) - (c2 * exp) where c1, c2 are numbers will be changed to (c1-c2) * exp
 * pointers to TreeNodes, if any are left on the stack they must be
 * explicitly deleted
 * @param postfix string representation of tree
 * @return true if postfix valid and tree was built, false otherwise
 */
TreeNode* ExpressionTree::SimplifyTree(TreeNode* tree) {
    if (IsOperator(tree->Left()->Data())) {
        tree->SetLeft(SimplifyTree(tree->Left()));
    }
    if (IsOperator(tree->Right()->Data())) {
        tree->SetRight(SimplifyTree(tree->Right()));
    }
    if (IsNumber(tree->Left()->Data()) && IsNumber(tree->Right()->Data())) {
        int left = stoi(tree->Left()->Data());
        int right = stoi(tree->Right()->Data());
        int result;
        if (tree->Data() == "+") {
            result = left + right;
        } else if (tree->Data() == "*") {
            result = left * right;
        }
        else {
            result = left - right;
        }
        delete tree;
        return new TreeNode(NumberOperand, std::to_string(result));
    }
     else if (tree->Data() == "*") {
        if (tree->Left()->IsZero() || tree->Right()->IsZero()) {
            delete tree;
            return new TreeNode(NumberOperand, "0");
        }
        else if (tree->Left()->IsOne()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Right()->Data());
            delete tree;
            return tmp;
        }
        else if (tree->Right()->IsOne()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Left()->Data());
            delete tree;
            return tmp;
        }
        else if (tree->Left()->IsNumber()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Left()->Data() + tree->Right()->Data());
            delete tree;
            return tmp;
        }
        else if (tree->Right()->IsNumber()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Right()->Data() + tree->Left()->Data());
            delete tree;
            return tmp;
        }
    }
    else if (tree->Data() == "+") {
        if (tree->Left()->Data() == tree->Right()->Data()) {
            TreeNode* tmp = new TreeNode(VariableOperand, "2" + tree->Left()->Data());
            delete tree;
            return tmp;
        }
        else if (tree->Left()->IsZero()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Right()->Data());
            delete tree;
            return tmp;
        } else if (tree->Right()->IsZero()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Left()->Data());
            delete tree;
            return tmp;
        }
    }
    else if (tree->Data() == "-") {
        if (tree->Left()->Data() == tree->Right()->Data()) {
            delete tree;
            return new TreeNode(NumberOperand, "0");
        }
        else if (tree->Left()->IsZero()) {
            TreeNode* tmp = new TreeNode(VariableOperand, "-" + tree->Right()->Data());
            delete tree;
            return tmp;
        } else if (tree->Right()->IsZero()) {
            TreeNode* tmp = new TreeNode(VariableOperand, tree->Left()->Data());
            delete tree;
            return tmp;
        }
    }


    return tree;
}

/**
 * Determine whether two tree structures represent the same expression
 * @param tree1 first tree structure
 * @param tree2 second tree structure
 * @return true if same, false otherwise
 */
bool ExpressionTree::IsSameTree(TreeNode* tree1, TreeNode* tree2) const {
    assert(false);
    return false;
}

/**
 * Produce an infix representation of the tree structure
 * @param tree
 * @param fNeedOuterParen - caller will generatlly pass false to eliminate outer set of paraentheses, recursive calls pass true
 * @return string representation
 */
string ExpressionTree::ToString(TreeNode* tree, bool fNeedOuterParen) const {
    string s;

    if (Operator == tree->Type()) {
        if (fNeedOuterParen) {
            s += "(";
        }
        s += ToString(tree->Left(), true);
        s += tree->Data();
        s += ToString(tree->Right(), true);
        if (fNeedOuterParen) {
            s += ")";
        }
    } else {
        s += tree->Data();
    }
    return s;
}

/**
 * Scans token to see if all characters are digits
 * @param token a string
 * @return true if nonempty and contains all digits, otherwise false
 */
bool IsNumber(string token) {
    if (token.length() == 0) {
        return false;
    }
    for (size_t i = 0; i < token.length(); i ++) {
        if (!isdigit(token[i]) ) {
            return false;
        }
    }
    return true;
}

/**
 * Scans token to see if it starts with letter and rest are letter or digits
 * @param token a string
 * @return true if nonempty,starts with letter, and rest are letters or digits
 */
bool IsVariable(string token)  {
    if (token.length() == 0 || !isalpha(token[0])) {
        return false;
    }
    for (size_t i = 0; i < token.length(); i ++) {
        if (!isalnum(token[i]) ) {
            return false;
        }
    }
    return true;
}

/**
 * CHecks if the stirng is a +, -, or *
 * @param token a string
 * @return true if its an operator
 */
bool IsOperator(string token)  {
    return (token.length() == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*'));
}

