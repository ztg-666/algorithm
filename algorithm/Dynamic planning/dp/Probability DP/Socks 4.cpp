#include <bits/stdc++.h> // 引入标准库的万能头文件，包含了大部分常用的C++标准库组件，方便快速开发。

// 问题描述
// 在高桥的抽屉里，有 N 种颜色的袜子，其中第 i 种颜色有 Aᵢ只。
//
// 初始时，高桥在抽屉外有一只颜色为 C 的袜子（这只袜子独立于抽屉内的袜子）。他重复执行以下操作，直到满足终止条件：
//
// 从抽屉中均匀随机地抽取 1 只袜子。
// 若此时抽屉外的两只袜子颜色相同，则终止操作。
// 否则，选择其中一只袜子放回抽屉。他总是选择放回的袜子，以最小化未来抽取袜子的期望次数。
//
// 求操作终止前抽取袜子的期望次数，结果对 998244353 取模。

//https://atcoder.jp/contests/abc412/tasks/abc412_f

using namespace std;     // 使用std命名空间，避免每次使用标准库函数或对象时都加上std::前缀。

const int c = 3005, mod = 998244353; // 定义两个常量：
                                     // c: 数组的最大尺寸。通常用来定义状态的最大值或数据范围上限。这里设定为3005，意味着处理的数值不会超过这个范围。
                                     // mod: 模数，用于所有算术运算的结果都对mod取模。这在组合数学、数论和动态规划中非常常见，目的是防止整数溢出，并使结果保持在特定范围内。998244353是一个常用的质数模数，因为它足够大，且为M * 2^k + 1的形式，在FFT等算法中表现良好。

long long n, cnt[c], dp[c], kezd, cur, sum, invsum; // 声明全局变量：
                                                    // n: 输入的数据数量，表示有多少个元素。
                                                    // cnt[c]: 一个数组，用于统计每个数值出现的频率。例如，cnt[x]表示数值x出现了多少次。
                                                    // dp[c]: 动态规划数组。dp[i]通常表示从状态i开始，达到目标状态的某种期望值、最小代价或方案数。具体含义取决于实际问题。
                                                    // kezd: 一个起始索引。题目中提到对这个索引的元素有特殊处理。
                                                    // cur: 存储在起始索引`kezd`处，经过特殊处理（加1）后的数值。
                                                    // sum: 存储所有输入数值的总和（可能在特殊处理前或后，需看代码逻辑）。
                                                    // invsum: `sum`的模逆元。在模算术中，除以一个数等价于乘以它的模逆元。

// `oszt` 函数：计算a的p次幂对mod取模的结果，也用于计算模逆元 (a^(mod-2) % mod)。
// 这是经典的“快速幂”（Binary Exponentiation）算法，时间复杂度为O(log p)。
long long oszt(long long a)
{
    long long ans = 1, p = mod - 2; // 初始化结果ans为1，指数p为mod-2。
                                    // 根据费马小定理：如果p是素数，那么a^(p-1) ≡ 1 (mod p)。
                                    // 因此，a^(p-2) ≡ a^(-1) (mod p)。这里mod是素数998244353，所以mod-2就是用来计算a的模逆元的。
    while (p)
    { // 当指数p大于0时循环
        if (p % 2)
        {                        // 如果p是奇数，则将a乘到结果ans中
            ans = ans * a % mod; // ans = (ans * a) % mod
        }
        a = a * a % mod; // a = (a * a) % mod，底数平方
        p /= 2;          // 指数p除以2
    }
    return ans; // 返回计算结果
}

int main()
{
    ios_base::sync_with_stdio(false); // 禁用C++标准I/O流与C标准I/O流的同步。
                                      // 这可以大大提高输入输出操作的速度，在竞技编程中非常常用。
    cin >> n >> kezd;                 // 读取输入的元素总数n，以及特殊处理的起始索引kezd。

    // 循环读取n个数值，并进行初始化操作
    for (int i = 1; i <= n; i++)
    {
        int x;    // 声明一个局部变量x，用于存储当前读取的数值
        cin >> x; // 读取一个数值x
        sum += x; // 将x累加到总和sum中

        // 如果当前循环的索引i与特殊起始索引kezd相等
        if (i == kezd)
        {
            x++;     // 将当前数值x加1 (这是特殊处理)
            cur = x; // 将处理后的x值存储到cur变量中。这个cur就是我们要最终查询其dp值的起始状态。
        }
        // cout << "fontos " << i << " " << x << "\n"; // 这是一行被注释掉的调试代码，用于打印重要信息。
        cnt[x]++; // 统计经过处理（如果i==kezd则加1）后的x值出现的次数。
    }

    invsum = oszt(sum); // 计算所有数值总和sum的模逆元。
                        // 这表明在后续的计算中，我们需要进行除法操作，而模运算中没有直接的除法，所以需要乘以模逆元。

    // 动态规划计算：从大到小计算dp值
    // 这个循环从c-2（一个接近最大值的状态）向下迭代到1。
    // 这种逆向迭代通常意味着dp[i]的值依赖于dp[j]（其中j > i），即从高状态向低状态转移。
    for (int i = c - 2; i >= 1; i--)
    {
        long long val = 1, opt = i - 1; // 初始化dp状态计算的临时变量：
                                        // val: 初始值为1，它将在内层循环中累加各项期望值。这个1通常代表当前一步的“代价”或“贡献”。
                                        // opt: 初始值为i-1，它将累加“有效值”或“概率质量”。

        // 内层循环：计算dp[i]所依赖的所有j > i的状态的贡献
        for (int j = i + 1; j < c; j++)
        {
            // opt的计算：累加所有大于当前状态i的数值j的“有效值”。
            // cnt[j]*j 表示所有值为j的元素的总和。
            // 这意味着opt可能是当前状态i以及所有可能从i“跳跃”到的更高状态j的总“权重”或“容量”。
            opt += cnt[j] * j;

            // val的计算：累加从状态i跳到状态j所带来的期望值贡献。
            // (cnt[j]*j)*invsum%mod: 这部分是 (cnt[j]*j) / sum。
            // 如果把 (cnt[j]*j) 看作是所有值为j的元素的总和，那么 (cnt[j]*j)/sum 可以理解为：
            // 在所有元素总和为sum的情况下，选择到数值为j的元素的“权重比例”或“加权概率”。
            // 这个权重比例乘以dp[j]（从状态j开始的期望值），就得到了从i通过j这个路径的期望贡献。
            val += (cnt[j] * j) * invsum % mod * dp[j] % mod;
        }
        val %= mod; // 确保val在每一步操作后都对mod取模，防止溢出。

        // 计算dp[i]的最终值
        // val * sum % mod: 将之前累加的各项期望贡献（已经除以sum）重新乘以sum。这可能是在做某种归一化或者逆运算。
        // oszt(opt): 计算opt的模逆元，因为这里需要进行除法 (val * sum) / opt。
        // 整个表达式 dp[i] = (val * sum) / opt % mod 可以写成：
        // dp[i] = ( (1 + Sum_{j>i} ((cnt[j]*j)/sum * dp[j])) * sum ) / (i-1 + Sum_{j>i} (cnt[j]*j)) % mod
        // 进一步简化：
        // dp[i] = ( sum + Sum_{j>i} (cnt[j]*j * dp[j]) ) / (i-1 + Sum_{j>i} (cnt[j]*j)) % mod
        // 这个形式通常出现在期望DP问题中，特别是当你有一个“总池子”或“总概率质量”的概念时。
        // 分子通常是“总代价”或“总收益”加上来自未来状态的加权期望；分母是“总的有效可能性”或“总的权重”。
        // 它可能在解决类似“在某种操作下，从当前状态i开始，期望多少步/时间/成本达到某个目标”的问题。
        dp[i] = val * sum % mod * oszt(opt) % mod;
    }

    cout << dp[cur] << "\n"; // 输出最终结果：在特殊处理后的起始数值cur所对应的期望值dp[cur]。
}
