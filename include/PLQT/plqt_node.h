#ifndef PLQTNODE_H
#define PLQTNODE_H

#include <vector>
#include <string>

/**
 * @brief Represents a node in a Primogenitary Linked Quad Tree (PLQT).
 * Each node contains binary data and links to its parent, siblings, and children.
 */
class PLQTNode {
public:
    /**
     * @brief Constructs a node with the given binary data.
     * @param data A vector of boolean values representing binary data.
     */
    explicit PLQTNode(const std::vector<bool>& data);

    /** @brief Returns the binary data of the node. */
    std::vector<bool> getData() const;

    /** @brief Sets the binary data of the node. */
    void setData(const std::vector<bool>& data);

    /** @brief Returns the parent of the node. */
    PLQTNode* getParent() const;

    /** @brief Sets the parent of the node. */
    void setParent(PLQTNode* parent);

    /** @brief Returns the next sibling of the node. */
    PLQTNode* getNextSibling() const;

    /** @brief Sets the next sibling of the node. */
    void setNextSibling(PLQTNode* sibling);

    /** @brief Returns the first child of the node. */
    PLQTNode* getFirstChild() const;

    /** @brief Sets the first child of the node. */
    void setFirstChild(PLQTNode* child);

    /** @brief Returns the successor order of the node. */
    int getSuccessorOrder() const;

    /** @brief Sets the successor order of the node. */
    void setSuccessorOrder(int order);

    /**
     * @brief Compares two nodes based on their data.
     * @param other The node to compare with.
     * @return True if both nodes have the same data.
     */
    bool operator==(const PLQTNode& other) const;

    /**
     * @brief Checks structural equality between this node and another.
     * Compares data, parent, sibling, child pointers and order.
     * @param other The node to compare with.
     * @return True if all members match (including pointers).
     */
    bool isStructurallyEqual(const PLQTNode& other) const;

    /**
     * @brief Recursively checks structural equality between this node and another.
     * This method will compare all child nodes recursively.
     * @param other The node to compare with.
     * @return True if both nodes and their entire tree structure are the same.
     */
    bool isDeepEqual(const PLQTNode& other) const;

    /**
     * @brief Searches for a node with the specified data in the tree.
     * This method will traverse the tree recursively.
     * @param data The data to search for.
     * @return A pointer to the found node, or nullptr if not found.
     */
    PLQTNode* searchNode(const std::vector<bool>& data);

    /**
     * @brief Returns a string representation of the node.
     * Format: "PLQTNode(data: 101, order: 2)"
     */
    std::string toString() const;

private:
    std::vector<bool> data_;
    PLQTNode* parent_;
    PLQTNode* nextSibling_;
    PLQTNode* firstChild_;
    int successorOrder_;
};

#endif // PLQTNODE_H
