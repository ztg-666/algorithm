#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int MAXN = 101;    // 最大轮盘长度限制
const int MAXC = 26;     // 字母表大小（小写字母）

class Solution {
private:
    vector<int> ring;    // 存储轮盘字符对应的数字索引（a->0, b->1...）
    vector<int> key;     // 存储钥匙字符对应的数字索引
    vector<int> size;    // size[c]记录字符c在轮盘中的出现次数
    vector<vector<int>> where;  // where[c]存储字符c在轮盘中的所有位置索引
    vector<vector<int>> dp;     // 记忆化数组：dp[i][j]表示从轮盘i位置开始拼写key[j..]的最小步数
    int n, m;            // n:轮盘长度，m:钥匙长度

    // 预处理函数：构建数据结构
    void build(const string& r, const string& k) {
        size.assign(MAXC, 0);  // 初始化各字符出现次数为0
        where.resize(MAXC, vector<int>(MAXN)); // 预分配每个字符的位置存储空间

        n = r.size();
        m = k.size();
        ring.resize(n);
        key.resize(m);

        // 预处理轮盘字符位置信息
        for (int i = 0; i < n; ++i) {
            int v = r[i] - 'a';        // 字符转数字索引
            where[v][size[v]++] = i;   // 记录字符v的位置，并增加出现计数
            ring[i] = v;               // 存储轮盘字符索引
        }

        // 预处理钥匙字符
        for (int i = 0; i < m; ++i) {
            key[i] = k[i] - 'a';  // 字符转数字索引
        }

        // 初始化记忆化数组为-1（表示未计算状态）
        dp.assign(MAXN, vector<int>(MAXN, -1));
    }

    // 记忆化搜索函数：i-当前轮盘位置，j-当前处理的钥匙字符索引
    int f(int i, int j) {
        if (j == m) return 0;  // 所有字符处理完成
        if (dp[i][j] != -1) return dp[i][j]; // 已计算过直接返回

        int ans;
        if (ring[i] == key[j]) { // 当前位置字符匹配
            // 直接按下按钮（+1），处理下一个字符
            ans = 1 + f(i, j + 1);
        } else { // 需要旋转找到目标字符
            // 顺时针查找最近目标字符
            int jump1 = clock(i, key[j]);
            int distance1 = (jump1 > i) ? (jump1 - i) : (n - i + jump1);

            // 逆时针查找最近目标字符
            int jump2 = counterClock(i, key[j]);
            int distance2 = (i > jump2) ? (i - jump2) : (i + n - jump2);

            // 取两种路径的最小值（旋转步数+按钮步数）
            ans = min(distance1 + 1 + f(jump1, j + 1),
                    distance2 + 1 + f(jump2, j + 1));
        }
        return dp[i][j] = ans; // 记忆结果并返回
    }

    // 顺时针查找：在where[v]的有序位置中找第一个大于i的位置
    int clock(int i, int v) {
        const vector<int>& sorted = where[v];
        int left = 0, right = size[v] - 1, find = -1;
        // 二分查找大于i的最小位置
        while (left <= right) {
            int mid = (left + right) / 2;
            if (sorted[mid] > i) {
                find = mid;     // 记录候选位置
                right = mid - 1;// 继续向左找更小的
            } else {
                left = mid + 1; // 向右查找
            }
        }
        // 找不到则循环到轮盘开头（最小的位置）
        return (find != -1) ? sorted[find] : sorted[0];
    }

    // 逆时针查找：在where[v]的有序位置中找最后一个小于i的位置
    int counterClock(int i, int v) {
        const vector<int>& sorted = where[v];
        int left = 0, right = size[v] - 1, find = -1;
        // 二分查找小于i的最大位置
        while (left <= right) {
            int mid = (left + right) / 2;
            if (sorted[mid] < i) {
                find = mid;     // 记录候选位置
                left = mid + 1; // 继续向右找更大的
            } else {
                right = mid - 1;// 向左查找
            }
        }
        // 找不到则循环到轮盘末尾（最大的位置）
        return (find != -1) ? sorted[find] : sorted[size[v] - 1];
    }

public:
    // 主入口函数
    int findRotateSteps(string ringStr, string keyStr) {
        build(ringStr, keyStr);     // 数据预处理
        return f(0, 0);             // 从轮盘位置0开始处理钥匙位置0
    }
};
