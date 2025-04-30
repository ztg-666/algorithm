#include <iostream>
#include <vector>
#include <numeric>   // 可能有用，但对于直接翻译不是严格必需的
#include <algorithm> // 用于 std::max
#include <deque>     // 用于 maxSum2 中的单调队列
#include <climits>   // 用于 INT_MIN
#include <cstdlib>   // 用于 rand() 和 srand()
#include <ctime>     // 用于 time()
// 删掉1个数字后长度为k的子数组最大累加和
// 给定一个数组nums，求必须删除一个数字后的新数组中
// 长度为k的子数组最大累加和，删除哪个数字随意
// 对数器验证
// 辅助函数 (等同于 Java 的 delete)
// 删除指定索引处的元素并返回一个新的 vector。
std::vector<int> deleteElement(const std::vector<int>& nums, int index) {
    int n = nums.size();
    if (index < 0 || index >= n) {
        // 如果索引无效，则返回副本，尽管原始逻辑假定索引有效
        return nums;
    }
    std::vector<int> ans;
    ans.reserve(n - 1); // 为效率预留空间
    for (int j = 0; j < n; ++j) {
        if (j != index) {
            ans.push_back(nums[j]);
        }
    }
    return ans;
}

// 辅助函数 (等同于 Java 的 lenKmaxSum)
// 在给定的 vector 中查找长度为 k 的子数组的最大和。
int lenKmaxSum(const std::vector<int>& nums, int k) {
    int n = nums.size();
    if (n < k || k <= 0) { // 添加了对 k <= 0 的检查
        // 决定对于无效 k 的行为，可能返回 0 或 INT_MIN
        // 返回 INT_MIN 与查找最大值保持一致
        return (n == 0 && k == 0) ? 0 : INT_MIN;
    }
    int ans = INT_MIN;
    long long current_sum = 0; // 使用 long long 存储和以防止潜在的溢出

    // 计算第一个窗口的和
    for(int i = 0; i < k; ++i) {
        current_sum += nums[i];
    }
    ans = static_cast<int>(current_sum); // 初始最大和

    // 滑动窗口
    for (int i = k; i < n; ++i) {
        current_sum += nums[i] - nums[i - k];
        ans = std::max(ans, static_cast<int>(current_sum));
    }
    // 处理不存在长度为 k 的子数组的情况（已由 n < k 检查覆盖）
    // 如果初始 ans 保持为 INT_MIN 且循环未运行，则这是正确的。
    return ans;
}


// 暴力方法 (等同于 Java 的 maxSum1)
int maxSum1(const std::vector<int>& nums, int k) {
    int n = nums.size();
    // 如果删除一个元素使得数组对于 k 长度的子数组来说太小
    if (n <= k) {
        return 0; // 如果 k > 0，根据要求，或者可能是 INT_MIN
    }
    int ans = INT_MIN;
    for (int i = 0; i < n; ++i) {
        std::vector<int> rest = deleteElement(nums, i);
        // 确保剩余数组足够大
        if (rest.size() >= k) {
             int current_max = lenKmaxSum(rest, k);
             // 仅当在 lenKmaxSum 中找到有效和时才更新 ans
             if (current_max != INT_MIN) {
                ans = std::max(ans, current_max);
             }
        }
    }
    // 如果 ans 从未更新（例如，对于任何 'rest' 数组，k 都太大了），
    // 根据原始逻辑的基本情况返回 0，或者如果找不到有效的子数组则返回 INT_MIN。
    // 遵循原始逻辑的明显意图：
     return (ans == INT_MIN && n > k) ? 0 : ans; // 如果删除后未找到有效子数组，则返回 0，与 n <= k 基本情况匹配
}

// 使用滑动窗口和单调双端队列的优化方法 (等同于 Java 的 maxSum2)
// 时间复杂度 O(N)
int maxSum2(const std::vector<int>& nums, int k) {
    int n = nums.size();
    if (n <= k) {
        return 0; // 或如前所述的 INT_MIN
    }

    // 存储索引的单调双端队列。按递增顺序存储元素的索引，
    // 用于高效地找到 k+1 窗口中的最小值。
    std::deque<int> window;
    long long current_sum_k_plus_1 = 0; // 大小为 k+1 的当前窗口的和
    int ans = INT_MIN;

    for (int i = 0; i < n; ++i) {
        // 维护单调双端队列（元素索引按递增顺序）
        while (!window.empty() && nums[window.back()] >= nums[i]) {
            window.pop_back();
        }
        window.push_back(i);

        current_sum_k_plus_1 += nums[i];

        // 一旦窗口大小达到 k+1
        if (i >= k) {
            // 在移除窗口 [i-k, i] 内的最小元素后，
            // 结束于 'i' 的 k 长度子数组的和是 current_sum_k_plus_1 - nums[window.front()]
            // 注意：window.front() 给出当前 k+1 窗口中最小元素的索引。
             long long current_k_sum = current_sum_k_plus_1 - nums[window.front()];
             // 需要将可能很大的 long long 转回 int 进行 max 比较
             ans = std::max(ans, static_cast<int>(current_k_sum));

            // 从和中移除刚滑出 k+1 窗口的元素
            current_sum_k_plus_1 -= nums[i - k];

            // 如果双端队列前端的索引在窗口 [i-k, i] 之外，则将其移除
            if (window.front() == i - k) {
                window.pop_front();
            }
        }
    }
    // 与 maxSum1 类似，为了在未找到有效子数组时保持一致的返回逻辑
    return (ans == INT_MIN && n > k) ? 0 : ans;
}


// 生成随机 vector (等同于 Java 的 randomArray)
std::vector<int> randomArray(int n, int v) {
    std::vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = rand() % (2 * v + 1) - v; // 生成 [-v, v] 范围内的随机数
    }
    return ans;
}

// 用于测试的主函数 (等同于 Java 的 main)
int main() {
    srand(time(0)); // 初始化随机数生成器种子

    int n = 200; // 最大长度
    int v = 1000; // 最大绝对值
    int testTimes = 10000; // 测试次数

    std::cout << "start" << std::endl;

    for (int i = 0; i < testTimes; ++i) {
        int len = (rand() % n) + 1;       // 1 到 n 之间的随机长度
        std::vector<int> nums = randomArray(len, v);
        int k = (rand() % (len + 1)) ; // 0 到 len 之间的随机 k (如果 k 必须 >= 1 则调整)
                                       // 让我们使 k 在 1 和 len 之间，如同很可能意图的那样
        if (len > 0) {
             k = (rand() % len) + 1;
        } else {
             k = 1; // 或者适当地处理 len=0 的情况
        }


        int ans1 = maxSum1(nums, k); // 暴力解
        int ans2 = maxSum2(nums, k); // 优化解

        if (ans1 != ans2) {
            std::cerr << "error!" << std::endl;
            std::cerr << "nums: ";
            for (int x : nums) std::cerr << x << " ";
            std::cerr << std::endl;
            std::cerr << "k: " << k << std::endl;
            std::cerr << "ans1 : " << ans1 << std::endl;
            std::cerr << "ans2 : " << ans2 << std::endl;
            // 你可能想在出错时在这里 break 或 return
             return 1; // 表示错误
        }
    }

    std::cout << "end" << std::endl;

    return 0; // 表示成功执行
}
