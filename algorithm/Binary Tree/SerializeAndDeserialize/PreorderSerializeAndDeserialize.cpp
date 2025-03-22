#include<bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;          // 节点的值
    TreeNode* left;   // 指向左子节点的指针
    TreeNode* right;  // 指向右子节点的指针

    // 构造函数，初始化节点值，左右子节点为空
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// 定义序列化和反序列化类
class Codec {
public:
    // 序列化二叉树为字符串
    std::string serialize(TreeNode* root) {
        std::ostringstream builder;  // 使用字符串流构建序列化字符串
        f(root, builder);            // 调用辅助函数进行递归序列化
        return builder.str();        // 返回构建好的字符串
    }

    // 辅助函数，递归地将二叉树序列化为字符串
    void f(TreeNode* root, std::ostringstream& builder) {
        if (root == nullptr) {
            builder << "#,";  // 如果节点为空，添加占位符"#,"
        } else {
            builder << root->val << ",";  // 添加节点值
            f(root->left, builder);       // 递归序列化左子树
            f(root->right, builder);      // 递归序列化右子树
        }
    }

    // 反序列化字符串为二叉树
    TreeNode* deserialize(const std::string& data) {
        std::vector<std::string> vals;  // 存储分割后的字符串值
        std::istringstream iss(data);     // 使用字符串流分割输入字符串
        std::string val;
        while (std::getline(iss, val, ',')) {  // 按逗号分割字符串
            vals.push_back(val);               // 将分割后的值存储到数组中
        }
        cnt = 0;  // 初始化计数器，用于跟踪当前处理的节点位置
        return g(vals);  // 调用辅助函数进行递归反序列化
    }

private:
    int cnt;  // 记录当前处理到字符串数组的哪个位置

    // 辅助函数，递归地将字符串反序列化为二叉树
    TreeNode* g(const std::vector<std::string>& vals) {
        std::string cur = vals[cnt++];  // 获取当前处理的字符串值，并递增计数器
        if (cur == "#") {
            return nullptr;  // 如果当前值为"#", 返回空节点
        } else {
            TreeNode* head = new TreeNode(std::stoi(cur));  // 创建新节点
            head->left = g(vals);                          // 递归反序列化左子树
            head->right = g(vals);                         // 递归反序列化右子树
            return head;                                   // 返回构建好的节点
        }
    }
};


int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->right->left = new TreeNode(4);
    root->right->right = new TreeNode(5);

    // 创建Codec对象
    Codec codec;

    // 序列化二叉树
    std::string serialized = codec.serialize(root);
    std::cout << "Serialized tree: " << serialized << std::endl;

    // 反序列化字符串为二叉树
    TreeNode* deserializedRoot = codec.deserialize(serialized);

    // 辅助函数：前序遍历二叉树并打印节点值
    std::function<void(TreeNode*)> preorderTraversal = [&](TreeNode* node) {
        if (node != nullptr) {
            std::cout << node->val << " ";
            preorderTraversal(node->left);
            preorderTraversal(node->right);
        }
    };

    // 打印反序列化后的二叉树的前序遍历结果
    std::cout << "Deserialized tree preorder traversal: ";
    preorderTraversal(deserializedRoot);
    std::cout << std::endl;

    // 释放内存（在实际应用中，应确保释放所有分配的内存）
    std::function<void(TreeNode*)> deleteTree = [&](TreeNode* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    };

    deleteTree(root);

    return 0;
}