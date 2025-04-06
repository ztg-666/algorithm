#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

const int MAXN = 100001;
int father[MAXN];    // 并查集的父节点数组
bool secret[MAXN];   // 标记每个集合是否包含知晓秘密的专家

// 初始化并查集和秘密状态
void build(int n, int first) {
    for (int i = 0; i < n; ++i) {
        father[i] = i; // 每个专家的父节点初始化为其自身
        secret[i] = false; // 初始时所有专家都不知晓秘密
    }
    father[first] = 0; // first 专家的父节点设为 0
    secret[0] = true;  // 专家 0 知晓秘密
}

// 查找节点 i 的根节点，并进行路径压缩
int find(int i) {
    if (father[i] != i) {
        father[i] = find(father[i]); // 路径压缩
    }
    return father[i]; // 返回根节点
}

// 合并两个集合，并传播秘密状态
void unionSet(int x, int y) {
    int fx = find(x); // 找到 x 的根节点
    int fy = find(y); // 找到 y 的根节点
    if (fx != fy) {
        father[fx] = fy; // 合并两个集合
        secret[fy] = secret[fy] || secret[fx]; // 传播秘密状态
    }
}

// 找出知晓秘密的所有专家
vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int first) {
    build(n, first); // 初始化并查集和秘密状态

    // 按会议时间升序排序
    sort(meetings.begin(), meetings.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[2] < b[2]; // 比较会议时间
    });

    int m = meetings.size();
    for (int l = 0; l < m; ) {
        int r = l;
        // 找到同一时间的所有会议
        while (r < m && meetings[l][2] == meetings[r][2]) {
            ++r;
        }
        --r; // 调整 r 到当前时间段的最后一个索引

        // 合并所有同一时间的会议
        for (int i = l; i <= r; ++i) {
            unionSet(meetings[i][0], meetings[i][1]); // 合并专家
        }

        // 处理未获得秘密的专家重置
        for (int i = l; i <= r; ++i) {
            int a = meetings[i][0], b = meetings[i][1];
            if (!secret[find(a)]) father[a] = a; // 重置未获得秘密的专家
            if (!secret[find(b)]) father[b] = b; // 重置未获得秘密的专家
        }

        l = r + 1; // 处理下一时间段
    }

    // 收集所有知晓秘密的专家
    vector<int> ans;
    for (int i = 0; i < n; ++i) {
        if (secret[find(i)]) {
            ans.push_back(i); // 添加知晓秘密的专家
        }
    }
    return ans;
}
// 找出知晓秘密的所有专家
// 给你一个整数 n ，表示有 n 个专家从 0 到 n - 1 编号
// 另外给你一个下标从 0 开始的二维整数数组 meetings
// 其中 meetings[i] = [xi, yi, timei] 表示专家 xi 和专家 yi 在时间 timei 要开一场会
// 一个专家可以同时参加 多场会议 。最后，给你一个整数 firstPerson
// 专家 0 有一个 秘密 ，最初，他在时间 0 将这个秘密分享给了专家 firstPerson
// 接着，这个秘密会在每次有知晓这个秘密的专家参加会议时进行传播
// 更正式的表达是，每次会议，如果专家 xi 在时间 timei 时知晓这个秘密
// 那么他将会与专家 yi 分享这个秘密，反之亦然。秘密共享是 瞬时发生 的
// 也就是说，在同一时间，一个专家不光可以接收到秘密，还能在其他会议上与其他专家分享
// 在所有会议都结束之后，返回所有知晓这个秘密的专家列表
// 你可以按 任何顺序 返回答案
// 链接测试 : https://leetcode.cn/problems/find-all-people-with-secret/

int main() {

    vector<vector<int>> meetings = {{3,1,3}, {1,2,2}, {0,3,3}};
    vector<int> result = findAllPeople(4, meetings, 3);
    // 预期输出: 0,1,3
    for (int num : result) cout << num << " ";
    return 0;
}
