#include "PLQT/plqt_node.h"
#include <sstream>

PLQTNode::PLQTNode(const std::vector<int>& data)
    : data_(data), parent_(nullptr), nextSibling_(nullptr), firstChild_(nullptr), successorOrder_(0) {}

std::vector<int> PLQTNode::getData() const {
    return data_;
}

void PLQTNode::setData(const std::vector<int>& data) {
    data_ = data;
}

PLQTNode* PLQTNode::getParent() const {
    return parent_;
}

void PLQTNode::setParent(PLQTNode* parent) {
    parent_ = parent;
}

PLQTNode* PLQTNode::getNextSibling() const {
    return nextSibling_;
}

void PLQTNode::setNextSibling(PLQTNode* sibling) {
    nextSibling_ = sibling;
}

PLQTNode* PLQTNode::getFirstChild() const {
    return firstChild_;
}

void PLQTNode::setFirstChild(PLQTNode* child) {
    firstChild_ = child;
}

int PLQTNode::getSuccessorOrder() const {
    return successorOrder_;
}

void PLQTNode::setSuccessorOrder(int order) {
    successorOrder_ = order;
}

bool PLQTNode::operator==(const PLQTNode& other) const {
    return data_ == other.data_;
}

bool PLQTNode::isStructurallyEqual(const PLQTNode& other) const {
    return data_ == other.data_ &&
           parent_ == other.parent_ &&
           nextSibling_ == other.nextSibling_ &&
           firstChild_ == other.firstChild_ &&
           successorOrder_ == other.successorOrder_;
}

bool PLQTNode::isDeepEqual(const PLQTNode& other) const {
    // Compare data and structural properties
    if (data_ != other.data_ || successorOrder_ != other.successorOrder_)
        return false;
    
    // Recursively compare children
    if (firstChild_ && other.firstChild_) {
        if (!firstChild_->isDeepEqual(*other.firstChild_))
            return false;
    } else if (firstChild_ || other.firstChild_) {
        return false;
    }
    
    // Recursively compare siblings
    if (nextSibling_ && other.nextSibling_) {
        if (!nextSibling_->isDeepEqual(*other.nextSibling_))
            return false;
    } else if (nextSibling_ || other.nextSibling_) {
        return false;
    }

    return true;
}

PLQTNode* PLQTNode::searchNode(const std::vector<int>& data) {
    if (data_ == data) {
        return this;
    }

    // Search in children
    if (firstChild_) {
        PLQTNode* found = firstChild_->searchNode(data);
        if (found) return found;
    }

    // Search in siblings
    if (nextSibling_) {
        return nextSibling_->searchNode(data);
    }

    return nullptr;
}

std::string PLQTNode::toString() const {
    std::ostringstream oss;
    oss << "PLQTNode(data: ";
    for (bool bit : data_) {
        oss << (bit ? '1' : '0');
    }
    oss << ", order: " << successorOrder_ << ")";
    return oss.str();
}
