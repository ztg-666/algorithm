namespace class01 {

// 定义一个模板类 Element，用于存储值
template <typename V>
class Element {
public:
    V value; // 存储的值

    // 构造函数，初始化值
    Element(V value) : value(value) {}
};

// 定义一个模板类 UnionFindSet，用于实现并查集数据结构
template <typename V>
class UnionFindSet {
private:
    std::unordered_map<V, Element<V>*> elementMap; // 存储值到Element对象的映射
    std::unordered_map<Element<V>*, Element<V>*> fatherMap; // 存储每个Element对象的父节点
    std::unordered_map<Element<V>*, int> rankMap; // 存储每个Element对象的秩（树的高度）

public:
    // 构造函数，初始化并查集
    UnionFindSet(const std::vector<V>& list) {
        for (const V& value : list) {
            Element<V>* element = new Element<V>(value); // 创建新的Element对象
            elementMap[value] = element; // 将值映射到Element对象
            fatherMap[element] = element; // 初始化父节点为自己
            rankMap[element] = 1; // 初始化秩为1
        }
    }

    // 析构函数，释放所有Element对象
    ~UnionFindSet() {
        for (auto& pair : elementMap) {
            delete pair.second; // 释放Element对象
        }
    }

private:
    // 查找元素的根节点，并进行路径压缩
    Element<V>* findHead(Element<V>* element) {
        std::stack<Element<V>*> path; // 用于存储路径上的节点
        while (element != fatherMap[element]) { // 查找根节点
            path.push(element); // 将路径上的节点压入栈
            element = fatherMap[element]; // 移动到父节点
        }
        while (!path.empty()) { // 路径压缩
            fatherMap[path.top()] = element; // 将路径上的节点的父节点设为根节点
            path.pop(); // 弹出栈顶元素
        }
        return element; // 返回根节点
    }


public:
    // 判断两个值是否属于同一个集合
    bool isSameSet(V a, V b) {
        if (elementMap.find(a) != elementMap.end() && elementMap.find(b) != elementMap.end()) { // 检查值是否存在
            return findHead(elementMap[a]) == findHead(elementMap[b]); // 比较两个值的根节点是否相同
        }
        return false; // 如果值不存在，返回false
    }

    // 合并两个集合
    void unite(V a, V b) {
        if (elementMap.find(a) != elementMap.end() && elementMap.find(b) != elementMap.end()) { // 检查值是否存在
            Element<V>* aF = findHead(elementMap[a]); // 查找a的根节点
            Element<V>* bF = findHead(elementMap[b]); // 查找b的根节点
            if (aF != bF) { // 如果根节点不同，进行合并
                Element<V>* big = rankMap[aF] >= rankMap[bF] ? aF : bF; // 选择秩较大的树作为合并后的根节点
                Element<V>* small = big == aF ? bF : aF; // 选择秩较小的树
                fatherMap[small] = big; // 将秩较小的树的根节点设为秩较大的树的根节点
                rankMap[big] = rankMap[aF] + rankMap[bF]; // 更新秩较大的树的秩
                rankMap.erase(small); // 移除秩较小的树的秩
            }
        }
    }
};
}

