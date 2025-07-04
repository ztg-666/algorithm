#include <iostream>
#include <algorithm>
using namespace std;

// 最长LR交替子串
// 给定一个长度为n的字符串，一开始字符串中全是'L'字符
// 有q次修改，每次指定一个位置i
// 如果i位置是'L'字符那么改成'R'字符
// 如果i位置是'R'字符那么改成'L'字符
// 如果一个子串是两种字符不停交替出现的样子，也就是LRLR... 或者RLRL...
// 那么说这个子串是有效子串
// 每次修改后，都打印当前整个字符串中最长交替子串的长度
// 测试链接 : https://www.luogu.com.cn/problem/P6492

const int MAXN = 200001;

// 原始数组，存储序列中的每个元素（0或1）
int arr[MAXN];

// 线段树节点信息
int len[MAXN << 2];  // 每个区间内交替最长子串的长度
int pre[MAXN << 2];  // 每个区间从左端点开始的交替最长前缀长度
int suf[MAXN << 2];  // 每个区间从右端点开始的交替最长后缀长度

// 线段树向上更新函数：根据左右子节点更新当前节点
void up(int l, int r, int i) {
    // 当前区间的最长交替子串长度至少是左右子区间的最大值
    len[i] = max(len[i << 1], len[i << 1 | 1]);

    // 当前区间的前缀长度初始为左子区间的前缀长度
    pre[i] = pre[i << 1];

    // 当前区间的后缀长度初始为右子区间的后缀长度
    suf[i] = suf[i << 1 | 1];

    int mid = (l + r) >> 1;  // 区间中点
    int ln = mid - l + 1;    // 左子区间长度
    int rn = r - mid;        // 右子区间长度

    // 如果左右子区间的边界元素不同，可以合并
    if (arr[mid] != arr[mid + 1]) {
        // 更新最长交替子串长度：可能跨越左右子区间
        len[i] = max(len[i], suf[i << 1] + pre[i << 1 | 1]);

        // 如果左子区间完全是交替序列，扩展前缀长度
        if (len[i << 1] == ln) {
            pre[i] = ln + pre[i << 1 | 1];
        }

        // 如果右子区间完全是交替序列，扩展后缀长度
        if (len[i << 1 | 1] == rn) {
            suf[i] = rn + suf[i << 1];
        }
    }
}

// 建立线段树
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点：单个元素的交替长度为1
        len[i] = 1;
        pre[i] = 1;
        suf[i] = 1;
    } else {
        int mid = (l + r) >> 1;
        // 递归建立左右子树
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        // 向上更新当前节点
        up(l, r, i);
    }
}

// 翻转指定位置的值（0变1，1变0）
void reverse(int jobi, int l, int r, int i) {
    if (l == r) {
        // 到达目标位置，执行翻转操作
        arr[jobi] ^= 1;  // 异或操作实现0/1翻转
    } else {
        int mid = (l + r) >> 1;
        // 根据目标位置选择左子树或右子树
        if (jobi <= mid) {
            reverse(jobi, l, mid, i << 1);
        } else {
            reverse(jobi, mid + 1, r, i << 1 | 1);
        }
        // 更新操作后向上更新当前节点
        up(l, r, i);
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;  // n为序列长度，q为操作次数

    // 建立线段树（注意：数组arr初始化为全0）
    build(1, n, 1);

    // 处理q次操作
    for (int i = 1; i <= q; i++) {
        int index;
        cin >> index;                    // 读入要翻转的位置
        reverse(index, 1, n, 1);         // 执行翻转操作
        cout << len[1] << "\n";          // 输出当前最长交替子串长度
    }

    return 0;
}
