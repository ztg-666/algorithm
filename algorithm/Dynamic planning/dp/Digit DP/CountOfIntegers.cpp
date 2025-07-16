#include <string>
#include <vector>
#include <cstring>

// 统计整数数目
// 给你两个数字字符串 num1 和 num2 ，以及两个整数max_sum和min_sum
// 如果一个整数 x 满足以下条件，我们称它是一个好整数
// num1 <= x <= num2
// min_sum <= digit_sum(x) <= max_sum
// 请你返回好整数的数目
// 答案可能很大，答案对 1000000007 取模
// 注意，digit_sum(x)表示x各位数字之和
// 测试链接 : https://leetcode.cn/problems/count-of-integers/

using namespace std;

const int MOD = 1000000007;
const int MAXN = 23;
const int MAXM = 401;
// 三维记忆化数组，维度含义：
// 1. 当前处理位数 (0 <= i < MAXN=23)
// 2. 当前数字和 (0 <= sum < MAXM=401)
// 3. 选择状态 (0-受限选择，1-自由选择)
int dp[MAXN][MAXM][2];
string num;       // 当前处理的数字字符串
int min_sum;      // 数字和最小值要求
int max_sum;      // 数字和最大值要求
int len;          // 当前处理数字的长度

// 初始化记忆化数组为-1（未计算状态）
void build() {
    memset(dp, -1, sizeof(dp)); // 使用memset快速初始化三维数组
}

// 校验当前数字num的各位和是否在[min_sum, max_sum]范围内
bool check() {
    int sum = 0;
    for (char c : num) {
        sum += c - '0';  // 字符转数字并累加
    }
    return sum >= min_sum && sum <= max_sum;
}

// 记忆化搜索核心函数
// 参数说明：
//   i    : 当前处理位数（从左到右）
//   sum  : 已确定的各位数字和
//   free : 选择模式（0-受限模式，1-自由模式）
int f(int i, int sum, int free) {
    // 剪枝条件1：当前和已超过最大值要求
    if (sum > max_sum) return 0;
    // 剪枝条件2：剩余位全选9仍不达最小值
    if (sum + (len - i) * 9 < min_sum) return 0;
    // 终止条件：所有位数处理完成
    if (i == len) return 1; // 有效路径计数为1
    // 记忆化读取
    if (dp[i][sum][free] != -1) return dp[i][sum][free];

    int cur = num[i] - '0'; // 当前位的数字值
    int ans = 0;

    if (free == 0) { // 受限选择模式
        // 选择比当前位小的数字（后续可自由选择）
        for (int pick = 0; pick < cur; pick++) {
            ans = (ans + f(i + 1, sum + pick, 1)) % MOD;
        }
        // 精确选择当前位数字（后续继续受限）
        ans = (ans + f(i + 1, sum + cur, 0)) % MOD;
    } else { // 自由选择模式
        // 所有数字（0-9）均可选择
        for (int pick = 0; pick <= 9; pick++) {
            ans = (ans + f(i + 1, sum + pick, 1)) % MOD;
        }
    }

    // 存储计算结果
    dp[i][sum][free] = ans;
    return ans;
}

// 主计算函数
// 参数说明：
//   num1 : 区间下限数字字符串
//   num2 : 区间上限数字字符串
//   min_s: 数字和最小值
//   max_s: 数字和最大值
int count(string num1, string num2, int min_s, int max_s) {
    min_sum = min_s;
    max_sum = max_s;

    // 计算<=num2的有效数个数
    num = num2;
    len = num2.size();
    build(); // 初始化记忆化数组
    int ans = f(0, 0, 0); // 从首位开始，初始和为0，受限模式

    // 减去<num1的有效数个数（容斥原理）
    num = num1;
    len = num1.size();
    build();
    ans = (ans - f(0, 0, 0) + MOD) % MOD; // 防止负数出现

    // 补偿计算：如果num1本身有效则+1
    if (check()) {
        ans = (ans + 1) % MOD;
    }

    return ans;
}


#include<iostream>

int main() {
    cout << count("1", "12", 1, 8) << endl;   // 输出：11
    cout << count("100", "203", 2, 5) << endl;// 输出：14
    return 0;
}