#include <iostream>
#include <stack>

class ReverseStackWithRecursive {
public:
    // 递归函数，用于逆序栈
    static void reverse(std::stack<int>& stack) {
        // 如果栈为空，直接返回
        if (stack.empty()) {
            return;
        }
        // 移除栈底元素，并将其存储在num中
        int num = bottomOut(stack);
        // 递归调用reverse函数，继续处理剩余的栈
        reverse(stack);
        // 将之前移除的栈底元素压入栈顶
        stack.push(num);
    }

    // 栈底元素移除掉，上面的元素盖下来
    // 返回移除掉的栈底元素
    static int bottomOut(std::stack<int>& stack) {
        // 获取栈顶元素并存储在ans中
        int ans = stack.top();
        // 移除栈顶元素
        stack.pop();
        // 如果栈为空，说明ans就是原来的栈底元素，直接返回
        if (stack.empty()) {
            return ans;
        } else {
            // 递归调用bottomOut函数，继续处理剩余的栈
            int last = bottomOut(stack);
            // 将之前移除的栈顶元素压入栈顶
            stack.push(ans);
            // 返回最后移除的栈底元素
            return last;
        }
    }
};

int main() {
    // 创建一个栈对象
    std::stack<int> stack;
    // 向栈中压入元素
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);
    // 调用reverse函数逆序栈
    ReverseStackWithRecursive::reverse(stack);
    // 输出栈中的元素，此时栈已经逆序
    while (!stack.empty()) {
        std::cout << stack.top() << std::endl;
        stack.pop();
    }
    return 0;
}
