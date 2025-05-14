#include <vector>      // 引入动态数组库
#include <algorithm>   // 引入算法库，用于 std::max
// #include <iostream> // 如果需要测试用的 main 函数，取消此行注释

// 移除盒子
// 给出一些不同颜色的盒子boxes，盒子的颜色由不同的正数表示
// 你将经过若干轮操作去去掉盒子，直到所有的盒子都去掉为止
// 每一轮你可以移除具有相同颜色的连续 k 个盒子（k >= 1）
// 这样一轮之后你将得到 k * k 个积分
// 返回你能获得的最大积分总和
// 测试链接 : https://leetcode.cn/problems/remove-boxes/

// 辅助函数的正向声明
int solve(const std::vector<int>& boxes, int l, int r, int k,
          std::vector<std::vector<std::vector<int>>>& dp);


// 时间复杂度: O(n^4)
// 共有 O(n^3) 个状态 (l, r, k 每个最多到 n)。
// 每个状态的计算涉及一个 'm' 的循环，该循环最多迭代 O(n) 次。
// 因此，n^3 个状态 * 每个状态 n 次操作 = O(n^4)。
// 空间复杂度: O(n^3)，用于 dp 表。
int removeBoxes(const std::vector<int>& boxes) {
    int n = boxes.size(); // 获取盒子的数量
    if (n == 0) {         // 如果没有盒子
        return 0;         // 返回0分
    }

    // dp[l][r][k] 存储处理子数组 boxes[l...r] 所能获得的最大积分，
    // 其中 'k' 是 boxes[l] 左边（不属于 l...r）且与 boxes[l] 颜色相同，
    // 并等待与 boxes[l...s] 一起被移除的盒子的数量。

    // 维度:
    // l: 0 到 n-1 (大小为 n)
    // r: 0 到 n-1 (大小为 n)
    // k: 0 到 n。前面相同颜色盒子的数量可以从 0 到 n
    //    (例如，如果所有 n 个盒子颜色相同，并且它们都是从上一步骤聚合而来的)。
    //    因此，dp 表的 k 维度的大小需要是 n + 1，以容纳索引 k=n。
    std::vector<std::vector<std::vector<int>>> dp(
        n, std::vector<std::vector<int>>(
               n, std::vector<int>(n + 1, 0) // 初始化所有 dp 值为 0
           )
    );
    // dp 表初始化为 0。因为积分是 k*k，所以它们是非负的。
    // 如果 l <= r，移除至少一个盒子的最小积分是 1*1 = 1。
    // 所以，dp 值为 0 意味着以下两种情况之一：
    // 1. 状态 (l, r, k) 尚未计算。
    // 2. 状态对应于 l > r 的基本情况，积分为 0。
    // 检查 `dp[l][r][k] > 0` 可以正确识别已计算且积分为正的状态。

    return solve(boxes, 0, n - 1, 0, dp); // 调用递归函数开始计算
}

// 带记忆化的递归辅助函数，用于计算最大积分。
// boxes: 输入的盒子颜色数组。
// l: 当前考虑的子数组的起始索引。
// r: 当前考虑的子数组的结束索引。
// k: boxes[l] 左侧紧邻的、与 boxes[l] 颜色相同，
//    并且与当前段 boxes[l...s] 一起分组的盒子的数量。
int solve(const std::vector<int>& boxes, int l, int r, int k,
          std::vector<std::vector<std::vector<int>>>& dp) {
    if (l > r) { // 基本情况：如果左索引超过右索引，没有盒子可移除，得0分。
        return 0;
    }

    // 记忆化：如果此状态的积分已经计算过并且为正，则直接返回。
    if (dp[l][r][k] > 0) {
        return dp[l][r][k];
    }

    // 阶段 1: 识别初始的连续同色盒子块。
    // 找到最右边的索引 's'，使得 boxes[l...s] 的颜色都与 boxes[l] 相同。
    int s = l;
    while (s + 1 <= r && boxes[l] == boxes[s + 1]) {
        s++;
    }
    // 现在，boxes[l...s] 的颜色都相同。
    // 这个特定块中的盒子数量是 (s - l + 1)。
    // 这些盒子与前面 'k' 个相同颜色的盒子合并。
    int count_in_current_group = k + (s - l + 1);

    // 阶段 2: 使用两种主要策略计算积分。

    // 策略 1: 立即移除当前组 (k 个前面的盒子 + boxes[l...s])。
    // 获得的积分是 (count_in_current_group)^2。
    // 然后，递归地解决剩余子数组 boxes[s+1...r] 的问题，此时前面没有任何类型的盒子 (传入的 k 为 0)。
    int max_points = count_in_current_group * count_in_current_group + solve(boxes, s + 1, r, 0, dp);

    // 策略 2: 尝试将当前组 (k 个前面的盒子 + boxes[l...s])
    // 与后面某个同样具有 boxes[l] 颜色的盒子块合并。
    // 遍历可能的合并点 'm'，从 s+2 到 r。
    // 段 boxes[s+1...m-1] 必须首先被移除。
    // 然后，我们递归地解决 boxes[m...r] 的问题，并将 'count_in_current_group'
    // 作为该子问题的新 'k' 值传递下去。
    for (int m = s + 2; m <= r; ++m) {
        // 我们寻找一个盒子 boxes[m]，它与 boxes[l] 颜色相同。
        // 条件 `boxes[m-1] != boxes[m]` 是一个优化：
        // 它确保我们只在 boxes[m] 是目标颜色新块的开始时才考虑合并。
        // 如果 boxes[m-1] 也等于 boxes[m]，那么从更早的点开始的递归调用
        // （或者 's' 延伸得更远）会处理这种情况。
        // 这有助于通过标准化合并点来避免冗余计算。
        if (boxes[l] == boxes[m] && boxes[m - 1] != boxes[m]) {
            max_points = std::max(max_points,
                                  solve(boxes, s + 1, m - 1, 0, dp) +  // 移除中间段获得的积分。
                                  solve(boxes, m, r, count_in_current_group, dp) // 合并并解决剩余部分获得的积分。
                                 );
        }
    }

    dp[l][r][k] = max_points; // 存储此状态计算出的最大积分。
    return max_points;
}


#include<iostream>
int main() {
    std::vector<int> boxes1 = {1, 3, 2, 2, 2, 3, 4, 3, 1};
    std::cout << "boxes1: " << removeBoxes(boxes1) << std::endl; // 预期: 23

    std::vector<int> boxes2 = {1, 1, 1};
    std::cout << "boxes2: " << removeBoxes(boxes2) << std::endl; // 预期: 9

    std::vector<int> boxes3 = {1};
    std::cout << "boxes3: " << removeBoxes(boxes3) << std::endl; // 预期: 1
    return 0;
}

