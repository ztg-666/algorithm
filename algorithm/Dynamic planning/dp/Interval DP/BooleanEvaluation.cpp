#include <string>
#include <vector>
#include <utility> // 用于 std::pair 和 std::make_pair
#include <optional>  // 用于 std::optional (C++17)

// 布尔运算
// 给定一个布尔表达式和一个期望的布尔结果 result
// 布尔表达式由 0 (false)、1 (true)、& (AND)、 | (OR) 和 ^ (XOR) 符号组成
// 布尔表达式一定是正确的，不需要检查有效性
// 但是其中没有任何括号来表示优先级
// 你可以随意添加括号来改变逻辑优先级
// 目的是让表达式能够最终得出result的结果
// 返回最终得出result有多少种不同的逻辑计算顺序
// 测试链接 : https://leetcode.cn/problems/boolean-evaluation-lcci/

class BooleanEvaluation {
public:

    static int countEval(const std::string& s_str, int result) {
        int n = s_str.length();


        // s.length 是奇数，范围在 1 到 19 之间。
        // 因此，根据约束，n=0 的情况可能不会被测试，但健壮的代码可以处理它。
        if (n == 0) {
            return 0; // 或者抛出错误，取决于对空字符串的预期行为
        }

        // dp[l][r] 将存储一个 std::optional<std::pair<int, int>>。
        // 该 pair 存储子表达式 s_str[l...r] 的 {计算为false的方法数, 计算为true的方法数}。
        // std::nullopt 表示该子问题尚未计算。
        std::vector<std::vector<std::optional<std::pair<int, int>>>> dp(
            n, std::vector<std::optional<std::pair<int, int>>>(n, std::nullopt)
        );

        std::pair<int, int> final_counts = f(s_str, 0, n - 1, dp);

        if (result == 0) {
            return final_counts.first; // 得到 false 的方法数
        } else {
            return final_counts.second; // 得到 true 的方法数
        }
    }

private:
    // 带记忆化的递归辅助函数
    // s: 输入的布尔表达式字符串
    // l: 当前子表达式的起始索引
    // r: 当前子表达式的结束索引
    // dp: 记忆化表格
    // 返回一个 pair: {子表达式计算结果为 false 的方法数, 计算结果为 true 的方法数}
    static std::pair<int, int> f(
        const std::string& s,
        int l,
        int r,
        std::vector<std::vector<std::optional<std::pair<int, int>>>>& dp
    ) {
        // 如果这个子问题已经解决，则返回存储的结果
        if (dp[l][r].has_value()) {
            return dp[l][r].value();
        }

        int false_count = 0;
        int true_count = 0;

        // 基本情况：子表达式是单个字符（'0' 或 '1'）
        if (l == r) {
            false_count = (s[l] == '0') ? 1 : 0;
            true_count = (s[l] == '1') ? 1 : 0;
        } else {
            // 递归步骤：遍历所有可能的运算符位置 'k'
            // 以将表达式 s[l...r] 分割为 s[l...k-1] op s[k+1...r]。
            // 'k' 将是运算符的索引。运算符位于 l+1, l+3, ...
            for (int k = l + 1; k < r; k += 2) {
                // 获取左子表达式 s[l...k-1] 的结果
                std::pair<int, int> left_res = f(s, l, k - 1, dp);
                int left_false = left_res.first;
                int left_true = left_res.second;

                // 获取右子表达式 s[k+1...r] 的结果
                std::pair<int, int> right_res = f(s, k + 1, r, dp);
                int right_false = right_res.first;
                int right_true = right_res.second;

                char op = s[k];

                if (op == '&') {
                    // 表达式为 True 当且仅当：左侧为 True 且 右侧为 True
                    true_count += left_true * right_true;
                    // 表达式为 False 当：
                    //   (左 False 且 右 False) 或
                    //   (左 False 且 右 True) 或
                    //   (左 True 且 右 False)
                    false_count += left_false * right_false +
                                   left_false * right_true +
                                   left_true * right_false;
                } else if (op == '|') {
                    // 表达式为 True 当：
                    //   (左 False 且 右 True) 或
                    //   (左 True 且 右 False) 或
                    //   (左 True 且 右 True)
                    true_count += left_false * right_true +
                                  left_true * right_false +
                                  left_true * right_true;
                    // 表达式为 False 当且仅当：左侧为 False 且 右侧为 False
                    false_count += left_false * right_false;
                } else { // op == '^' (XOR 异或)
                    // 表达式为 True 当：
                    //   (左 False 且 右 True) 或
                    //   (左 True 且 右 False)
                    true_count += left_false * right_true +
                                  left_true * right_false;
                    // 表达式为 False 当：
                    //   (左 False 且 右 False) 或
                    //   (左 True 且 右 True)
                    false_count += left_false * right_false +
                                   left_true * right_true;
                }
            }
        }

        // 存储并返回此子问题的计算结果
        dp[l][r] = std::make_pair(false_count, true_count);
        return dp[l][r].value();
    }
};



#include <iostream>

int main() {
    std::string expr1 = "1^0|0";
    int result1_false = 0; // 期望结果为 false
    int result1_true = 1;  // 期望结果为 true
    std::cout << "表达式: " << expr1 << std::endl;
    std::cout << "得到 False (0) 的方法数: " << BooleanEvaluation::countEval(expr1, result1_false) << std::endl; // 期望: 2 (基于LeetCode示例，但实际计算可能不同)
    std::cout << "得到 True (1) 的方法数: " << BooleanEvaluation::countEval(expr1, result1_true) << std::endl;   // 期望: 0 (基于LeetCode示例，但实际计算可能不同)

    std::string expr2 = "0&0&0&1^1|0";
    int result2 = 1; // 期望结果为 true
    std::cout << "表达式: " << expr2 << std::endl;
    // LeetCode 预期值（如果这是一个示例）：132 (此处仅为假设，实际应参照题目)
    std::cout << "得到 True (1) 的方法数: " << BooleanEvaluation::countEval(expr2, result2) << std::endl;

    return 0;
}

