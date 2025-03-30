#include <iostream>
#include <vector>
#include <algorithm>
using ll = long long;
using namespace std;

// 构建前缀和数组
void build(vector<vector<int>>& m) {
    for (int i = 1; i < m.size(); i++) {
        for (int j = 1; j < m[0].size(); j++) {
            // 当前位置的值等于当前值加上上方和左侧的值，减去重复计算的部分
            m[i][j] += m[i - 1][j] + m[i][j - 1] - m[i - 1][j - 1];
        }
    }
}

// 在指定范围内增加值
void add(vector<vector<int>>& diff, int a, int b, int c, int d) {
    // 差分矩阵中 (a, b) 位置增加 1
    diff[a][b] += 1;
    // 差分矩阵中 (c + 1, d + 1) 位置增加 1
    diff[c + 1][d + 1] += 1;
    // 差分矩阵中 (c + 1, b) 位置减少 1
    diff[c + 1][b] -= 1;
    // 差分矩阵中 (a, d + 1) 位置减少 1
    diff[a][d + 1] -= 1;
}

// 排序数组并去除重复值，返回有效长度
int sort1(vector<ll>& nums) {
    if (nums.empty()) return 0;
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    return nums.size();
}

// 二分查找，返回 v 在有序数组 nums 中的编号（从 1 开始）
int rank1(vector<ll>& nums, ll v, int size) {
    int l = 0;
    int r = size - 1;
    int m, ans = 0;
    while (l <= r) {
        m = (l + r) / 2;
        if (nums[m] >= v) {
            ans = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return ans + 1;
}

// 时间复杂度 O(n^2)，额外空间复杂度 O(n^2)，n 是力场的个数
int fieldOfGreatestBlessing(vector<vector<int>>& fields) {
    int n = fields.size();
    // n : 矩形的个数，x 2*n 个坐标
    vector<ll> xs(n << 1);
    vector<ll> ys(n << 1);
    for (int i = 0, k = 0, p = 0; i < n; i++) {
        ll x = fields[i][0];
        ll y = fields[i][1];
        ll r = fields[i][2];
        xs[k++] = (x << 1) - r;  // 左边界
        xs[k++] = (x << 1) + r;  // 右边界
        ys[p++] = (y << 1) - r;  // 下边界
        ys[p++] = (y << 1) + r;  // 上边界
    }
    // xs 数组中，排序了且相同值只留一份，返回有效长度
    int sizex = sort1(xs);
    // ys 数组中，排序了且相同值只留一份，返回有效长度
    int sizey = sort1(ys);
    // n 个力场，sizex : 2 * n, sizey : 2 * n
    vector<vector<int>> diff(sizex + 2, vector<int>(sizey + 2, 0));
    for (int i = 0, a, b, c, d; i < n; i++) {
        ll x = fields[i][0];
        ll y = fields[i][1];
        ll r = fields[i][2];
        a = rank1(xs, (x << 1) - r, sizex);  // 左边界编号
        b = rank1(ys, (y << 1) - r, sizey);  // 下边界编号
        c = rank1(xs, (x << 1) + r, sizex);  // 右边界编号
        d = rank1(ys, (y << 1) + r, sizey);  // 上边界编号
        add(diff, a, b, c, d);  // 在差分矩阵中记录力场
    }
    int ans = 0;
    build(diff);
    for (int i = 1; i < diff.size(); i++) {
        for (int j = 1; j < diff[0].size(); j++) {
            ans = max(ans, diff[i][j]);  // 更新最大力场强度
        }
    }
    return ans;
}

int main() {
    // 示例输入
    vector<vector<int>> fields = {
        {1, 1, 1},
        {2, 2, 2},
        {3, 3, 3}
    };

    // 调用 fieldOfGreatestBlessing 函数并输出结果
    int result = fieldOfGreatestBlessing(fields);
    cout << "The greatest blessing field strength is: " << result << endl;

    return 0;
}
