#include <iostream>
#include <cstring>
#include <vector>
#include <string>

// 萌数
// 如果一个数字，存在长度至少为2的回文子串，那么这种数字被称为萌数
// 比如101、110、111、1234321、45568
// 求[l,r]范围上，有多少个萌数
// 由于答案可能很大，所以输出答案对1000000007求余
// 测试链接 : https://www.luogu.com.cn/problem/P3413

using namespace std;
using ll = long long;
const ll MOD = 1000000007;       // 题目要求的模数
const ll MAXN = 1001;            // 最大数字位数
ll dp[MAXN][11][11][2];          // 记忆化数组：[位置][前前位][前位][自由模式]

/**
 * 初始化记忆化数组
 * @param n 数字的最大位数
 */
void build(ll n) {
    // 将所有状态初始化为未计算(-1)
    for (ll a = 0; a < n; a++)
        for (ll b = 0; b <= 10; b++)
            for (ll c = 0; c <= 10; c++)
                for (ll d = 0; d < 2; d++)
                    dp[a][b][c][d] = -1;
}

/**
 * 检查数字是否为萌数
 * @param num 数字的字符数组表示
 * @return 存在长度>=2的回文子串返回true，否则false
 */
bool check(const vector<char>& num) {
    for (ll i = 0; i < num.size(); i++) {
        // 检查相邻两位相同或隔一位相同的情况
        if (i >= 1 && num[i] == num[i-1]) return true;  // 形如aa
        if (i >= 2 && num[i] == num[i-2]) return true;  // 形如aba
    }
    return false;
}

/**
 * 数位DP核心函数：计算合法数字的数量
 * @param num  原始数字的字符数组
 * @param i    当前处理的位置
 * @param pp   前前位的数字（初始为10表示未设置）
 * @param p    前一位的数字（初始为10表示未设置）
 * @param free 自由模式：1-可自由选择，0-受原始数字限制
 * @return 返回满足条件的数字数量
 */
ll f(const vector<char>& num, ll i, ll pp, ll p, ll free) {
    // 递归终止条件：处理完所有位数
    if (i == num.size()) return 1;
    
    // 记忆化搜索：如果当前状态已计算过，直接返回结果
    if (dp[i][pp][p][free] != -1) return dp[i][pp][p][free];

    ll limit = (free == 0) ? (num[i] - '0') : 9; // 当前位可选数字的上界
    ll ans = 0;

    if (free == 0) { // 严格受原始数字限制模式
        if (p == 10) { // 当前是最高位且未设置过数字
            // 情况1：继续不设置数字（保持前导零）
            ans = (ans + f(num, i+1, 10, 10, 1)) % MOD;
            
            // 情况2：设置1~当前位-1的数字作为起始
            for (ll cur = 1; cur < limit; cur++)
                ans = (ans + f(num, i+1, p, cur, 1)) % MOD;
            
            // 情况3：严格设置当前位数字（需满足非萌数条件）
            if (pp != limit && p != limit)
                ans = (ans + f(num, i+1, p, limit, 0)) % MOD;
        } else { // 已设置过前位数字
            // 遍历可选的数字（0~当前位-1）
            for (ll cur = 0; cur < limit; cur++) {
                // 检查是否形成回文：不与前两位和前位相同
                if (cur != pp && cur != p)
                    ans = (ans + f(num, i+1, p, cur, 1)) % MOD;
            }
            // 处理严格设置当前位的情况
            if (limit != pp && limit != p)
                ans = (ans + f(num, i+1, p, limit, 0)) % MOD;
        }
    } else { // 自由选择模式
        if (p == 10) { // 未设置过数字（允许保持前导零）
            // 情况1：继续不设置数字
            ans = (ans + f(num, i+1, 10, 10, 1)) % MOD;
            
            // 情况2：设置1-9作为起始数字（避免前导零）
            for (ll cur = 1; cur <= 9; cur++)
                ans = (ans + f(num, i+1, p, cur, 1)) % MOD;
        } else { // 已设置过前位数字
            // 遍历所有可能的数字（0-9）
            for (ll cur = 0; cur <= 9; cur++) {
                // 检查是否形成回文：不与前两位和前位相同
                if (cur != pp && cur != p)
                    ans = (ans + f(num, i+1, p, cur, 1)) % MOD;
            }
        }
    }

    dp[i][pp][p][free] = ans; // 存储计算结果
    return ans;
}

/**
 * 计算[0, num]范围内的非萌数数量
 * @param num 数字的字符数组表示
 * @return 返回非萌数的数量（模MOD）
 */
ll cnt(const vector<char>& num) {
    // 处理边界情况：0不算萌数但需要特殊处理
    if (num.size() == 1 && num[0] == '0') return 0;
    
    // 计算数字本身的值（模MOD处理大数）
    long long all = 0;
    for (char c : num)
        all = (all * 10 + (c - '0')) % MOD;
    
    // 计算非萌数数量并返回萌数数量
    build(num.size());
    ll non_meng = f(num, 0, 10, 10, 0);
    return (all - non_meng + MOD) % MOD; // 防止负数出现
}

/**
 * 计算区间[l, r]的萌数数量
 * @param l 区间左端点
 * @param r 区间右端点
 * @return 返回萌数数量（模MOD）
 */
ll compute(const vector<char>& l, const vector<char>& r) {
    ll cr = cnt(r);              // [0,r]的萌数数量
    ll cl = cnt(l);              // [0,l)的萌数数量
    ll ans = (cr - cl + MOD) % MOD; // 前缀和相减
    
    // 检查左端点自身是否为萌数
    if (check(l)) ans = (ans + 1) % MOD;
    return ans;
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(0);

    string sl, sr;
    cin >> sl >> sr;
    
    // 将字符串转换为字符数组
    vector<char> l(sl.begin(), sl.end());
    vector<char> r(sr.begin(), sr.end());
    
    cout << compute(l, r) << endl;

    return 0;
}
