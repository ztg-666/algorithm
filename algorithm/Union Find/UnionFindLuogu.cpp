#include <iostream>
using namespace std;

const int MAXN = 200001;  // 定义最大节点数
int father[MAXN];         // 并查集的父节点数组
int n;                    // 节点数量

// 初始化并查集
void build() {
    for (int i = 0; i <= n; ++i) {
        father[i] = i;  // 每个节点的父节点初始化为其自身
    }
}

// 查找节点 x 的根节点，并进行路径压缩
int find(int x) {
    int root = x;
    // 找到根节点
    while (root != father[root]) {
        root = father[root];
    }
    // 路径压缩
    while (x != root) {
        int next = father[x];
        father[x] = root;
        x = next;
    }
    return root;
}

// 判断节点 x 和 y 是否属于同一集合
bool isSameSet(int x, int y) {
    return find(x) == find(y);  // 如果根节点相同，则属于同一集合
}

// 合并节点 x 和 y 所在的集合
void unite(int x, int y) {
    father[find(x)] = find(y);  // 将 x 的根节点的父节点设为 y 的根节点
}
// 并查集模版(洛谷)
// 本实现用递归函数实现路径压缩，而且省掉了小挂大的优化，一般情况下可以省略
// 测试链接 : https://www.luogu.com.cn/problem/P3367
int main() {
    // 添加 IO 优化语句
    ios::sync_with_stdio(false);  // 关闭与 C 标准库的同步，提高 cin/cout 速度
    cin.tie(0);                   // 解除 cin 与 cout 的绑定，提高输入输出效率
    cout.tie(0);                  // 可选优化，部分编译器有效

    int m;  // 操作数量
    // 循环读取输入，直到输入结束
    while (cin >> n >> m) {  
        build();  // 初始化并查集
        while (m--) {  // 处理 m 次操作
            int z, x, y;
            cin >> z >> x >> y;  // 读取操作类型和两个节点

            if (z == 1) {
                unite(x, y);  // 合并操作
            } else {
                // 查询操作，输出是否属于同一集合
                cout << (isSameSet(x, y) ? "Y" : "N") << '\n';  // 使用 '\n' 代替 endl 提高效率
            }
        }
    }
    return 0;
}
