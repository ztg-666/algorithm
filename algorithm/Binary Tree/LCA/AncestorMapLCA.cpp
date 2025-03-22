//适用于需要频繁查询最低公共祖先且树结构不频繁变化的场景。构造函数时间复杂度和空间复杂度较高，但每次查询的时间复杂度最低。
class Record2 {
private:
    unordered_map<Node*, unordered_map<Node*, Node*>> map;

public:
    Record2(Node* head) {
        initMap(head);
        setMap(head);
    }

private:
    void initMap(Node* head) {
        if (head == nullptr) {
            return;
        }
        map[head] = unordered_map<Node*, Node*>();
        initMap(head->left);
        initMap(head->right);
    }

    void setMap(Node* head) {
        if (head == nullptr) {
            return;
        }
        headRecord(head->left, head);
        headRecord(head->right, head);
        subRecord(head);
        setMap(head->left);
        setMap(head->right);
    }

    void headRecord(Node* n, Node* h) {
        if (n == nullptr) {
            return;
        }
        map[n][h] = h;
        headRecord(n->left, h);
        headRecord(n->right, h);
    }

    void subRecord(Node* head) {
        if (head == nullptr) {
            return;
        }
        preLeft(head->left, head->right, head);
        subRecord(head->left);
        subRecord(head->right);
    }

    void preLeft(Node* l, Node* r, Node* h) {
        if (l == nullptr) {
            return;
        }
        preRight(l, r, h);
        preLeft(l->left, r, h);
        preLeft(l->right, r, h);
    }

    void preRight(Node* l, Node* r, Node* h) {
        if (r == nullptr) {
            return;
        }
        map[l][r] = h;
        preRight(l, r->left, h);
        preRight(l, r->right, h);
    }

public:
    Node* query(Node* o1, Node* o2) {
        if (o1 == o2) {
            return o1;
        }
        if (map.find(o1) != map.end() && map[o1].find(o2) != map[o1].end()) {
            return map[o1][o2];
        }
        if (map.find(o2) != map.end() && map[o2].find(o1) != map[o2].end()) {
            return map[o2][o1];
        }
        return nullptr;
    }
};

