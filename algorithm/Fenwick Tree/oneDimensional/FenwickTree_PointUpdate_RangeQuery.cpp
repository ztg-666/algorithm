#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 500001; // 定义树状数组的最大大小

// 注意下标一定从1开始，不从0开始
vector<int> tree(MAXN); // 树状数组，用于存储前缀和信息

int n, m; // n表示数组的大小，m表示操作的数量

// 得到i最右侧的1的状态
// 其他位都是0
int lowbit(int i) {
    return i & -i; // 返回i的二进制表示中最右边的1的位置
}

// 在树状数组中增加值v到位置i
void add(int i, int v) {
    while (i <= n) { // 循环直到超出数组范围
        tree[i] += v; // 在当前位置增加值v
        i += lowbit(i); // 移动到下一个需要更新的位置
    }
}

// 返回1~i范围累加和
int sum(int i) {
    int ans = 0; // 初始化累加和为0
    while (i > 0) { // 循环直到i为0
        ans += tree[i]; // 累加当前位置的值
        i -= lowbit(i); // 移动到前一个需要累加的位置
    }
    return ans; // 返回累加和
}

// 返回l到r范围的累加和
int range(int l, int r) {
    return sum(r) - sum(l - 1); // 使用sum函数计算范围和
}

int main() {
    ios::sync_with_stdio(false); // 关闭C风格的输入输出流同步
    cin.tie(0); // 解绑cin和cout，提高输入输出速度

    cin >> n >> m; // 读取数组大小n和操作数量m

    // 初始化树状数组，读取初始数组的值并进行单点增加操作
    for (int i = 1, v; i <= n; i++) {
        cin >> v; // 读取数组的第i个元素
        add(i, v); // 将值v增加到树状数组的第i个位置
    }

    // 处理m个操作
    for (int i = 1, a, b, c; i <= m; i++) {
        cin >> a >> b >> c; // 读取操作类型a和操作参数b, c
        if (a == 1) {
            add(b, c); // 如果是单点增加操作，增加c到位置b
        } else {
            cout << range(b, c) << endl; // 如果是范围查询操作，输出b到c的累加和
        }
    }

    return 0; // 程序结束
}
