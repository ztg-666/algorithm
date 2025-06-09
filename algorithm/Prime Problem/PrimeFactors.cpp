#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// 按公因数计算最大组件大小
// 给定一个由不同正整数的组成的非空数组 nums
// 如果 nums[i] 和 nums[j] 有一个大于1的公因子，那么这两个数之间有一条无向边
// 返回 nums中最大连通组件的大小。
// 测试链接 : https://leetcode.cn/problems/largest-component-size-by-common-factor/

using namespace std;

const int MAXV = 100001;       // 质因子可能的最大值
int factors[MAXV];             // 记录质因子最早出现的下标，用于合并相同质因子的元素

const int MAXN = 20001;        // 最大数组元素数量
int father[MAXN];              // 并查集父节点数组
int size_[MAXN];               // 集合大小（用size_避免与std::size冲突）
int n;                         // 当前数组长度

/**
 * 初始化并查集和质因子记录数组
 * 每个元素初始父节点是自己，集合大小为1
 * 质因子数组初始化为-1表示未被记录
 */
void build() {
    for (int i = 0; i < n; ++i) {
        father[i] = i;
        size_[i] = 1;
    }
    memset(factors, -1, sizeof(factors)); // 初始化质因子记录为-1
}

/**
 * 路径压缩查找根节点
 * @param i 要查找的节点
 * @return 根节点下标
 */
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]); // 路径压缩
    }
    return father[i];
}

/**
 * 合并两个集合（按大小合并）
 * @param x 第一个元素下标
 * @param y 第二个元素下标
 */
void union_set(int x, int y) {
    int fx = find(x);
    int fy = find(y);
    if (fx != fy) {
        father[fx] = fy;          // 将fx的父节点指向fy
        size_[fy] += size_[fx];   // 合并集合大小
    }
}

/**
 * 查找当前最大集合的大小
 * @return 最大集合的元素数量
 */
int maxSize() {
    int ans = 0;
    for (int i = 0; i < n; ++i) {
        ans = max(ans, size_[i]);
    }
    return ans;
}

/**
 * 计算数组中通过质因子连接的最大组件大小
 * @param arr 输入数组
 * @return 最大连通组件大小
 */
int largestComponentSize(vector<int>& arr) {
    n = arr.size();
    build(); // 初始化数据结构

    // 遍历每个数字，分解质因子并建立连接
    for (int i = 0; i < n; ++i) {
        int x = arr[i];

        // 分解质因子
        for (int j = 2; j * j <= x; ++j) {
            if (x % j == 0) {
                // 如果当前质因子尚未记录，记录首次出现的下标
                if (factors[j] == -1) {
                    factors[j] = i;
                } else {
                    // 否则将当前元素与首个具有该质因子的元素合并
                    union_set(factors[j], i);
                }
                // 去除所有该质因子
                while (x % j == 0) {
                    x /= j;
                }
            }
        }

        // 处理剩余的大于1的质因子
        if (x > 1) {
            if (factors[x] == -1) {
                factors[x] = i;
            } else {
                union_set(factors[x], i);
            }
        }
    }

    return maxSize(); // 返回最大集合大小
}

/**
 * 分解质因子测试函数
 * @param n 要分解的数字
 */
void f(int n) {
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            cout << i << endl;
            while (n % i == 0) {
                n /= i;
            }
        }
    }
    if (n > 1) {
        cout << n << endl;
    }
}

int main() {
    // 测试质因子分解
    int test_num = 4012100;
    cout << "分解质因子测试：" << test_num << endl;
    f(test_num); // 应输出2,5,7,17,17

    // 测试最大连通组件
    vector<int> nums = {4,6,15,35};
    cout << "最大组件大小测试：" << endl;
    cout << largestComponentSize(nums) << endl; // 应输出3（元素4,6,15通过质因子2,3,5连接）

    return 0;
}
