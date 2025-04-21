#include <gtest/gtest.h>
#include "PLQT/plqt.h"
#include "PLQT/plqt_node.h"

TEST(PLQTConstructorTest, ValidConstructorInitializesCorrectly)
{
    std::vector<int> data = {1, 2, 3};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT* plqt = new PLQT(3, root);
    EXPECT_EQ(plqt->getDimension(), 3);
    EXPECT_EQ(plqt->getRoot()->getData(), data);
    EXPECT_EQ(plqt->getRoot()->getFirstChild(), nullptr);
    EXPECT_EQ(plqt->getRoot()->getNextSibling(), nullptr);
    EXPECT_EQ(plqt->getRoot()->getSuccessorOrder(), 0);
    EXPECT_EQ(plqt->getRoot()->getParent(), nullptr);
}

TEST(PLQTConstructorTest, ThrowsOnZeroDimension)
{
    PLQTNode* dummyRoot = nullptr;

    EXPECT_THROW({
        PLQT tree(0, dummyRoot);
    }, std::invalid_argument);
}

TEST(PLQTConstructorTest, ThrowsOnNegativeDimension)
{
    PLQTNode* dummyRoot = nullptr;

    EXPECT_THROW({
        PLQT tree(-5, dummyRoot);
    }, std::invalid_argument);
}


TEST(PLQTInsertTest, InsertSingleNode)
{
    std::vector<int> data = {1, 2, 3};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);
    std::vector<int> newData = {4, 5, 6};
    PLQTNode* insertedNode = plqt.insert(newData);
    
    EXPECT_EQ(insertedNode->getData(), newData);
    EXPECT_EQ(root->getFirstChild(), insertedNode);
}

TEST(PLQTInsertTest, InsertMultipleNodesFirstLevel)
{
    std::vector<int> data = {3, 36, 27};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);
    std::vector<int> newData1 = {92, 76, 39};
    std::vector<int> newData2 = {59, 81, 72};
    std::vector<int> newData3 = {67, 30, 32};
    
    PLQTNode* insertedNode1 = plqt.insert(newData1);
    PLQTNode* insertedNode2 = plqt.insert(newData2);
    PLQTNode* insertedNode3 = plqt.insert(newData3);
    
    EXPECT_EQ(insertedNode1->getData(), newData1);
    EXPECT_EQ(insertedNode2->getData(), newData2);
    EXPECT_EQ(insertedNode3->getData(), newData3);
    
    EXPECT_EQ(root->getFirstChild(), insertedNode1);
}

TEST(PLQTInsertTest, InsertMultipleNodesSecondLevel)
{
    std::vector<int> data = {3, 36, 27};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);
    std::vector<int> newData1 = {92, 76, 39};
    std::vector<int> newData2 = {59, 81, 72};
    std::vector<int> newData3 = {67, 30, 32};
    std::vector<int> newData4 = {80, 84, 26};
    
    PLQTNode* insertedNode1 = plqt.insert(newData1);
    PLQTNode* insertedNode2 = plqt.insert(newData2);
    PLQTNode* insertedNode3 = plqt.insert(newData3);
    PLQTNode* insertedNode4 = plqt.insert(newData4);

    EXPECT_EQ(insertedNode4->getData(), newData4);
}

TEST(PLQTInsertTest, InsertRepeatedNode)
{
    std::vector<int> data = {3, 36, 27};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);
    std::vector<int> newData1 = {92, 76, 39};
    std::vector<int> newData2 = {59, 81, 72};
    std::vector<int> newData3 = {67, 30, 32};
    std::vector<int> newData4 = {80, 84, 26}; 
    std::vector<int> newData5 = {80, 84, 26};
    
    PLQTNode* insertedNode1 = plqt.insert(newData1);
    PLQTNode* insertedNode2 = plqt.insert(newData2);
    PLQTNode* insertedNode3 = plqt.insert(newData3);
    PLQTNode* insertedNode4 = plqt.insert(newData4);
    PLQTNode* insertedNode5 = plqt.insert(newData5);

    EXPECT_EQ(insertedNode5->getData(), newData4);
    EXPECT_EQ(insertedNode5->getSuccessorOrder(), 4);
    EXPECT_EQ(insertedNode5, insertedNode4);
}


TEST(PLQTInsertTest, PhiFunction)
{
    std::vector<int> data = {3, 36, 27};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);

    EXPECT_EQ(root->getSuccessorOrder(), 0);

    std::vector<int> newData1 = {92, 76, 39};
    std::vector<int> newData2 = {59, 81, 72};
    std::vector<int> newData3 = {67, 30, 32};
    std::vector<int> newData4 = {80, 84, 26};
    
    PLQTNode* insertedNode1 = plqt.insert(newData1);
    PLQTNode* insertedNode2 = plqt.insert(newData2);
    PLQTNode* insertedNode3 = plqt.insert(newData3);
    PLQTNode* insertedNode4 = plqt.insert(newData4);

    EXPECT_EQ(insertedNode1->getSuccessorOrder(), 0);
    EXPECT_EQ(insertedNode2->getSuccessorOrder(), 1);
    EXPECT_EQ(insertedNode3->getSuccessorOrder(), 2);
    EXPECT_EQ(insertedNode4->getSuccessorOrder(), 4);
}

TEST(PLQTInsertTest, ThrowsOnInvalidDataSize)
{
    std::vector<int> data = {1, 2, 3};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);
    std::vector<int> invalidData = {1, 2}; // Invalid size
    
    EXPECT_THROW({
        plqt.insert(invalidData);
    }, std::invalid_argument);
}

TEST(PLQTSearchTest, SearchNodeFound)
{
    std::vector<int> data = {3, 36, 27};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);

    EXPECT_EQ(root->getSuccessorOrder(), 0);

    std::vector<int> newData1 = {92, 76, 39};
    std::vector<int> newData2 = {59, 81, 72};
    std::vector<int> newData3 = {67, 30, 32};
    std::vector<int> newData4 = {80, 84, 26};
    
    PLQTNode* insertedNode1 = plqt.insert(newData1);
    PLQTNode* insertedNode2 = plqt.insert(newData2);
    PLQTNode* insertedNode3 = plqt.insert(newData3);
    PLQTNode* insertedNode4 = plqt.insert(newData4);

    PLQTNode* foundNode = plqt.search(newData2);
    EXPECT_EQ(foundNode->getData(), newData2);
    EXPECT_EQ(foundNode, insertedNode2);

    foundNode = plqt.search(newData4);
    EXPECT_EQ(foundNode->getData(), newData4);
    EXPECT_EQ(foundNode, insertedNode4);

    foundNode = plqt.search(newData1);
    EXPECT_EQ(foundNode->getData(), newData1);
    EXPECT_TRUE(foundNode->isStructurallyEqual(*insertedNode1));
}

TEST(PLQTSearchTest, SearchNodeNotFound)
{
    std::vector<int> data = {3, 36, 27};
    PLQTNode* root = new PLQTNode(data);
    
    PLQT plqt(3, root);

    EXPECT_EQ(root->getSuccessorOrder(), 0);

    std::vector<int> newData1 = {92, 76, 39};
    std::vector<int> newData2 = {59, 81, 72};
    std::vector<int> newData3 = {67, 30, 32};
    std::vector<int> newData4 = {80, 84, 26};
    
    PLQTNode* insertedNode1 = plqt.insert(newData1);
    PLQTNode* insertedNode2 = plqt.insert(newData2);
    PLQTNode* insertedNode3 = plqt.insert(newData3);
    PLQTNode* insertedNode4 = plqt.insert(newData4);

    std::vector<int> notFoundData = {100, 200, 300};
    PLQTNode* foundNode = plqt.search(notFoundData);
    EXPECT_EQ(foundNode, nullptr);
}