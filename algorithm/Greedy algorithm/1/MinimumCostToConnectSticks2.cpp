#include <iostream>
#include <cstdio>
using namespace std;

// 连接棒材的最低费用
// 你有一些长度为正整数的棍子
// 这些长度以数组sticks的形式给出
// sticks[i]是第i个木棍的长度
// 你可以通过支付x+y的成本将任意两个长度为x和y的棍子连接成一个棍子
// 你必须连接所有的棍子，直到剩下一个棍子
// 返回以这种方式将所有给定的棍子连接成一个棍子的最小成本
// 测试链接 : https://www.luogu.com.cn/problem/P1090

const int MAXN = 10001;       // 最大木棍数量限制
int heap[MAXN];               // 手写堆的存储数组
int size_ = 0;                // 当前堆大小（使用下划线避免与标准库命名冲突）

// 交换堆中两个位置的元素
// 参数：i, j - 需要交换的节点索引
void swap(int i, int j) {
    int tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

// 向堆中添加元素（上浮调整）
// 参数：x - 要插入的新元素
void add(int x) {
    heap[size_] = x;          // 将新元素放在堆末尾
    int i = size_++;          // 记录当前位置并增加堆大小

    // 上浮过程：与父节点比较，直到满足堆性质或到达根节点
    while (heap[i] < heap[(i - 1) / 2]) {
        swap(i, (i - 1) / 2); // 与父节点交换
        i = (i - 1) / 2;      // 更新当前位置
    }
}

// 弹出堆顶元素（最小值）并进行下沉调整
// 返回值：堆顶元素值
int pop() {
    int ans = heap[0];        // 保存堆顶元素（最小值）
    swap(0, --size_);         // 将堆尾元素换到堆顶，并减少堆大小

    int i = 0;                // 从根节点开始下沉
    int l = 1;                // 左子节点初始位置
    int best;                 // 记录最小子节点位置

    // 下沉过程：与子节点比较，直到满足堆性质或到达叶子节点
    while (l < size_) {
        // 选择较小的子节点（如果存在右子节点且更小）
        best = (l + 1 < size_ && heap[l + 1] < heap[l]) ? l + 1 : l;
        // 比较当前节点与最小子节点
        best = heap[best] < heap[i] ? best : i;

        if (best == i) break; // 当前节点已是最小，停止下沉

        swap(i, best);        // 与较小子节点交换
        i = best;             // 更新当前位置
        l = i * 2 + 1;        // 计算新的左子节点位置
    }
    return ans;
}

// 快速读取整数（优化输入速度）
inline int readInt() {
    int x = 0;
    char c = getchar();
    // 跳过空白字符
    while (c <= ' ') c = getchar();
    // 逐位读取数字字符
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0'); // 累加计算整数值
        c = getchar();
    }
    return x;
}

int main() {
    // 关闭同步以加速C++标准输入输出流
    ios::sync_with_stdio(false);
    cin.tie(0);  // 解除cin与cout的绑定

    int n;
    // 循环处理多个测试用例（直到文件结束）
    while (scanf("%d", &n) != EOF) {
        size_ = 0; // 重置堆大小

        // 读取所有木棍并构建初始小根堆
        for (int i = 0; i < n; ++i) {
            add(readInt()); // 读取并插入堆中
        }

        long long total = 0; // 使用64位整数防止大数溢出

        // 贪心算法核心：每次合并最短的两根木棍
        while (size_ > 1) {
            // 取出当前最短的两根木棍
            int a = pop();
            int b = pop();

            // 计算合并成本并累加
            int sum = a + b;
            total += sum;

            // 将新木棍重新插入堆中
            add(sum);
        }

        // 输出当前测试用例的结果
        cout << total << '\n';
    }
    
    return 0;
}
