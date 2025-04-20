#include "PLQT/PLQTNode.h"
#include <gtest/gtest.h>

TEST(PLQTNodeTest, ConstructorAndGetters) {
    std::vector<bool> data = {1, 0, 1};
    PLQTNode node(data);

    EXPECT_EQ(node.getData(), data);
    EXPECT_EQ(node.getParent(), nullptr);
    EXPECT_EQ(node.getNextSibling(), nullptr);
    EXPECT_EQ(node.getFirstChild(), nullptr);
    EXPECT_EQ(node.getSuccessorOrder(), 0);
}

TEST(PLQTNodeTest, SettersAndGetters) {
    PLQTNode node({0, 0, 0});
    PLQTNode parent({1, 1});
    PLQTNode sibling({0, 1});
    PLQTNode child({1, 0});

    node.setParent(&parent);
    node.setNextSibling(&sibling);
    node.setFirstChild(&child);
    node.setSuccessorOrder(2);

    EXPECT_EQ(node.getParent(), &parent);
    EXPECT_EQ(node.getNextSibling(), &sibling);
    EXPECT_EQ(node.getFirstChild(), &child);
    EXPECT_EQ(node.getSuccessorOrder(), 2);
}

TEST(PLQTNodeTest, EqualityOperator) {
    PLQTNode node1({1, 0, 1});
    PLQTNode node2({1, 0, 1});
    PLQTNode node3({0, 1, 1});

    EXPECT_TRUE(node1 == node2);
    EXPECT_FALSE(node1 == node3);
}

TEST(PLQTNodeTest, StructuralEquality) {
    PLQTNode node1({1, 0, 0});
    PLQTNode node2({1, 0, 0});
    PLQTNode parent({0, 1, 1});
    PLQTNode child({1, 1, 1});
    PLQTNode sibling({0, 0, 1});

    node1.setParent(&parent);
    node1.setNextSibling(&sibling);
    node1.setFirstChild(&child);
    node1.setSuccessorOrder(7);

    node2.setParent(&parent);
    node2.setNextSibling(&sibling);
    node2.setFirstChild(&child);
    node2.setSuccessorOrder(7);

    EXPECT_TRUE(node1.isStructurallyEqual(node2));

    // Slight difference
    node2.setSuccessorOrder(8);
    EXPECT_FALSE(node1.isStructurallyEqual(node2));
}

TEST(PLQTNodeTest, DeepEquality) {
    PLQTNode node1({1, 0, 0});
    PLQTNode node2({1, 0, 0});
    PLQTNode parent({0, 1, 1});
    PLQTNode child1({1, 1, 1});
    PLQTNode child2({1, 1, 1});
    PLQTNode sibling({0, 0, 1});

    node1.setParent(&parent);
    node1.setNextSibling(&sibling);
    node1.setFirstChild(&child1);
    node1.setSuccessorOrder(7);

    node2.setParent(&parent);
    node2.setNextSibling(&sibling);
    node2.setFirstChild(&child2);
    node2.setSuccessorOrder(7);

    EXPECT_TRUE(node1.isDeepEqual(node2));

    // Change child and check deep equality
    node2.getFirstChild()->setData({0, 0, 1});
    EXPECT_FALSE(node1.isDeepEqual(node2));
}

TEST(PLQTNodeTest, SearchNode) {
    PLQTNode node1({1, 0, 1});
    PLQTNode node2({0, 1, 1});
    node1.setFirstChild(&node2);

    PLQTNode* foundNode = node1.searchNode({0, 1, 1});
    EXPECT_EQ(foundNode, &node2);

    foundNode = node1.searchNode({1, 0, 1});
    EXPECT_EQ(foundNode, &node1);

    foundNode = node1.searchNode({1, 1, 1});
    EXPECT_EQ(foundNode, nullptr);
}

TEST(PLQTNodeTest, ToStringRepresentation) {
    PLQTNode node({1, 0, 1});
    node.setSuccessorOrder(3);
    std::string expected = "PLQTNode(data: 101, order: 3)";
    EXPECT_EQ(node.toString(), expected);
}
