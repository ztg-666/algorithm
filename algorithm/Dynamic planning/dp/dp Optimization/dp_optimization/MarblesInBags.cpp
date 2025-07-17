#include <vector>
#include <algorithm>


// 将珠子放进背包中
// 给定一个长度为n的数组weights，背包一共有k个
// 其中weights[i]是第i个珠子的重量
// 请你按照如下规则将所有的珠子放进k个背包
// 1，没有背包是空的
// 2，如果第i个珠子和第j个珠子在同一个背包里，那么i到j所有珠子都要在这个背包里
// 一个背包如果包含i到j的所有珠子，这个背包的价格是weights[i]+weights[j]
// 一个珠子分配方案的分数，是所有k个背包的价格之和
// 请返回所有分配方案中，最大分数与最小分数的差值为多少
// 1 <= n, k <= 10^5
// 测试链接 : https://leetcode.cn/problems/put-marbles-in-bags/

using namespace std;

class Solution {
public:
    long long putMarbles(vector<int>& weights, int k) {
        int n = weights.size();
        // 当k=1时无需分割，直接返回0
        if (k == 1) return 0;

        // 创建相邻元素和的分割点数组
        vector<long long> split(n - 1);
        // 计算所有可能的相邻分割点权重和
        for (int i = 1; i < n; i++) {
            split[i - 1] = (long long)weights[i - 1] + weights[i];
        }

        // 对分割点数组进行排序
        sort(split.begin(), split.end());

        long long min_score = 0;  // 最小分割方案总分
        long long max_score = 0;  // 最大分割方案总分

        // 选取最小的k-1个分割点（最小方案）和最大的k-1个分割点（最大方案）
        // k个分组需要k-1个分割点，故循环k-1次
        for (int i = 0, j = n - 2, count = 0; count < k - 1; i++, j--, count++) {
            min_score += split[i];   // 累加最小分割点
            max_score += split[j];   // 累加最大分割点
        }

        // 返回最大方案与最小方案的差值
        return max_score - min_score;
    }
};
