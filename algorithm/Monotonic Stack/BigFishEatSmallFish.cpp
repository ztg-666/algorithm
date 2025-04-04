#include <vector>
#include <stack>
#include <algorithm> // 用于std::max
#include <iostream>
using namespace std;
// 大鱼吃小鱼问题
// 给定一个数组arr，每个值代表鱼的体重
// 每一轮每条鱼都会吃掉右边离自己最近比自己体重小的鱼，每条鱼向右找只吃一条
// 但是吃鱼这件事是同时发生的，也就是同一轮在A吃掉B的同时，A也可能被别的鱼吃掉
// 如果有多条鱼在当前轮找到的是同一条小鱼，那么在这一轮，这条小鱼同时被这些大鱼吃
// 请问多少轮后，鱼的数量就固定了
// 比如 : 8 3 1 5 6 7 2 4
// 第一轮 : 8吃3；3吃1；5、6、7吃2；4没有被吃。数组剩下 8 5 6 7 4
// 第二轮 : 8吃5；5、6、7吃4。数组剩下 8 6 7
// 第三轮 : 8吃6。数组剩下 8 7
// 第四轮 : 8吃7。数组剩下 8。
// 过程结束，返回4
// 测试链接 : https://www.nowcoder.com/practice/77199defc4b74b24b8ebf6244e1793de
// 测试链接 : https://leetcode.cn/problems/steps-to-make-array-non-decreasing/

// 使用栈实现的totalSteps函数
int totalSteps(vector<int>& arr) {
    stack<pair<int, int>> st; // 栈存储<数值，操作次数>
    int ans = 0;

    // 从右向左遍历数组
    for (int i = arr.size() - 1; i >= 0; --i) {
        int curTurns = 0;

        // 维护单调递减栈
        while (!st.empty() && st.top().first < arr[i]) {
            curTurns = max(curTurns + 1, st.top().second);
            st.pop();
        }

        st.push({arr[i], curTurns}); // 压入当前元素
        ans = max(ans, curTurns);    // 更新最大值
    }
    return ans;
}

const int MAXN = 100001;    // 最大数据量
int arr[MAXN];              // 静态数组存储输入数据
int stk[MAXN][2];           // 二维静态数组模拟栈 [0]存数值 [1]存轮次
int r;                      // 栈顶指针

// 使用静态数组实现的turns函数
int turns(int n) {
    r = 0;                  // 重置栈顶指针
    int ans = 0;
    for (int i = n - 1; i >= 0; --i) { // 从右向左遍历
        int curTurns = 0;
        // 维护单调递减栈
        while (r > 0 && stk[r - 1][0] < arr[i]) {
            curTurns = max(curTurns + 1, stk[--r][1]);
        }
        // 压入栈顶
        stk[r][0] = arr[i];
        stk[r][1] = curTurns;
        ++r;                // 栈指针后移
        ans = max(ans, curTurns);
    }
    return ans;
}


int main() {
    // 测试数据
    vector<int> testArr = {5, 3, 4, 4, 7, 3, 6, 11, 8, 5, 11};
    int n = testArr.size();

    // 将测试数据复制到静态数组中
    for (int i = 0; i < n; ++i) {
        arr[i] = testArr[i];
    }

    // 调用totalSteps函数
    int result1 = totalSteps(testArr);
    cout << "Result from totalSteps: " << result1 << endl;

    // 调用turns函数
    int result2 = turns(n);
    cout << "Result from turns: " << result2 << endl;

    return 0;
}
