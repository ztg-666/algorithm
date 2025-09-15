#include <bits/stdc++.h>

// 序列计数
// 有一个概念叫，最左端最大值位置，表示一段范围上
// 如果最大值有一个，那么最大值所在的位置，就是最左端最大值位置
// 如果最大值有多个，最左的那个所在的位置，就是最左端最大值位置
// 给定一个长度为n的数组A，那么必然存在等长的数组B，当选择同样的子范围时
// 两者在这段范围上，最左端最大值位置是相同的，不仅存在这样的数组B，而且数量无限多
// 现在要求，数组B中的每个值都在[1,m]范围，返回有多少个这样的数组，答案对 1000000007 取模
// 2 <= n、m <= 2 * 10^5    1 <= A[i] <= m    n * m <= 10^6
// 测试链接 : https://codeforces.com/problemset/problem/1748/E

using namespace std;

const int MOD = 1000000007;      // 模数
const int MAXN = 1000001;        // 最大数组长度

int arr[MAXN];                   // 原始数组A
int leftc[MAXN], rightc[MAXN];   // leftc[i]: i节点的左子树根，rightc[i]: i节点的右子树根
int stk[MAXN];                   // 单调栈，用于构建笛卡尔树
long long tmp[MAXN];             // 临时数组，用于计算dp转移
int n, m;

// 构建笛卡尔树
// 笛卡尔树的性质：
// 1. 中序遍历得到原数组的顺序
// 2. 树的形状由数组元素大小决定，较大元素在较小元素的上方
// 3. 每个节点代表以该位置为根的子数组的最大值位置
void build() {
    int top = 0;  // 栈顶指针

    // 遍历每个位置
    for (int i = 1; i <= n; i++) {
        int pos = top;

        // 维护单调栈性质：栈中元素从底到顶单调递减
        // 如果当前元素比栈顶大，则弹出栈顶元素
        while (pos > 0 && arr[stk[pos]] < arr[i]) {
            pos--;
        }

        // 如果栈不为空，当前元素i将成为栈顶元素的右子树
        if (pos > 0) {
            rightc[stk[pos]] = i;
        }

        // 如果有元素被弹出，最后一个被弹出的元素将成为当前元素i的左子树
        if (pos < top) {
            leftc[i] = stk[pos + 1];
        }

        // 将当前元素入栈
        stk[++pos] = i;
        top = pos;
    }
}

// 深度优先搜索计算动态规划
// dp[u][j] 表示以u为根的子树中，所有节点值都不超过j的方案数
void dfs(int u, vector<vector<int>>& dp) {
    if (u == 0) return;  // 空节点直接返回

    // 递归处理左右子树
    dfs(leftc[u], dp);
    dfs(rightc[u], dp);

    // 计算当前节点的dp值
    // 根据笛卡尔树的性质，节点u必须是其子树的最大值
    // 因此u节点的值必须 >= max(左子树所有值, 右子树所有值)
    for (int j = 1; j <= m; j++) {
        // tmp[j] 表示当u节点取值为j时的方案数
        // 左子树所有值 <= j-1，右子树所有值 <= j
        tmp[j] = 1LL * dp[leftc[u]][j - 1] * dp[rightc[u]][j] % MOD;
    }

    // 前缀和优化：dp[u][j] = dp[u][j-1] + tmp[j]
    // dp[u][j] 表示u子树中所有值都不超过j的总方案数
    for (int j = 1; j <= m; j++) {
        dp[u][j] = (dp[u][j - 1] + tmp[j]) % MOD;
    }
}

// 清理树结构，为下一个测试用例做准备
void clearTree() {
    fill(leftc + 1, leftc + n + 1, 0);
    fill(rightc + 1, rightc + n + 1, 0);
}

// 主计算函数
long long compute() {
    build();  // 构建笛卡尔树

    // 动态开辟dp数组，避免内存超限
    // dp[i][j] 表示以节点i为根的子树，所有值都不超过j的方案数
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // 初始化：空节点（编号0）对任何值限制都有1种方案（什么都不选）
    for (int j = 0; j <= m; j++) {
        dp[0][j] = 1;
    }

    // 从树根开始DFS计算
    dfs(stk[1], dp);  // stk[1]是笛卡尔树的根节点

    clearTree();  // 清理，准备下一轮

    // 返回整棵树所有值都在[1,m]范围内的方案数
    return dp[stk[1]][m];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int cases;
    cin >> cases;

    while (cases--) {
        cin >> n >> m;

        // 读入数组（注意：数组下标从1开始）
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
        }

        cout << compute() << "\n";
    }

    return 0;
}
