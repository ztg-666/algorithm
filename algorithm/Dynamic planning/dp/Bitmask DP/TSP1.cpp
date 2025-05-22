#include <iostream>    // 标准输入输出流



// 售货员的难题 - TSP问题
// 某乡有n个村庄(1<=n<=20)，有一个售货员，他要到各个村庄去售货
// 各村庄之间的路程s(1<=s<=1000)是已知的
// 且A村到B村的路程，与B到A的路大多不同(有向带权图)
// 为了提高效率，他从商店出发到每个村庄一次，然后返回商店所在的村，
// 假设商店所在的村庄为1
// 他不知道选择什么样的路线才能使所走的路程最短
// 请你帮他选择一条最短的路
// 测试链接 : https://www.luogu.com.cn/problem/P1171

// 全局常量和数组
const int MAX_N_INPUT = 20; // 问题规定的最大村庄数 (1 <= n <= 20)

int graph[MAX_N_INPUT][MAX_N_INPUT];    // 邻接矩阵，存储村庄间的距离
int dp[1 << MAX_N_INPUT][MAX_N_INPUT]; // DP 表: dp[mask][last_village_index]
                                       // mask 表示已访问村庄的集合，last_village_index 表示当前所在的村庄
int n_actual;                          // 当前测试用例的实际村庄数量

// 初始化 DP 表，将所有条目设为 -1 (表示状态尚未计算)
void initialize_dp() {
    for (int i = 0; i < (1 << n_actual); ++i) {
        for (int j = 0; j < n_actual; ++j) {
            dp[i][j] = -1;
        }
    }
}

// 使用动态规划和位掩码解决 TSP 的递归函数
// status: 一个位掩码，表示已访问村庄的集合。
//         如果第 k 位被设置，则村庄 k 已被访问。
// current_village_idx: 当前路径中最后访问的村庄的索引。
int solve(int status, int current_village_idx) {
    // 基本情况：所有村庄都已访问
    if (status == (1 << n_actual) - 1) {
        // 所有村庄都已访问，返回从 current_village_idx 回到起始村庄 (0) 的成本
        return graph[current_village_idx][0];
    }

    // 如果这个状态已经计算过，返回存储的结果
    if (dp[status][current_village_idx] != -1) {
        return dp[status][current_village_idx];
    }

    int min_cost = INT_MAX; // 初始化最小成本为最大整数

    // 遍历所有可能的下一个村庄
    for (int next_village_idx = 0; next_village_idx < n_actual; ++next_village_idx) {
        // 检查 next_village_idx 是否尚未被访问
        // (status & (1 << next_village_idx)) == 0 表示 next_village_idx 对应的位是 0
        if (!((status >> next_village_idx) & 1)) { // 等价于 (status & (1 << next_village_idx)) == 0
            int current_path_cost = graph[current_village_idx][next_village_idx] +
                                   solve(status | (1 << next_village_idx), next_village_idx);
            // 更新最小成本
            if (current_path_cost < min_cost) {
                min_cost = current_path_cost;
            }
        }
    }

    // 存储并返回当前状态的最小成本
    dp[status][current_village_idx] = min_cost;
    return min_cost;
}

int main() {
    // C++ 快速 I/O 设置
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);


    // std::cin >> n_actual 会实现这一点；当遇到 EOF 或无效输入时，它会失败 (循环终止)。
    while (std::cin >> n_actual) {
        if (n_actual == 0) { // 问题约束是 1 <= n <= 20，所以 n=0 可能是一个 EOF 标记或无效输入。
            break;
        }

        initialize_dp(); // 为新的测试用例清空 DP 表

        // 读取图的邻接矩阵
        for (int i = 0; i < n_actual; ++i) {
            for (int j = 0; j < n_actual; ++j) {
                std::cin >> graph[i][j];
            }
        }

        // 问题从“村庄1”（商店）开始，在0索引数组中即为村庄0。
        // 对 solve 的初始调用：
        // - status 是 (1 << 0) = 1 (只有村庄0被访问)。
        // - current_village_idx 是 0 (我们当前在村庄0)。
        // 如果 n_actual 是 1, solve(1, 0) 会根据基本情况正确返回 graph[0][0]。
        std::cout << solve(1, 0) << std::endl;
    }

    return 0;
}

