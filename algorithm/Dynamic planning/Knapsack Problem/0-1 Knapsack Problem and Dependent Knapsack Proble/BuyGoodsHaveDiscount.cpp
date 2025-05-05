#include <iostream>     // 标准输入输出流
#include <vector>       // 向量容器 (虽然这里为了匹配Java用了固定数组)
#include <numeric>      // 用于 std::fill (或者 <algorithm>)
#include <algorithm>    // 用于 std::max 和 std::fill

using namespace std;

// 夏季特惠
// 某公司游戏平台的夏季特惠开始了，你决定入手一些游戏
// 现在你一共有X元的预算，平台上所有的 n 个游戏均有折扣
// 标号为 i 的游戏的原价a_i元，现价只要b_i元
// 也就是说该游戏可以优惠 a_i - b_i，并且你购买该游戏能获得快乐值为w_i
// 由于优惠的存在，你可能做出一些冲动消费导致最终买游戏的总费用超过预算
// 只要满足 : 获得的总优惠金额不低于超过预算的总金额
// 那在心理上就不会觉得吃亏。
// 现在你希望在心理上不觉得吃亏的前提下，获得尽可能多的快乐值。
// 测试链接 : https://leetcode.cn/problems/tJau2o/

const int MAXN = 501;     // 最大物品数量 + 1 (如果需要N个物品，0基索引需要N)
const int MAXX = 100001;  // 最大可能预算 + 1


// 使用 C++ 中典型的基于 0 的索引
int cost[MAXN];          // 存储需要考虑的物品的“成本”(-well)
long long val[MAXN];     // 存储需要考虑的物品的快乐值
long long dp[MAXX];      // DP 表: dp[j] = 预算为 j 时的最大快乐值

int main() {
    // 更快的 I/O 设置
    ios_base::sync_with_stdio(false); // 禁止同步 C++ 标准流和 C 标准流
    cin.tie(NULL);                   // 解除 cin 和 cout 的绑定
    cout.tie(NULL);                  // 解除 cout 的绑定

    int n; // 游戏数量
    int x; // 初始预算

    // 读取输入直到文件结束 (EOF)
    while (cin >> n >> x) {
        int m = 0;              // 需要放入背包的物品计数 (基于 0 的索引)
        long long ans = 0;      // 累加“必买”物品的快乐值
        long long current_budget_ll = x; // 临时使用 long long 存储当前预算，防止增加时溢出

        for (int i = 0; i < n; ++i) {
            int pre; // 原价
            int cur; // 折扣价
            long long happy; // 快乐值
            cin >> pre >> cur >> happy;

            // 计算 well = (优惠金额) - (花费) = (pre - cur) - cur
            // 使用 long long 进行中间计算以防止溢出
            long long well = (long long)pre - cur - cur;

            if (well >= 0) {
                // 这个物品实际上增加了预算。必买。
                // well = (pre - cur) - cur >= 0  => pre - cur >= cur
                // 意味着优惠金额不低于实际花费，心理不亏，且还能增加等效预算
                current_budget_ll += well; // 增加等效预算
                ans += happy;             // 累加快乐值
            } else {
                // 这个物品相对于其优惠有一个净成本。
                // 将其视为背包问题中的一个物品。
                // 成本 Cost = -well = cur - (pre - cur)
                // 意味着优惠金额低于实际花费，心理上会亏，这个“亏损值”就是背包物品的“成本”
                cost[m] = -well; // 记录“亏损值”作为成本
                val[m] = happy;  // 记录快乐值
                m++;             // 增加待考虑物品的数量
            }
        }

        // 更新预算 x，确保它不超过数组边界
        // 如果有效预算过大，将其限制在 dp 数组允许的最大索引
        if (current_budget_ll >= MAXX) {
             x = MAXX - 1; // 预算不能超过 dp 数组的大小限制
        } else {
             x = (int)current_budget_ll; // 如果在范围内，转回 int
        }


        // --- 0/1 背包问题 ---
        // 为当前有效预算 x 初始化 DP 表
        // 我们需要 dp[0] 到 dp[x] 的值
        fill(dp, dp + x + 1, 0LL); // 将 dp[0...x] 初始化为 0 (使用 0LL 表示 long long 类型的 0)

        // 使用 0/1 背包逻辑填充 DP 表
        for (int i = 0; i < m; ++i) {           // 遍历 'm' 个待考虑的物品
            for (int j = x; j >= cost[i]; --j) { // 从预算 x 向下遍历到物品的成本
                // 决定是否包含物品 i
                // dp[j] 的值要么是：
                // 1. 不选物品 i，保持原来的 dp[j]
                // 2. 选择物品 i，值为 dp[j - cost[i]] (花费 cost[i] 之前的最大快乐值) + val[i] (物品 i 的快乐值)
                dp[j] = max(dp[j], dp[j - cost[i]] + val[i]);
            }
        }

        // 最终答案是 "必买" 物品的快乐值加上
        // 在最终预算 x 内，通过背包问题能获得的最大快乐值
        cout << ans + dp[x] << "\n"; // 输出总快乐值
    }

    return 0; // 程序正常结束
}
