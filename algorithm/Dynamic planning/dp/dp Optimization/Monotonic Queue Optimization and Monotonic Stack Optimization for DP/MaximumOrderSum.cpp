#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstring>

// 子数组最大变序和
// 给定一个长度为n的数组arr，变序和定义如下
// 数组中每个值都可以减小或者不变，必须把整体变成严格升序的
// 所有方案中，能得到的最大累加和，叫做数组的变序和
// 比如[1,100,7]，变序和14，方案为变成[1,6,7]
// 比如[5,4,9]，变序和16，方案为变成[3,4,9]
// 比如[1,4,2]，变序和3，方案为变成[0,1,2]
// 返回arr所有子数组的变序和中，最大的那个
// 1 <= n、arr[i] <= 10^6

using namespace std;


class MaximumOrderSum {
public:
    /**
     * 暴力方法 - 用于验证正确性
     * 时间复杂度：O(n * v)，其中n为数组长度，v为数组中最大值
     * 空间复杂度：O(n * v)
     */
    static long long maxSum1(const vector<int>& arr) {
        int n = arr.size();

        // 找到数组中的最大值，用于确定DP数组的大小
        int maxVal = 0;
        for (int num : arr) {
            maxVal = max(maxVal, num);
        }

        long long ans = 0;
        // dp[i][j]表示以位置i结尾，且该位置的值不超过j时的最大变序和
        vector<vector<long long>> dp(n, vector<long long>(maxVal + 1, -1));

        // 枚举所有可能的子数组结尾位置
        for (int i = 0; i < n; i++) {
            ans = max(ans, f1(arr, i, arr[i], dp));
        }
        return ans;
    }

    /**
     * 递归函数 - 暴力方法的核心逻辑
     * @param arr 原数组
     * @param i 当前处理的位置
     * @param p 当前位置的值不能超过p（保证严格升序）
     * @param dp 记忆化数组
     * @return 以位置i结尾，且i位置变成的数字不能大于p的情况下，子数组的最大变序和
     */
    static long long f1(const vector<int>& arr, int i, int p, vector<vector<long long>>& dp) {
        // 边界条件：如果p <= 0或者i超出范围，返回0
        if (p <= 0 || i == -1) {
            return 0;
        }

        // 如果已经计算过，直接返回记忆化的结果
        if (dp[i][p] != -1) {
            return dp[i][p];
        }

        // 当前位置的值：原值和限制值p的较小者
        int cur = min(arr[i], p);

        // 递归计算前一个位置的最大变序和（限制值为cur-1，保证严格升序）
        long long next = f1(arr, i - 1, cur - 1, dp);

        // 当前子数组的最大变序和
        long long ans = (long long)cur + next;

        // 记忆化存储结果
        dp[i][p] = ans;
        return ans;
    }

    /**
     * 优化方法 - 使用单调栈
     * 时间复杂度：O(n)
     * 空间复杂度：O(n)
     *
     * 核心思想：
     * 使用单调栈维护一个递增序列，通过巧妙的计算避免重复计算
     */
    static long long maxSum2(const vector<int>& arr) {
        int n = arr.size();
        vector<int> stack(n);  // 单调栈，存储数组下标
        int size = 0;          // 栈的大小
        vector<long long> dp(n, 0);  // dp[i]表示以位置i结尾的最大变序和
        long long ans = 0;     // 全局最大变序和

        for (int i = 0; i < n; i++) {
            int curIdx = i;        // 当前处理的位置
            int curVal = arr[i];   // 当前位置的值

            // 处理单调栈，维护递增性质
            while (curVal > 0 && size > 0) {
                int topIdx = stack[size - 1];  // 栈顶元素的下标
                int topVal = arr[topIdx];      // 栈顶元素的值

                if (topVal >= curVal) {
                    // 如果栈顶元素大于等于当前值，直接弹出栈顶
                    size--;
                } else {
                    // 栈顶元素小于当前值，需要进行计算
                    int idxDiff = curIdx - topIdx;  // 位置差
                    int valDiff = curVal - topVal;  // 值差

                    if (valDiff >= idxDiff) {
                        // 如果值差大于等于位置差，可以构造一个完整的递增序列
                        dp[i] += sum(curVal, idxDiff) + dp[topIdx];
                        curVal = 0;  // 当前值处理完毕
                        curIdx = 0;  // 重置当前位置
                        break;
                    } else {
                        // 如果值差小于位置差，只能构造部分递增序列
                        dp[i] += sum(curVal, idxDiff);
                        curVal -= idxDiff;  // 更新当前值
                        curIdx = topIdx;    // 更新当前位置
                        size--;             // 弹出栈顶
                    }
                }
            }

            // 处理剩余的curVal
            if (curVal > 0) {
                dp[i] += sum(curVal, curIdx + 1);
            }

            // 将当前位置入栈
            stack[size++] = i;

            // 更新全局最大值
            ans = max(ans, dp[i]);
        }
        return ans;
    }

    /**
     * 计算等差数列的和
     * @param maxVal 最大值（首项）
     * @param n 项数
     * @return 从maxVal开始，公差为-1的等差数列的前n项和（只考虑正数部分）
     *
     * 例如：sum(5, 3) = 5 + 4 + 3 = 12
     *      sum(3, 5) = 3 + 2 + 1 = 6 (因为只取正数部分)
     */
    static long long sum(int maxVal, int n) {
        n = min(maxVal, n);  // 确保不会计算负数
        // 等差数列求和公式：(首项 + 末项) * 项数 / 2
        // 首项：maxVal，末项：maxVal - n + 1，项数：n
        return ((long long)maxVal * 2 - n + 1) * n / 2;
    }

    /**
     * 生成随机数组用于测试
     * @param n 数组长度
     * @param v 数值范围[0, v-1]
     * @return 随机生成的数组
     */
    static vector<int> randomArray(int n, int v) {
        vector<int> ans(n);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, v - 1);

        for (int i = 0; i < n; i++) {
            ans[i] = dis(gen);
        }
        return ans;
    }

    /**
     * 测试函数
     * 包含正确性测试和性能测试
     */
    static void test() {
        // 正确性测试参数
        int n = 100;
        int v = 100;
        int testTimes = 50000;
        cout << "" << endl;

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> sizeDis(1, n);

        // 进行多次随机测试，验证两种方法的结果是否一致
        for (int i = 0; i < testTimes; i++) {
            int size = sizeDis(gen);
            vector<int> arr = randomArray(size, v);
            long long ans1 = maxSum1(arr);  // 暴力方法结果
            long long ans2 = maxSum2(arr);  // 优化方法结果
            if (ans1 != ans2) {
                cout << "!" << endl;
                return;
            }
        }
        cout << "" << endl;

        // 性能测试
        cout << "" << endl;
        n = 1000000;
        v = 1000000;
        cout << " : " << n << endl;
        cout << " : " << v << endl;

        vector<int> arr = randomArray(n, v);
        auto start = chrono::high_resolution_clock::now();
        maxSum2(arr);  // 只测试优化方法的性能
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << " : " << duration.count() << " " << endl;
        cout << "" << endl;
    }
};

/**
 * 主函数
 * 运行测试程序
 */
int main() {
    MaximumOrderSum::test();
    return 0;
}
