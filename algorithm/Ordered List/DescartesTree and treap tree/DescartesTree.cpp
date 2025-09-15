#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>

#define LL long long

using namespace std;

// 最大数据范围：1 <= n <= 10^7
// 所以必须用静态数组，全局申请
const int MAXN = 10000001;

int arr[MAXN]; // 原始数组，1 ~ n
int ls[MAXN];  // ls[i] 表示节点 i 的左孩子下标
int rs[MAXN];  // rs[i] 表示节点 i 的右孩子下标
int sta[MAXN]; // 单调栈，用来辅助建树
int n;         // 数组大小

// 构建笛卡尔树
// 时间复杂度 O(n)
void build() {
    int top = 0; // 栈顶指针
    for (int i = 1; i <= n; i++) {
        int pos = top;
        // 单调栈：保持 arr[sta[1..pos]] 单调递增
        // 如果当前数比栈顶数小，就弹出栈顶
        while (pos > 0 && arr[sta[pos]] > arr[i]) {
            pos--;
        }
        // 如果栈里还有元素，说明 arr[sta[pos]] < arr[i]
        // 那么 i 应该作为 sta[pos] 的右孩子
        if (pos > 0) {
            rs[sta[pos]] = i;
        }
        // 如果弹过栈，说明有比 arr[i] 大的数被踢掉了
        // 那么 i 应该作为 sta[pos+1] 的左孩子
        if (pos < top) {
            ls[i] = sta[pos + 1];
        }
        // 把 i 压栈
        sta[++pos] = i;
        top = pos;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入 n 和数组
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 建树
    build();

    // 最终要求打印两个值：
    // ans1 = ⊕(i * (ls[i] + 1))，即所有节点的左孩子信息异或和
    // ans2 = ⊕(i * (rs[i] + 1))，即所有节点的右孩子信息异或和
    // 之所以 +1，是为了把 0（没有孩子）区分开来
    long long ans1 = 0, ans2 = 0;
    for (int i = 1; i <= n; i++) {
        ans1 ^= 1LL * i * (ls[i] + 1);
        ans2 ^= 1LL * i * (rs[i] + 1);
    }

    // 输出验证结果
    cout << ans1 << " " << ans2 << endl;
    return 0;
}
