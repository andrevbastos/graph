#include <vector>

class Node {
public:
    Node(int v) : v(v) {};
    void linkTo(Node* n) { adj.push_back(n); };
    int getValue() const { return v; };
    std::vector<Node*> getNeighbors() const { return adj; };

private:
    int v;
    std::vector<Node*> adj;
};