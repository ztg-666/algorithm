#include <iostream>
#include <vector>
#include <algorithm>

// 相邻与结果不为0的最长子序列
// 给定一个长度为n的数组arr，你可以随意选择数字组成子序列
// 但是要求任意相邻的两个数&的结果不能是0，这样的子序列才是合法的
// 返回最长合法子序列的长度
// 1 <= n <= 10^5
// 0 <= arr[i] <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P4310

using namespace std;

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // 读取数组元素
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // pre[j]表示以第j位二进制位为1结尾的最长合法子序列长度
    // 由于数字最大为10^9，约为2^30，所以需要32位
    vector<int> pre(32, 0);

    // 遍历每个数字
    for (int i = 0; i < n; i++) {
        int num = arr[i];
        int cur = 1; // 当前数字自己单独构成的子序列长度为1

        // 第一次遍历：找到当前数字能够接在哪些位置之后
        // 如果当前数字的第j位为1，那么它可以接在任何第j位为1的数字之后
        // 因为两个数字的第j位都为1时，它们的&运算结果至少第j位为1，不为0
        for (int j = 0; j < 31; j++) {
            if (((num >> j) & 1) == 1) { // 检查第j位是否为1
                cur = max(cur, pre[j] + 1); // 更新当前能达到的最长长度
            }
        }

        // 第二次遍历：更新pre数组
        // 对于当前数字的每个为1的二进制位，更新对应的pre值
        for (int j = 0; j < 31; j++) {
            if (((num >> j) & 1) == 1) { // 如果第j位为1
                pre[j] = max(pre[j], cur); // 更新以第j位为1结尾的最长长度
            }
        }
    }

    // 找到所有位置中的最大值作为答案
    int ans = 0;
    for (int j = 0; j < 31; j++) {
        ans = max(ans, pre[j]);
    }

    cout << ans << endl;

    return 0;
}
