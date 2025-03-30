// 对数组进行排序并去重，返回去重后的数组长度
int sort(long nums[], int length) {
    sort(nums, nums + length);
    int size = 1;
    for (int i = 1; i < length; i++) {
        if (nums[i] != nums[size - 1]) {
            nums[size++] = nums[i];
        }
    }
    return size;
}

// 在有序数组nums的前size个元素中查找值v的排名（1-based index）
int rank1(long nums[], long v, int size) {
    long ans = 0;
    auto it = std::lower_bound(nums,nums+size, v);
    // 计算 num 在 unique_arr 中的索引位置
    ans = distance(nums, it);
    return ans + 1;
}

int sort(std::vector<long>& nums) {
    std::sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    return nums.size();
}

// 在有序vector的前size个元素中查找值v的排名（1-based index）
int rank1(const std::vector<long>& nums, long v, int size) {
    int ans = 0;
    // 使用二分查找找到 num 在 unique_arr 中的位置
    auto it = std::lower_bound(nums.begin(), nums.end(), v);
    // 计算 num 在 unique_arr 中的索引位置
    ans = std::distance(nums.begin(), it);
    return ans + 1;
}

int rank1(vector<ll>& nums, ll v, int size) {
    int l = 0;
    int r = size - 1;
    int m, ans = 0;
    while (l <= r) {
        m = (l + r) / 2;
        if (nums[m] >= v) {
            ans = m;
            r = m - 1;
        }
        else {
            l = m + 1;
        }
    }
    return ans + 1;
}
