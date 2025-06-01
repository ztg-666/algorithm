#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
using namespace std;

// 暴力验证方法（仅用于小数据验证）
// 递归尝试所有分组可能性，返回最小累加和
int f(const vector<int>& arr, int i, vector<int>& sum, vector<int>& cnt) {
    if (i == arr.size()) { // 所有元素分配完毕
        int ans = 0;
        for (int j = 0; j < sum.size(); ++j) {
            if (cnt[j] == 0) return INT_MAX; // 无效分组（有空集合）
            ans += sum[j] / cnt[j];         // 累加各集合平均值（向下取整）
        }
        return ans;
    } else {
        int min_val = INT_MAX;
        // 尝试将当前元素放入每个可能的集合
        for (int j = 0; j < sum.size(); ++j) {
            sum[j] += arr[i];  // 加入第j个集合
            cnt[j]++;          // 该集合元素数+1
            min_val = min(min_val, f(arr, i + 1, sum, cnt)); // 递归处理下一个元素
            sum[j] -= arr[i];  // 回溯
            cnt[j]--;
        }
        return min_val;
    }
}

// 暴力方法入口（时间复杂度O(k^n)，仅用于验证）
int minAverageSum1(const vector<int>& arr, int k) {
    vector<int> sum(k, 0); // 存储每个集合的和
    vector<int> cnt(k, 0); // 存储每个集合的元素数量
    return f(arr, 0, sum, cnt);
}

// 优化算法（时间复杂度O(n logn)）
// 策略：排序后，前k-1个最小元素各自成组，剩余元素合并为一组
int minAverageSum2(vector<int>& arr, int k) {
    sort(arr.begin(), arr.end()); // 升序排序

    int ans = 0;
    // 前k-1个元素各自单独成组（最小化每个组的平均值）
    for (int i = 0; i < k - 1; ++i) {
        ans += arr[i]; // 单独元素的平均值就是自身
    }

    // 剩余元素合并为一组（大数尽可能合并以减少平均值总和）
    int total = 0;
    for (int i = k - 1; i < arr.size(); ++i) {
        total += arr[i];
    }
    ans += total / (arr.size() - k + 1); // 计算平均值并累加

    return ans;
}

// 生成随机测试数组（元素范围[0, v]）
vector<int> randomArray(int n, int v) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % (v + 1); // 随机生成元素值
    }
    return arr;
}

int main() {
    srand(time(nullptr)); // 初始化随机数种子

    const int N = 8;      // 测试数组最大长度
    const int V = 10000;  // 数组元素最大值
    const int TEST_TIMES = 2000; // 测试次数

    cout << "" << endl;
    for (int i = 1; i <= TEST_TIMES; ++i) {
        int n = rand() % N + 1; // 随机数组长度[1, N]
        int k = rand() % n + 1; // 随机k值[1, n]
        vector<int> arr = randomArray(n, V);

        // 拷贝数组用于两种方法（防止原地修改影响）
        vector<int> arr1 = arr;
        vector<int> arr2 = arr;

        // 计算两种方法结果
        int ans1 = minAverageSum1(arr1, k);
        int ans2 = minAverageSum2(arr2, k);

        // 验证结果一致性
        if (ans1 != ans2) {
            cout << "" << endl;
            cout << "" << ans1 << endl;
            cout << "" << ans2 << endl;
            return -1; // 提前终止测试
        }

        // 进度提示
        if (i % 100 == 0) {
            cout << " " << i << " " << endl;
        }
    }
    cout << "" << endl;
    return 0;
}
