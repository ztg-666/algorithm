#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>


class MajorityChecker {
private:
    int n;

    // 用于线段树的数据结构
    // cand: 存储区间的候选众数
    // hp: 存储候选众数的“血量”（根据Boyer-Moore投票算法）
    std::vector<int> cand;
    std::vector<int> hp;

    // 用于快速计数的辅助数据结构
    // 存储 {数值, 原始索引} 对
    std::vector<std::pair<int, int>> nums;

public:
    // 构造函数：用数组 arr 初始化数据结构
    MajorityChecker(std::vector<int>& arr) {
        n = arr.size();

        // 1. 初始化并构建用于计数的 `nums` 数组
        // 这个数组存储了 (值, 原始下标) 对，并按值和下标排序
        // 这使得我们可以在 O(log n) 时间内查询一个值在任意范围内的出现次数
        nums.resize(n);
        for (int i = 0; i < n; ++i) {
            nums[i] = {arr[i], i};
        }
        // 按值升序排序，值相同时按下标升序排序
        std::sort(nums.begin(), nums.end());

        // 2. 初始化并构建线段树
        // 线段树用于在 O(log n) 时间内找出任意子数组的“候选众数”
        // 这个候选者是子数组中唯一可能满足“海王数”条件的数
        cand.resize(n * 4);
        hp.resize(n * 4);
        buildTree(arr, 1, n, 1);
    }

    // 查询函数：返回 arr[left...right] 上的海王数
    int query(int left, int right, int threshold) {
        // 1. 使用线段树找到 arr[left...right] 的候选众数
        // 注意：线段树使用的是1-based索引
        std::pair<int, int> result = findCandidate(left + 1, right + 1, 1, n, 1);
        int candidate = result.first;

        // 2. 验证该候选众数的出现次数是否 >= threshold
        if (countInRange(left, right, candidate) >= threshold) {
            return candidate;
        }
        return -1;
    }

private:
    // --- 线段树相关方法 ---

    // 合并左右子节点的信息到父节点
    void up(int i) {
        int left_cand = cand[i << 1];
        int left_hp = hp[i << 1];
        int right_cand = cand[i << 1 | 1];
        int right_hp = hp[i << 1 | 1];

        if (left_cand == right_cand) {
            // 如果左右子节点的候选者相同，直接合并血量
            cand[i] = left_cand;
            hp[i] = left_hp + right_hp;
        } else {
            // 如果不同，血量多者胜出，血量为二者之差
            if (left_hp >= right_hp) {
                cand[i] = left_cand;
                hp[i] = left_hp - right_hp;
            } else {
                cand[i] = right_cand;
                hp[i] = right_hp - left_hp;
            }
        }
    }

    // 递归构建线段树
    // l, r: 当前节点代表的范围 (1-based)
    // i: 当前节点在线段树数组中的索引
    void buildTree(const std::vector<int>& arr, int l, int r, int i) {
        if (l == r) {
            // 叶子节点，代表单个元素
            cand[i] = arr[l - 1]; // arr 是 0-based
            hp[i] = 1;
        } else {
            int mid = l + ((r - l) >> 1);
            buildTree(arr, l, mid, i << 1);
            buildTree(arr, mid + 1, r, i << 1 | 1);
            up(i);
        }
    }

    // 在线段树上查询 [jobl, jobr] 范围内的候选众数
    // jobl, jobr: 查询的目标范围 (1-based)
    // l, r: 当前节点代表的范围 (1-based)
    // i: 当前节点在线段树数组中的索引
    std::pair<int, int> findCandidate(int jobl, int jobr, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前节点范围完全被查询范围覆盖
            return {cand[i], hp[i]};
        }

        int mid = l + ((r - l) >> 1);
        if (jobr <= mid) {
            // 查询范围完全在左子树
            return findCandidate(jobl, jobr, l, mid, i << 1);
        }
        if (jobl > mid) {
            // 查询范围完全在右子树
            return findCandidate(jobl, jobr, mid + 1, r, i << 1 | 1);
        }

        // 查询范围横跨左右子树，需要合并结果
        auto left_result = findCandidate(jobl, jobr, l, mid, i << 1);
        auto right_result = findCandidate(jobl, jobr, mid + 1, r, i << 1 | 1);

        // 使用与 up 方法相同的逻辑合并左右两边的结果
        if (left_result.first == right_result.first) {
            return {left_result.first, left_result.second + right_result.second};
        } else {
            if (left_result.second >= right_result.second) {
                return {left_result.first, left_result.second - right_result.second};
            } else {
                return {right_result.first, right_result.second - left_result.second};
            }
        }
    }

    // --- 快速计数相关方法 ---

    // 使用二分查找计算值 `val` 在原始数组 arr[left...right] 中出现的次数
    int countInRange(int left, int right, int val) {
        // 次数 = (<= right 的 val 个数) - (< left 的 val 个数)
        // bs(val, right) 返回的是在原始数组下标 <= right 的范围内，值 val 出现的次数
        return bs(val, right) - bs(val, left - 1);
    }

    // 在已排序的 `nums` 数组中，计算值 `val` 且原始下标 `<= index` 的元素有多少个
    // 这等价于在 `nums` 中查找有多少个 pair `p` 满足 `p <= {val, index}`
    int bs(int val, int index) {
        // std::upper_bound 返回第一个大于 {val, index} 的元素的迭代器
        // 从开始到这个迭代器的距离就是 <= {val, index} 的元素数量
        auto it = std::upper_bound(nums.begin(), nums.end(), std::make_pair(val, index));
        return std::distance(nums.begin(), it);
    }
};

