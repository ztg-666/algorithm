#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <ctime>
// 最大数
// 给定一组非负整数nums
// 重新排列每个数的顺序（每个数不可拆分）使之组成一个最大的整数
// 测试链接 : https://leetcode.cn/problems/largest-number/
using namespace std;

// 生成随机字符串数组
// 参数：n - 控制数组最大长度，m - 控制字符串最大长度，v - 控制字符种类范围
// 返回值：包含随机字符串的vector
vector<string> randomStringArray(int n, int m, int v) {
    int size = rand() % n + 1; // 随机数组长度1~n
    vector<string> ans(size);
    for (int i = 0; i < size; i++) {
        int len = rand() % m + 1; // 随机字符串长度1~m
        string str;
        for (int j = 0; j < len; j++) {
            // 生成随机字符：'a'~'a'+v-1 范围内的字符
            str.push_back('a' + rand() % v);
        }
        ans[i] = str;
    }
    return ans;
}

// 递归生成全排列（回溯法）
// 参数：
//   strs - 待排列字符串数组（引用传递减少拷贝）
//   index - 当前处理的位置
//   ans - 存储所有排列结果的容器
void f(vector<string>& strs, int index, vector<string>& ans) {
    if (index == strs.size()) {
        // 生成完整排列字符串
        string s;
        for (string str : strs) {
            s += str;
        }
        ans.push_back(s);
    } else {
        // 通过交换元素生成所有排列
        for (int i = index; i < strs.size(); i++) {
            swap(strs[index], strs[i]);  // 选择当前元素
            f(strs, index + 1, ans);     // 递归处理后续位置
            swap(strs[index], strs[i]);  // 恢复原始状态（回溯）
        }
    }
}

// 暴力法求最小字典序拼接
// 通过生成所有排列后排序找到最小结果
// 时间复杂度：O(n! * n) （n为字符串数量）
string way1(vector<string>& strs) {
    vector<string> ans;
    f(strs, 0, ans);            // 生成所有排列
    sort(ans.begin(), ans.end()); // 字典序排序
    return ans.empty() ? "" : ans[0]; // 返回最小结果
}

// 排序法求最小字典序拼接（优化方法）
// 通过自定义比较器直接排序得到最优结果
// 时间复杂度：O(n*logn)
string way2(vector<string> strs) {
    // 关键比较逻辑：a+b < b+a 时认为a应该在前
    sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
        return (a + b) < (b + a);
    });
    string s;
    for (string str : strs) {
        s += str;
    }
    return s;
}

// 求解最大数的主函数
// 算法思路：
// 1. 将数字转为字符串
// 2. 自定义排序：比较b+a和a+b的字典序
// 3. 处理全0的特殊情况
string largestNumber(vector<int>& nums) {
    vector<string> strs(nums.size());
    // 数字转字符串
    for (int i = 0; i < nums.size(); i++) {
        strs[i] = to_string(nums[i]);
    }
    // 关键排序逻辑：b+a > a+b 时认为b应该在前
    sort(strs.begin(), strs.end(), [](const string& a, const string& b) {
        return (a + b) > (b + a);
    });
    // 处理全0情况（如输入[0,0]）
    if (strs[0] == "0") {
        return "0";
    }
    // 拼接结果
    string ans;
    for (string s : strs) {
        ans += s;
    }
    return ans;
}

int main() {
    srand(time(0)); // 初始化随机种子

    // 对数器测试：验证way1和way2结果一致性
    int n = 8, m = 5, v = 4;     // 测试参数
    int testTimes = 2000;        // 测试次数
    cout << "" << endl;
    for (int i = 1; i <= testTimes; i++) {
        vector<string> strs = randomStringArray(n, m, v);
        string ans1 = way1(strs); // 暴力法
        string ans2 = way2(strs); // 优化法

        // 结果比对
        if (ans1 != ans2) {
            cout << "！" << endl;
        }
        // 进度输出
        if (i % 100 == 0) {
            cout << " " << i << " " << endl;
        }
    }
    cout << "" << endl;

    // 示例测试
    vector<int> nums = {3, 30, 34, 5, 9};
    cout << largestNumber(nums) << endl; // 预期输出：9534330

    return 0;
}
