#include <bits/stdc++.h>
using namespace std;

// 彩灯
// 一共有n个灯泡，开始都是不亮的状态，有m个开关
// 每个开关能改变若干灯泡的状态，改变是指，亮变不亮、不亮变亮
// 比如n=5，某个开关为XXOOO，表示这个开关只能改变后3个灯泡的状态
// 可以随意使用开关，返回有多少种亮灯的组合，全不亮也算一种组合
// 答案可能很大，对 2008 取模
// 1 <= n <= 50
// 1 <= m <= 50
// 测试链接 : https://www.luogu.com.cn/problem/P3857

const int MAXN = 51;  // 最大灯泡数和开关数
const int MOD = 2008; // 结果取模

long long arr[MAXN];   // 每个开关对应的灯泡状态，用二进制表示
long long basis[MAXN]; // 线性基数组，basis[i] 表示最高位在 i 的基向量
int n, m;              // n: 灯泡数-1（因为下标从0开始），m: 开关数

// 向线性基中插入一个状态 num
// 如果线性基中新增了向量，返回 true，否则返回 false
bool insert(long long num) {
    // 从高位到低位尝试插入
    for (int i = n; i >= 0; i--) {
        // 检查第 i 位是否为 1
        if ((num >> i) & 1) {
            if (basis[i] == 0) {
                // 如果第 i 位没有基向量，就直接存入
                basis[i] = num;
                return true; // 新增基向量
            }
            // 如果第 i 位已有基向量，就消元
            num ^= basis[i];
        }
    }
    // num 被现有基向量完全表示，没有新增
    return false;
}

// 计算线性基大小（也就是独立向量的个数）
int compute() {
    int size = 0;
    for (int i = 1; i <= m; i++) {
        if (insert(arr[i])) {
            size++; // 每插入一个独立向量，线性基大小加 1
        }
    }
    return size;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入 n 和 m
    cin >> n >> m;
    n -= 1; // 因为灯泡编号从 0 开始，所以最高位是 n-1

    // 读取每个开关的灯泡控制模式
    for (int i = 1; i <= m; i++) {
        string s;
        cin >> s;
        long long num = 0; // 用 num 记录当前开关控制的灯泡状态
        for (int j = 0; j <= n; j++) {
            if (s[j] == 'O') {
                // 如果第 j 个灯泡被控制，就在 num 的 j 位设为 1
                num |= (1LL << j);
            }
        }
        arr[i] = num;
    }

    // 计算线性基大小
    int size = compute();

    // 答案为 2^size，表示能组成的不同灯泡组合
    // 因为线性基大小 = 独立自由度，每个自由度可取 0 或 1
    long long ans = 1LL << size;
    cout << (ans % MOD) << "\n";

    return 0;
}
