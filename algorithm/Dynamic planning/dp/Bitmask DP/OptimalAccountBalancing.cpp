#include <vector>
#include <numeric>   // 如果需要，可用于 std::vector 初始化，但此处并非严格需要
#include <algorithm> // 用于 std::fill, std::max
#include <map>       // 可用于债务记录，但对于固定范围的MAX_PERSON_ID_RANGE，向量也适用

// 最优账单平衡
// 给你一个表示交易的数组 transactions
// 其中 transactions[i] = [fromi, toi, amounti]
// 表示 ID = fromi 的人给 ID = toi 的人共计 amounti
// 请你计算并返回还清所有债务的最小交易笔数
// 测试链接 : https://leetcode.cn/problems/optimal-account-balancing/

const int MAX_PERSON_ID_RANGE = 13; // 最大人员ID + 1 (人员ID范围 0 到 12)

// debt: 存储非零余额的向量
// set_mask: 位掩码，表示来自 'debt' 向量的当前人员子集
// current_sum_to_balance: 当前组试图达成的目标总和 (或者说，当前组还需要多少钱才能平衡)
// num_people_with_debt: 'debt' 向量中的总人数（即 'debt' 的大小）
// dp: 记忆化表格，dp[set_mask] 存储 f(debt, set_mask, 0, ...) 的结果
int solve_recursive(const std::vector<int>& debt, int set_mask, int current_sum_to_balance, int num_people_with_debt, std::vector<int>& dp){
    // 如果这个子问题的结果已经计算过，则直接返回
    if (dp[set_mask] != -1) {
        return dp[set_mask];
    }

    int max_groups_found = 0; // 初始化找到的最大组数为0

    // 基本情况：如果 set_mask 中有 0 或 1 个人，(set_mask & (set_mask - 1)) 将为 0。
    // 在这种情况下，max_groups_found 保持为 0，这是正确的，因为 f 计算的是能够形成的“零和组”的数量。
    // 一个或零个人无法自行形成一个需要多方平衡的组。
    if ((set_mask & (set_mask - 1)) != 0) { // 当集合中人数大于1时为真
        if (current_sum_to_balance == 0) {
            // 我们正在尝试开始一个新的零和组。
            // 从 set_mask 中任意选择一个人 'i' 作为这个新组的起点。
            // 递归调用将尝试完成这个组（通过平衡掉 -debt[i]）
            // 然后继续在剩下的人中寻找更多的组。因为开启了一个新组，所以结果 +1。
            // 对于能形成的最大组数 S 来说结果是一样的。
            for (int i = 0; i < num_people_with_debt; ++i) {
                if ((set_mask & (1 << i)) != 0) { // 如果人员 'i' 在当前 set_mask 中
                    // 移除 person 'i'，并让剩余的人尝试平衡掉 person 'i' 的债务
                    max_groups_found = solve_recursive(debt, set_mask ^ (1 << i), current_sum_to_balance - debt[i], num_people_with_debt, dp) + 1;
                    break;
                }
            }
        } else {
            // 我们正在尝试完成当前已经开始的组，这个组还需要 'current_sum_to_balance' 才能平衡。
            // 遍历 set_mask 中的每一个人 'i'。
            // 尝试将人员 'i' 加入当前正在形成的组中。
            // 对剩下的人和新的需要平衡的总和 (current_sum_to_balance - debt[i]) 进行递归调用。
            // 取所有这些尝试中的最大值。
            for (int i = 0; i < num_people_with_debt; ++i) {
                if ((set_mask & (1 << i)) != 0) { // 如果人员 'i' 在当前 set_mask 中
                    max_groups_found = std::max(max_groups_found, solve_recursive(debt, set_mask ^ (1 << i), current_sum_to_balance - debt[i], num_people_with_debt, dp));
                }
            }
        }
    }

    dp[set_mask] = max_groups_found; // 存储结果
    return max_groups_found;
}


std::vector<int> calculate_net_debts(const std::vector<std::vector<int>>& transactions) {
    std::vector<int> person_balances(MAX_PERSON_ID_RANGE, 0); // 存储每个人的净余额
    for (const auto& tran : transactions) {
        person_balances[tran[0]] -= tran[2]; // 付款人余额减少
        person_balances[tran[1]] += tran[2]; // 收款人余额增加
    }

    std::vector<int> non_zero_debts; // 只存储非零的净余额
    for (int balance : person_balances) {
        if (balance != 0) {
            non_zero_debts.push_back(balance);
        }
    }
    return non_zero_debts;
}


int minTransfers(const std::vector<std::vector<int>>& transactions) {
    std::vector<int> debt_list = calculate_net_debts(transactions);

    if (debt_list.empty()) {
        return 0; // 没有债务，所以是0笔交易
    }

    int num_people_with_debt = debt_list.size(); // 有净债务的人数
    // dp 表的大小是 2^N，其中 N 是有非零债务的人数
    std::vector<int> dp(1 << num_people_with_debt, -1); // 初始化dp表，-1表示未计算

    // 结果是 N - S，其中 N 是有债务的人数，S 是能形成的最大零和组的数量。
    // solve_recursive 函数计算 S。
    // 对 solve_recursive 的初始调用是针对所有负债人员的完整集合 (set_mask 全为1)
    // 并且 current_sum_to_balance 为 0 (意味着开始尝试形成一个新的组)。
    int max_zero_sum_groups = solve_recursive(debt_list, (1 << num_people_with_debt) - 1, 0, num_people_with_debt, dp);

    return num_people_with_debt - max_zero_sum_groups;
}



#include <iostream>
int main() {
    std::vector<std::vector<int>> transactions1 = {{0, 1, 10}, {2, 0, 5}};
    std::cout << " 1: " << minTransfers(transactions1) << std::endl; // 预期: 2

    std::vector<std::vector<int>> transactions2 = {{0, 1, 10}, {1, 0, 1}, {1, 2, 5}, {2, 0, 5}};
    std::cout << " 2: " << minTransfers(transactions2) << std::endl; // 预期: 1

    std::vector<std::vector<int>> transactions3 = {{0,1,2},{1,2,1},{2,0,3}}; // debt: p0: -2+3=1, p1: 2-1=1, p2: 1-3=-2 => {1,1,-2}
    std::cout << " 3: " << minTransfers(transactions3) << std::endl; // 预期: 3-1=2 (因为 {1,1,-2} 可以形成一个零和组)

    std::vector<std::vector<int>> transactions4 = {{0,1,1},{0,2,1},{0,3,1},{0,4,1}}; // debt: p0: -4, p1:1, p2:1, p3:1, p4:1
    std::cout << " 4: " << minTransfers(transactions4) << std::endl; // 预期: 5-1 = 4 (因为 {-4,1,1,1,1} 可以形成一个零和组)
    return 0;
}

