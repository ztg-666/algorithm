#include <bits/stdc++.h>

// 不含递增三元组的排列方法数
// 数字从1到n，可以形成很多排列，要求任意从左往右的三个位置，不能出现依次递增的样子
// 返回排列的方法数，答案对 1000000 取模
// 1 <= n <= 1000
// 测试链接 : https://www.luogu.com.cn/problem/SP7897
// 测试链接 : https://www.spoj.com/problems/SKYLINE


using namespace std;

const int MOD = 1000000;  // 结果对 1000000 取模
const int MAXN = 1001;    // n的最大值为1000，因此最大状态是f[1000]

// f 数组存储从 0 到 MAXN 的结果
long long f[MAXN];

// 预处理 f 数组，计算每个 f[i] 的值
void build() {
    f[0] = f[1] = 1;  // 边界情况，空排列或单个元素排列只有1种情况
    // 从 2 到 MAXN 进行递推计算
    for (int i = 2; i < MAXN; i++) {
        f[i] = 0;  // 每次计算 f[i] 都从 0 开始
        // 计算 f[i] = Σ f[l] * f[r] 其中 l 和 r 分别遍历前面所有情况
        for (int l = 0, r = i - 1; l < i; l++, r--) {
            f[i] = (f[i] + f[l] * f[r] % MOD) % MOD;  // 对结果取模
        }
    }
}

int main() {
    ios::sync_with_stdio(false);  // 禁用同步以提高输入输出效率
    cin.tie(nullptr);  // 解除 cin 和 cout 的绑定

    build();  // 预处理所有 f 数组的值

    int n;
    // 读取输入直到遇到0
    while (cin >> n) {
        if (n == 0) break;  // n 为0时结束
        // 输出对应的 f[n] 结果
        cout << f[n] % MOD << "\n";  // 对结果取模
    }

    return 0;
}
