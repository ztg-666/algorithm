#include <iostream>
#include <vector>
#include <cstdio> // For scanf and printf (fast I/O)
#include <numeric> // For std::fill (optional, can use loops)

// 使用标准命名空间以简化代码
using namespace std;
// 节点数为n高度不大于m的二叉树个数
// 现在有n个节点，计算出有多少个不同结构的二叉树
// 满足节点个数为n且树的高度不超过m的方案
// 因为答案很大，所以答案需要模上1000000007后输出
// 测试链接 : https://www.nowcoder.com/practice/aaefe5896cce4204b276e213e725f3ea
// 最大节点数和高度限制 (根据题目或Java代码设定)
const int MAXN = 51;
// 模数
const int MOD = 1000000007;

// --- 方法 1: 记忆化搜索 ---
// dp1[i][j] 存储节点数为 i，高度不超过 j 的二叉树数量
vector<vector<long long>> dp1(MAXN, vector<long long>(MAXN, -1));

// 计算节点数为 n，高度不超过 m 的二叉树数量
long long compute1_recursive(int n, int m) {
    // 基本情况 1: 0 个节点，只有一种结构 (空树)，高度为 0
    if (n == 0) {
        return 1;
    }
    // 基本情况 2: n > 0 但高度限制为 0，无法形成树
    if (m == 0) {
        return 0;
    }
    // 检查记忆化表
    if (dp1[n][m] != -1) {
        return dp1[n][m];
    }

    long long ans = 0;
    // 枚举左子树的节点数 k (从 0 到 n-1)
    // n 个节点，头节点占掉 1 个
    for (int k = 0; k < n; k++) {
        // 右子树节点数为 n - 1 - k
        // 左右子树的高度都不能超过 m-1
        long long left_ways = compute1_recursive(k, m - 1);
        long long right_ways = compute1_recursive(n - 1 - k, m - 1);
        // 乘法原理，并取模
        ans = (ans + (left_ways * right_ways) % MOD) % MOD;
    }

    // 存入记忆化表并返回
    dp1[n][m] = ans;
    return ans;
}

// 方法1 的入口函数
int compute1(int n, int m) {
    // 如果需要多次调用，确保 dp1 在每次主调用前被恰当重置为 -1
    // (这里假设每次调用 main 中的循环都是独立计算)
    // for(int i=0; i<=n; ++i) fill(dp1[i].begin(), dp1[i].begin() + m + 1, -1); // 更精确的重置范围
    return (int)compute1_recursive(n, m);
}


// --- 方法 2: 严格位置依赖的动态规划 (二维 DP 表) ---
// dp2[i][j] 存储节点数为 i，高度不超过 j 的二叉树数量
vector<vector<long long>> dp2(MAXN, vector<long long>(MAXN, 0));

int compute2(int n, int m) {
    // 初始化：0 个节点，高度任意，都只有 1 种 (空树)
    for (int j = 0; j <= m; ++j) {
        dp2[0][j] = 1;
    }

    // 填 DP 表
    // i: 节点数 (从 1 到 n)
    for (int i = 1; i <= n; ++i) {
        // j: 高度限制 (从 1 到 m)
        for (int j = 1; j <= m; ++j) {
            dp2[i][j] = 0; // 初始化当前状态
            // k: 左子树节点数 (从 0 到 i-1)
            for (int k = 0; k < i; ++k) {
                // 右子树节点数: i - 1 - k
                // 左右子树高度限制: j - 1
                long long left_ways = dp2[k][j - 1];
                long long right_ways = dp2[i - 1 - k][j - 1];
                dp2[i][j] = (dp2[i][j] + (left_ways * right_ways) % MOD) % MOD;
            }
        }
    }
    return (int)dp2[n][m];
}

// --- 方法 3: 严格位置依赖的动态规划 + 空间压缩 (一维 DP 数组) ---
// dp3[i] 在计算高度 j 时，存储节点数为 i，高度不超过 j-1 的二叉树数量
vector<long long> dp3(MAXN);

int compute3(int n, int m) {
    // 初始化: 对应高度为 0 的情况
    fill(dp3.begin(), dp3.end(), 0); // 先全部置 0
    dp3[0] = 1; // 0 个节点有 1 种方式 (空树)

    // 外层循环：高度限制 j (从 1 到 m)
    for (int j = 1; j <= m; ++j) {
        // 内层循环：节点数 i (从 n 到 1，必须倒序)
        // 因为计算 dp3[i] (代表高度 j) 需要用到 dp3[k] 和 dp3[i-1-k] (代表高度 j-1)
        // 倒序确保我们使用的是上一轮高度 (j-1) 的结果
        for (int i = n; i >= 1; --i) {
            // 计算 dp3[i] 对于当前高度 j 的值
            dp3[i] = 0; // 重置 dp3[i] 以累加当前高度的结果
            // k: 左子树节点数 (从 0 到 i-1)
            for (int k = 0; k < i; ++k) {
                // 右子树节点数: i - 1 - k
                // 左右子树都依赖上一高度 (j-1) 的结果，这些结果仍在 dp3 数组中
                long long left_ways = dp3[k];
                long long right_ways = dp3[i - 1 - k];
                dp3[i] = (dp3[i] + (left_ways * right_ways) % MOD) % MOD;
            }
        }
        // 注意: dp3[0] 保持为 1，因为它在内层循环中未被修改 (i>=1)
        // 且后续计算仍然需要 dp3[0]=1 作为基础
    }
    // 循环结束后，dp3[n] 存储的是节点数为 n，高度不超过 m 的结果
    return (int)dp3[n];
}


int main() {
    // 可选: 关闭 C++ 标准流与 C 标准流的同步，解除 cin 和 cout 的绑定，以加速 cin/cout
    // ios_base::sync_with_stdio(false);
    // cin.tie(NULL);

    int n, m;

    // 使用 scanf 读取输入直到文件结束 (EOF)
    // scanf 返回成功读取的项数，对于 "%d %d"，成功时返回 2
    while (scanf("%d %d", &n, &m) == 2) {
        // 调用最优化版本 compute3 并使用 printf 输出结果
        printf("%d\n", compute3(n, m));
        // 或者调用其他版本进行测试:
        // printf("%d\n", compute1(n, m));
        // printf("%d\n", compute2(n, m));
    }

    return 0;
}
