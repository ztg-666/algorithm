#include <iostream>
#include <vector>
using namespace std;

// 定义最大节点数
const int MAXN = 1000001;

// 并查集类
class UnionFind {
private:
    int father[MAXN]{};  // 每个节点的父节点
    int size_[MAXN]{};   // 每个集合的大小
    int stack[MAXN]{};   // 用于路径压缩的栈
    int n = 0;           // 当前节点数

public:
    // 初始化并查集
    void build(int _n) {
        n = _n;
        for (int i = 0; i <= n; ++i) {
            father[i] = i;  // 每个节点的父节点初始化为其自身
            size_[i] = 1;   // 每个节点的集合大小初始化为1
        }
    }

    // 查找节点的根节点，并进行路径压缩
    int find(int i) {
        int stack_size = 0;
        // 找到根节点，并将路径上的节点记录到栈中
        while (i != father[i]) {
            stack[stack_size++] = i;
            i = father[i];
        }
        // 将路径上的所有节点直接连接到根节点
        while (stack_size > 0) {
            father[stack[--stack_size]] = i;
        }
        return i;
    }

    // 判断两个节点是否属于同一个集合
    bool isSameSet(int x, int y) {
        return find(x) == find(y);
    }

    // 合并两个集合
    void unite(int x, int y) {  // 避免使用union关键字
        int fx = find(x);
        int fy = find(y);
        if (fx != fy) {
            // 将小集合合并到大集合中
            if (size_[fx] >= size_[fy]) {
                size_[fx] += size_[fy];
                father[fy] = fx;
            } else {
                size_[fy] += size_[fx];
                father[fx] = fy;
            }
        }
    }
};
// 并查集模版(牛客)
// 路径压缩 + 小挂大
// 测试链接 : https://www.nowcoder.com/practice/e7ed657974934a30b2010046536a5372
int main() {
    // 关闭C++与C标准库的流同步，提高输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    UnionFind uf;

    // 循环读取输入
    while (cin >> n) {
        uf.build(n);  // 初始化并查集
        cin >> m;
        // 处理m个操作
        while (m--) {
            int op, x, y;
            cin >> op >> x >> y;
            if (op == 1) {
                // 查询两个节点是否属于同一个集合
                cout << (uf.isSameSet(x, y) ? "Yes" : "No") << endl;
            } else {
                // 合并两个集合
                uf.unite(x, y);
            }
        }
    }
    return 0;
}
