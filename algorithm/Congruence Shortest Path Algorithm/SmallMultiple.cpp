#include <bits/stdc++.h>
using namespace std;

// 正整数倍的最小数位和
// 给定一个整数k，求一个k的正整数倍s，使得在十进制下，s的数位累加和最小
// 2 <= k <= 10^5
// 测试链接 : https://www.luogu.com.cn/problem/AT_arc084_b
// 测试链接 : https://atcoder.jp/contests/abc077/tasks/arc084_b

const int MAXK = 100001;
int k;
bool visited[MAXK]; // 记录余数状态是否访问过
deque<pair<int,int>> dq;

// 01-BFS 寻找最小数位和
int bfs() {

    dq.emplace_back(1, 1); // 起始状态：余数 1，数位和 1

    while (!dq.empty()) {
        auto [mod, cost] = dq.front();
        dq.pop_front();
        if (visited[mod]) continue;
        visited[mod] = true;

        if (mod == 0) return cost; // 找到结果

        // 添加数字 0：cost 不变，优先处理，放前端
        dq.emplace_front((mod * 10) % k, cost);

        // 添加数字 1：cost + 1，放后端
        dq.emplace_back((mod + 1) % k, cost + 1);
    }
    return -1; // 不会到达
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> k;
    cout << bfs() << "\n";
    return 0;
}
