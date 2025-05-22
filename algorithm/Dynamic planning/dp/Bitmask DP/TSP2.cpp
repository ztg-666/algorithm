#include <iostream>
#include <vector>
#include <algorithm> // For std::min
#include <limits>    // For std::numeric_limits<int>::max()

// “其他”村庄的最大数量 (总村庄数 - 1)
// 如果总村庄数 N_max = 20, 那么 other_N_max = 19.
const int MAX_OTHER_VILLAGES = 19;

// 从实际起点（商店）到每个“其他”村庄的成本
int costs_from_shop[MAX_OTHER_VILLAGES];

// 从每个“其他”村庄回到实际起点（商店）的成本
int costs_to_shop[MAX_OTHER_VILLAGES];

// “其他”村庄之间的成本图
int graph_others[MAX_OTHER_VILLAGES][MAX_OTHER_VILLAGES];

// 针对“其他”村庄路径的DP表
// dp[mask][last_other_village_idx]
// mask: 已访问的“其他”村庄的位掩码
// last_other_village_idx: 在“其他”村庄中最后访问的那个村庄的索引
int dp[1 << MAX_OTHER_VILLAGES][MAX_OTHER_VILLAGES];

int n_others; // 当前测试用例中“其他”村庄的实际数量 (N_total - 1)

// 初始化“其他”村庄的DP表
void initialize_dp_others() {
    for (int s = 0; s < (1 << n_others); ++s) {
        for (int i = 0; i < n_others; ++i) {
            dp[s][i] = -1; // -1 表示状态未计算
        }
    }
}

// 解决“其他”村庄的TSP问题
// status: 已访问的“其他”村庄的位掩码
// current_other_village_idx: “其他”村庄中，当前所在村庄的索引
int solve_tsp_for_others(int status, int current_other_village_idx) {
    // 基本情况：所有“其他”村庄都已访问
    if (status == (1 << n_others) - 1) {
        // 返回从 current_other_village_idx 回到商店的成本
        return costs_to_shop[current_other_village_idx];
    }

    // 如果此状态已计算过，则返回存储的结果
    if (dp[status][current_other_village_idx] != -1) {
        return dp[status][current_other_village_idx];
    }

    int min_total_cost_from_here = std::numeric_limits<int>::max();

    // 遍历所有可能的下一个“其他”村庄
    for (int next_other_village_idx = 0; next_other_village_idx < n_others; ++next_other_village_idx) {
        // 如果 next_other_village_idx 尚未在 status 中 (即未访问)
        if (!((status >> next_other_village_idx) & 1)) {
            int path_cost = graph_others[current_other_village_idx][next_other_village_idx] +
                            solve_tsp_for_others(status | (1 << next_other_village_idx), next_other_village_idx);
            if (path_cost < min_total_cost_from_here) {
                min_total_cost_from_here = path_cost;
            }
        }
    }
    dp[status][current_other_village_idx] = min_total_cost_from_here;
    return min_total_cost_from_here;
}


int calculate_overall_shortest_path() {
    int overall_min_ans = std::numeric_limits<int>::max();

    // 如果没有“其他”村庄 (即总共只有商店一个村庄)，此循环不执行。
    // 这种情况已在 main 函数中处理。
    if (n_others == 0) { // 对应总村庄数为1的情况
        return 0; // 逻辑上路径为0
    }

    // 遍历每个“其他”村庄，将其作为离开商店后的第一个停留点
    for (int first_other_stop_idx = 0; first_other_stop_idx < n_others; ++first_other_stop_idx) {
        // 成本 = (商店 到 first_other_stop) +
        //        (在“其他”村庄中以 first_other_stop 为起点进行TSP，然后返回商店的成本)
        int current_path_total = costs_from_shop[first_other_stop_idx] +
                                 solve_tsp_for_others(1 << first_other_stop_idx, first_other_stop_idx);
        if (current_path_total < overall_min_ans) {
            overall_min_ans = current_path_total;
        }
    }
    return overall_min_ans;
}

// 主函数，处理输入和输出
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n_total_villages;
    while (std::cin >> n_total_villages) {
        if (n_total_villages <= 0) { // 处理可能的无效输入或EOF的某种形式
             break;
        }

        if (n_total_villages == 1) {
            // 如果只有一个村庄（商店），TSP 路径长度为 0。
            // 这里我们输出逻辑上的正确值 0。
            std::cout << 0 << std::endl;
            // 如果输入格式保证 N=1 时后面没有矩阵数据，则 continue 是安全的。
            // 如果 N=1 后面仍有（无效的）矩阵数据，则需要根据具体情况消耗掉它们。
            // 假设对于N=1，输入就是 "1"，然后是下一个测试用例或EOF。
            continue;
        }

        n_others = n_total_villages - 1;
        initialize_dp_others();


        // 读取从商店出发的那一行距离
        // 然后在读取 `start[i]` 的循环之前还有一个 `in.nextToken();`
        // 这第一个 `in.nextToken()` 消耗了商店到商店自身的距离（通常是0），不被使用。
        int shop_to_shop_distance_ignored;
        std::cin >> shop_to_shop_distance_ignored;

        for (int i = 0; i < n_others; ++i) {
            std::cin >> costs_from_shop[i]; // 从商店到“其他村庄i”的成本
        }

        // 读取“其他”村庄的行
        for (int i = 0; i < n_others; ++i) { // 对应原始矩阵中“其他村庄”的行
            std::cin >> costs_to_shop[i];    // 从“其他村庄i”回到商店的成本（该行第一个元素）
            for (int j = 0; j < n_others; ++j) {
                std::cin >> graph_others[i][j]; // “其他村庄i”到“其他村庄j”的成本
            }
        }

        std::cout << calculate_overall_shortest_path() << std::endl;
    }
    return 0;
}

