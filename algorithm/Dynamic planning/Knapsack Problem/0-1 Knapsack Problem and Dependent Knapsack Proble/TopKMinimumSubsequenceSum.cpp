#include <iostream>      // 包含输入输出流头文件
#include <vector>        // 包含向量头文件
#include <numeric>       // 包含 std::accumulate (用于 topKSum2 中的求和)
#include <algorithm>     // 包含 std::sort, std::min
#include <queue>         // 包含 std::priority_queue (优先队列/堆)
#include <utility>       // 包含 std::pair (对)
#include <functional>    // 包含 std::greater (用于最小堆)
#include <random>        // 包含 C++11 随机数生成工具
#include <chrono>        // 包含时间库，用于随机数种子
// 非负数组前k个最小的子序列累加和
// 给定一个数组nums，含有n个数字，都是非负数
// 给定一个正数k，返回所有子序列中累加和最小的前k个累加和
// 子序列是包含空集的
// 1 <= n <= 10^5
// 1 <= nums[i] <= 10^6
// 1 <= k <= 10^5
// 注意这个数据量，用01背包的解法是不行的，时间复杂度太高了
// 对数器验证
using namespace std; // 使用标准命名空间

// 为 long long 定义类型别名，用于存储累加和，防止溢出
using ll = long long;

// --- 暴力递归方法 (topKSum1 & generate_all_sums_recursive) ---

/**
 * @brief 递归辅助函数，生成所有子序列的和。
 * @param nums 输入的非负数数组。
 * @param index 当前正在考虑的索引。
 * @param current_sum 到目前为止累积的和。
 * @param all_sums 用于存储所有生成的子序列和的向量。
 */
void generate_all_sums_recursive(const vector<int>& nums, int index, ll current_sum, vector<ll>& all_sums) {
    // 基本情况：到达数组末尾
    if (index == nums.size()) {
        all_sums.push_back(current_sum); // 将当前和添加到结果列表
        return;
    }
    // 选择 1: 不包含 nums[index]
    generate_all_sums_recursive(nums, index + 1, current_sum, all_sums);
    // 选择 2: 包含 nums[index]
    generate_all_sums_recursive(nums, index + 1, current_sum + nums[index], all_sums);
}

/**
 * @brief 使用暴力递归查找前 k 个最小的子序列和。
 * 生成所有 2^n 个和，对它们进行排序，并返回前 k 个。
 * 对于大的 n 效率低下。
 * @param nums 输入的非负数数组。
 * @param k 需要返回的最小和的数量。
 * @return 包含 k 个最小子序列和的向量。
 */
vector<ll> topKSum1(const vector<int>& nums, int k) {
    vector<ll> all_sums; // 存储所有子序列和
    generate_all_sums_recursive(nums, 0, 0, all_sums); // 生成所有和
    sort(all_sums.begin(), all_sums.end()); // 对所有和进行排序

    // 确保请求的和的数量不超过实际存在的数量
    k = min(k, (int)all_sums.size());

    // 从排序后的列表中取出前 k 个
    vector<ll> ans(all_sums.begin(), all_sums.begin() + k);
    return ans;
}

// --- 0/1 背包 DP 方法 (topKSum2) ---

/**
 * @brief 使用动态规划（0/1 背包变种）查找前 k 个最小的子序列和。
 * 计算每个可能的和对应的子序列数量。
 * 警告：对于给定的约束条件（n, nums[i] 高达 10^5/10^6），此方法不可行，
 * 因为潜在的总和可能非常巨大（高达 10^11），导致 dp 表需要过多的内存。
 * 提供此翻译仅为完整性。
 * @param nums 输入的非负数数组。
 * @param k 需要返回的最小和的数量。
 * @return 包含 k 个最小子序列和的向量（如果可行）。
 */
vector<ll> topKSum2(const vector<int>& nums, int k) {
    ll total_sum = 0; // 计算所有元素总和
    for (int num : nums) {
        total_sum += num;
    }

    // dp[j] = 和为 j 的子序列的数量
    // 大小需要是 total_sum + 1。这是内存方面的瓶颈。
    // 同时，计数本身也可能超过 int，因此使用 vector<ll>。
    if (total_sum > 20000000) { // 使用启发式限制来防止过多的内存分配
         cerr << "error" << total_sum <<"" << endl;
         // 或者可以抛出异常/返回错误码
         return {}; // 表示不可行
    }

    vector<ll> dp(total_sum + 1, 0);
    dp[0] = 1; // 得到和为 0 的方法有一种（空子序列）

    for (int num : nums) {
        // 从后往前遍历，确保每个物品只用一次
        for (ll j = total_sum; j >= num; --j) {
            if (dp[j - num] > 0) { // 仅当之前的状态可达时才进行
               // 如果计数可能非常大，在增加计数值前检查潜在的溢出
               // 对于合理的 k 和 n，直接相加可能没问题，但严格来说是 dp[j] += dp[j-num];
                dp[j] += dp[j - num]; // 增加和为 j 的子序列数量

            }
        }
    }

    vector<ll> ans; // 存储最终结果
    ans.reserve(k); // 预分配空间
    int count = 0; // 已找到的和的数量
    // 遍历所有可能的和
    for (ll j = 0; j <= total_sum && count < k; ++j) {
        // 对于每个和 j，添加 dp[j] 次到结果中
        for (int i = 0; i < dp[j] && count < k; ++i) {
            ans.push_back(j);
            count++;
        }
    }
    return ans;
}

// --- 最优的基于堆的方法 (topKSum3) ---

/**
 * @brief 使用最小堆查找前 k 个最小的子序列和。
 * 最优方法，时间复杂度 O(n log n + k log k)。
 * @param nums 输入的非负数数组。会通过排序被修改。
 * @param k 需要返回的最小和的数量。
 * @return 包含 k 个最小子序列和的向量。
 */
vector<ll> topKSum3(vector<int>& nums, int k) {
    vector<ll> ans; // 存储结果
    if (k <= 0) return ans; // 处理无效 k
    int n = nums.size();

    // 对输入数组排序 - 这是堆逻辑的关键
    sort(nums.begin(), nums.end());

    // 最小堆，存储 pair: {当前和, 考虑的最后一个元素的索引}
    // 使用 pair<ll, int> 确保和是 long long 类型，并主要按和排序。
    // greater<pair<ll, int>> 使其成为最小堆（顶部是最小元素）
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> heap;

    ans.push_back(0); // 最小的和总是 0 (空子序列)
    if (k == 1) return ans; // 如果只需要 k=1，返回 {0}

    // 如果数组不为空，将第一个元素的和加入堆中
    if (n > 0) {
        heap.push({(ll)nums[0], 0}); // {和 = nums[0], 索引 = 0}
    }

    // 提取 k-1 个最小的和（在初始的 0 之后）
    while (ans.size() < k && !heap.empty()) {
        pair<ll, int> top = heap.top(); // 获取堆顶（当前最小和）
        heap.pop(); // 弹出堆顶

        ll current_sum = top.first;   // 当前和
        int current_index = top.second; // 产生这个和的子序列中考虑的最后一个元素的索引

        ans.push_back(current_sum); // 将当前最小和加入结果

        // 优化：如果我们刚加入了第 k 个元素，就停止。
        if (ans.size() == k) {
            break;
        }

        // 基于下一个索引处的元素生成下一个候选和
        int next_index = current_index + 1;
        if (next_index < n) {
            // 候选 1: 扩展产生 current_sum 的子序列
            // 将 nums[next_index] 添加进去。
            // 新状态代表一个以 nums[next_index] 结尾的子序列。
            heap.emplace(current_sum + nums[next_index], next_index);

            // 候选 2: 将 nums[current_index] 替换为 nums[next_index]
            // 这代表取得到 current_sum 的子序列，移除 nums[current_index]，然后加上 nums[next_index]。
            // 实质上是探索使用 nums[next_index] 的子序列，这些子序列源自与 'top' 相同的（不含nums[current_index]的）前缀子序列。
            // 新状态也代表一个以 nums[next_index] 结尾的子序列。
            // （避免重复计算同一子序列和的关键在于这种生成方式和堆的性质）
            heap.emplace(current_sum - nums[current_index] + nums[next_index], next_index);
        }
    }

    return ans;
}

// --- 测试工具 ---

/**
 * @brief 生成一个随机整数向量。
 * @param len 向量的长度。
 * @param maxValue 元素的最大值（不包含）。
 * @return 一个随机非负整数向量。
 */
vector<int> randomArray(int len, int maxValue) {
    // 使用更好的随机数生成器
    // 使用 static 确保每次调用函数时不是重新创建和播种
    static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, maxValue - 1); // 生成 [0, maxValue-1] 范围内的整数
    vector<int> ans(len);
    for (int i = 0; i < len; ++i) {
        ans[i] = dist(rng);
    }
    return ans;
}

/**
 * @brief 检查两个 long long 向量是否相等。
 * @param v1 第一个向量。
 * @param v2 第二个向量。
 * @return 如果相等则为 true，否则为 false。
 */
bool equals(const vector<ll>& v1, const vector<ll>& v2) {
    return v1 == v2; // std::vector 内置了比较运算符
}


// --- 用于测试的主函数 ---

int main() {
    // 关闭 C++ 流与 C 标准 IO 的同步，加速 cin/cout
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n_max = 15; // 测试的最大长度 (保持较低以便暴力/DP方法可行)
    int v_max = 40; // 测试的最大值
    int testTime = 5000; // 测试次数

    cout << "start..." << endl;

    bool all_passed = true; // 标记是否所有测试都通过

    // 为主循环结构使用一致的随机生成器
    static mt19937 main_rng(chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < testTime; ++i) {
        // 随机生成数组长度
        uniform_int_distribution<int> len_dist(1, n_max);
        int len = len_dist(main_rng);

        // 生成随机数组
        vector<int> nums = randomArray(len, v_max);
        vector<int> nums_copy_for_sort = nums; // 复制一份给 topKSum3，因为它会排序

        // 最大可能的 k 是 2^len
        ll max_k_ll = 1LL << len;
        // 确保 k 不超过合理限制或潜在的 int 最大值
        int max_k = (max_k_ll > numeric_limits<int>::max() -1) ? numeric_limits<int>::max() -1 : (int)max_k_ll;

        // 在 1 到 max_k (含) 之间生成 k
        uniform_int_distribution<int> k_dist(1, max_k > 0 ? max_k : 1); // 处理 len=0 的情况 (max_k=1)
        int k = k_dist(main_rng);


        // 运行算法
        vector<ll> ans1 = topKSum1(nums, k); // 暴力解
        // vector<ll> ans2 = topKSum2(nums, k); // 可能不可行，如果总和很大则跳过直接比较
        vector<ll> ans3 = topKSum3(nums_copy_for_sort, k); // 最优堆解 (传入副本，因为它会排序)

        // 比较结果 (比较 ans1 和 ans3)
        if (!equals(ans1, ans3)) {
            cerr << "error" << endl;
            cerr << "nums: {";
            for(size_t j=0; j<nums.size(); ++j) cerr << nums[j] << (j == nums.size()-1 ? "" : ", ");
            cerr << "}" << endl;
            cerr << "k: " << k << endl;
            cerr << "Ans1 (1): {";
            for(size_t j=0; j<ans1.size(); ++j) cerr << ans1[j] << (j == ans1.size()-1 ? "" : ", ");
            cerr << "}" << endl;
            cerr << "Ans3 (2): {";
             for(size_t j=0; j<ans3.size(); ++j) cerr << ans3[j] << (j == ans3.size()-1 ? "" : ", ");
            cerr << "}" << endl;
            all_passed = false;
            break; // 出现第一个错误时停止
        }

        // 可选：如果可行范围允许，添加对 ans2 的检查
        // if (/* 检查 total_sum 是否足够小以至于 ans2 可行 */ && !equals(ans1, ans2)) { ... }

        // 每完成 500 次测试打印一次进度
        if ((i + 1) % 500 == 0) {
            cout << "finish " << (i + 1) << " times" << endl;
        }
    }

    if (all_passed) {
        cout << "success" << endl;
    } else {
         cout << "error" << endl;
    }

    return 0;
}
