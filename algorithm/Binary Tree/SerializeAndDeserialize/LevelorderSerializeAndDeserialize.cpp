#include<bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;          // 节点值
    TreeNode* left;   // 左子节点指针
    TreeNode* right;  // 右子节点指针

    // 构造函数，初始化节点值，左右子节点为空
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// 按层序列化和反序列化二叉树的类
class Codec {
public:
    static const int MAXN = 10001;  // 队列最大容量
    TreeNode* queue[MAXN];         // 辅助队列，用于层次遍历
    int l, r;                       // 队列的头尾指针

    // 序列化二叉树为字符串
    std::string serialize(TreeNode* root) {
        std::ostringstream builder;  // 字符串构建器
        if (root != nullptr) {
            builder << root->val << ",";  // 将根节点值加入字符串
            l = 0;
            r = 0;
            queue[r++] = root;  // 根节点入队
            while (l < r) {
                root = queue[l++];  // 出队一个节点
                if (root->left != nullptr) {
                    builder << root->left->val << ",";  // 左子节点值加入字符串
                    queue[r++] = root->left;  // 左子节点入队
                } else {
                    builder << "#,";  // 左子节点为空，加入占位符
                }
                if (root->right != nullptr) {
                    builder << root->right->val << ",";  // 右子节点值加入字符串
                    queue[r++] = root->right;  // 右子节点入队
                } else {
                    builder << "#,";  // 右子节点为空，加入占位符
                }
            }
        }
        return builder.str();  // 返回序列化后的字符串
    }

    // 反序列化字符串为二叉树
    TreeNode* deserialize(const std::string& data) {
        if (data.empty()) {
            return nullptr;  // 如果字符串为空，返回空指针
        }
        std::vector<std::string> nodes;  // 存储节点值的数组
        std::stringstream ss(data);
        std::string item;
        while (std::getline(ss, item, ',')) {
            nodes.push_back(item);  // 将字符串按逗号分割存入数组
        }
        int index = 0;
        TreeNode* root = generate(nodes[index++]);  // 生成根节点
        l = 0;
        r = 0;
        queue[r++] = root;  // 根节点入队
        while (l < r) {
            TreeNode* cur = queue[l++];  // 出队一个节点
            cur->left = generate(nodes[index++]);  // 生成左子节点
            cur->right = generate(nodes[index++]);  // 生成右子节点
            if (cur->left != nullptr) {
                queue[r++] = cur->left;  // 左子节点不为空，入队
            }
            if (cur->right != nullptr) {
                queue[r++] = cur->right;  // 右子节点不为空，入队
            }
        }
        return root;  // 返回反序列化后的二叉树根节点
    }

private:
    // 根据字符串生成二叉树节点
    TreeNode* generate(const std::string& val) {
        return val == "#" ? nullptr : new TreeNode(std::stoi(val));  // 如果字符串为"#"，返回空指针；否则，返回新节点
    }
};

// 辅助函数：打印二叉树（前序遍历）
void printTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    cout << root->val << " ";
    printTree(root->left);
    printTree(root->right);
}
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(5);

    // 序列化二叉树
    Codec codec;
    string serialized = codec.serialize(root);
    cout << "Serialized: " << serialized << endl;

    // 反序列化二叉树
    TreeNode* deserializedRoot = codec.deserialize(serialized);
    cout << "Deserialized: ";
    printTree(deserializedRoot);
    cout << endl;

    // 释放原始树
    freeTree(root); // 替换原来的5个delete操作

    // 释放反序列化后的树
    freeTree(deserializedRoot); // 新增这行

    return 0;
}