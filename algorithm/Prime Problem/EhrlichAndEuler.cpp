#include <vector>
#include <cmath>

// 计数质数
// 给定整数n，返回小于非负整数n的质数的数量
// 测试链接 : https://leetcode.cn/problems/count-primes/

using namespace std;

// 传统埃拉托斯特尼筛法（时间复杂度 O(n log log n)）
int ehrlich(int n) {
    if (n < 2) return 0;
    vector<bool> visit(n + 1, false); // 标记非素数

    // 核心筛法逻辑
    for (int i = 2; i * i <= n; ++i) {
        if (!visit[i]) { // 发现素数
            // 标记素数的倍数为非素数
            for (int j = i * i; j <= n; j += i) {
                visit[j] = true;
            }
        }
    }

    // 统计素数个数
    int cnt = 0;
    for (int i = 2; i <= n; ++i) {
        if (!visit[i]) ++cnt;
    }
    return cnt;
}

// 欧拉筛法（线性筛，时间复杂度 O(n)）
int euler(int n) {
    vector<bool> visit(n + 1, false);
    vector<int> primes(n/2 + 1); // 预分配素数存储空间
    int cnt = 0; // 素数计数器

    for (int i = 2; i <= n; ++i) {
        if (!visit[i]) {
            primes[cnt++] = i; // 记录新发现的素数
        }
        // 用当前数与已知素数相乘标记合数
        for (int j = 0; j < cnt; ++j) {
            if (i * primes[j] > n) break;
            visit[i * primes[j]] = true;
            // 关键优化：保证每个合数只被最小质因数标记
            if (i % primes[j] == 0) break;
        }
    }
    return cnt;
}

// 优化版埃氏筛（跳过偶数处理）
int ehrlich2(int n) {
    if (n <= 1) return 0;
    vector<bool> visit(n + 1, false);
    int cnt = (n+1)/2;  // 初始值：假设所有奇数都是素数（含2的特殊处理）

    // 仅处理奇数（从3开始，步长2）
    for (int i = 3; i*i <= n; i += 2) {
        // 当i是素数时才进行标记
        if (!visit[i]) {
            // 从i*i开始标记，步长2i（跳过偶数倍）
            for (int j = i * i; j <= n; j += 2 * i) {
                if(!visit[j]) {
                    visit[j] = true;
                    --cnt; // 每标记一个合数就减少计数
                }
            }
        }
    }
    return cnt;
}

// 计算小于n的素数个数（题目要求）
int countPrimes(int n) {
    // 参数调整为n-1，因为题目要求统计严格小于n的素数
    return ehrlich(n - 1);
}
