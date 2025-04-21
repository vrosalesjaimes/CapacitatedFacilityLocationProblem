#ifndef PLQT_H
#define PLQT_H

#include "PLQT/plqt_node.h"
#include "PLQT/power_of_two.h"
#include <vector>
#include <string>

/**
 * @class PLQT
 * @brief Represents a Primogenitary Linked Quad Tree (PLQT) structure for storing binary vectors.
 */
class PLQT {
private:
    PLQTNode* root;
    int dimension;
    PowerOfTwo powerOfTwo;

    void preOrderTraversal(PLQTNode* node, std::vector<std::string>& output, int indent = 0) const;
    PLQTNode* insertNode(PLQTNode* current, const std::vector<int>& data);
    bool compareTrees(PLQTNode* a, PLQTNode* b) const;
    int calculateSuccessorship(PLQTNode* node, const std::vector<int>& data) const;
    PLQTNode* searchNode(PLQTNode* current, const std::vector<int>& data) const;
public:
    /**
     * @brief Constructs an empty PLQT with a given vector dimension.
     * @param dim Dimension of binary vectors.
     */
    PLQT(int dim, PLQTNode* root);

    /**
     * @brief Destructor. Frees memory of the tree.
     */
    ~PLQT();

    /**
     * @brief Inserts a binary vector into the PLQT.
     * @param data The binary vector to insert.
     * @return Pointer to the inserted node.
     */
    PLQTNode* insert(const std::vector<int>& data);

    /**
     * @brief Searches for a binary vector in the PLQT.
     * @param data The binary vector to search for.
     * @return Pointer to the node if found, nullptr otherwise.
     */
    PLQTNode* search(const std::vector<int>& data) const;

    /**
     * @brief Compares two PLQTs for deep equality.
     * @param other Another PLQT.
     * @return true if trees are equal, false otherwise.
     */
    bool isEqual(const PLQT& other) const;

    /**
     * @brief Returns a hierarchical string representation of the PLQT.
     * @return String representation.
     */
    std::string toString() const;

    /**
     * @brief Gets the root node.
     * @return Pointer to the root node.
     */
    PLQTNode* getRoot() const;

    /**
     * @brief Gets the dimension of the PLQT.
     * @return Dimension of the PLQT.
     */
    int getDimension() const;
};

#endif // PLQT_H
