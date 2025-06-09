#include <cmath> // 不需要显式包含，但sqrt可能需要，不过原逻辑用i*i更优
// 判断较小的数字是否是质数
// 时间复杂度O(根号n)
bool isPrime(long long n) {
    if (n <= 1) {
        return false;
    }
    // 用 long long 防止大数时 i*i 溢出
    for (long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
