#include <vector>
#include <unordered_set>
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

class TwoNumbersMaximumXor {
public:
    // 使用前缀树的方法找到数组中两个数的最大异或值
    int findMaximumXOR1(vector<int>& nums) {
        // 构建前缀树
        build(nums);
        int ans = 0;
        // 遍历数组中的每个数，计算其与树中其他数的最大异或值
        for (int num : nums) {
            ans = max(ans, maxXor(num));
        }
        // 清空前缀树
        clear();
        return ans;
    }

    // 使用哈希表的方法找到数组中两个数的最大异或值
    int findMaximumXOR2(vector<int>& nums) {
        int maxVal = INT_MIN;
        // 找到数组中的最大值
        for (int num : nums) {
            maxVal = max(maxVal, num);
        }
        int ans = 0;
        unordered_set<int> set;
        // 从最高有效位开始逐位尝试构建可能的最大异或值
        for (int i = getHighBit(maxVal); i >= 0; i--) {
            int better = ans | (1 << i);
            set.clear();
            // 遍历数组，检查是否存在能够构成当前位更好的异或值的数
            for (int num : nums) {
                num = (num >> i) << i;
                set.insert(num);
                if (set.find(better ^ num) != set.end()) {
                    ans = better;
                    break;
                }
            }
        }
        return ans;
    }

private:
    // 前缀树目前使用了多少空间
    int cnt = -1;

    // 数字只需要从哪一位开始考虑
    int high = -1;

    // 准备这么多静态空间就够了，实验出来的
    // 如果测试数据升级了规模，就改大这个值
    static const int MAXN = 30001;
    int tree[MAXN][2]{};

    // 构建前缀树
    void build(vector<int>& nums) {
        cnt = 1;
        // 找个最大值
        int maxVal = INT_MIN;
        for (int num : nums) {
            maxVal = max(maxVal, num);
        }
        // 计算数组最大值的二进制状态，有多少个前缀的0可以忽略
        high = getHighBit(maxVal);
        // 将所有数字插入前缀树
        for (int num : nums) {
            insert(num);
        }
    }

    // 插入一个数字到前缀树中
    void insert(int num) {
        int cur = 1;
        // 从最高位开始逐位插入
        for (int i = high, path; i >= 0; i--) {
            path = (num >> i) & 1; // 获取当前位的值
            if (tree[cur][path] == 0) { // 如果该路径不存在，则创建
                tree[cur][path] = ++cnt;
            }
            cur = tree[cur][path]; // 移动到下一个节点
        }
    }

    // 计算一个数字与其他数字的最大异或值
    int maxXor(int num) {
        // 最终异或的结果(尽量大)
        int ans = 0;
        // 前缀树目前来到的节点编号
        int cur = 1;
        // 从最高位开始逐位计算最大异或值
        for (int i = high, status, want; i >= 0; i--) {
            // status : num第i位的状态
            status = (num >> i) & 1;
            // want : num第i位希望遇到的状态（取反）
            want = status ^ 1;
            if (tree[cur][want] == 0) { // 如果希望的路径不存在
                want ^= 1; // 只能选择存在的路径
            }
            // 更新异或结果
            ans |= (status ^ want) << i;
            cur = tree[cur][want]; // 移动到下一个节点
        }
        return ans;
    }

    // 清空前缀树
    void clear() {
        for (int i = 1; i <= cnt; i++) {
            tree[i][0] = tree[i][1] = 0;
        }
    }

    // 获取数字的最高有效位
    int getHighBit(int num) {
        if (num == 0) return -1; // 如果数字为0，返回-1
        int high = 0; // 初始化最高位
        while (num >>= 1) { // 不断右移，直到数字为0
            high++;
        }
        return high; // 返回最高位
    }
};



int main() {
    TwoNumbersMaximumXor solution;
    std::vector<int> nums = {3, 10, 5, 25, 2, 8};

    // 测试 findMaximumXOR1 方法
    int result1 = solution.findMaximumXOR1(nums);
    std::cout << "Maximum XOR using Trie: " << result1 << std::endl;

    // 测试 findMaximumXOR2 方法
    int result2 = solution.findMaximumXOR2(nums);
    std::cout << "Maximum XOR using Hash Set: " << result2 << std::endl;

    return 0;
}