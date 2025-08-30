#include <bits/stdc++.h>

// 不同结构的二叉树数量
// 一共有n个节点，认为节点之间无差别，返回能形成多少种不同结构的二叉树
// 1 <= n <= 19
// 测试链接 : https://leetcode.cn/problems/unique-binary-search-trees/

using namespace std;

class Solution {
public:
    // 计算不同结构的二叉树数量
    int numTrees(int n) {
        vector<long long> f(n + 1, 0);
        f[0] = f[1] = 1;  // 初始化：空树和单节点树都只有 1 种结构
        for (int i = 2; i <= n; i++) {
            for (int l = 0, r = i - 1; l < i; l++, r--) {
                // f[i] = sum(f[l] * f[i-1-l])
                f[i] += f[l] * f[r];
            }
        }
        return (int)f[n];
    }
};
