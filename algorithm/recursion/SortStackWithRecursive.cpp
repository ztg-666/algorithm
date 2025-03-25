#include <iostream>
#include <stack>
#include <cstdlib>
#include <ctime>

class SortStackWithRecursive {
public:
    // 对栈进行排序，使用递归方法
    static void sort(std::stack<int>& stack) {
        int deep = _deep(stack); // 获取栈的深度
        while (deep > 0) {
            int maxVal = max(stack, deep); // 找到栈中前deep个元素的最大值
            int k = times(stack, deep, maxVal); // 计算最大值出现的次数
            down(stack, deep, maxVal, k); // 将最大值沉底
            deep -= k; // 减少深度，继续处理剩余的栈
        }
    }

    // 返回栈的深度，不改变栈的数据状况
    static int _deep(std::stack<int>& stack) {
        if (stack.empty()) {
            return 0;
        }
        int num = stack.top(); // 保存栈顶元素
        stack.pop(); // 弹出栈顶元素
        int depth = _deep(stack) + 1; // 递归计算深度
        stack.push(num); // 恢复栈顶元素
        return depth;
    }

    // 从栈当前的顶部开始，往下数deep层，返回这deep层里的最大值
    static int max(std::stack<int>& stack, int deep) {
        if (deep == 0) {
            return INT_MIN; // 如果深度为0，返回最小整数
        }
        int num = stack.top(); // 保存栈顶元素
        stack.pop(); // 弹出栈顶元素
        int restMax = max(stack, deep - 1); // 递归找到剩余部分的最大值
        int maxVal = std::max(num, restMax); // 计算当前最大值
        stack.push(num); // 恢复栈顶元素
        return maxVal;
    }

    // 从栈当前的顶部开始，往下数deep层，已知最大值是max，返回max出现了几次，不改变栈的数据状况
    static int times(std::stack<int>& stack, int deep, int maxVal) {
        if (deep == 0) {
            return 0; // 如果深度为0，返回0
        }
        int num = stack.top(); // 保存栈顶元素
        stack.pop(); // 弹出栈顶元素
        int restTimes = times(stack, deep - 1, maxVal); // 递归计算剩余部分的最大值出现次数
        int times = restTimes + (num == maxVal ? 1 : 0); // 计算当前最大值出现次数
        stack.push(num); // 恢复栈顶元素
        return times;
    }

    // 从栈当前的顶部开始，往下数deep层，已知最大值是max，出现了k次，请把这k个最大值沉底，剩下的数据状况不变
    static void down(std::stack<int>& stack, int deep, int maxVal, int k) {
        if (deep == 0) {
            for (int i = 0; i < k; i++) {
                stack.push(maxVal); // 将最大值压入栈底
            }
        } else {
            int num = stack.top(); // 保存栈顶元素
            stack.pop(); // 弹出栈顶元素
            down(stack, deep - 1, maxVal, k); // 递归处理剩余部分
            if (num != maxVal) {
                stack.push(num); // 如果不是最大值，恢复栈顶元素
            }
        }
    }

    // 为了测试，生成随机栈
    static std::stack<int> randomStack(int n, int v) {
        std::stack<int> ans;
        for (int i = 0; i < n; i++) {
            ans.push(rand() % v); // 生成随机数并压入栈
        }
        return ans;
    }

    // 为了测试，检测栈是不是从顶到底依次有序
    static bool isSorted(std::stack<int>& stack) {
        int step = INT_MIN;
        std::stack<int> tempStack;
        while (!stack.empty()) {
            if (step > stack.top()) {
                return false; // 如果当前元素小于前一个元素，返回false
            }
            step = stack.top(); // 更新前一个元素
            tempStack.push(step); // 保存当前元素
            stack.pop(); // 弹出当前元素
        }
        // 恢复原始栈
        while (!tempStack.empty()) {
            stack.push(tempStack.top());
            tempStack.pop();
        }
        return true;
    }
};

int main() {
    std::stack<int> test;
    test.push(1);
    test.push(5);
    test.push(4);
    test.push(5);
    test.push(3);
    test.push(2);
    test.push(3);
    test.push(1);
    test.push(4);
    test.push(2);
    SortStackWithRecursive::sort(test); // 对栈进行排序
    while (!test.empty()) {
        std::cout << test.top() << std::endl; // 输出栈顶元素
        test.pop(); // 弹出栈顶元素
    }

    // 随机测试
    int N = 20;
    int V = 20;
    int testTimes = 20000;
    std::cout << "测试开始" << std::endl;
    for (int i = 0; i < testTimes; i++) {
        int n = rand() % N;
        std::stack<int> stack = SortStackWithRecursive::randomStack(n, V); // 生成随机栈
        SortStackWithRecursive::sort(stack); // 对随机栈进行排序
        if (!SortStackWithRecursive::isSorted(stack)) { // 检查栈是否有序
            std::cout << "出错了!" << std::endl;
            break;
        }
    }
    std::cout << "测试结束" << std::endl;
}
