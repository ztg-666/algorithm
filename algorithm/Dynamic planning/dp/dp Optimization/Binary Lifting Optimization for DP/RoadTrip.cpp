#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

// 开车旅行
// 给定一个长度为n的数组arr，下标1 ~ n范围，数组无重复值
// 近的定义、距离的定义，和题目4一致
// a和b同坐一辆车开始往右旅行，a先开车，b后开车，此后每到达一点都换人驾驶
// 如果a在某点驾驶，那么车去往该点右侧第二近的点，如果b在某点驾驶，那么车去往该点右侧第一近的点
// a和b从s位置出发，如果开车总距离超过x，或轮到某人时右侧无点可选，那么旅行停止
// 问题1 : 给定距离x0，返回1 ~ n-1中从哪个点出发，a行驶距离 / b行驶距离，比值最小
//         如果从多个点出发时，比值都为最小，那么返回arr中的值最大的点
// 问题2 : 给定s、x，返回旅行停止时，a开了多少距离、b开了多少距离
// 问题1只调用1次
// 问题2调用m次，每组有不同的s、x
// 1 <= n、m、s <= 10^5
// -10^9 <= arr[i] <= 10^9
// 1 <= x0、x <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P1081

using namespace std;

const int MAXN = 100002;  // 最大节点数
const int MAXP = 20;      // 倍增表的最大层数 (2^20 > 10^6)

// 基础数据
int arr[MAXN];            // 存储每个位置的坐标值
int to1[MAXN];            // 每个位置右侧第一近的点
int dist1[MAXN];          // 到第一近点的距离
int to2[MAXN];            // 每个位置右侧第二近的点
int dist2[MAXN];          // 到第二近点的距离
pair<int, int> rank_arr[MAXN];  // 用于排序的数组：(坐标值, 位置索引)
int last_arr[MAXN];       // 链表结构：当前位置的前一个位置
int next_arr[MAXN];       // 链表结构：当前位置的后一个位置

// 倍增表，用于快速查询
int stto[MAXN][MAXP + 1];      // stto[i][p] : 从i位置出发，a和b轮流开2^p轮之后，车到达了几号点
long long stdist[MAXN][MAXP + 1];  // stdist[i][p] : 从i位置出发，a和b轮流开2^p轮之后，总距离是多少
long long sta[MAXN][MAXP + 1];     // sta[i][p] : 从i位置出发，a和b轮流开2^p轮之后，a行驶了多少距离
long long stb[MAXN][MAXP + 1];     // stb[i][p] : 从i位置出发，a和b轮流开2^p轮之后，b行驶了多少距离

int n, m;                 // n: 节点数量, m: 查询数量
long long a_dist, b_dist; // a和b的行驶距离

/**
 * 更新位置i的最近邻和次近邻信息
 * @param i 当前位置
 * @param j 候选邻居位置
 */
void update(int i, int j) {
    if (j == 0) {  // j为0表示无效位置
        return;
    }
    int dist = abs(arr[i] - arr[j]);  // 计算距离

    // 如果j是更好的最近邻 (距离更小，或距离相同但坐标值更小)
    if (to1[i] == 0 || dist < dist1[i] || (dist == dist1[i] && arr[j] < arr[to1[i]])) {
        to2[i] = to1[i];      // 原来的最近邻变为次近邻
        dist2[i] = dist1[i];
        to1[i] = j;           // 更新最近邻
        dist1[i] = dist;
    }
    // 如果j是更好的次近邻
    else if (to2[i] == 0 || dist < dist2[i] || (dist == dist2[i] && arr[j] < arr[to2[i]])) {
        to2[i] = j;           // 更新次近邻
        dist2[i] = dist;
    }
}

/**
 * 从链表中删除节点i
 * @param i 要删除的节点
 */
void delete_node(int i) {
    int l = last_arr[i];  // 前一个节点
    int r = next_arr[i];  // 后一个节点
    if (l != 0) {
        next_arr[l] = r;  // 前一个节点指向后一个节点
    }
    if (r != 0) {
        last_arr[r] = l;  // 后一个节点指向前一个节点
    }
}

/**
 * 计算每个位置的最近邻和次近邻
 * 使用排序 + 双向链表的方法，每次处理完一个位置后将其从链表中删除
 */
void near() {
    // 按坐标值排序
    for (int i = 1; i <= n; i++) {
        rank_arr[i] = {arr[i], i};
    }
    sort(rank_arr + 1, rank_arr + n + 1);

    // 设置边界节点
    rank_arr[0] = {0, 0};
    rank_arr[n + 1] = {0, 0};

    // 构建双向链表 (按坐标值排序)
    for (int i = 1; i <= n; i++) {
        last_arr[rank_arr[i].second] = rank_arr[i - 1].second;
        next_arr[rank_arr[i].second] = rank_arr[i + 1].second;
    }

    // 对每个位置计算最近邻和次近邻
    for (int i = 1; i <= n; i++) {
        to1[i] = 0;
        dist1[i] = 0;
        to2[i] = 0;
        dist2[i] = 0;

        // 考虑左侧的两个邻居
        update(i, last_arr[i]);
        update(i, last_arr[last_arr[i]]);

        // 考虑右侧的两个邻居
        update(i, next_arr[i]);
        update(i, next_arr[next_arr[i]]);

        // 处理完位置i后，将其从链表中删除
        delete_node(i);
    }
}

/**
 * 构建倍增表
 * 用于快速查询a和b轮流开车多轮后的状态
 */
void st() {
    // 倍增初始化：每个位置开始的一轮 (a开到第二近，b开到第一近)
    for (int i = 1; i <= n; i++) {
        stto[i][0] = to1[to2[i]];              // 一轮后到达的位置
        stdist[i][0] = dist2[i] + dist1[to2[i]]; // 一轮的总距离
        sta[i][0] = dist2[i];                   // a在这一轮的距离
        stb[i][0] = dist1[to2[i]];              // b在这一轮的距离
    }

    // 生成倍增表：dp[i][p] = dp[i][p-1] + dp[next[i][p-1]][p-1]
    for (int p = 1; p <= MAXP; p++) {
        for (int i = 1; i <= n; i++) {
            stto[i][p] = stto[stto[i][p - 1]][p - 1];  // 2^p轮后到达的位置
            if (stto[i][p] != 0) {  // 如果还有有效位置
                stdist[i][p] = stdist[i][p - 1] + stdist[stto[i][p - 1]][p - 1]; // 总距离累加
                sta[i][p] = sta[i][p - 1] + sta[stto[i][p - 1]][p - 1];         // a的距离累加
                stb[i][p] = stb[i][p - 1] + stb[stto[i][p - 1]][p - 1];         // b的距离累加
            }
        }
    }
}

/**
 * 模拟从位置s出发，在距离限制x下的旅行过程
 * @param s 起始位置
 * @param x 距离限制
 */
void travel(int s, long long x) {
    a_dist = 0;  // a的总距离
    b_dist = 0;  // b的总距离

    // 使用倍增表快速跳跃
    for (int p = MAXP; p >= 0; p--) {
        // 如果可以跳跃2^p轮且不超过距离限制
        if (stto[s][p] != 0 && x >= stdist[s][p]) {
            x -= stdist[s][p];        // 更新剩余距离
            a_dist += sta[s][p];      // 累加a的距离
            b_dist += stb[s][p];      // 累加b的距离
            s = stto[s][p];           // 更新当前位置
        }
    }

    // 最后检查是否还能再走一步 (a开车)
    if (dist2[s] <= x) {
        a_dist += dist2[s];
    }
}

/**
 * 问题1：找到使得a行驶距离/b行驶距离比值最小的起始位置
 * @param x0 距离限制
 * @return 最优起始位置
 */
int best(long long x0) {
    int ans = 0;              // 最优起始位置
    double min_ratio = 1e18;  // 最小比值
    double cur;               // 当前比值

    // 遍历所有可能的起始位置 (1 到 n-1)
    for (int i = 1; i < n; i++) {
        travel(i, x0);  // 模拟从位置i出发的旅行

        // 计算比值 (如果b距离为0，比值设为无穷大)
        cur = (b_dist == 0) ? 1e18 : ((double) a_dist / (double) b_dist);

        // 更新最优解 (比值更小，或比值相同但坐标值更大)
        if (ans == 0 || cur < min_ratio || (cur == min_ratio && arr[i] > arr[ans])) {
            ans = i;
            min_ratio = cur;
        }
    }
    return ans;
}

/**
 * 主函数
 */
int main() {
    ios::sync_with_stdio(false);  // 加速IO
    cin.tie(nullptr);

    // 读入数据
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 预处理
    near();  // 计算最近邻和次近邻
    st();    // 构建倍增表

    // 问题1：找最优起始位置
    long long x0;
    cin >> x0;
    cout << best(x0) << "\n";

    // 问题2：回答m个旅行查询
    cin >> m;
    for (int i = 1; i <= m; i++) {
        int s;
        long long x;
        cin >> s >> x;
        travel(s, x);  // 模拟旅行
        cout << a_dist << " " << b_dist << "\n";
    }

    return 0;
}
