#include <vector>
#include <algorithm> // 用于 std::min
#include <limits>    // 用于 std::numeric_limits

// 多边形三角剖分的最低得分
// 你有一个凸的 n 边形，其每个顶点都有一个整数值
// 给定一个整数数组values，其中values[i]是第i个顶点的值(顺时针顺序)
// 假设将多边形 剖分 为 n - 2 个三角形
// 对于每个三角形，该三角形的值是顶点标记的乘积
// 三角剖分的分数是进行三角剖分后所有 n - 2 个三角形的值之和
// 返回 多边形进行三角剖分后可以得到的最低分
// 测试链接 : https://leetcode.cn/problems/minimum-score-triangulation-of-polygon/

class MinimumScoreTriangulationOfPolygon {
public:
    // 记忆化搜索
    // f(arr, l, r, dp) 计算由顶点 arr[l] 到 arr[r] 构成的多边形（必须包含边 l-r）
    // 进行三角剖分（不包括边l-r自身形成的那个“外部”区域，而是内部剖分）的最低得分。
    // 具体来说，当 l 和 r 是原始多边形中两个不相邻的顶点时，
    // f(arr, l, r, dp) 代表的是连接 l 和 r 后，形成的那个子多边形（顶点为l, l+1, ..., r）的三角剖分最低分。
    // 而题目中定义的三角剖分总是指整个 n 边形剖分成 n-2 个三角形。
    // 递归的含义是：以边 (l,r) 为一条基准边，枚举所有可能的顶点 m (l < m < r)
    // 来与 l 和 r 形成一个三角形 (l, m, r)。
    // 这个三角形的得分是 arr[l] * arr[m] * arr[r]。
    // 剩下的部分被这条边 (l,m) 和 (m,r) 分割成两个子问题（如果它们形成多边形的话）。
    // f(arr, l, m, dp) 是指多边形 l, l+1, ..., m 的三角剖分得分。
    // f(arr, m, r, dp) 是指多边形 m, m+1, ..., r 的三角剖分得分。
    static int f(const std::vector<int>& arr, int l, int r, std::vector<std::vector<int>>& dp) {
        if (dp[l][r] != -1) {
            return dp[l][r];
        }

        int ans = INT_MAX;
        // 当 l 和 r 相邻或相同，意味着它们之间不能再形成三角形，
        // 或者说，它们所代表的“子多边形”边数小于3，无法三角剖分，得分为0。
        if (l == r || l + 1 == r) { // Java: l == r || l == r - 1
            ans = 0;
        } else {
            // l....r 至少有3个顶点 (r - l + 1 >= 3, 即 r >= l + 2)
            // 枚举中间顶点 m，形成三角形 (l, m, r)
            for (int m = l + 1; m < r; ++m) {
                ans = std::min(ans, f(arr, l, m, dp) + f(arr, m, r, dp) + arr[l] * arr[m] * arr[r]);
            }
        }
        dp[l][r] = ans;
        return ans;
    }

    static int minScoreTriangulation1(const std::vector<int>& arr) {
        int n = arr.size();
        if (n < 3) { // 少于3个顶点不能形成多边形，也就无法三角剖分
            return 0;
        }
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));
        // 我们要求解的是以 arr[0]...arr[n-1] 为顶点的整个多边形的三角剖分
        // 递归函数 f(arr, l, r, dp) 的设计是以 l 和 r 为固定边的子问题。
        // 对于整个多边形，可以将其看作是固定边 (0, n-1)，然后寻找中间点 m，
        // 形成三角形 (0, m, n-1)，并递归处理子多边形 (0, ..., m) 和 (m, ..., n-1)。
        return f(arr, 0, n - 1, dp);
    }

    // 严格位置依赖的动态规划
    static int minScoreTriangulation2(const std::vector<int>& arr) {
        int n = arr.size();
        if (n < 3) {
            return 0;
        }
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // dp[l][r] 表示由顶点 arr[l]...arr[r] 构成的子多边形（以边l-r为一条基准边）
        // 的三角剖分最低得分。
        // 长度 len 代表 r - l + 1 (子多边形的顶点数)
        // 最小的子多边形有3个顶点 (len=3), 例如 (l, l+1, l+2)
        // 此时 r = l + 2。

        // l 的迭代顺序是从 n-3 向下到 0。
        // r 的迭代顺序是从 l+2 向上到 n-1。
        // 这确保了当计算 dp[l][r] 时，所有需要的子问题 dp[l][m] 和 dp[m][r] 都已经被计算过。
        // 因为 m 在 (l, r) 之间，所以 l < m < r。
        // dp[l][m] 中区间的长度是 m - l + 1
        // dp[m][r] 中区间的长度是 r - m + 1
        // 这两个长度都小于当前区间长度 r - l + 1。

        // Java代码的循环顺序 (l 从 n-3 到 0, r 从 l+2 到 n-1) 是正确的。
        // 对于 dp[l][r] 依赖的 dp[l][m] 和 dp[m][r]:
        // 如果是 dp[l][m], m < r, 所以 r 坐标更小 (或者说区间长度更小)
        // 如果是 dp[m][r], m > l, 所以 l 坐标更大 (或者说区间长度更小)
        // 这种依赖关系通常通过改变区间长度 len 来遍历，或者像Java代码那样按 l 递减，r 递增。

        // len 是多边形中顶点的数量 (r - l + 1)
        // 我们从 len = 3 (即 r = l + 2) 开始计算
        for (int len = 3; len <= n; ++len) { // 遍历子多边形的顶点数
            for (int l = 0; l <= n - len; ++l) { // 遍历起始顶点 l
                int r = l + len - 1; // 计算结束顶点 r
                dp[l][r] = INT_MAX;
                // 枚举中间顶点 m，形成三角形 (l, m, r)
                for (int m = l + 1; m < r; ++m) {
                    dp[l][r] = std::min(dp[l][r], dp[l][m] + dp[m][r] + arr[l] * arr[m] * arr[r]);
                }
            }
        }
        return dp[0][n - 1];
    }
};


#include <iostream>
int main() {
    std::vector<int> values1 = {1, 2, 3};
    std::cout << "Values: {1, 2, 3}" << std::endl;
    std::cout << "Min Score Triangulation (Memoization): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation1(values1) << std::endl; // 1*2*3 = 6
    std::cout << "Min Score Triangulation (DP): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation2(values1) << std::endl;          // 6
    std::cout << std::endl;

    std::vector<int> values2 = {3, 7, 4, 5};
    // Triangles: (3,7,4) + (3,4,5) -> 3*7*4 + 3*4*5 = 84 + 60 = 144
    // Triangles: (3,7,5) + (7,4,5) -> 3*7*5 + 7*4*5 = 105 + 140 = 245  <- This interpretation is for a different problem (matrix chain style)
    // The problem is about polygon triangulation.
    // For {v0, v1, v2, v3}, we fix one edge, say (v0, v3).
    // Then choose a middle vertex k.
    // If k=v1, triangle (v0,v1,v3), score v0*v1*v3. Remaining polygon (v1,v2,v3). Score for (v1,v2,v3) is v1*v2*v3. Total = v0v1v3 + v1v2v3
    // (3,7,5) -> arr[0]*arr[1]*arr[3] = 3*7*5 = 105. Then polygon (7,4,5) -> arr[1]*arr[2]*arr[3] = 7*4*5 = 140. Total 105+140=245.
    // If k=v2, triangle (v0,v2,v3), score v0*v2*v3. Remaining polygon (v0,v1,v2). Score for (v0,v1,v2) is v0*v1*v2. Total = v0v2v3 + v0v1v2
    // (3,4,5) -> arr[0]*arr[2]*arr[3] = 3*4*5 = 60. Then polygon (3,7,4) -> arr[0]*arr[1]*arr[2] = 3*7*4 = 84. Total 60+84=144.
    // So, for {3,7,4,5}, min score is 144.
    std::cout << "Values: {3, 7, 4, 5}" << std::endl;
    std::cout << "Min Score Triangulation (Memoization): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation1(values2) << std::endl; // Expected: 144
    std::cout << "Min Score Triangulation (DP): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation2(values2) << std::endl;          // Expected: 144
    std::cout << std::endl;

    std::vector<int> values3 = {1, 3, 1, 4, 1, 5};
    std::cout << "Values: {1, 3, 1, 4, 1, 5}" << std::endl;
    std::cout << "Min Score Triangulation (Memoization): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation1(values3) << std::endl; // Expected: 13
    std::cout << "Min Score Triangulation (DP): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation2(values3) << std::endl;          // Expected: 13
    std::cout << std::endl;

    std::vector<int> values4 = {2,1,1}; // n=3
    std::cout << "Values: {2,1,1}" << std::endl;
    std::cout << "Min Score Triangulation (Memoization): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation1(values4) << std::endl; // Expected: 2*1*1=2
    std::cout << "Min Score Triangulation (DP): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation2(values4) << std::endl;          // Expected: 2
    std::cout << std::endl;

    std::vector<int> values5 = {1,2}; // n=2
    std::cout << "Values: {1,2}" << std::endl;
    std::cout << "Min Score Triangulation (Memoization): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation1(values5) << std::endl; // Expected: 0
    std::cout << "Min Score Triangulation (DP): " << MinimumScoreTriangulationOfPolygon::minScoreTriangulation2(values5) << std::endl;          // Expected: 0
    std::cout << std::endl;

    return 0;
}

