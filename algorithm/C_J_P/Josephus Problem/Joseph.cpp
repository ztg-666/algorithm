#include <bits/stdc++.h>
using namespace std;

// 约瑟夫环问题
// 给定 n 个人（编号 1~n），围成一圈报数
// 从 1 号开始报数，每次数到 k 的人被淘汰
// 淘汰后从下一个人重新从 1 开始报数
// 最后只剩下一个人，返回他的编号
// 1 <= n, k <= 1e6

// 迭代公式解法（避免递归栈溢出）：
// f(1) = 1
// f(c) = (f(c-1) + k - 1) % c + 1   (c 表示当前总人数)
// 最终答案是 f(n)
int compute(int n, int k) {
    int ans = 1; // 初始情况：只有 1 个人时，存活的是 1 号
    for (int c = 2; c <= n; c++) {
        // 每增加一个人（人数 = c），
        // 新的幸存者编号可以通过公式递推得到
        ans = (ans + k - 1) % c + 1;
    }
    return ans; // 返回最终幸存者的编号
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;              // 输入 n 和 k
    cout << compute(n, k) << "\n"; // 输出最后幸存者编号

    return 0;
}
