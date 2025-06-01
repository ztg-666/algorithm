#include <iostream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// 两个0和1数量相等区间的最大长度
// 给出一个长度为n的01串，现在请你找到两个区间
// 使得这两个区间中，1的个数相等，0的个数也相等
// 这两个区间可以相交，但是不可以完全重叠，即两个区间的左右端点不可以完全一样

using namespace std;

// 暴力验证方法
// 时间复杂度：O(n²)，用于验证正确性
int len1(const vector<int>& arr) {
    // 使用嵌套哈希表记录每种(0的数量, 1的数量)组合出现的次数
    unordered_map<int, unordered_map<int, int>> map;
    for (int i = 0; i < arr.size(); i++) {
        int zero = 0, one = 0;
        // 遍历所有以i开头的子数组
        for (int j = i; j < arr.size(); j++) {
            arr[j] == 0 ? zero++ : one++;
            // 更新哈希表计数
            if (!map.count(zero)) {
                map[zero] = unordered_map<int, int>();
            }
            map[zero][one]++;
        }
    }
    int ans = 0;
    // 遍历所有可能的0和1数量组合
    for (auto& [zeros, inner] : map) {
        for (auto& [ones, cnt] : inner) {
            // 当同一组合出现次数大于1次时，可以形成两个不同区间
            if (cnt > 1) {
                ans = max(ans, zeros + ones); // 区间长度为0和1数量之和
            }
        }
    }
    return ans;
}

// 优化解法（正式方法）
// 时间复杂度：O(n)，空间复杂度：O(1)
int len2(const vector<int>& arr) {
    // 初始化四个关键位置索引
    int leftZero = -1;  // 第一个0的位置
    int rightZero = -1; // 最后一个0的位置
    int leftOne = -1;   // 第一个1的位置
    int rightOne = -1;  // 最后一个1的位置

    // 寻找第一个0的位置
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == 0) {
            leftZero = i;
            break;
        }
    }
    // 寻找第一个1的位置
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == 1) {
            leftOne = i;
            break;
        }
    }
    // 寻找最后一个0的位置
    for (int i = arr.size()-1; i >= 0; i--) {
        if (arr[i] == 0) {
            rightZero = i;
            break;
        }
    }
    // 寻找最后一个1的位置
    for (int i = arr.size()-1; i >= 0; i--) {
        if (arr[i] == 1) {
            rightOne = i;
            break;
        }
    }
    // 计算两种可能的最大区间长度
    int p1 = (rightZero != -1 && leftZero != -1) ? rightZero - leftZero : 0;
    int p2 = (rightOne != -1 && leftOne != -1) ? rightOne - leftOne : 0;
    return max(p1, p2);
}

// 生成随机01数组
// 参数：len - 数组长度
vector<int> randomArray(int len) {
    vector<int> ans(len);
    for (int i = 0; i < len; i++) {
        ans[i] = rand() % 2; // 生成0或1
    }
    return ans;
}

// 主函数：测试两种方法的正确性
int main() {
    srand(time(nullptr)); // 初始化随机种子
    int N = 500;          // 最大数组长度基数
    int testTimes = 2000; // 测试次数

    cout << "" << endl;
    for (int i = 1; i <= testTimes; i++) {
        int n = rand() % N + 2; // 生成2~501的随机长度
        auto arr = randomArray(n);
        int ans1 = len1(arr);   // 暴力方法结果
        int ans2 = len2(arr);   // 优化方法结果

        // 结果比对
        if (ans1 != ans2) {
            cout << "" << endl;
        }
        // 进度提示
        if (i % 100 == 0) {
            cout << "" << i << "" << endl;
        }
    }
    cout << "" << endl;
    return 0;
}
