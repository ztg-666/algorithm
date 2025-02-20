//适用于需要多次查询最低公共祖先且树结构不频繁变化的场景。构造函数时间复杂度较高，但每次查询的时间复杂度较低。
class Record1 {
private:
    unordered_map<Node*, Node*> map; // 存储每个节点及其父节点的映射

public:
    // 构造函数，初始化记录表
    Record1(Node* head) {
        if (head != nullptr) {
            map[head] = nullptr; // 根节点的父节点为nullptr
        }
        setMap(head); // 递归设置每个节点的父节点
    }

private:
    // 递归设置每个节点的父节点
    void setMap(Node* head) {
        if (head == nullptr) {
            return; // 如果当前节点为空，直接返回
        }
        if (head->left != nullptr) {
            map[head->left] = head; // 记录左子节点的父节点为当前节点
        }
        if (head->right != nullptr) {
            map[head->right] = head; // 记录右子节点的父节点为当前节点
        }
        setMap(head->left); // 递归处理左子树
        setMap(head->right); // 递归处理右子树
    }

public:
    // 查询两个节点的最低公共祖先
    Node* query(Node* o1, Node* o2) {
        unordered_set<Node*> path; // 用于存储从o1到根节点的路径

        // 从o1开始，沿着父节点链向上遍历，直到到达根节点
        while (map.find(o1) != map.end()) {
            path.insert(o1); // 将当前节点插入路径集合
            o1 = map[o1]; // 移动到父节点
        }

        // 从o2开始，沿着父节点链向上遍历，直到找到一个在路径集合中的节点
        while (path.find(o2) == path.end()) {
            o2 = map[o2]; // 移动到父节点
        }

        return o2; // 返回找到的最低公共祖先节点
    }
};
