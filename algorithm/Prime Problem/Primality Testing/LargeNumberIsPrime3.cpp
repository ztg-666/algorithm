// 判断较大的数字是否是质数（Miller-Rabin测试）
// 使用128位整型处理大数运算（核心在第11行）
// 测试链接 : https://www.luogu.com.cn/problem/U148828

#include <bits/stdc++.h>
using namespace std;

// 使用__int128处理大数运算（注意：该类型在部分编译器可能不支持IO操作）
typedef __int128 ll; // 核心：使用128位整型防止乘法溢出
typedef pair<int, int> pii;

// 模板化IO处理（适配__int128类型）
template<typename T> inline T read() {
    T x = 0, f = 1; char ch = 0;
    for(; !isdigit(ch); ch = getchar()) if(ch == '-') f = -1;
    for(; isdigit(ch); ch = getchar()) x = (x << 3) + (x << 1) + (ch - '0');
    return x * f;
}

// 递归实现大数输出
template<typename T> inline void write(T x) {
    if(x < 0) putchar('-'), x = -x;
    if(x > 9) write(x / 10);
    putchar(x % 10 + '0');
}

// 带换行的输出函数
template<typename T> inline void print(T x, char ed = '\n') {
    write(x), putchar(ed);
}

ll t, n; // 测试用例数和待检测数字

// 快速幂取模（防止溢出的关键函数）
ll qpow(ll a, ll b, ll mod) {
    ll ret = 1;
    while(b) {
        if(b & 1) ret = (ret * a) % mod; // 二进制最后一位为1时累乘
        a = (a * a) % mod;               // 平方运算取模
        b >>= 1;                         // 右移处理下一位
    }
    return ret % mod;
}

// Miller-Rabin测试基（覆盖2^64范围的确定性测试集）
vector<ll> p = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

// Miller-Rabin确定性素数测试
bool miller_rabin(ll n) {
    // 处理小数和偶数
    if(n < 3 || n % 2 == 0) return n == 2;

    // 分解n-1为u*2^t的形式
    ll u = n - 1, t = 0;
    while(u % 2 == 0) u /= 2, ++t;

    // 使用多组基进行测试
    for(auto a : p) {
        if(n == a) return true;   // 直接命中基
        if(n % a == 0) return false; // 存在小因子

        ll v = qpow(a, u, n);     // 计算a^u mod n
        if(v == 1) continue;      // 符合条件继续测试

        // 二次探测（检查v是否为n-1）
        ll s = 1;
        for(; s <= t; ++s) {
            if(v == n - 1) break; // 满足条件则中断循环
            v = v * v % n;        // 平方探测
        }
        if(s > t) return false;   // 所有次探测失败则非质数
    }
    return true; // 通过所有测试则为质数
}

int main() {
    t = read<ll>();
    while(t--) {
        n = read<ll>();
        puts(miller_rabin(n) ? "Yes" : "No"); // 输出检测结果
    }
    return 0;
}
