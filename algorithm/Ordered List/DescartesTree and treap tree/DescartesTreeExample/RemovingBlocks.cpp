#include <iostream>
#include <vector>

// 砖块消除
// 给定一个长度为n的数组arr，arr[i]为i号砖块的重量
// 选择一个没有消除的砖块进行消除，收益为被消除砖块联通区域的重量之和，比如arr = {3,5,2,1}
// 如果先消除5，那么获得3+5+2+1的收益，arr = {3,X,2,1}
// 如果再消除1，那么获得2+1的收益，arr = {3,X,2,X}
// 如果再消除2，那么获得2的收益，arr = {3,X,X,X}
// 如果再消除3，那么获得3的收益，arr = {X,X,X,X}
// 一共有n!种消除方案，返回所有消除方案的收益总和，答案对 1000000007 取模
// 1 <= n <= 10^5    1 <= arr[i] <= 10^9
// 测试链接 : https://atcoder.jp/contests/agc028/tasks/agc028_b

using namespace std;

const int MOD = 1000000007;
const int MAXN = 100001;

// 所有数字，arr[i]表示第i个砖块的重量
vector<long long> arr(MAXN);

// 连续数字逆元表，inv[i]表示i在模MOD意义下的逆元
vector<long long> inv(MAXN);

// sum[i] = (1/1 + 1/2 + 1/3 + ... + 1/i)，%MOD意义下的余数
// 这是调和数列的前缀和
vector<long long> sum(MAXN);

int n; // 砖块数量

/**
* 预处理逆元和调和数列前缀和
* 使用线性求逆元的方法: inv[i] = -(MOD/i) * inv[MOD%i] % MOD
* 等价于 inv[i] = MOD - (MOD/i) * inv[MOD%i] % MOD
*/
void build() {
    inv[1] = 1; // 1的逆元是1

    // 线性求逆元，时间复杂度O(n)
    for (int i = 2; i <= n; i++) {
        inv[i] = MOD - (long long)(MOD / i) * inv[MOD % i] % MOD;
    }

    // 计算调和数列前缀和：sum[i] = 1/1 + 1/2 + ... + 1/i
    for (int i = 1; i <= n; i++) {
        sum[i] = (sum[i - 1] + inv[i]) % MOD;
    }
}

/**
* 计算所有消除方案的收益总和
* 核心思想：对每个位置i，计算它在所有排列中的贡献
*
* 对于位置i的砖块，当它被消除时：
* - 左边有j个砖块未被消除的概率 ∝ 1/(j+1)
* - 右边有k个砖块未被消除的概率 ∝ 1/(k+1)
* - 总贡献 = arr[i] * Σ(1/(j+1) + 1/(k+1)) * n!
*
* 最终公式：每个位置i的贡献 = arr[i] * (sum[i] + sum[n-i+1] - 1) * n!
*/
long long compute() {
    build(); // 预处理逆元和前缀和

    long long ans = 0;

    // 计算每个位置的贡献
    for (int i = 1; i <= n; i++) {
        // sum[i]：当前位置作为左端点时的贡献
        // sum[n-i+1]：当前位置作为右端点时的贡献
        // 减1是因为位置i本身被重复计算了一次
        long long contribution = (sum[i] + sum[n - i + 1] - 1 + MOD) % MOD;
        ans = (ans + contribution * arr[i]) % MOD;
    }

    // 乘以n!，因为有n!种不同的消除顺序
    for (int i = 1; i <= n; i++) {
        ans = ans * i % MOD;
    }

    return ans;
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入砖块数量
    cin >> n;

    // 读入每个砖块的重量
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 计算并输出结果
    cout << compute() << endl;

    return 0;
}
