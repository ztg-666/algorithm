#include <vector>
#include <algorithm> // 需要包含 algorithm 头文件用于 std::max

// 跳跃游戏II
// 给定一个长度为n的整数数组nums
// 你初始在0下标，nums[i]表示你可以从i下标往右跳的最大距离
// 比如，nums[0] = 3
// 表示你可以从0下标去往：1下标、2下标、3下标
// 你达到i下标后，可以根据nums[i]的值继续往右跳
// 返回你到达n-1下标的最少跳跃次数
// 测试用例可以保证一定能到达
// 测试链接 : https://leetcode.cn/problems/jump-game-ii/

using namespace std;

int jump(vector<int>& arr) {
    int n = arr.size();
    int cur = 0;    // 当前步能覆盖的最远位置
    int next = 0;   // 下一步能覆盖的最远位置
    int ans = 0;    // 跳跃次数

    for (int i = 0; i < n; i++) {
        if (cur < i) {      // 当前位置超出当前步覆盖范围
            ans++;          // 必须多跳一步
            cur = next;     // 更新当前步覆盖范围
        }
        next = max(next, i + arr[i]); // 持续更新下一步能到达的最远位置
    }

    return ans;
}
