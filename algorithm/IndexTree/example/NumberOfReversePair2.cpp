#include <cstdio>
#include <algorithm>

// 逆序对数量(值域树状数组)
// 给定一个长度为n的数组arr
// 如果 i < j 且 arr[i] > arr[j]
// 那么(i,j)就是一个逆序对
// 求arr中逆序对的数量
// 1 <= n <= 5 * 10^5
// 1 <= arr[i] <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P1908

using namespace std;

const int MAXN = 500010;

int arr[MAXN];      // 原始数组
int sortArr[MAXN];  // 用于排序和离散化的数组
int tree[MAXN];     // 树状数组
int n, m;           // n为数组长度，m为离散化后的不同元素个数

// 获取x的最低位1及其右边的0组成的数
inline int lowbit(int x) {
    return x & -x;
}

// 在树状数组的idx位置增加val
void add(int idx, int val) {
    while (idx <= m) {
        tree[idx] += val;
        idx += lowbit(idx);  // 向上更新父节点
    }
}

// 查询树状数组前idx项的和
long long query(int idx) {
    long long res = 0;
    while (idx > 0) {
        res += tree[idx];
        idx -= lowbit(idx);  // 向下查询子节点
    }
    return res;
}

// 查找值v在离散化后数组中的排名（位置）
int rak(int v) {
    int l = 1, r = m, ans = 0;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (sortArr[mid] >= v) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

// 计算逆序对数量的主函数
long long compute() {
    // 对数组进行排序，用于离散化
    sort(sortArr + 1, sortArr + n + 1);

    // 离散化：去除重复元素，压缩值域
    m = 1;
    for (int i = 2; i <= n; ++i) {
        if (sortArr[i] != sortArr[m]) {
            sortArr[++m] = sortArr[i];
        }
    }

    // 将原数组中的值替换为离散化后的排名
    for (int i = 1; i <= n; ++i) {
        arr[i] = rak(arr[i]);
    }

    // 初始化树状数组
    for (int i = 1; i <= m; ++i) {
        tree[i] = 0;
    }

    long long ans = 0;
    // 从右向左遍历数组，计算逆序对
    for (int i = n; i >= 1; --i) {
        // 查询比当前元素小的元素个数（已经处理过的）
        if (arr[i] > 1) {
            ans += query(arr[i] - 1);
        }
        // 将当前元素加入树状数组
        add(arr[i], 1);
    }
    return ans;
}

int main() {
    // 读入数组长度
    scanf("%d", &n);

    // 读入数组元素
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &arr[i]);
        sortArr[i] = arr[i];  // 同时复制到sortArr用于排序
    }

    // 输出逆序对数量
    printf("%lld\n", compute());
    return 0;
}
