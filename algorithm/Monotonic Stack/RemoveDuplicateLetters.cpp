#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

string removeDuplicateLetters(string str) {
    const int MAXN = 26;  // 常量，表示字母表的大小（假设只处理小写字母）
    int cnts[MAXN];         // 用于统计每个字符出现的次数
    bool enter[MAXN];       // 用于标记每个字符是否已经进入栈中
    char stack[MAXN];       // 单调栈，用于构建结果字符串
    int r = 0;              // 栈顶指针，指向栈顶元素的下一个位置

    // 初始化数组
    memset(cnts, 0, sizeof(cnts));  // 将cnts数组所有元素初始化为0
    memset(enter, 0, sizeof(enter));  // 将enter数组所有元素初始化为false

    // 统计每个字符出现的次数
    for (char c : str) {
        cnts[c - 'a']++;  // 对应字符的计数加1
    }

    // 处理每个字符
    for (char cur : str) {
        int idx = cur - 'a';  // 计算当前字符在cnts和enter数组中的索引
        if (!enter[idx]) {    // 如果当前字符还没有进入栈中
            // 维护单调栈：栈非空 且 栈顶字符大于当前字符 且 栈顶字符后续还会出现
            while (r > 0 && stack[r-1] > cur && cnts[stack[r-1] - 'a'] > 0) {
                enter[stack[r-1] - 'a'] = false;  // 标记栈顶字符已不在栈中
                r--;  // 弹出栈顶字符
            }
            stack[r++] = cur;    // 当前字符入栈
            enter[idx] = true;   // 标记当前字符已进入栈中
        }
        cnts[idx]--; // 剩余出现次数减一
    }

    // 构建并返回结果字符串
    return string(stack, stack + r);
}
int main() {
    // 测试用例
    vector<string> testCases = {
        "bcabc",
        "cbacdcbc",
        "aabbcc",
        "abcd",
        "ecbacba",
        "leetcode",
        "banana"
    };

    // 预期输出
    vector<string> expectedResults = {
        "abc",
        "acdb",
        "abc",
        "abcd",
        "eacb",
        "letcod",
        "ban"
    };

    // 运行测试
    for (size_t i = 0; i < testCases.size(); ++i) {
        string result = removeDuplicateLetters(testCases[i]);
        cout << "Test case " << i + 1 << ": " << testCases[i] << endl;
        cout << "Expected: " << expectedResults[i] << endl;
        cout << "Result:   " << result << endl;
        cout << (result == expectedResults[i] ? "PASS" : "FAIL") << endl;
        cout << "--------------------------" << endl;
    }

    return 0;
}