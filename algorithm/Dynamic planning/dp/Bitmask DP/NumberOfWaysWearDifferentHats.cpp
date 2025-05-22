#include <vector>
#include <numeric>   // 对于此版本并非严格需要，但通常很有用
#include <algorithm> // 用于 std::max

// 每个人戴不同帽子的方案数
// 总共有 n 个人和 40 种不同的帽子，帽子编号从 1 到 40
// 给你一个整数列表的列表 hats ，其中 hats[i] 是第 i 个人所有喜欢帽子的列表
// 请你给每个人安排一顶他喜欢的帽子，确保每个人戴的帽子跟别人都不一样，并返回方案数
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/number-of-ways-to-wear-different-hats-to-each-other

// 全局常量，用于模运算
const int MOD = 1000000007;

class Solution {
public:
    // hats_preferences: 一个二维向量，hats_preferences[i] 是第 i 个人所有喜欢帽子的列表
    // 返回值: 每个人戴不同帽子的方案数
    int numberWays(const std::vector<std::vector<int>>& hats_preferences) {
        int max_hat_id = 0;
        for (const auto& person_prefs : hats_preferences) {
            for (int hat : person_prefs) {
                max_hat_id = std::max(max_hat_id, hat);
            }
        }

        int num_people = hats_preferences.size();
        if (num_people == 0) {
            return 1; // 没有人，一种方案 (不分配帽子)
        }

        // hat_to_people_map[hat_id] 存储一个位掩码，表示喜欢 hat_id 的人。
        // 如果第 'p' 个人喜欢 hat_id，则其对应的位 'p' 被设置。
        // 帽子 ID 是从 1 开始的，所以大小是 max_hat_id + 1。
        std::vector<int> hat_to_people_map(max_hat_id + 1, 0);
        for (int person_idx = 0; person_idx < num_people; ++person_idx) {
            for (int hat_id : hats_preferences[person_idx]) {
                hat_to_people_map[hat_id] |= (1 << person_idx);
            }
        }

        // 记忆化搜索表: memo[hat_idx][assigned_mask]
        // 存储从 hat_idx 号帽子开始分配，当 assigned_mask 中的人已经分配了帽子时的方案数。
        // 帽子索引从 1 到 max_hat_id。
        // hat_idx 的表大小需要能容纳到 max_hat_id。
        // 递归函数使用 current_hat_idx + 1 进行调用。
        // 基本情况是 current_hat_idx > max_hat_id。
        // 因此，memo 需要能索引到 max_hat_id。
        // LeetCode题目中帽子编号从1到40，所以max_hat_id最大是40。
        // dp表的第一维对应帽子，最大可以取到max_hat_id，所以大小应该是max_hat_id+1（如果索引用0到max_hat_id）。
        // 或者，如果索引用1到max_hat_id，递归时传入的current_hat_idx最大为max_hat_id+1（作为退出条件），则表大小应为max_hat_id+2。
        // 当前实现中，current_hat_idx从1开始，当 current_hat_idx > max_hat_id 时退出，
        // 因此 memo[current_hat_idx] 的有效索引是 1 到 max_hat_id。
        // memo 的大小设置为 max_hat_id + 1 是因为我们用 current_hat_idx 直接作为索引，且其值从1开始。
        std::vector<std::vector<int>> memo(max_hat_id + 1, std::vector<int>(1 << num_people, -1));

        // 参数：喜欢帽子的映射，最大帽子ID，总人数，当前考虑的帽子ID(从1开始)，已分配帽子的人员状态(初始为0)，记忆化表
        return solve(hat_to_people_map, max_hat_id, num_people, 1, 0, memo);
    }

private:
    // hat_to_people_map: 将帽子 ID 映射到喜欢该帽子的人的位掩码。
    // max_hat_id: 最大的帽子 ID。
    // num_people: 总人数。
    // current_hat_idx: 当前正在考虑的帽子 ID (从 1 到 max_hat_id)。
    // assigned_people_mask: 已经分配了帽子的人的位掩码。
    // memo: 记忆化搜索表。
    // 返回值: 在当前状态下的分配方案数
    int solve(const std::vector<int>& hat_to_people_map,
              int max_hat_id,
              int num_people,
              int current_hat_idx,
              int assigned_people_mask,
              std::vector<std::vector<int>>& memo) {

        // 基本情况：所有人都已经分配了帽子。
        if (assigned_people_mask == (1 << num_people) - 1) {
            return 1; // 找到一种有效方案
        }

        // 基本情况：没有更多帽子可以考虑，但并非所有人都已分配帽子。
        if (current_hat_idx > max_hat_id) {
            return 0; // 无法满足所有人
        }

        // 检查记忆化搜索表
        if (memo[current_hat_idx][assigned_people_mask] != -1) {
            return memo[current_hat_idx][assigned_people_mask];
        }

        // 方案1：跳过 current_hat_idx 号帽子 (不将其分配给任何人)。
        // 递归地用下一个帽子寻找方案。
        long long ways = solve(hat_to_people_map, max_hat_id, num_people,
                               current_hat_idx + 1, assigned_people_mask, memo);

        // 方案2：将 current_hat_idx 号帽子分配给一个喜欢它且尚未戴帽子的人。
        int people_liking_current_hat = hat_to_people_map[current_hat_idx]; // 喜欢当前帽子的人的位掩码

        // 遍历每个喜欢当前帽子的人 (用一个位表示)。
        // 这里使用了类似于 Brian Kernighan 算法的技术来分离出设置的位。
        int temp_person_mask = people_liking_current_hat;
        while (temp_person_mask > 0) {
            // 分离出最右边的 '1' 位 (例如，如果 temp_person_mask = 0...01010, person_bit = 0...00010)
            // person_bit 代表一个喜欢当前帽子的人的掩码 (只有一位是1)
            int person_bit = temp_person_mask & (-temp_person_mask);

            // 如果这个人 (person_bit) 还没有被分配帽子 (即在 assigned_people_mask 中对应的位是0)
            if ((assigned_people_mask & person_bit) == 0) {
                // 将 current_hat_idx 号帽子分配给这个人，并为下一个帽子进行递归。
                // 更新 assigned_people_mask，将 person_bit 对应的位设为1。
                ways = (ways + solve(hat_to_people_map, max_hat_id, num_people,
                                     current_hat_idx + 1, assigned_people_mask | person_bit, memo)) % MOD;
            }
            // 从 temp_person_mask 中移除这个人 (将最右边的1置0)，以便处理下一个喜欢这顶帽子的人。
            temp_person_mask ^= person_bit;
        }

        // 将结果存储在记忆化搜索表中并返回。
        return memo[current_hat_idx][assigned_people_mask] = static_cast<int>(ways);
    }
    int solve2(const std::vector<int>& hat_to_people_map,
             int max_hat_id,
             int num_people,
             int current_hat_idx,
             int assigned_people_mask,
             std::vector<std::vector<int>>& memo) {
        // (参数与 solve 函数类似)
        // hat_to_people_map: 帽子ID到喜欢它的人的位掩码的映射
        // max_hat_id: 最大帽子ID
        // num_people: 总人数
        // current_hat_idx: 当前考虑的帽子ID
        // assigned_people_mask: 已分配帽子的人的位掩码
        // memo: 记忆化表

        if (assigned_people_mask == (1 << num_people) - 1) { // 所有人都分配了帽子
            return 1;
        }
        if (current_hat_idx > max_hat_id) { // 没有更多帽子，但人未分配完
            return 0;
        }
        if (memo[current_hat_idx][assigned_people_mask] != -1) { // 查表
            return memo[current_hat_idx][assigned_people_mask];
        }

        // 方案1: 当前帽子不分配
        long long ways = solve2(hat_to_people_map, max_hat_id, num_people,
                                  current_hat_idx + 1, assigned_people_mask, memo);

        int people_liking_current_hat = hat_to_people_map[current_hat_idx]; // 喜欢当前帽子的人
        // 方案2: 当前帽子分配给一个可能的人
        for (int p_idx = 0; p_idx < num_people; ++p_idx) { // 遍历每一个人
            // 检查第 p_idx 个人是否喜欢当前帽子: (people_liking_current_hat >> p_idx) & 1
            // 检查第 p_idx 个人是否还未分配帽子: (assigned_people_mask >> p_idx) & 1 == 0
            if (((people_liking_current_hat >> p_idx) & 1) != 0 &&
                !((assigned_people_mask >> p_idx) & 1) ) { // 等价于 (assigned_people_mask & (1 << p_idx)) == 0
                ways = (ways + solve2(hat_to_people_map, max_hat_id, num_people,
                                        current_hat_idx + 1, assigned_people_mask | (1 << p_idx), memo)) % MOD;
                }
        }
        return memo[current_hat_idx][assigned_people_mask] = static_cast<int>(ways);
    }
};





