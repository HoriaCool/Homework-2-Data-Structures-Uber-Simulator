// Copyright 2019 Nedelcu Horia (nedelcu.horia.alexandru@gmail.com)

/**
 * list_graph.h
 */

#ifndef LIST_GRAPH_H_
#define LIST_GRAPH_H_

#include <iostream>
#include <vector>
#include <queue>

/**
 * Neighbors list implementation.
 */

template <typename Tinfo>
class ListGraph {
 private:
    /**
    * Node structure is useful only for the neighbors list implementation.
    */
    struct Node {
        std::vector<int> neighbors_;
    };

    int size_;
    std::vector<Node> node_;
    std::vector<Tinfo> node_info_;

 public:
    // Constructor
    explicit ListGraph(int size);

    // Destructor
    ~ListGraph();

    /**
     * Checks if the node'index is correct.
     * 
     * @param node Node that will be checked.
     * @throw exception if the node is grater or equal to size of the graph.
     */
    inline void checkNode(int node);

    /**
     * Adds information to existing node.
     * 
     * @param node Node that will change information.
     */
    void addInfo(int node, Tinfo info);

    /**
     * Gets information associated to existing node.
     * 
     * @param node Node of whose information will be returned.
     * @return information associated to the given node.
     */
    Tinfo getInfo(int node);

    /**
     * Adds an edge between two existing nodes.
     *
     * @param src Source node of the edge to be added.
     * @param dst Destination node of the edge to be added.
     */
    void addEdge(int src, int dst);

    /**
     * Removes an existing edge from the graph.
     *
     * @param src Source node of the edge to be removed.
     * @param dst Destination node of the edge to be removed.
     */
    void removeEdge(int src, int dst);

    /**
     * Checks if there is an edge between two existing nodes.
     * 
     * @param src Source node of the edge.
     * @param dst Destination node of the edge.
     * @return True if there is an edge between src and dst, False otherwise.
     */
    bool hasEdge(int src, int dst);

    /**
     * Gets the vector of neighbors associated with the given node.
     *
     * @param node Node whose neighbors will get returned.
     * @return A vector containing the neighbors of the given node.
     */
    std::vector<int> getNeighbors(int node);

    /**
     * Gets the number of neighbors associated with the given node.
     *
     * @param node Node whose number of neighbors will get returned.
     * @return the size of the vector containing the neighbors of the given node.
     */
    int sizeNeighbors(int node);

    /**
     * Gets the index-th neighbor of the given node.
     *
     * @param node Node whose neighbor will get returned.
     * @param index Neighbor's index.
     * @return the index-th neighbor of node.
     */
    int neighbor(int node, int index);

    /**
     * Sets the graph new size. Reset edges.
     */
    void setSize(int size);

    /**
     * Gets the graph size.
     *
     * @return Number of nodes in the graph.
     */
    int getSize();

    /**
     * Checks if there is a path from a given node to another node.
     * 
     * @param src Source node.
     * @param dst Destination node.
     * @return True if there is a path from src to dst, False otherwise.
     */
    bool pathFrom(int src, int dst);

    /**
     * Gets the shortest distance from a given node to another node.
     * 
     * @param src Source node.
     * @param dst Destination node.
     * @return distance if there is a path from src to dst, -1 otherwise.
     */
    int distFrom(int src, int dst);

    /**
     * Gets the vector of shortest distances of nodes from the given node.
     *
     * @param node Node from which paths start.
     * @return A vector containing the distances of nodes from the given node.
     * If there is not a path from the given node to another one, distance = -1.
     */
    std::vector<int> getDistNodes(int node);
};

template <typename Tinfo>
ListGraph<Tinfo>::ListGraph(int size):
    size_(size), node_(size), node_info_(size) {}

template <typename Tinfo>
ListGraph<Tinfo>::~ListGraph() {}

template <typename Tinfo>
inline void ListGraph<Tinfo>::checkNode(int node) {
    if (node < 0 || node >= size_) {
        std::cerr << "Standard exception: node outside the bounds\n";
    }
}

template <typename Tinfo>
void ListGraph<Tinfo>::addInfo(int node, Tinfo info) {
    checkNode(node);

    node_info_[node] = info;
}

template <typename Tinfo>
Tinfo ListGraph<Tinfo>::getInfo(int node) {
    checkNode(node);

    return node_info_[node];
}

template <typename Tinfo>
void ListGraph<Tinfo>::addEdge(int src, int dst) {
    checkNode(src);
    checkNode(dst);

    if (!hasEdge(src, dst)) {
        node_[src].neighbors_.push_back(dst);
    }
}

template <typename Tinfo>
void ListGraph<Tinfo>::removeEdge(int src, int dst) {
    checkNode(src);
    checkNode(dst);

    for (auto it = node_[src].neighbors_.begin();
        it != node_[src].neighbors_.end(); ++it) {
        if (*it == dst) {
            it = node_[src].neighbors_.erase(it);
            break;
        }
    }
}

template <typename Tinfo>
bool ListGraph<Tinfo>::hasEdge(int src, int dst) {
    checkNode(src);
    checkNode(dst);

    for (auto it = node_[src].neighbors_.begin();
        it != node_[src].neighbors_.end(); ++it) {
        if (*it == dst) {
            return true;
        }
    }

    return false;
}

template <typename Tinfo>
std::vector<int> ListGraph<Tinfo>::getNeighbors(int node) {
    checkNode(node);

    return node_[node].neighbors_;
}

template <typename Tinfo>
int ListGraph<Tinfo>::sizeNeighbors(int node) {
    checkNode(node);

    return node_[node].neighbors_.size();
}

template <typename Tinfo>
int ListGraph<Tinfo>::neighbor(int node, int index) {
    checkNode(node);

    return node_[node].neighbors_[index];
}

template <typename Tinfo>
void ListGraph<Tinfo>::setSize(int size) {
    size_ = size;
    node_ = std::vector<Node>(size);
    node_info_ = std::vector<Tinfo>(size);
}

template <typename Tinfo>
int ListGraph<Tinfo>::getSize() {
    return size_;
}

template <typename Tinfo>
bool ListGraph<Tinfo>::pathFrom(int src, int dst) {
    checkNode(src);
    checkNode(dst);

    std::vector<bool> visited(size_, false);
    std::queue<int> q;
    int node;

    visited[src] = true;
    q.push(src);

    while (!q.empty()) {
        node = q.front();
        q.pop();

        if (node == dst) {
            return true;
        }

        for (auto it = node_[node].neighbors_.begin();
            it != node_[node].neighbors_.end(); ++it) {
            if (!visited[*it]) {
                visited[*it] = true;
                q.push(*it);
            }
        }
    }

    return false;
}

template <typename Tinfo>
int ListGraph<Tinfo>::distFrom(int src, int dst) {
    checkNode(src);
    checkNode(dst);

    std::vector<int> dist(size_, -1);
    std::queue<int> q;
    int node;

    dist[src] = 0;
    q.push(src);

    while (!q.empty()) {
        node = q.front();
        q.pop();

        if (node == dst) {
            break;
        }

        for (auto it = node_[node].neighbors_.begin();
            it != node_[node].neighbors_.end(); ++it) {
            if (dist[*it] == -1) {
                dist[*it] = dist[node] + 1;
                q.push(*it);
            }
        }
    }

    return dist[dst];
}

template <typename Tinfo>
std::vector<int> ListGraph<Tinfo>::getDistNodes(int node) {
    checkNode(node);

    std::vector<int> dist(size_, -1);
    std::queue<int> q;

    dist[node] = 0;
    q.push(node);

    while (!q.empty()) {
        node = q.front();
        q.pop();

        for (auto it = node_[node].neighbors_.begin();
            it != node_[node].neighbors_.end(); ++it) {
            if (dist[*it] == -1) {
                dist[*it] = dist[node] + 1;
                q.push(*it);
            }
        }
    }

    return dist;
}

#endif  // LIST_GRAPH_H_
