
class BinarySearchTree : public AbstractBinarySearchTree {
public:
    Node* createNode(int value, Node* parent, Node* left, Node* right) override {
        return new Node(value, parent, left, right);
    }
};

