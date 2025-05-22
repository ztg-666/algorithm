#include <vector>
#include <algorithm> // For std::sort
// #include <numeric> // For std::accumulate, not strictly needed here

// 分配重复整数
// 给你一个长度为n的整数数组nums，这个数组中至多有50个不同的值
// 同时你有m个顾客的订单quantity，其中整数quantity[i]是第i位顾客订单的数目
// 请你判断是否能将nums中的整数分配给这些顾客，且满足：
// 第i位顾客恰好有quantity[i]个整数、第i位顾客拿到的整数都是相同的
// 每位顾客都要满足上述两个要求，返回是否能都满足
// 测试链接 : https://leetcode.cn/problems/distribute-repeating-integers/

class Solution {
private:
    // cnt: 向量，存储nums中不同数字的出现次数。
    // sum_quantities: 预计算的顾客子集订单总量。
    //      sum_quantities[mask] 是掩码 'mask' 代表的顾客子集所需的物品总数。
    // status: 位掩码，表示哪些顾客的订单仍需满足。
    //         如果第i位为1，表示顾客i的订单待处理。
    // num_idx: 当前考虑的cnt向量中的数字种类的索引。
    // dp[status][num_idx]: 记忆化表格。
    //      0: 状态未计算。
    //      1: true (表示从当前状态出发，可以满足status中的订单)。
    //     -1: false。
    bool f(const std::vector<int>& cnt, const std::vector<int>& sum_quantities,
           int status, int num_idx, std::vector<std::vector<int>>& dp) {

        if (status == 0) { // 所有顾客的订单都已满足
            return true;
        }
        // 仍有订单待处理，但没有更多种类的数字可以使用
        if (num_idx == cnt.size()) {
            return false;
        }

        if (dp[status][num_idx] != 0) { // 结果已计算过
            return dp[status][num_idx] == 1;
        }

        bool possible_to_satisfy = false;

        // 决策1: 使用当前种类的数字 (cnt[num_idx]) 去尝试满足 'status' 中的一个子集 'sub_status_to_fulfill'。
        // 当前可用数字的个数为 cnt[num_idx]。
        int current_num_available = cnt[num_idx];
        // 遍历 'status' 的所有非空子集 'sub_status_to_fulfill'。
        // (s_outer - 1) & s_outer 会得到 s_outer 的下一个子集（如果按降序迭代的话）。
        for (int sub_status_to_fulfill = status; sub_status_to_fulfill > 0; sub_status_to_fulfill = (sub_status_to_fulfill - 1) & status) {
            if (sum_quantities[sub_status_to_fulfill] <= current_num_available) {
                // 如果当前种类的数字足够满足子集 'sub_status_to_fulfill' 的需求：
                // 尝试用下一个种类的数字 (num_idx + 1) 去满足剩余的订单 (status ^ sub_status_to_fulfill)。
                if (f(cnt, sum_quantities, status ^ sub_status_to_fulfill, num_idx + 1, dp)) {
                    possible_to_satisfy = true;
                    break; // 找到一种方案，提前退出循环
                }
            }
        }

        // 决策2: 不使用当前种类的数字 (cnt[num_idx]) 来满足 'status' 中的任何订单。
        // 这种情况仅在“决策1”未能找到解时尝试。
        // 尝试用下一个种类的数字 (num_idx + 1) 去满足当前 'status' 中的所有订单。
        if (!possible_to_satisfy) {
            if (f(cnt, sum_quantities, status, num_idx + 1, dp)) {
                possible_to_satisfy = true;
            }
        }

        dp[status][num_idx] = possible_to_satisfy ? 1 : -1;
        return possible_to_satisfy;
    }

public:
    // 时间复杂度 O(N_distinct * 3^m)，其中 N_distinct 是 cnt.size() (nums中不同值的数量)，m 是 quantity.size() (顾客数量)。
    // 空间复杂度 O(N_distinct * 2^m)，用于DP表。
    bool canDistribute(std::vector<int>& nums, std::vector<int>& quantity) {
        std::sort(nums.begin(), nums.end());

        // 统计nums中每种不同数字的出现频率
        std::vector<int> cnt;
        if (!nums.empty()) {
            int current_val_freq = 1;
            for (size_t i = 1; i < nums.size(); ++i) {
                if (nums[i] == nums[i-1]) {
                    current_val_freq++;
                } else {
                    cnt.push_back(current_val_freq);
                    current_val_freq = 1;
                }
            }
            cnt.push_back(current_val_freq); // 加入最后一组数字的频率
        }

        int m = quantity.size(); // 顾客/订单的数量

        if (m == 0) { // 没有订单需要满足
            return true;
        }
        // 如果有订单，但nums为空 (因此cnt也为空)，则无法满足
        if (cnt.empty()) {
            return false;
        }

        int num_customer_subsets = 1 << m; // 顾客子集的总数 (2^m)
        std::vector<int> sum_quantities(num_customer_subsets, 0);

        // 预计算每个顾客子集所需的物品总量
        // sum_quantities[mask] 表示掩码 'mask' 所代表的顾客子集所需的物品总和
        for (int i = 0; i < m; ++i) { // 遍历每个顾客 i
            int order_qty_for_customer_i = quantity[i];
            int customer_i_bit = 1 << i; // 顾客 i 对应的位
            // 遍历所有不包含顾客 i 的子集 'prev_subset_mask'
            // (prev_subset_mask 的范围是 0 到 (1<<i) - 1)
            // 通过将顾客 i 加入这些子集来构建新的子集和
            for (int prev_subset_mask = 0; prev_subset_mask < customer_i_bit; ++prev_subset_mask) {
                sum_quantities[prev_subset_mask | customer_i_bit] = sum_quantities[prev_subset_mask] + order_qty_for_customer_i;
            }
        }

        // dp[status_mask][num_idx]：表示用 cnt[num_idx] 及之后的数字种类，
        // 是否能满足 status_mask 所代表的顾客订单。
        // 初始化DP表，0表示未计算状态。
        std::vector<std::vector<int>> dp(num_customer_subsets, std::vector<int>(cnt.size(), 0));

        // 初始调用：
        // status = num_customer_subsets - 1 (二进制全1，表示所有顾客的订单都需要满足)
        // num_idx = 0 (从cnt中的第一个数字种类开始考虑)
        return f(cnt, sum_quantities, num_customer_subsets - 1, 0, dp);
    }
};
