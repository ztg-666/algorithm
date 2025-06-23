#include <cstdio>
#include <algorithm>

// 逆序对数量(归并分治)
// 给定一个长度为n的数组arr
// 如果 i < j 且 arr[i] > arr[j]
// 那么(i,j)就是一个逆序对
// 求arr中逆序对的数量
// 1 <= n <= 5 * 10^5
// 1 <= arr[i] <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P1908

using namespace std;

const int MAXN = 500001;  // 数组最大容量
int arr[MAXN];           // 原始数组
int help[MAXN];          // 归并排序辅助数组

/**
 * 归并过程并统计逆序对数量
 * @param l 左边界
 * @param m 中点
 * @param r 右边界
 * @return 跨越左右两部分的逆序对数量
 */
long long merge(int l, int m, int r) {
    long long ans = 0;

    // 核心逻辑：统计跨越左右两部分的逆序对
    // 逆序对定义：对于i < j，如果arr[i] > arr[j]，则(i,j)构成逆序对
    // 由于左半部分[l,m]和右半部分[m+1,r]已经各自有序
    // 我们需要统计左半部分的每个元素与右半部分形成的逆序对

    for (int i = m, j = r; i >= l; i--) {  // 从左半部分最右端开始向左扫描
        // 在右半部分中找到比arr[i]小的元素
        // 由于右半部分有序，使用双指针从右向左找到第一个小于arr[i]的位置
        while (j >= m + 1 && arr[i] <= arr[j]) {
            j--;  // 如果arr[i] <= arr[j]，说明arr[j]不能与arr[i]构成逆序对
        }
        // 此时j指向右半部分中最后一个小于arr[i]的元素
        // 那么右半部分中从m+1到j的所有元素都小于arr[i]
        // 这些元素都能与arr[i]构成逆序对
        ans += j - m;  // 累加逆序对数量
    }

    // 标准归并排序：合并两个有序子数组
    int i = l;      // help数组的索引
    int a = l;      // 左半部分的指针
    int b = m + 1;  // 右半部分的指针

    // 比较两个子数组的元素，将较小的放入help数组
    while (a <= m && b <= r) {
        help[i++] = arr[a] <= arr[b] ? arr[a++] : arr[b++];
    }

    // 处理左半部分剩余元素
    while (a <= m) {
        help[i++] = arr[a++];
    }

    // 处理右半部分剩余元素
    while (b <= r) {
        help[i++] = arr[b++];
    }

    // 将排序后的结果复制回原数组
    for (i = l; i <= r; i++) {
        arr[i] = help[i];
    }

    return ans;  // 返回跨越左右两部分的逆序对数量
}

/**
 * 分治递归函数：计算区间[l,r]内的逆序对数量
 * @param l 左边界
 * @param r 右边界
 * @return 区间[l,r]内的逆序对总数
 *
 * 算法思想：
 * 1. 将数组分为左右两部分
 * 2. 递归计算左半部分的逆序对数量
 * 3. 递归计算右半部分的逆序对数量
 * 4. 计算跨越左右两部分的逆序对数量
 * 5. 总逆序对 = 左半部分逆序对 + 右半部分逆序对 + 跨越逆序对
 */
long long f(int l, int r) {
    // 递归终止条件：只有一个元素时，逆序对数量为0
    if (l == r) {
        return 0;
    }

    int m = (l + r) / 2;  // 计算中点

    // 分治策略：
    // 1. f(l, m)：计算左半部分内部的逆序对
    // 2. f(m + 1, r)：计算右半部分内部的逆序对
    // 3. merge(l, m, r)：计算跨越左右两部分的逆序对
    return f(l, m) + f(m + 1, r) + merge(l, m, r);
}

int main() {
    int n;
    scanf("%d", &n);  // 读入数组长度

    // 读入数组元素
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // 调用分治函数计算逆序对数量并输出
    // 时间复杂度：O(n log n)
    // 空间复杂度：O(n)
    printf("%lld\n", f(0, n - 1));

    return 0;
}
