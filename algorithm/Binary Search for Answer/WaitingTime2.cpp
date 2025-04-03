#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;

class WaitingTime2 {
public:
    // 计算完成w趟旅行所需的最少时间
    static long long minimumTime(vector<int>& arr, int w) {
        // 找到数组中的最小值
        int minVal = INT_MAX;
        for (int x : arr) {
            minVal = min(minVal, x);
        }

        // 初始化二分查找的左右边界
        long long ans = 0;
        for (long long l = 0, r = static_cast<long long>(minVal) * w, m; l <= r;) {
            // 计算中间值
            m = l + ((r - l) >> 1);

            // 检查在时间m内是否可以完成至少w趟旅行
            if (f(arr, m) >= w) {
                // 如果可以完成，尝试更短的时间
                ans = m;
                r = m - 1;
            } else {
                // 如果不能完成，尝试更长的时间
                l = m + 1;
            }
        }
        return ans;
    }

private:
    // 计算在给定时间内可以完成的旅行次数
    static long long f(vector<int>& arr, long long time) {
        long long ans = 0;
        for (int num : arr) {
            // 累加每个交通工具在给定时间内可以完成的旅行次数
            ans += (time / num);
        }
        return ans;
    }
};


int main() {
    // 示例输入
    vector<int> arr = {1, 2, 3}; // 每个交通工具完成一趟旅行所需的时间
    int w = 5; // 需要完成的旅行次数

    // 计算完成w趟旅行所需的最少时间
    long long result = WaitingTime2::minimumTime(arr, w);

    // 输出结果
    cout << " " << w << "  " << result << " " << endl;

    return 0;
}
