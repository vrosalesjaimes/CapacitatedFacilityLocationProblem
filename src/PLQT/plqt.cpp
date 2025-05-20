#include "PLQT/plqt.h"
#include "PLQT/plqt_node.h"
#include <sstream>
#include <cmath>
#include <iostream>

PLQT::PLQT(int dim, PLQTNode *root) : dimension(dim), root(root), powerOfTwo(0)
{
    if (dim <= 0)
    {
        throw std::invalid_argument("Dimension must be greater than 0.");
    }
    powerOfTwo = PowerOfTwo(dim);
}

PLQT::PLQT() : dimension(0), root(nullptr), powerOfTwo(0)
{
    // Default constructor
}

PLQT::~PLQT()
{
    delete root;
}

PLQTNode *PLQT::insert(const std::vector<int> &data)
{
    if (data.size() != dimension)
    {
        throw std::invalid_argument("Data vector size must match the dimension of the PLQT.");
    }

    return insertNode(root, data);
}

int PLQT::calculateSuccessorship(PLQTNode *node, const std::vector<int> &data) const
{
    std::vector<int> phi = std::vector<int>(dimension, 0);

    for (int i = 0; i < dimension; ++i)
    {
        phi[i] = node->getData()[i] <= data[i] ? 0 : 1;
    }

    int successor = 0;
    for (int i = 0; i < dimension; ++i)
    {
        successor += phi[i] * powerOfTwo.getPower(i);
    }

    return successor;
}

PLQTNode *PLQT::insertNode(PLQTNode *current, const std::vector<int> &data)
{
    if (current == nullptr)
    {
        return new PLQTNode(data);
    }

    int successor = calculateSuccessorship(current, data);

    if (successor < 0)
    {
        throw std::out_of_range("Successor index is out of range.");
    }

    if (successor == 0 && current->getData() == data)
    {
        return current;
    }

    if (current->getFirstChild() == nullptr ||
        current->getFirstChild()->getSuccessorOrder() > successor)
    {
        PLQTNode *newChild = new PLQTNode(data);

        newChild->setNextSibling(current->getFirstChild());
        current->setFirstChild(newChild);
        newChild->setParent(current);
        newChild->setFirstChild(nullptr);
        newChild->setSuccessorOrder(successor);
        return newChild;
    }

    PLQTNode *firstChild = current->getFirstChild();
    
    while (firstChild->getNextSibling() != nullptr &&
           firstChild->getNextSibling()->getSuccessorOrder() <= successor)
    {
        firstChild = firstChild->getNextSibling();
    }

    if (firstChild->getSuccessorOrder() == successor)
    {
        return insertNode(firstChild, data);
    }
    else
    {
        PLQTNode *newChild = new PLQTNode(data);
        newChild->setNextSibling(firstChild->getNextSibling());
        firstChild->setNextSibling(newChild);
        newChild->setParent(current);
        newChild->setFirstChild(nullptr);
        newChild->setSuccessorOrder(successor);
        return newChild;
    }
}

PLQTNode *PLQT::search(const std::vector<int> &data) const
{
    if (data.size() != dimension)
    {
        throw std::invalid_argument("Data vector size must match the dimension of the PLQT.");
    }

    return searchNode(root, data);
}

PLQTNode *PLQT::searchNode(PLQTNode *node, const std::vector<int> &data) const
{
    if (node == nullptr)
        return nullptr;

    if (node->getData() == data)
    {
        return node;
    }

    PLQTNode *found = searchNode(node->getFirstChild(), data);
    if (found != nullptr)
    {
        return found;
    }

    return searchNode(node->getNextSibling(), data);
}

int PLQT::getDimension() const {
    return dimension;
}

PLQTNode* PLQT::getRoot() const {
    return root;
}