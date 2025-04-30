#include <vector>
#include <numeric>      // 用于 std::accumulate（可选，也可以使用循环）
#include <algorithm>    // 用于 std::max
#include <iostream>
#include <limits>       // 用于 std::numeric_limits
#include <random>       // 用于生成随机数
#include <ctime>        // 用于为随机数生成器设置种子


// 魔法卷轴
// 给定一个数组nums，其中可能有正、负、0
// 每个魔法卷轴可以把nums中连续的一段全变成0
// 你希望数组整体的累加和尽可能大
// 卷轴使不使用、使用多少随意，但一共只有2个魔法卷轴
// 请返回数组尽可能大的累加和
// 对数器验证

// --- 暴力求解法（用于验证）---

// 暴力求解的辅助函数：计算在 nums[l..r] 中恰好使用一次卷轴的最大和。
// 使用 long long 类型来存储和，以防止潜在的溢出，返回值为 int 类型。
long long mustOneScroll(const std::vector<int>& nums, int l, int r) {
    // 如果范围无效或为空，使用一次卷轴得到的和显然为 0 。
    if (l > r) {
        return 0; // 或者如果不允许空范围，也许返回最小值？Java 返回 MIN_VALUE 。
                  // 让我们更紧密地匹配 Java 的行为。
    }
    if (l < 0 || r >= nums.size()) {
         // 如果有必要，处理无效的输入索引，尽管主逻辑应该能避免这种情况。
         // 为了简单起见，根据调用上下文假设 l 和 r 是有效的。
    }

    long long max_sum = std::numeric_limits<long long>::min();

    // 考虑滚动整个范围 [l, r] 的情况 -> 和为 0
    if (l <= r) { // 仅当范围有效时
         max_sum = 0;
    } else { // 如果最初 l > r ，意味着空范围。下面的循环不会运行。
         // 应该返回什么呢？如果外层循环不运行，Java 版本返回 MIN_VALUE 。
         // 让我们初始化为最小值，并且仅当发生有效的滚动时才计算为 0 。
         max_sum = std::numeric_limits<long long>::min(); // 对于空范围输入，更好地匹配 Java 的逻辑
    }


    // 遍历 [l, r] 内所有可能的滚动起始点 (a) 和结束点 (b)
    for (int a = l; a <= r; ++a) {
        for (int b = a; b <= r; ++b) {
            // 计算将 nums[a...b] 置为 0 后的和
            long long current_sum = 0;
            // 计算滚动前的元素和
            for (int i = l; i < a; ++i) {
                current_sum += nums[i];
            }
            // 计算滚动后的元素和
            for (int i = b + 1; i <= r; ++i) {
                current_sum += nums[i];
            }
            max_sum = std::max(max_sum, current_sum);
        }
    }
     // 处理如果 l > r 时循环可能不运行的边界情况
     // 如果循环不运行，Java 版本返回 Integer.MIN_VALUE 。
     // 如果 l > r ，我们的初始化会处理这种情况。
     // 如果 l <= r ，循环至少运行一次（a = l, b = l ），将 [l..l] 置为 0
     // 计算剩余元素的和。
     // 如果调用 mustOneScroll(nums, 0, -1) 会怎样呢？Java 循环不会运行 -> MIN_VALUE 。
     // C++ ：l = 0, r = -1 -> 循环条件 a <= r 为假 -> 返回初始最小值。正确。

     // 如果范围 [l, r] 是有效的，但滚动后没有剩余元素
     // （例如，滚动整个范围），max_sum 可能为 0 。
     // 如果在所有检查之后，max_sum 仍然是 long long 类型可能的最小值，
     // 这意味着存在问题或初始范围 [l, r] 为空。
     // 如果范围最初为空（l > r），让我们返回 0 ，否则返回计算得到的 max_sum 。
     // 然而，如果输入范围为空（例如 r < l ），Java 代码确实返回 MIN_VALUE 。
     // 如果没有有效地计算出滚动结果，让我们坚持返回最小值。
     if (l > r) return INT_MIN; // 对于空范围输入，显式匹配 Java 的行为

     // 如果唯一计算的滚动是整个范围 [l,r] ，max_sum 将为 0 。
     // 如果循环运行了，max_sum 保存了最佳结果。

     // 最终检查：C++ 结果应该是 int 类型。检查结果是否合适。
     if (max_sum < std::numeric_limits<int>::min() || max_sum > std::numeric_limits<int>::max()) {
         // 考虑到典型的约束条件，这种情况不太可能发生，但最好还是要注意。
         // 决定如何处理：截断、报错，或者根据问题规范假设结果合适。
         // 目前让我们假设结果合适，匹配 Java 的直接返回方式。
         // 然而，如果 max_sum 仍然是其初始最小值，返回 int::min 。
        if (max_sum == std::numeric_limits<long long>::min())
            return std::numeric_limits<int>::min();

     }

    return static_cast<int>(max_sum); // 将可能较大的 long long 结果转换回 int 类型
}


// 暴力求解主函数
int maxSum1(const std::vector<int>& nums) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    // 情况 1：不使用卷轴
    long long p1_ll = 0;
    for (int num : nums) {
        p1_ll += num;
    }
    // 如果 p1 溢出 int 类型，进行截断，尽管如果满足约束条件，最终结果不太可能溢出
     int p1 = (p1_ll > std::numeric_limits<int>::max())? std::numeric_limits<int>::max() :
              (p1_ll < std::numeric_limits<int>::min())? std::numeric_limits<int>::min() :
              static_cast<int>(p1_ll);


    // 情况 2：恰好使用一次卷轴（应用于整个数组范围）
    // 需要处理 mustOneScroll 可能返回 MIN_VALUE 的情况
    long long p2_ll = mustOneScroll(nums, 0, n - 1);
    int p2 = (p2_ll == std::numeric_limits<long long>::min())? std::numeric_limits<int>::min() : static_cast<int>(p2_ll);


    // 情况 3：恰好使用两次卷轴
    long long p3_ll = std::numeric_limits<long long>::min();
    // 分割点 'i' ：卷轴 1 用于 [0...i-1] ，卷轴 2 用于 [i...n-1]
    for (int i = 1; i < n; ++i) {
        long long scroll1_res = mustOneScroll(nums, 0, i - 1);
        long long scroll2_res = mustOneScroll(nums, i, n - 1);

        // 如果任一子问题返回 MIN_VALUE ，对于使用两次卷轴的情况，该分割是无效的
        if (scroll1_res != std::numeric_limits<long long>::min() && scroll1_res != std::numeric_limits<int>::min() &&
            scroll2_res != std::numeric_limits<long long>::min() && scroll2_res != std::numeric_limits<int>::min())
        {
            p3_ll = std::max(p3_ll, scroll1_res + scroll2_res);
        }
         // 由于 mustOneScroll 的返回类型现在是 int ，也添加了对 int::min 的检查
    }
     int p3 = (p3_ll == std::numeric_limits<long long>::min())? std::numeric_limits<int>::min() : static_cast<int>(p3_ll);


    // 返回三种情况中的最大值
    return std::max({p1, p2, p3}); // 使用初始化列表调用 std::max
}


// --- 动态规划解法（高效，时间复杂度为 O(n)）---

int maxSum2(const std::vector<int>& nums) {
    int n = nums.size();
    if (n == 0) {
        return 0;
    }

    // 情况 1：不使用卷轴
    long long p1_ll = 0;
    for (int num : nums) {
        p1_ll += num;
    }
     // 如果最终比较有必要，将 p1_ll 截断到 int 类型范围
     int p1 = (p1_ll > std::numeric_limits<int>::max())? std::numeric_limits<int>::max() :
              (p1_ll < std::numeric_limits<int>::min())? std::numeric_limits<int>::min() :
              static_cast<int>(p1_ll);


    // --- 计算前缀数组 ---
    // prefix[i] ：在 nums[0...i] 范围内恰好使用一次卷轴的最大和。
    std::vector<long long> prefix(n);
    // 基本情况：prefix[0] - 必须在 [0..0] 范围内使用卷轴，意味着 nums[0] 变为 0 。和为 0 。
    prefix[0] = 0;
    long long current_sum_prefix = nums[0];
    // maxPresum ：到目前为止遇到的最大前缀和（对于卷轴在索引 i 处结束的情况很重要）
    long long maxPresum_prefix = std::max(0LL, (long long)nums[0]); // 如果我们将以索引 0 结尾的后缀置为 0 ，得到的最大和

    for (int i = 1; i < n; ++i) {
        // 选项 1：[0...i-1] 范围内存在最优的卷轴使用方式，并且 nums[i] 不进行滚动处理。
        long long option1 = prefix[i - 1] + nums[i];
        // 选项 2：卷轴包含索引 i （将其置为 0 ）。
        // 最佳和是通过保留卷轴开始前的最大前缀和来实现的。
        long long option2 = maxPresum_prefix;
        prefix[i] = std::max(option1, option2);

        // 为下一次迭代更新 current_sum 和 maxPresum
        current_sum_prefix += nums[i];
        maxPresum_prefix = std::max(maxPresum_prefix, current_sum_prefix);
    }

    // 情况 2：必须恰好使用一次卷轴（结果是整个范围的最优解）
    long long p2_ll = prefix[n - 1];
     int p2 = (p2_ll > std::numeric_limits<int>::max())? std::numeric_limits<int>::max() :
              (p2_ll < std::numeric_limits<int>::min())? std::numeric_limits<int>::min() :
              static_cast<int>(p2_ll);


    // --- 计算后缀数组 ---
    // suffix[i] ：在 nums[i...n-1] 范围内恰好使用一次卷轴的最大和。
    std::vector<long long> suffix(n);
    // 基本情况：suffix[n-1] - 必须在 [n-1..n-1] 范围内使用卷轴，意味着 nums[n-1] 变为 0 。和为 0 。
    suffix[n - 1] = 0;
    long long current_sum_suffix = nums[n - 1];
    // maxSufsum ：到目前为止遇到的最大后缀和（如果卷轴从索引 i 处开始，这很重要）
    long long maxSufsum_suffix = std::max(0LL, (long long)nums[n - 1]); // 如果我们将以索引 n-1 开始的前缀置为 0 ，得到的最大和

    for (int i = n - 2; i >= 0; --i) {
        // 选项 1：[i+1...n-1] 范围内存在最优的卷轴使用方式，并且 nums[i] 不进行滚动处理。
        long long option1 = (long long)nums[i] + suffix[i + 1];
         // 选项 2：卷轴包含索引 i （将其置为 0 ）。
        // 最佳和是通过保留卷轴结束后的最大后缀和来实现的。
        long long option2 = maxSufsum_suffix;
        suffix[i] = std::max(option1, option2);

        // 为下一次迭代更新 current_sum 和 maxSufsum
        current_sum_suffix += nums[i];
        maxSufsum_suffix = std::max(maxSufsum_suffix, current_sum_suffix);
    }

    // 情况 3：必须恰好使用两次卷轴
    long long p3_ll = std::numeric_limits<long long>::min(); // 初始化为最小值
    // 分割点 'i' ：卷轴 1 用于 [0...i-1] （prefix[i-1]），卷轴 2 用于 [i...n-1] （suffix[i]）
    for (int i = 1; i < n; ++i) {
        // 确保 prefix[i-1] 和 suffix[i] 都表示有效的和
        // （如果计算正确，它们应该总是 >= 0 ，因为滚动始终是一种选择）
        p3_ll = std::max(p3_ll, prefix[i - 1] + suffix[i]);
    }
     // 如果 n = 1 ，循环不运行，p3_ll 仍然是最小值。
     int p3 = (p3_ll == std::numeric_limits<long long>::min())? std::numeric_limits<int>::min() :
              (p3_ll > std::numeric_limits<int>::max())? std::numeric_limits<int>::max() :
              (p3_ll < std::numeric_limits<int>::min())? std::numeric_limits<int>::min() :
              static_cast<int>(p3_ll);


    // 返回三种情况中的最大值
     // 如果 n < 2 ，确保 max 函数正确处理 p3 的默认值（min_int）
    return std::max({p1, p2, p3});
}


// --- 测试辅助函数 ---

// 生成一个在 [-v, v] 范围内的随机整数向量
std::vector<int> randomArray(int n, int v) {
    if (n <= 0) return {};
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(-v, v); // 范围 [-v, v]

    std::vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = distrib(gen);
    }
    return ans;
}

// --- 主测试函数 ---

int main() {
    const int n = 50;  // 最大长度
    const int v = 100; // 最大绝对值
    const int testTime = 10000; // 测试次数

    // 为用于生成长度的较简单的生成器设置种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "Testing started..." << std::endl;
    bool error_found = false;
    for (int i = 0; i < testTime; ++i) {
        // 生成 0 到 n-1 （包含 0 和 n-1 ）之间的随机长度
        int len = std::rand() % n;
        std::vector<int> nums = randomArray(len, v);

        int ans1 = maxSum1(nums);
        int ans2 = maxSum2(nums);

        if (ans1 != ans2) {
            std::cout << "Error!" << std::endl;
            std::cout << "Input: {";
            for(size_t k=0; k<nums.size(); ++k) {
                std::cout << nums[k] << (k == nums.size()-1 ? "" : ", ");
            }
            std::cout << "}" << std::endl;
            std::cout << "Brute force (ans1): " << ans1 << std::endl;
            std::cout << "DP (ans2): " << ans2 << std::endl;
            error_found = true;
            // break; // 可选：在第一次出错时停止
        }
    }

    if (!error_found) {
         std::cout << "All tests passed!" << std::endl;
    }
    std::cout << "Testing finished." << std::endl;

    return 0;
}