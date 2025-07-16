#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>

// 过河踩过的最少石子数
// 在河上有一座独木桥，一只青蛙想沿着独木桥从河的一侧跳到另一侧
// 在桥上有一些石子，青蛙很讨厌踩在这些石子上
// 我们可以把独木桥上青蛙可能到达的点看成数轴上的一串整点0...n
// 其中n是桥的长度，坐标为0的点表示桥的起点，坐标为n的点表示桥的终点
// 青蛙从桥的起点开始，不停的向终点方向跳跃，一次跳跃的距离是[s,t]之间的任意正整数
// 当青蛙跳到或跳过坐标为n的点时，就算青蛙已经跳出了独木桥
// 题目给出独木桥的长度n，青蛙跳跃的距离范围s、t，题目还给定m个桥上石子的位置
// 你的任务是确定青蛙要想过河，最少需要踩到的石子数
// 1 <= n <= 10^7
// 1 <= s <= t <= 10
// 1 <= m <= 100
// 测试链接 : https://www.luogu.com.cn/problem/P1052

using namespace std;

// 常量定义
const int MAXN = 101;      // 最大石子数量
const int MAXL = 100001;   // 最大桥长度（压缩后）
const int MAXK = 201;      // 用于计算安全距离的最大值

// 全局数组
int arr[MAXN];      // 存储石子的原始位置
int where[MAXN];    // 存储石子的压缩位置
int dp[MAXL];       // 动态规划数组，dp[i]表示到达位置i最少踩到的石子数
bool stone[MAXL];   // 标记压缩后的位置是否有石子
bool reach[MAXK];   // 用于计算安全距离的辅助数组

int n, s, t, m, safe;  // n:桥长度, s:最小跳跃距离, t:最大跳跃距离, m:石子数量, safe:安全距离

/**
 * 计算安全距离
 * 当相邻两个石子距离超过safe时，可以压缩为safe
 * 这样可以避免处理过长的空白区域
 */
int reduce(int s, int t) {
    fill(reach, reach + MAXK, false);  // 初始化可达数组
    int cnt = 0;  // 连续可达的位置数
    int ans = 0;  // 返回的安全距离

    for (int i = 0; i < MAXK; i++) {
        // 从位置i可以跳到的所有位置
        for (int j = i + s; j < min(i + t + 1, MAXK); j++) {
            reach[j] = true;
        }

        if (!reach[i]) {
            cnt = 0;  // 如果位置i不可达，重置计数
        } else {
            cnt++;    // 如果位置i可达，增加计数
        }

        // 如果连续t个位置都可达，说明找到了安全距离
        if (cnt == t) {
            ans = i;
            break;
        }
    }
    return ans;
}

/**
 * 主要计算函数
 */
int compute() {
    // 对石子位置进行排序
    sort(arr + 1, arr + m + 1);

    // 特殊情况：跳跃距离固定
    if (s == t) {
        int ans = 0;
        // 只有在跳跃路径上的石子才会被踩到
        for (int i = 1; i <= m; i++) {
            if (arr[i] % s == 0) {
                ans++;
            }
        }
        return ans;
    } else { // s < t，跳跃距离可变
        // 计算安全距离
        safe = 201;
        safe = reduce(s, t);

        // 初始化石子标记数组
        fill(stone, stone + MAXL, false);

        // 距离压缩：将相邻石子间过长的距离压缩为安全距离
        for (int i = 1; i <= m; i++) {
            where[i] = where[i - 1] + min(arr[i] - arr[i - 1], safe);
            stone[where[i]] = true;  // 标记压缩后位置有石子
        }

        // 更新桥的长度为压缩后的长度
        n = where[m] + safe;

        // 初始化dp数组，除了起点外都设为最大值
        fill(dp + 1, dp + n + 1, MAXN);

        // 动态规划求解
        for (int i = 1; i <= n; i++) {
            // 从所有可能的前一个位置转移到当前位置
            for (int j = max(i - t, 0); j <= i - s; j++) {
                // 状态转移方程：dp[i] = min(dp[j] + (当前位置有石子 ? 1 : 0))
                dp[i] = min(dp[i], dp[j] + (stone[i] ? 1 : 0));
            }
        }

        // 找到越过最后一个石子后的最小值
        int ans = MAXN;
        for (int i = where[m] + 1; i <= n; i++) {
            ans = min(ans, dp[i]);
        }
        return ans;
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入
    cin >> n >> s >> t >> m;

    // 读取石子位置
    for (int i = 1; i <= m; i++) {
        cin >> arr[i];
    }

    // 输出结果
    cout << compute() << endl;

    return 0;
}
