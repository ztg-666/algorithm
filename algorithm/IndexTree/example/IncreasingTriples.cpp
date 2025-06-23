#include <iostream>
#include <cstdio>
#include <algorithm>

// 升序三元组数量
// 给定一个数组arr，如果i < j < k且arr[i] < arr[j] < arr[k]
// 那么称(i, j, k)为一个升序三元组
// 返回arr中升序三元组的数量
// 测试链接 : https://www.luogu.com.cn/problem/P1637

using namespace std;

const int MAXN = 30001;

// 原始数组
int arr[MAXN];
// 用于排序和去重的辅助数组
int sortArr[MAXN];
// tree1: 记录每个值出现的次数
long tree1[MAXN];
// tree2: 记录每个值前面比它小的数的个数
long tree2[MAXN];
int n, m; // n为数组长度，m为去重后的元素个数

// 树状数组的核心函数：获取最低位的1
inline int lowbit(int i) {
    return i & -i;
}

// 树状数组更新操作：在位置i增加值c
void add(long tree[], int i, long c) {
    while (i <= m) {
        tree[i] += c;
        i += lowbit(i); // 向上更新父节点
    }
}

// 树状数组查询操作：计算前缀和[1, i]
long sum(long tree[], int i) {
    long ans = 0;
    while (i > 0) {
        ans += tree[i];
        i -= lowbit(i); // 向左查询相关节点
    }
    return ans;
}

// 二分查找函数：找到值v在去重排序数组中的位置（离散化）
int rak(int v) {
    int l = 1, r = m, mid;
    int ans = 0;
    while (l <= r) {
        mid = (l + r) / 2;
        if (sortArr[mid] >= v) {
            ans = mid; // 找到第一个大于等于v的位置
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

// 核心计算函数：计算逆序三元组数量
long compute() {
    // 第一步：对数组进行排序
    sort(sortArr + 1, sortArr + n + 1);

    // 第二步：去重，将相同元素合并
    m = 1;
    for (int i = 2; i <= n; i++) {
        if (sortArr[m] != sortArr[i]) {
            sortArr[++m] = sortArr[i];
        }
    }

    // 第三步：离散化处理，将原数组中的值映射到排序后的位置
    for (int i = 1; i <= n; i++) {
        arr[i] = rak(arr[i]);
    }

    // 第四步：初始化两个树状数组
    fill(tree1 + 1, tree1 + m + 1, 0);
    fill(tree2 + 1, tree2 + m + 1, 0);

    long ans = 0;

    // 第五步：从左到右遍历数组，统计逆序三元组
    for (int i = 1; i <= n; i++) {
        // 对于当前元素arr[i]，计算有多少个逆序三元组以它作为中间元素
        // sum(tree2, arr[i] - 1) 表示：
        // 在位置i之前，有多少对(j,k)满足 j<k<i 且 arr[j]>arr[k]>arr[i]
        ans += sum(tree2, arr[i] - 1);

        // 更新tree1：记录当前值出现的次数
        add(tree1, arr[i], 1);

        // 更新tree2：对于当前值arr[i]，
        // sum(tree1, arr[i] - 1)表示在位置i之前有多少个元素小于arr[i]
        // 这些元素可以与arr[i]组成逆序对，arr[i]可以作为这些逆序对的较大元素
        add(tree2, arr[i], sum(tree1, arr[i] - 1));
    }

    return ans;
}

int main() {
    // 读入数组长度
    scanf("%d", &n);

    // 读入数组元素
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
        sortArr[i] = arr[i]; // 同时复制到sortArr用于后续排序
    }

    // 输出逆序三元组的数量
    printf("%ld\n", compute());
    return 0;
}
