#include <iostream>
#include <cstring>
#include <algorithm>
// 表格填数问题
// 给定一个长度为n的数组arr，arr[i]表示i位置上方的正方形格子数量
// 那么从1位置到n位置，每个位置就是一个直方图，所有的直方图紧靠在一起
// 在这片区域中，你要放入k个相同数字，不能有任意两个数字在同一行或者同一列
// 注意在这片区域中，如果某一行中间断开了，使得两个数字无法在这一行连通，则不算违规
// 返回填入数字的方法数，答案对 1000000007 取模
// 1 <= n、k <= 500    0 <= arr[i] <= 10^6
// 测试链接 : https://www.luogu.com.cn/problem/P6453
using namespace std;

const int MOD = 1000000007;     // 取模的值
const int MAXN = 501;           // n的最大值+1
const int MAXH = 1000000;       // arr[i]的最大值

// 输入数组，arr[i]表示第i列的高度
int arr[MAXN];

// 预处理的阶乘表，用于快速计算组合数
int fac[MAXH + 1];

// 预处理的阶乘逆元表，用于快速计算组合数
int inv[MAXH + 1];

// 笛卡尔树的左右子树指针
int left_child[MAXN];   // left_child[i]表示节点i的左儿子
int right_child[MAXN];  // right_child[i]表示节点i的右儿子
int stack_arr[MAXN];    // 构建笛卡尔树时使用的栈

// DFS过程中需要的数据
int siz[MAXN];          // siz[u]表示以u为根的子树中节点的个数

// 动态规划表
// dp[u][i]表示在以u为根的子树中放置i个数字的方案数
int dp[MAXN][MAXN];

// 临时数组，用于合并左右子树的DP结果
int tmp[MAXN];

int n, k;               // n为列数，k为要放置的数字个数

/**
 * 快速幂算法，计算 x^p mod MOD
 * @param x 底数
 * @param p 指数
 * @return x^p mod MOD
 */
long long power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) {
        if ((p & 1) == 1) {
            ans = (ans * x) % MOD;
        }
        x = (x * x) % MOD;
        p >>= 1;
    }
    return ans;
}

/**
 * 计算组合数 C(n, k) mod MOD
 * @param n 总数
 * @param k 选择数
 * @return C(n, k) mod MOD
 */
int c(int n, int k) {
    if (n < k) return 0;
    return (long long)fac[n] * inv[k] % MOD * inv[n - k] % MOD;
}

/**
 * 预处理和构建笛卡尔树
 */
void build() {
    // 预处理阶乘表
    fac[0] = fac[1] = inv[0] = 1;
    for (int i = 2; i <= MAXH; i++) {
        fac[i] = (long long)fac[i - 1] * i % MOD;
    }

    // 预处理阶乘逆元表，使用费马小定理
    inv[MAXH] = power(fac[MAXH], MOD - 2);
    for (int i = MAXH - 1; i >= 1; i--) {
        inv[i] = (long long)inv[i + 1] * (i + 1) % MOD;
    }

    // 使用单调栈构建笛卡尔树
    // 笛卡尔树的性质：中序遍历是原数组顺序，且满足堆性质（小根堆）
    int top = 0, pos;
    for (int i = 1; i <= n; i++) {
        pos = top;
        // 找到栈中第一个不大于arr[i]的元素
        while (pos > 0 && arr[stack_arr[pos]] > arr[i]) {
            pos--;
        }
        // 如果栈不为空，则当前元素i成为栈顶元素的右儿子
        if (pos > 0) {
            right_child[stack_arr[pos]] = i;
        }
        // 如果弹出了元素，则被弹出的最后一个元素成为当前元素的左儿子
        if (pos < top) {
            left_child[i] = stack_arr[pos + 1];
        }
        stack_arr[++pos] = i;
        top = pos;
    }
}

/**
 * 深度优先搜索，计算动态规划值
 * @param u 当前节点
 * @param fa 父节点，用于计算高度差
 */
void dfs(int u, int fa) {
    if (u == 0) {  // 空节点
        return;
    }

    // 递归处理左右子树
    dfs(left_child[u], u);
    dfs(right_child[u], u);

    // 计算当前子树的节点数
    siz[u] = siz[left_child[u]] + siz[right_child[u]] + 1;

    // 清空临时数组
    memset(tmp, 0, (k + 1) * sizeof(int));

    // 合并左右子树的DP结果
    // tmp[l+r]表示左子树选l个，右子树选r个的方案数
    // 总时间复杂度O(n^2)
    for (int l = 0; l <= min(siz[left_child[u]], k); l++) {
        for (int r = 0; r <= min(siz[right_child[u]], k - l); r++) {
            tmp[l + r] = (tmp[l + r] + (long long)dp[left_child[u]][l] * dp[right_child[u]][r] % MOD) % MOD;
        }
    }

    // 计算当前节点的DP值
    // dp[u][i]表示在以u为根的子树中放置i个数字的方案数
    // 总时间复杂度O(min(n^3, n * k^2))
    for (int i = 0; i <= min(siz[u], k); i++) {
        for (int p = 0; p <= i; p++) {
            // p表示子树中选择的数字个数
            // i-p表示在当前层放置的数字个数
            // C(siz[u] - p, i - p): 从剩余列中选择i-p列
            // C(arr[u] - arr[fa], i - p): 从当前层的可用行中选择i-p行
            // fac[i - p]: 对选出的i-p个位置进行排列
            // tmp[p]: 子树中选择p个数字的方案数
            dp[u][i] = (dp[u][i] + (long long)c(siz[u] - p, i - p) * c(arr[u] - arr[fa], i - p) % MOD
                       * fac[i - p] % MOD * tmp[p] % MOD) % MOD;
        }
    }
}

/**
 * 主计算函数
 * @return 答案
 */
int compute() {
    build();           // 预处理和构建笛卡尔树
    dp[0][0] = 1;     // 空节点放0个数字的方案数为1
    dfs(stack_arr[1], 0);  // 从根节点开始DFS，stack_arr[1]是笛卡尔树的根
    return dp[stack_arr[1]][k];  // 返回在整棵树中放置k个数字的方案数
}

/**
 * 主函数
 */
int main() {
    // 加速输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入
    cin >> n >> k;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出答案
    cout << compute() << endl;

    return 0;
}
