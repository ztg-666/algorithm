
// 表中第i条记录arr[i] = [firsti, lasti, num]
// 意味着在从 firsti 到 lasti 数组元素加上 num
vector<int> difference_array(vector<vector<int>>& arr, int n) {
    // 初始化一个大小为 n+2 的差分数组，初始值为 0
    vector<int> cnt(n + 2, 0);

    // 遍历每一条操作记录
    for (const auto& a : arr) {
        // 在 firsti 位置增加 num
        cnt[a[0]] += a[2];
        // 在 lasti+1 位置减少 num，这样在后续计算前缀和时，num 的影响仅限于 [firsti, lasti] 范围内
        cnt[a[1] + 1] -= a[2];
    }

    // 计算前缀和，将差分数组转换为结果数组
    for (int i = 1; i < cnt.size(); i++) {
        cnt[i] += cnt[i - 1];
    }

    // 初始化结果数组，大小为 n
    vector<int> ans(n);
    // 将 cnt 数组的有效部分复制到结果数组 ans 中
    for (int i = 0; i < n; i++) {
        ans[i] = cnt[i + 1];
    }

    // 返回结果数组
    return ans;
}
