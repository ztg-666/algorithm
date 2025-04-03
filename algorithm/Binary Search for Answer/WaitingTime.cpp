#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstdlib>
#include <ctime>
// 计算等位时间
// 给定一个数组arr长度为n，表示n个服务员，每服务一个人的时间
// 给定一个正数m，表示有m个人等位，如果你是刚来的人，请问你需要等多久？
// 假设m远远大于n，比如n <= 10^3, m <= 10^9，该怎么做是最优解？
// 谷歌的面试，这个题连考了2个月
// 找不到测试链接，所以用对数器验证
class WaitingTime {
public:
    // 堆模拟
    // 验证方法，不是重点
    // 如果m很大，该方法会超时
    // 时间复杂度O(m * log(n))，额外空间复杂度O(n)
    static int waitingTime1(const std::vector<int>& arr, int m) {
        // 使用优先队列（最小堆）
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> heap;
        int n = arr.size();
        // 初始化每个服务员的醒来时间为 0，服务时间为 arr[i]
        for (int i = 0; i < n; i++) {
            heap.push({0, arr[i]});
        }
        // 模拟 m 个人依次等待服务
        for (int i = 0; i < m; i++) {
            auto cur = heap.top(); // 取出最早醒来的服务员
            heap.pop();
            cur.first += cur.second; // 更新服务员的醒来时间为当前时间加上服务时间
            heap.push(cur); // 将更新后的时间重新放入堆中
        }
        return heap.top().first; // 返回最后一个服务结束的时间
    }

    // 二分答案法
    // 最优解
    // 时间复杂度O(n * log(min * w))，额外空间复杂度O(1)
    static int waitingTime2(const std::vector<int>& arr, int w) {
        int minVal = INT_MAX;
        // 找到服务员服务时间的最小值
        for (int x : arr) {
            minVal = std::min(minVal, x);
        }
        int ans = 0;
        // 二分查找范围 [0, minVal * w]
        for (int l = 0, r = minVal * w, m; l <= r;) {
            // 计算中间值 m
            m = l + ((r - l) >> 1);
            // 检查在 m 时间内是否可以接待 w + 1 个客人
            if (f(arr, m) >= w + 1) {
                ans = m; // 如果可以，尝试更小的时间
                r = m - 1;
            } else {
                l = m + 1; // 如果不可以，尝试更大的时间
            }
        }
        return ans; // 返回可以让 w 个客人等待的最短时间
    }

    // 如果每个服务员工作 time，可以接待几位客人（结束的、开始的客人都算）
    static int f(const std::vector<int>& arr, int time) {
        int ans = 0;
        // 计算在 time 时间内每个服务员可以接待的客人数量
        for (int num : arr) {
            ans += (time / num) + 1; // 每个服务员可以接待 (time / num) 个客人，并且可以开始一个新的客人
        }
        return ans; // 返回总接待的客人数量
    }
};

// 对数器测试
static std::vector<int> randomArray(int n, int v) {
    std::vector<int> arr(n);
    // 生成长度为 n 的随机数组，每个元素在 [1, v] 之间
    for (int i = 0; i < n; i++) {
        arr[i] = (std::rand() % v) + 1;
    }
    return arr;
}

// 对数器测试
int main() {
    std::cout << "start" << std::endl;
    int N = 50; // 最大服务员数量
    int V = 30; // 最大服务时间
    int M = 3000; // 最大等待人数
    int testTime = 20000; // 测试次数
    std::srand(std::time(0)); // 设置随机种子

    // 进行 testTime 次测试
    for (int i = 0; i < testTime; i++) {
        int n = (std::rand() % N) + 1; // 随机生成服务员数量
        std::vector<int> arr = randomArray(n, V); // 生成随机的服务时间数组
        int m = (std::rand() % M); // 随机生成等待人数
        int ans1 = WaitingTime::waitingTime1(arr, m); // 使用堆模拟方法计算等待时间
        int ans2 = WaitingTime::waitingTime2(arr, m); // 使用二分答案法计算等待时间
        if (ans1 != ans2) {
            std::cout << "false" << std::endl; // 如果结果不一致，输出错误信息
        }
    }
    std::cout << "true" << std::endl; // 如果所有测试通过，输出成功信息
    return 0;
}
