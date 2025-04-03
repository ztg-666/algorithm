#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <algorithm> // 用于std::max

using namespace std;

// 动态规划验证方法（仅用于对数器验证）
// 目前没有讲动态规划，所以不需要理解这个函数
// 这个函数只是为了验证二分答案的方法是否正确的
// 纯粹为了写对数器验证才设计的方法，血量比较大的时候会超时
// 这个方法不做要求，此时并不需要理解，可以在学习完动态规划章节之后来看看这个函数
int f1(const vector<int>& cuts, const vector<int>& poisons, int i, int r, int p, vector<vector<vector<int>>>& dp) {
    r -= p; // 减去当前回合的中毒效果
    if (r <= 0) return i + 1; // 如果怪兽血量小于等于0，返回当前回合数加1
    if (i == cuts.size()) { // 如果已经没有回合了
        if (p == 0) return INT_MAX; // 如果没有中毒效果，返回无穷大
        else return cuts.size() + 1 + (r + p - 1) / p; // 否则返回剩余血量除以中毒效果的回合数
    }
    if (dp[i][r][p] != 0) return dp[i][r][p]; // 如果已经计算过，直接返回结果

    // 选择刀砍
    int p1 = (r <= cuts[i]) ? (i + 1) : f1(cuts, poisons, i + 1, r - cuts[i], p, dp);
    // 选择毒杀
    int p2 = f1(cuts, poisons, i + 1, r, p + poisons[i], dp);

    int ans = min(p1, p2); // 取刀砍和毒杀中较小的回合数
    dp[i][r][p] = ans; // 记录结果
    return ans;
}

// 动态规划主函数
int fast1(const vector<int>& cuts, const vector<int>& poisons, int hp) {
    int sum = 0;
    for (int num : poisons) sum += num; // 计算所有中毒效果的总和
    // 初始化三维DP数组
    vector<vector<vector<int>>> dp(cuts.size(), vector<vector<int>>(hp + 1, vector<int>(sum + 1, 0)));
    return f1(cuts, poisons, 0, hp, 0, dp); // 调用递归函数
}

// 二分答案法主算法
// 时间复杂度O(n * log(hp))，额外空间复杂度O(1)
bool canKill(const vector<int>& cuts, const vector<int>& poisons, long long hp, int limit) {
    int n = min((int)cuts.size(), limit); // 计算实际需要处理的回合数
    for (int i = 0, j = 1; i < n; ++i, ++j) {
        long long cut = cuts[i]; // 当前回合刀砍效果
        long long poison = (long long)(limit - j) * poisons[i]; // 当前回合及之后的中毒效果
        hp -= max(cut, poison); // 减去刀砍和中毒效果中较大的一个
        if (hp <= 0) return true; // 如果怪兽血量小于等于0，返回true
    }
    return false; // 否则返回false
}

// 二分查找主函数
int fast2(const vector<int>& cuts, const vector<int>& poisons, int hp) {
    int ans = INT_MAX; // 初始化答案为无穷大
    int l = 1, r = hp + 1; // 初始化二分查找的左右边界
    while (l <= r) {
        int m = l + (r - l) / 2; // 计算中间值
        if (canKill(cuts, poisons, hp, m)) { // 如果可以在m回合内杀死怪兽
            ans = m; // 更新答案
            r = m - 1; // 继续在左半部分查找
        } else {
            l = m + 1; // 否则在右半部分查找
        }
    }
    return ans; // 返回最小的回合数
}

// 随机数组生成
vector<int> randomArray(int n, int v) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % v + 1; // 生成1到v之间的随机数
    }
    return arr;
}

// 对数器测试
int main() {
    srand(time(nullptr)); // 初始化随机种子
    cout << "测试开始" << endl;
    const int N = 30; // 最大回合数
    const int V = 20; // 最大刀砍或中毒效果值
    const int H = 300; // 最大血量
    const int TEST_TIMES = 10000; // 测试次数

    for (int i = 0; i < TEST_TIMES; ++i) {
        int n = rand() % N + 1; // 随机生成回合数
        vector<int> cuts = randomArray(n, V); // 生成刀砍效果数组
        vector<int> poisons = randomArray(n, V); // 生成中毒效果数组
        int hp = rand() % H + 1; // 生成初始血量

        int ans1 = fast1(cuts, poisons, hp); // 使用动态规划方法计算结果
        int ans2 = fast2(cuts, poisons, hp); // 使用二分查找方法计算结果

        if (ans1 != ans2) {
            cout << "出错了!" << endl; // 如果两个方法结果不一致，输出错误信息
        }
    }
    cout << "测试结束" << endl;
    return 0;
}
