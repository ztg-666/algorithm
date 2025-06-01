#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstdlib>
#include <algorithm>

// 加入差值绝对值直到长度固定
// 给定一个非负数组arr，计算任何两个数差值的绝对值
// 如果arr中没有，都要加入到arr里，但是只加一份
// 然后新的arr继续计算任何两个数差值的绝对值，
// 如果arr中没有，都要加入到arr里，但是只加一份
// 一直到arr大小固定，返回arr最终的长度

using namespace std;

// 暴力方法验证（模拟绝对差值生成过程）
// 时间复杂度：O(n^3) 最坏情况，仅用于验证正确性
int len1(vector<int>& arr) {
    vector<int> list;   // 动态数组存储当前所有数字
    unordered_set<int> set; // 哈希集合用于快速存在性判断
    // 初始化原始数组
    for (int num : arr) {
        list.push_back(num);
        set.insert(num);
    }
    bool changed; // 标记是否发生变动
    do {
        changed = false;
        size_t len = list.size(); // 当前数组长度
        // 双重循环计算所有两数差值
        for (size_t i = 0; i < len; ++i) {
            for (size_t j = i + 1; j < len; ++j) {
                int abs_val = abs(list[i] - list[j]);
                // 如果新差值不存在则加入集合
                if (set.find(abs_val) == set.end()) {
                    list.push_back(abs_val);
                    set.insert(abs_val);
                    changed = true; // 标记变动
                }
            }
        }
    } while (changed); // 循环直到不再产生新数字
    return list.size();
}

// 计算最大公约数（欧几里得算法）
int gcd(int m, int n) {
    return n == 0 ? m : gcd(n, m % n);
}

// 优化方法（数学规律推导）
// 时间复杂度：O(n)
int len2(vector<int>& arr) {
    int max_val = 0;    // 记录数组最大值
    int gcd_val = 0;    // 计算非零元素的GCD

    // 第一遍遍历：计算最大值和初始非零GCD
    for (int num : arr) {
        max_val = max(max_val, num);
        if (num != 0) {
            gcd_val = num; // 找到第一个非零值作为GCD初始值
        }
    }

    // 处理全零数组的特殊情况
    if (gcd_val == 0) {
        return arr.size(); // 全零数组无法生成新差值
    }

    unordered_map<int, int> cnts; // 统计每个数字出现次数
    // 第二遍遍历：计算实际GCD并统计频次
    for (int num : arr) {
        if (num != 0) {
            gcd_val = gcd(gcd_val, num); // 更新GCD
        }
        cnts[num]++; // 统计频率
    }

    // 计算最终结果的三部分：
    int ans = max_val / gcd_val; // 1. 数值范围覆盖的间隔数
    int max_cnt = 0; // 记录非零元素的最大频次

    // 遍历统计结果
    for (auto& [num, cnt] : cnts) {
        if (num != 0) {
            ans += cnt - 1; // 2. 非零元素的重复次数贡献
        }
        max_cnt = max(max_cnt, cnt); // 跟踪最大频次
    }
    // 3. 处理零元素的特殊贡献（当存在多个零时额外+1）
    ans += cnts.count(0) ? (max_cnt > 1 ? 1 : 0) : 0;

    return ans;
}

// 生成随机测试数组
// 参数n：数组长度，v：元素最大值
vector<int> randomArray(int n, int v) {
    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = rand() % (v + 1); // 生成[0,v]范围的随机数
    }
    return ans;
}

int main() {
    srand(time(0)); // 初始化随机种子
    int N = 50;     // 最大数组长度
    int V = 100;    // 元素最大值
    int testTimes = 20000; // 测试次数

    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; ++i) {
        int n = rand() % N + 1; // 随机数组长度[1,50]
        vector<int> nums = randomArray(n, V);
        // 对比暴力解法和优化解法的结果
        int ans1 = len1(nums);
        int ans2 = len2(nums);
        if (ans1 != ans2) { // 结果不一致时报错
            cout << "出错了！" << endl;
        }
    }
    cout << "测试结束" << endl;
    return 0;
}
