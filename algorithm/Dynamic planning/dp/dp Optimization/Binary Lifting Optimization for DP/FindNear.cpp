#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_set>
#include <cmath>
#include <cstdlib>
#include <ctime>

// 寻找最近和次近
// 给定一个长度为n的数组arr，下标1 ~ n范围，数组无重复值
// 关于近的定义，距离的定义如下:
// 对i位置的数字x来说，只关注右侧的数字，和x的差值绝对值越小就越近
// 距离为差值绝对值，如果距离一样，数值越小的越近
// 数值 : 3 5 7 1
// 下标 : 1 2 3 4
// 对1位置的数字3来说，第一近是4位置的1，距离为2；第二近是2位置的5，距离为2
// 每个位置的数字都求第一近的位置及其距离、第二近的位置及其距离
// 分别用to1、dist1、to2、dist2数组表示，0表示不存在
// 有序表的实现 + 数组手搓双向链表的实现

const int MAXN = 10001;

// 全局变量定义
int arr[MAXN];      // 输入数组
int n;              // 数组大小

// 存储每个位置的最近和次近元素信息
int to1[MAXN];      // 第一近元素的位置
int dist1[MAXN];    // 到第一近元素的距离
int to2[MAXN];      // 第二近元素的位置
int dist2[MAXN];    // 到第二近元素的距离

// 以下三个数组只有near2方法需要
int rank_arr[MAXN][2];  // 排序后的数组：[位置][0]=原始索引，[1]=值
int last_arr[MAXN];     // 双向链表：每个位置的前驱
int next_arr[MAXN];     // 双向链表：每个位置的后继

// 用于验证的备份数组
int a[MAXN];    // 备份to1
int b[MAXN];    // 备份dist1
int c[MAXN];    // 备份to2
int d[MAXN];    // 备份dist2

/**
 * 更新位置i的最近和次近元素
 * @param i 当前位置
 * @param j 候选位置（i的右侧）
 *
 * 规则：
 * 1. 距离越小越好
 * 2. 距离相同时，值越小越好
 */
void update(int i, int j) {
    if (j == 0) {
        return;  // j为0表示无效位置
    }

    int dist = abs(arr[i] - arr[j]);  // 计算距离（值的差的绝对值）

    // 检查是否可以更新第一近元素
    if (to1[i] == 0 || dist < dist1[i] || (dist == dist1[i] && arr[j] < arr[to1[i]])) {
        // 当前第一近变成第二近
        to2[i] = to1[i];
        dist2[i] = dist1[i];
        // 更新第一近
        to1[i] = j;
        dist1[i] = dist;
    }
    // 检查是否可以更新第二近元素
    else if (to2[i] == 0 || dist < dist2[i] || (dist == dist2[i] && arr[j] < arr[to2[i]])) {
        to2[i] = j;
        dist2[i] = dist;
    }
}

/**
 * 从双向链表中删除节点i
 * 用于near2算法中维护有序结构
 */
void deleteNode(int i) {
    int l = last_arr[i];  // 前驱
    int r = next_arr[i];  // 后继

    // 更新前驱的后继指针
    if (l != 0) {
        next_arr[l] = r;
    }
    // 更新后继的前驱指针
    if (r != 0) {
        last_arr[r] = l;
    }
}

/**
 * 方法1：使用有序集合（set）的实现
 * 时间复杂度：O(n log n)
 * 空间复杂度：O(n)
 *
 * 思路：
 * 1. 从右到左遍历数组
 * 2. 对于每个位置，在有序集合中查找最接近的元素
 * 3. 查找小于等于当前值的最大元素和次大元素
 * 4. 查找大于等于当前值的最小元素和次小元素
 * 5. 将当前元素加入有序集合
 */
void near1() {
    std::set<std::pair<int, int>> s; // {value, index} 有序集合

    // 从右到左遍历
    for (int i = n; i >= 1; i--) {
        // 初始化当前位置的结果
        to1[i] = 0;
        dist1[i] = 0;
        to2[i] = 0;
        dist2[i] = 0;

        std::pair<int, int> cur = {arr[i], i};

        // 查找小于等于当前值的元素
        auto it1 = s.upper_bound(cur);  // 找到第一个大于cur的元素
        if (it1 != s.begin()) {
            --it1;  // 回退到小于等于cur的最大元素
            update(i, it1->second);

            // 查找次小的元素
            if (it1 != s.begin()) {
                --it1;
                update(i, it1->second);
            }
        }

        // 查找大于等于当前值的元素
        auto it2 = s.lower_bound(cur);  // 找到第一个大于等于cur的元素
        if (it2 != s.end()) {
            update(i, it2->second);

            // 查找次大的元素
            ++it2;
            if (it2 != s.end()) {
                update(i, it2->second);
            }
        }

        s.insert(cur);  // 将当前元素插入有序集合
    }
}

/**
 * 方法2：使用数组模拟双向链表的实现
 * 时间复杂度：O(n log n)（主要是排序）
 * 空间复杂度：O(n)
 *
 * 思路：
 * 1. 将数组按值排序，构建双向链表
 * 2. 按原始顺序遍历每个位置
 * 3. 对于每个位置，检查其在排序后的前驱和后继
 * 4. 处理完后从链表中删除该节点
 */
void near2() {
    // 准备排序数组
    for (int i = 1; i <= n; i++) {
        rank_arr[i][0] = i;         // 原始索引
        rank_arr[i][1] = arr[i];    // 值
    }

    // 使用vector进行排序（更简单的方式）
    std::vector<std::pair<int, int>> temp;
    for (int i = 1; i <= n; i++) {
        temp.emplace_back(arr[i], i);  // {值, 索引}
    }

    std::sort(temp.begin(), temp.end());  // 按值排序

    // 将排序结果写回rank_arr
    for (int i = 0; i < n; i++) {
        rank_arr[i + 1][0] = temp[i].second;  // 原始索引
        rank_arr[i + 1][1] = temp[i].first;   // 值
    }

    // 设置边界
    rank_arr[0][0] = 0;      // 左边界
    rank_arr[n + 1][0] = 0;  // 右边界

    // 构建双向链表
    for (int i = 1; i <= n; i++) {
        int idx = rank_arr[i][0];           // 原始索引
        last_arr[idx] = rank_arr[i - 1][0]; // 前驱
        next_arr[idx] = rank_arr[i + 1][0]; // 后继
    }

    // 按原始顺序处理每个位置
    for (int i = 1; i <= n; i++) {
        // 初始化
        to1[i] = 0;
        dist1[i] = 0;
        to2[i] = 0;
        dist2[i] = 0;

        // 检查前驱和前驱的前驱
        update(i, last_arr[i]);
        update(i, last_arr[last_arr[i]]);

        // 检查后继和后继的后继
        update(i, next_arr[i]);
        update(i, next_arr[next_arr[i]]);

        // 处理完后从链表中删除当前节点
        deleteNode(i);
    }
}

/**
 * 生成随机数组，确保没有重复值
 * @param v 值的范围：[-v, v]
 */
void random(int v) {
    std::unordered_set<int> s;
    for (int i = 1; i <= n; i++) {
        int cur;
        do {
            cur = rand() % (v * 2) - v;  // 生成[-v, v]范围内的随机数
        } while (s.count(cur));          // 确保不重复
        s.insert(cur);
        arr[i] = cur;
    }
}

/**
 * 验证两种算法的结果是否一致
 * @return true表示结果一致，false表示不一致
 */
bool check() {
    // 运行第一种算法
    near1();

    // 备份第一种算法的结果
    for (int i = 1; i <= n; i++) {
        a[i] = to1[i];
        b[i] = dist1[i];
        c[i] = to2[i];
        d[i] = dist2[i];
    }

    // 运行第二种算法
    near2();

    // 比较第一近元素的结果
    for (int i = 1; i <= n; i++) {
        if (a[i] != to1[i] || b[i] != dist1[i]) {
            return false;
        }
    }

    // 比较第二近元素的结果
    for (int i = 1; i <= n; i++) {
        if (c[i] != to2[i] || d[i] != dist2[i]) {
            return false;
        }
    }
    return true;
}

/**
 * 输出结果（调试用）
 */
void printResults() {
    std::cout << "Position -> (First Near: pos, dist) (Second Near: pos, dist)" << std::endl;
    for (int i = 1; i <= n; i++) {
        std::cout << i << " -> (" << to1[i] << ", " << dist1[i] << ") "
                  << "(" << to2[i] << ", " << dist2[i] << ")" << std::endl;
    }
}

/**
 * 主函数：测试两种算法的正确性
 */
int main() {
    srand(time(nullptr));  // 初始化随机数种子

    // 测试参数
    n = 100;              // 数组大小
    int v = 500;          // 值的范围（要大于n以确保无重复）
    int testTime = 10000; // 测试次数

    std::cout << "..." << std::endl;

    // 进行多次随机测试
    for (int i = 1; i <= testTime; i++) {
        random(v);        // 生成随机数组
        if (!check()) {   // 验证两种算法结果是否一致
            std::cout << "error" << std::endl;
            return 1;
        }
    }

    std::cout << "end" << std::endl;
    return 0;
}
