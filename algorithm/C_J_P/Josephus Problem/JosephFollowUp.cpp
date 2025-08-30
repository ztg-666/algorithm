#include <bits/stdc++.h>

// 约瑟夫环问题加强
// 一共有1~n这些点，组成首尾相接的环，游戏一共有n-1轮，每轮给定一个数字arr[i]
// 第一轮游戏中，1号点从数字1开始报数，哪个节点报到数字arr[1]，就删除该节点
// 然后下一个节点从数字1开始重新报数，游戏进入第二轮
// 第i轮游戏中，哪个节点报到数字arr[i]，就删除该节点
// 然后下一个节点从数字1开始重新报数，游戏进入第i+1轮
// 最终环上会剩下一个节点, 返回该节点的编号
// 1 <= n, arr[i] <= 10^6

using namespace std;

// 约瑟夫环问题加强版
// n 个点 (1~n)，依次报数
// 第 i 轮用 arr[i] 作为报数目标
// 报到 arr[i] 的人出局，下一轮从下一个人开始
// 直到剩下 1 个点，返回它的编号
// 1 <= n, arr[i] <= 1e6

// ---------------------
// 暴力模拟解法 O(n^2) （用于测试正确性）
// ---------------------
int joseph1(int n, const vector<int>& arr) {
    if (n == 1) return 1;

    // next[i] 表示 i 的下一个存活节点
    vector<int> nxt(n + 1);
    for (int i = 1; i < n; i++) {
        nxt[i] = i + 1;
    }
    nxt[n] = 1; // 首尾相连

    int pre = n, cur = 1;
    for (int i = 1; i < n; i++) {
        // 数 arr[i] 下
        for (int cnt = 1; cnt < arr[i]; cnt++) {
            pre = cur;
            cur = nxt[cur];
        }
        // 删除 cur
        nxt[pre] = nxt[cur];
        cur = nxt[cur];
    }
    return cur;
}

// ---------------------
// 正式解法 O(n)
// 递推公式：类似普通约瑟夫环，只是每轮 k 变化
// ---------------------
int joseph2(int n, const vector<int>& arr) {
    if (n == 1) return 1;
    int ans = 1;
    // 从 2 个人开始推，arr 从后往前用
    for (int c = 2, i = n - 1; c <= n; c++, i--) {
        ans = (ans + arr[i] - 1) % c + 1;
    }
    return ans;
}

// ---------------------
// 随机生成 arr
// ---------------------
vector<int> randomArray(int n, int v) {
    vector<int> arr(n);
    for (int i = 1; i < n; i++) {
        arr[i] = rand() % v + 1;
    }
    return arr;
}

// ---------------------
// 测试器
// ---------------------
int main() {
    srand(time(nullptr));
    int N = 100;   // 最大 n
    int V = 500;   // 最大报数值
    int test = 10000;
    cout << "\n";
    for (int t = 1; t <= test; t++) {
        int n = rand() % N + 1;
        auto arr = randomArray(n, V);
        int ans1 = joseph1(n, arr);
        int ans2 = joseph2(n, arr);
        if (ans1 != ans2) {
            cout << "! n=" << n << "\n";
            return 0;
        }
    }
    cout << "\n";
    return 0;
}
