#include <iostream>      // 包含输入输出流头文件
#include <vector>        // 包含向量头文件
#include <numeric>       // 可能有用，虽然在最终 DP 中未直接使用 (比如 accumulate)
#include <algorithm>     // 包含 std::max 和 std::fill
#include <cstdio>        // 如果需要，可用于更快的 scanf/printf，但这里使用 cin/cout

using namespace std; // 使用标准命名空间

// --- 常量与全局变量 ---

const int MAX_ITEMS = 61;     // 最大物品数量 (m <= 60) + 1 用于 1-based 索引
const int MAX_BUDGET = 32001; // 最大预算 (n <= 32000) + 1

vector<int> cost(MAX_ITEMS);      // cost[i]: 物品 i 的花费
vector<int> val(MAX_ITEMS);       // val[i]: 物品 i 的价值 (花费 * 重要度)
vector<bool> king(MAX_ITEMS);     // king[i]: 如果物品 i 是主件 (q=0)，则为 true
vector<int> fans(MAX_ITEMS, 0);   // fans[i]: 主件 i 的附件数量
vector<vector<int>> follows(MAX_ITEMS, vector<int>(2, 0)); // follows[i][0/1]: 主件 i 的附件索引 (如果没有则为 0)
vector<int> dp(MAX_BUDGET);       // 用于空间优化解法的 DP 表

int n; // 总预算
int m; // 物品总数
// 有依赖的背包(模版)
// 物品分为两大类：主件和附件
// 主件的购买没有限制，钱够就可以；附件的购买有限制，该附件所归属的主件先购买，才能购买这个附件
// 例如，若想买打印机或扫描仪这样的附件，必须先购买电脑这个主件
// 以下是一些主件及其附件的展示：
// 电脑：打印机，扫描仪 | 书柜：图书 | 书桌：台灯，文具 | 工作椅：无附件
// 每个主件最多有2个附件，并且附件不会再有附件，主件购买后，怎么去选择归属附件完全随意，钱够就可以
// 所有的物品编号都在1~m之间，每个物品有三个信息：价格v、重要度p、归属q
// 价格就是花费，价格 * 重要度 就是收益，归属就是该商品是依附于哪个编号的主件
// 比如一件商品信息为[300,2,6]，花费300，收益600，该商品是6号主件商品的附件
// 再比如一件商品信息[100,4,0]，花费100，收益400，该商品自身是主件(q==0)
// 给定m件商品的信息，给定总钱数n，返回在不违反购买规则的情况下最大的收益
// 测试链接 : https://www.luogu.com.cn/problem/P1064
// 测试链接 : https://www.nowcoder.com/practice/f9c6f980eeec43ef85be20755ddbeaf4

// --- 辅助函数 ---


// 在处理新的测试用例之前，重置每个潜在主件的附件计数。
void clean() {
    // 重置物品 1 到 m (或到 MAX_ITEMS，更安全) 的 fans 计数
    fill(fans.begin(), fans.begin() + m + 1, 0);
    // 注意: 如果 fans 为 0，follows 严格来说不需要清理，但这是好习惯：
    // for (int i = 1; i <= m; ++i) {
    //     follows[i][0] = follows[i][1] = 0;
    // }
}

// --- DP 计算 ---

// 使用严格的位置依赖计算最大价值 (效率较低)
int compute1() {
    vector<vector<int>> dp2D(m + 1, vector<int>(n + 1, 0));
    // dp2D[i][j] : 考虑索引 i 为止的主件，预算为 j 时的最大价值

    int last_main_item_idx = 0; // 上一个处理过的主件组的索引

    for (int i = 1; i <= m; ++i) {
        if (king[i]) { // 只处理主件
            int fan1_idx = (fans[i] >= 1) ? follows[i][0] : 0; // 获取附件 1 的索引 (如果没有则为 0)
            int fan2_idx = (fans[i] >= 2) ? follows[i][1] : 0; // 获取附件 2 的索引 (如果没有则为 0)

            for (int j = 0; j <= n; ++j) {
                // 选项 1: 不选择当前主件组 (继承自上一个主件的状态)
                dp2D[i][j] = dp2D[last_main_item_idx][j];

                // 选项 2: 只选择主件
                if (j >= cost[i]) {
                    dp2D[i][j] = max(dp2D[i][j], dp2D[last_main_item_idx][j - cost[i]] + val[i]);
                }

                // 选项 3: 选择主件 + 附件 1
                if (fan1_idx != 0 && j >= cost[i] + cost[fan1_idx]) {
                    dp2D[i][j] = max(dp2D[i][j], dp2D[last_main_item_idx][j - cost[i] - cost[fan1_idx]] + val[i] + val[fan1_idx]);
                }

                // 选项 4: 选择主件 + 附件 2
                if (fan2_idx != 0 && j >= cost[i] + cost[fan2_idx]) {
                    dp2D[i][j] = max(dp2D[i][j], dp2D[last_main_item_idx][j - cost[i] - cost[fan2_idx]] + val[i] + val[fan2_idx]);
                }

                // 选项 5: 选择主件 + 附件 1 + 附件 2
                if (fan1_idx != 0 && fan2_idx != 0 && j >= cost[i] + cost[fan1_idx] + cost[fan2_idx]) {
                    dp2D[i][j] = max(dp2D[i][j], dp2D[last_main_item_idx][j - cost[i] - cost[fan1_idx] - cost[fan2_idx]] + val[i] + val[fan1_idx] + val[fan2_idx]);
                }
            }
            last_main_item_idx = i; // 更新最后处理的主件索引
        } else {
            // 如果是附件，则从前一个物品复制状态
            // 这确保了如果下一个物品回溯时，dp2D[i] 保持正确的状态
             if (last_main_item_idx > 0) { // 确保我们至少处理过一个主件
                 for(int j=0; j<=n; ++j) {
                     dp2D[i][j] = dp2D[last_main_item_idx][j];
                 }
             }
             // 如果还没有看到主件 (last_main_item_idx == 0)，dp2D[i] 保持为 0 是正确的。
        }
    }
    // 结果是考虑了直到最后处理的主件为止的所有物品可实现的最大价值
    return dp2D[last_main_item_idx][n];
}


// 使用一维 DP 数组计算最大价值。
int compute2() {
    // 为当前测试用例初始化 DP 表
    fill(dp.begin(), dp.begin() + n + 1, 0);
    // dp[j] = 预算为 j 时可实现的最大价值

    for (int i = 1; i <= m; ++i) {
        if (king[i]) { // 只处理主件
            int fan1_idx = (fans[i] >= 1) ? follows[i][0] : 0; // 附件 1 索引 (如果没有则为 0)
            int fan2_idx = (fans[i] >= 2) ? follows[i][1] : 0; // 附件 2 索引 (如果没有则为 0)

            // 为了 0/1 背包的空间优化，进行反向迭代
            for (int j = n; j >= cost[i]; --j) {
                // 将主件及其潜在附件视为一个“组”

                // 选项 1: 只选择主件
                // 我们通过考虑将 *这个组* 添加到先前的状态来更新 dp[j]。
                // 组的基本情况：只选择主件。
                 dp[j] = max(dp[j], dp[j - cost[i]] + val[i]);

                // 选项 2: 选择主件 + 附件 1
                if (fan1_idx != 0 && j >= cost[i] + cost[fan1_idx]) {
                    dp[j] = max(dp[j], dp[j - cost[i] - cost[fan1_idx]] + val[i] + val[fan1_idx]);
                }

                // 选项 3: 选择主件 + 附件 2
                if (fan2_idx != 0 && j >= cost[i] + cost[fan2_idx]) {
                    dp[j] = max(dp[j], dp[j - cost[i] - cost[fan2_idx]] + val[i] + val[fan2_idx]);
                }

                // 选项 4: 选择主件 + 附件 1 + 附件 2
                if (fan1_idx != 0 && fan2_idx != 0 && j >= cost[i] + cost[fan1_idx] + cost[fan2_idx]) {
                    dp[j] = max(dp[j], dp[j - cost[i] - cost[fan1_idx] - cost[fan2_idx]] + val[i] + val[fan1_idx] + val[fan2_idx]);
                }
                // 选项 5 (不选择该组) 被隐式处理，因为如果所有 max 条件都不满足，dp[j] 会保留上一个主件迭代的值。
            }
        }
    }
    return dp[n];
}


// --- 主函数 ---
int main() {
    // 更快的 I/O (可选，但在竞赛编程中推荐使用)
    ios_base::sync_with_stdio(false); // 关闭 C++ 标准流与 C 标准流的同步
    cin.tie(NULL); // 解除 cin 与 cout 的绑定

    int v, p, q; // 用于读取输入的临时变量

    // 读取输入直到文件结束 (EOF) 或无效输入
    while (cin >> n >> m) {
        // 为新的测试用例做准备
        clean();

        // 读取物品数据
        for (int i = 1; i <= m; ++i) {
            cin >> v >> p >> q; // 读入花费 v, 重要度 p, 归属 q
            cost[i] = v;        // 记录花费
            val[i] = v * p;     // 预先计算价值 = 花费 * 重要度
            king[i] = (q == 0); // 检查是否是主件 (q==0 表示主件)

            if (!king[i]) { // 如果是附件 (q != 0)
                // 将此附件的索引 'i' 存储在其主件 'q' 的 'follows' 列表中。
                // fans[q] 跟踪下一个可用槽位 (0 或 1)，并自增。
                follows[q][fans[q]++] = i;
            }
        }

        // 使用空间优化方法计算并打印结果
        cout << compute2() << "\n";

        // 可选: 使用二维方法计算并打印以进行验证
        // cout << "Compute1 结果: " << compute1() << "\n";
    }

    return 0; // 程序正常退出
}
