#include <string>
#include <vector>
#include <algorithm>

// 最大回文数字
// 给你一个仅由数字（0 - 9）组成的字符串num
// 请你找出能够使用num中数字形成的最大回文整数
// 并以字符串形式返回，该整数不含前导零
// 你无需使用num中的所有数字，但你必须使用至少一个数字，数字可以重新排列
// 测试链接 : https://leetcode.cn/problems/largest-palindromic-number/

using namespace std;
string largestPalindromic(string num) {
    int n = num.size();
    vector<int> cnts(58, 0); // 对应ASCII码 '0'-'9' 的索引范围
    for (char c : num) {
        cnts[c]++;
    }

    string ans(n, '\0'); // 预分配足够空间
    int leftSize = 0;
    char mid = '\0';

    // 处理'9'到'1'的数字
    for (char i = '9'; i >= '1'; i--) {
        if ((cnts[i] & 1) && !mid) {
            mid = i;
        }
        for (int j = cnts[i] / 2; j > 0; j--) {
            ans[leftSize++] = i;
        }
    }

    // 处理没有非零数字的情况
    if (leftSize == 0) {
        if (mid) { // 存在奇数次数的非零数字
            return string(1, mid);
        } else { // 所有非零数字出现次数<=1，检查是否有0
            if (cnts['0'] > 0) return "0";
            return ""; // 理论上不会走到这里，因为题目保证至少一个数字
        }
    }

    // 处理'0'的偶数部分
    for (int j = cnts['0'] / 2; j > 0; j--) {
        ans[leftSize++] = '0';
    }

    // 确定中点
    if (mid == 0 && (cnts['0'] & 1) == 1) {
        mid = '0';
    }

    int len = leftSize;
    if (mid) {
        ans[len++] = mid;
    }

    // 镜像右半部分
    for (int i = leftSize - 1; i >= 0; i--) {
        ans[len++] = ans[i];
    }

    return ans.substr(0, len);
}
