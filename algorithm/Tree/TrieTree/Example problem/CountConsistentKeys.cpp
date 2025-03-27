#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

// 定义最大节点数
const int MAXN = 20001;

// Trie树类，用于存储和查询字符串前缀
class Trie {
public:
    // tree数组：表示Trie树的结构，每个节点有12个子节点（0-9, '#', '-'）
    // pass数组：记录经过该节点的次数
    // cnt变量：记录当前使用的节点数
    int tree[MAXN][12];
    int pass[MAXN];
    int cnt;

    // 构造函数，初始化Trie树
    Trie() {
        build();
    }

    // 初始化Trie树的根节点
    void build() {
        cnt = 1; // 从节点1开始计数
        memset(tree[0], 0, sizeof(tree[0])); // 将根节点的所有子节点初始化为0
        pass[0] = 0; // 根节点的pass值初始化为0
    }

    // 将字符映射为Trie树中的路径索引
    int path(char cha) {
        if (cha == '#') return 10; // 特殊字符'#'映射为索引10
        if (cha == '-') return 11; // 特殊字符'-'映射为索引11
        return cha - '0'; // 数字字符'0'-'9'映射为索引0-9
    }

    // 插入一个字符串到Trie树中
    void insert(const string& word) {
        int cur = 1; // 从根节点开始
        pass[cur]++; // 经过根节点的次数加1
        for (char c : word) { // 遍历字符串中的每个字符
            int p = path(c); // 获取字符对应的路径索引
            if (tree[cur][p] == 0) { // 如果当前路径不存在
                tree[cur][p] = ++cnt; // 创建新节点，并更新路径
                memset(tree[cnt], 0, sizeof(tree[cnt])); // 初始化新节点的子节点
                pass[cnt] = 0; // 新节点的pass值初始化为0
            }
            cur = tree[cur][p]; // 移动到下一个节点
            pass[cur]++; // 经过当前节点的次数加1
        }
    }

    // 查询以某个前缀开头的字符串数量
    int count(const string& pre) {
        int cur = 1; // 从根节点开始
        for (char c : pre) { // 遍历前缀字符串中的每个字符
            int p = path(c); // 获取字符对应的路径索引
            if (tree[cur][p] == 0) return 0; // 如果路径不存在，返回0
            cur = tree[cur][p]; // 移动到下一个节点
        }
        return pass[cur]; // 返回以该前缀开头的字符串数量
    }

    // 清空Trie树
    void clear() {
        for (int i = 1; i <= cnt; i++) {
            memset(tree[i], 0, sizeof(tree[i])); // 将所有节点的子节点清零
            pass[i] = 0; // 将所有节点的pass值清零
        }
        cnt = 1; // 重置节点计数器
    }
};

// 计算一致密钥的数量
vector<int> countConsistentKeys(vector<vector<int>>& b, vector<vector<int>>& a) {
    Trie trie; // 创建Trie树实例
    // 将数组a中的差分序列插入Trie树
    for (auto& nums : a) {
        ostringstream oss; // 使用ostringstream将差分序列转换为字符串
        for (int i = 1; i < nums.size(); ++i) {
            oss << (nums[i] - nums[i-1]) << "#"; // 差分值之间用'#'分隔
        }
        trie.insert(oss.str()); // 将差分序列字符串插入Trie树
    }

    vector<int> ans(b.size(), 0); // 存储结果
    // 对数组b中的每个差分序列，在Trie树中查询匹配的数量
    for (int i = 0; i < b.size(); ++i) {
        ostringstream oss; // 使用ostringstream将差分序列转换为字符串
        auto& nums = b[i];
        for (int j = 1; j < nums.size(); ++j) {
            oss << (nums[j] - nums[j-1]) << "#"; // 差分值之间用'#'分隔
        }
        ans[i] = trie.count(oss.str()); // 查询匹配的数量并存储到结果中
    }

    trie.clear(); // 清空Trie树
    return ans; // 返回结果
}


int main() {
    // 示例输入
    vector<vector<int>> b = {
        {{1, 2, 3, 4, 5}, {2, 4, 6, 8}, {1, 4, 7, 10}}
    };
    vector<vector<int>> a = {
        {{3, 4, 5, 6, 7, 8}, {2, 4, 6, 8}, {1, 3, 5, 7, 9}}
    };

    // 调用countConsistentKeys函数
    vector<int> result = countConsistentKeys(b, a);

    // 输出结果
    cout << "result: ";
    for (int count : result) {
        cout << count << " ";
    }
    cout << endl;

    return 0;
}