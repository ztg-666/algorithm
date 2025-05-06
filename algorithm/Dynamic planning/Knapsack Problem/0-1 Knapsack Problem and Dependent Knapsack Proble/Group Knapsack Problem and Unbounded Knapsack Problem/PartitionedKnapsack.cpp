#include <iostream> // 用于标准输入输出流
#include <vector>   // 用于使用动态数组 vector
#include <algorithm> // 用于 sort 和 max 函数
#include <numeric>   // 在某些上下文中可能需要 std::max（尽管 <algorithm> 通常足够）
// 分组背包(模版)
// 给定一个正数m表示背包的容量，有n个货物可供挑选
// 每个货物有自己的体积(容量消耗)、价值(获得收益)、组号(分组)
// 同一个组的物品只能挑选1件，所有挑选物品的体积总和不能超过背包容量
// 怎么挑选货物能达到价值最大，返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1757

// 定义常量，类似于 Java 中的 MAXN 和 MAXM
const int MAXN = 1001; // 最大物品数量 + 1
const int MAXM = 1001; // 最大背包容量 + 1

// 定义结构体来存储物品属性
struct Item {
    int v; // 体积 (volume)
    int w; // 价值 (worth)
    int g; // 组号 (group id)
};

// 全局变量，类似于 Java 代码
int m, n;                 // m: 背包容量, n: 物品数量
std::vector<Item> arr(MAXN); // 存储物品的向量 (使用从 1 开始的索引以便直接翻译)
std::vector<int> dp_1d(MAXM); // 用于空间优化版 (一维 DP) 的 DP 表

// 严格位置依赖的动态规划函数 (二维 DP 表)
int compute1() {
    if (n == 0) return 0; // 处理没有物品的边界情况

    // 计算不同组的总数
    int teams = 1; // 组数
    // 注意：排序在此函数调用之前完成。
    // 在 main 函数中，物品已按组号排序。
    for (int i = 2; i <= n; ++i) {
        if (arr[i - 1].g != arr[i].g) {
            teams++;
        }
    }

    // dp_2d[i][j]: 表示只考虑前 'i' 组物品，在容量不超过 'j' 的情况下能获得的最大价值
    // 初始化 DP 表为 0
    std::vector<std::vector<int>> dp_2d(teams + 1, std::vector<int>(m + 1, 0));

    int current_group_index = 1; // DP 表中组维度的当前索引
    int start = 1; // 已排序的 'arr' 中当前组的起始索引

    while (start <= n) {
        int end = start;
        // 找到当前组的结束索引
        while (end + 1 <= n && arr[end + 1].g == arr[start].g) {
            end++;
        }
        // arr[start ... end] 是当前组的所有物品

        // 处理第 'current_group_index' 组（物品索引从 'start' 到 'end'）
        for (int j = 0; j <= m; ++j) {
            // 决策 1：不选择当前组的任何物品
            // 那么最大价值等于只考虑前 i-1 组，容量为 j 时的最大价值
            dp_2d[current_group_index][j] = dp_2d[current_group_index - 1][j];

            // 决策 2：尝试从当前组中选择一个物品 'k'
            for (int k = start; k <= end; ++k) {
                // 检查物品 'k' 是否能放入背包 (当前容量 j >= 物品 k 的体积)
                if (j >= arr[k].v) {
                    // 如果选择物品 'k' 能得到更大的价值，则更新 dp 值
                    // dp_2d[current_group_index - 1][j - arr[k].v] 表示不选物品k，在前i-1组中，容量为j-v[k]的最大价值
                    // 加上 arr[k].w 就是选了物品k的总价值
                    dp_2d[current_group_index][j] = std::max(dp_2d[current_group_index][j],
                                                             dp_2d[current_group_index - 1][j - arr[k].v] + arr[k].w);
                }
            }
        }

        // 移动到下一组
        start = end + 1;
        current_group_index++;
    }

    // 最终结果存储在考虑了所有组 (teams) 且容量为 m 的状态中
    return dp_2d[teams][m];
}

// 空间优化的动态规划函数 (一维 DP 表)
int compute2() {
     if (n == 0) return 0; // 处理没有物品的边界情况

    // 初始化一维 DP 表为 0
    std::fill(dp_1d.begin(), dp_1d.begin() + m + 1, 0);
    // dp_1d[j] 表示在当前组之前的决策中，容量为 j 时的最大价值

    int start = 1; // 已排序 'arr' 中当前组的起始索引

    // 物品已在 main 函数中按组号排序
    while (start <= n) {
        int end = start;
        // 找到当前组的结束索引
        while (end + 1 <= n && arr[end + 1].g == arr[start].g) {
            end++;
        }
        // arr[start ... end] 是当前组的所有物品

        // 处理当前组（物品索引从 'start' 到 'end'）
        // 必须从大到小遍历容量 j (这是空间优化的关键)
        // 确保计算 dp_1d[j] 时使用的 dp_1d[j - arr[k].v] 是上一组（或更早组）的状态
        for (int j = m; j >= 0; --j) {
            // 尝试选择当前组中的每一个物品 'k'
            for (int k = start; k <= end; ++k) {
                // 检查物品 'k' 是否能放入背包
                if (j >= arr[k].v) {
                    // dp_1d[j] 当前存储的是不选当前组物品k，容量为j的最大价值
                    // dp_1d[j - arr[k].v] 存储的是不选物品k，在前若干组中，容量为j-v[k]的最大价值
                    // arr[k].w + dp_1d[j - arr[k].v] 是选了物品k的总价值
                    dp_1d[j] = std::max(dp_1d[j], arr[k].w + dp_1d[j - arr[k].v]);
                }
            }
        }
        // 移动到下一组
        start = end + 1;
    }

    // 最终结果存储在 dp_1d[m] 中
    return dp_1d[m];
}

int main() {
    // 加速输入输出
    std::ios_base::sync_with_stdio(false); // 关闭 C++ 标准流与 C 标准流的同步
    std::cin.tie(NULL); // 解除 cin 和 cout 的绑定
    std::cout.tie(NULL); // 解除 cout 和 cin 的绑定 (有时对输出加速有用)

    // 循环读取输入直到文件结束 (EOF)，类似于 Java 的循环
    while (std::cin >> m >> n) { // 读取背包容量和物品数量
        // 读取物品详情 (使用从 1 开始的索引)
        for (int i = 1; i <= n; ++i) {
            std::cin >> arr[i].v >> arr[i].w >> arr[i].g; // 读取体积、价值、组号
        }

        // 根据组号对物品进行排序 (arr[1] 到 arr[n])
        // Lambda 函数定义了基于组号 'g' 的比较规则
        std::sort(arr.begin() + 1, arr.begin() + n + 1, [](const Item& a, const Item& b) {
            return a.g < b.g; // 按组号升序排序
        });

        // 选择要使用的计算函数：
        // std::cout << compute1() << std::endl; // 使用二维 DP 版本
         std::cout << compute2() << std::endl; // 使用一维 DP (空间优化) 版本

    }

    return 0; // 程序正常退出
}
