#include <vector>
#include <string>
#include <algorithm>

// 最大为N的数字组合
// 给定一个按 非递减顺序 排列的数字数组 digits
// 已知digits一定不包含'0'，可能包含'1' ~ '9'，且无重复字符
// 你可以用任意次数 digits[i] 来写的数字
// 例如，如果 digits = ['1','3','5']
// 我们可以写数字，如 '13', '551', 和 '1351315'
// 返回 可以生成的小于或等于给定整数 n 的正整数的个数
// 测试链接 : https://leetcode.cn/problems/numbers-at-most-n-given-digit-set/

using namespace std;

// 递归辅助函数：深度优先搜索生成有效数字组合
// 参数说明：
//   - digits   : 可用的有序数字集合
//   - num      : 目标数值上限
//   - offset   : 当前处理的数字位权重（如千位、百位等）
//   - len      : 剩余需要处理的位数
//   - free     : 是否可自由选择标志（0-受限，1-自由）
//   - fix      : 是否已确定前缀标志（0-未确定，1-已确定）

int f1(const vector<int>& digits, int num, int offset, int len, int free, int fix) {
    if (len == 0) {
        // 终止条件：当所有位数处理完成时，只有已确定前缀的情况计数为1
        return fix == 1 ? 1 : 0;
    }
    int ans = 0;
    // 获取当前处理位的数字值（如千位数字、百位数字等）
    int cur = (num / offset) % 10;

    // 情况1：尚未确定任何前缀
    if (fix == 0) {
        // 选择跳过当前位，继续处理后续位数（保持未确定前缀状态）
        ans += f1(digits, num, offset / 10, len - 1, 1, 0);
    }

    // 情况2：处理当前位的选择
    if (free == 0) { // 受限选择模式（必须<=原数字对应位）
        for (int i : digits) {
            if (i < cur) {
                // 选择小于当前位的数字，后续位数可自由选择
                ans += f1(digits, num, offset / 10, len - 1, 1, 1);
            } else if (i == cur) {
                // 选择等于当前位的数字，后续位数继续受限
                ans += f1(digits, num, offset / 10, len - 1, 0, 1);
            } else {
                // 遇到大于当前位的数字立即终止（数字已排序）
                break;
            }
        }
    } else { // 自由选择模式（所有可用数字均可选）
        // 当前位有digits.size()种选择，每种选择后续位数自由处理
        ans += digits.size() * f1(digits, num, offset / 10, len - 1, 1, 1);
    }
    return ans;
}

// 解法1：递归回溯法
// 时间复杂度O(L * D)，L为num的位数，D为可用数字数量
int atMostNGivenDigitSet1(vector<string>& strs, int num) {
    if (num == 0) return 0; // 处理边界情况

    // 计算num的位数和最高位权重
    int tmp = num / 10;
    int len = 1;
    int offset = 1;
    while (tmp > 0) {
        tmp /= 10;
        len++;
        offset *= 10;
    }

    // 转换输入数字
    vector<int> digits(strs.size());
    for (int i = 0; i < strs.size(); i++) {
        digits[i] = stoi(strs[i]);
    }
    // 启动递归过程（初始状态：未自由选择，未确定前缀）
    return f1(digits, num, offset, len, 0, 0);
}

// 解法2的辅助函数：组合数学优化
// 参数说明：
//   - cnt : 预计算的组合数数组，cnt[k]表示k位数的自由组合数
int f2(const vector<int>& digits, const vector<int>& cnt, int num, int offset, int len) {
    if (len == 0) {
        // 当所有位数精确匹配时，计数为1
        return 1;
    }
    int cur = (num / offset) % 10;
    int ans = 0;
    for (int i : digits) {
        if (i < cur) {
            // 当前位选择较小数字，后续位数自由组合
            ans += cnt[len - 1];
        } else if (i == cur) {
            // 当前位精确匹配，继续处理后续位数
            ans += f2(digits, cnt, num, offset / 10, len - 1);
        } else {
            // 遇到较大数字立即终止（数字已排序）
            break;
        }
    }
    return ans;
}

// 解法2：数学组合优化法
// 时间复杂度O(L)，L为num的位数
int atMostNGivenDigitSet2(vector<string>& strs, int num) {
    if (num == 0) return 0;

    // 转换并排序输入数字
    vector<int> digits(strs.size());
    for (int i = 0; i < strs.size(); i++) {
        digits[i] = stoi(strs[i]);
    }
    sort(digits.begin(), digits.end());

    // 计算num的位数结构
    int len = 1;
    int offset = 1;
    int tmp = num / 10;
    while (tmp > 0) {
        tmp /= 10;
        len++;
        offset *= 10;
    }

    // 预计算组合数数组
    // cnt[k] 表示k位数的自由组合数（k < len）
    vector<int> cnt(len);
    cnt[0] = 1; // 基准情况
    int ans = 0;
    // 累加所有位数小于len的情况（自由组合数）
    for (int i = digits.size(), k = 1; k < len; k++, i *= digits.size()) {
        cnt[k] = i;
        ans += i;
    }

    // 处理位数等于len的情况（精确匹配情况）
    return ans + f2(digits, cnt, num, offset, len);
}
