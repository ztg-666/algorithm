
// 函数 getNearLessNoRepeat
// 功能：对于数组中的每个元素，找到其左边和右边第一个比它小的元素的索引，不考虑重复值。
std::vector<std::vector<int>> getNearLessNoRepeat(const std::vector<int>& arr) {
    // 结果数组，每个元素是一个包含两个整数的向量，分别表示左边和右边第一个比它小的元素的索引
    std::vector<std::vector<int>> res(arr.size(), std::vector<int>(2, 0));
    // 栈，用于存储元素的索引
    std::stack<int> stack;

    // 遍历数组中的每个元素
    for (int i = 0; i < arr.size(); i++) {
        // 当栈不为空且栈顶元素大于当前元素时，弹出栈顶元素
        while (!stack.empty() && arr[stack.top()] > arr[i]) {
            int popIndex = stack.top(); // 弹出的元素索引
            stack.pop();
            // 计算左边第一个比弹出元素小的元素的索引
            int leftLessIndex = stack.empty() ? -1 : stack.top();
            // 记录结果
            res[popIndex][0] = leftLessIndex;
            res[popIndex][1] = i; // 右边第一个比弹出元素小的元素的索引为当前元素索引
        }
        // 将当前元素索引压入栈
        stack.push(i);
    }

    // 处理栈中剩余的元素
    while (!stack.empty()) {
        int popIndex = stack.top(); // 弹出的元素索引
        stack.pop();
        // 计算左边第一个比弹出元素小的元素的索引
        int leftLessIndex = stack.empty() ? -1 : stack.top();
        // 记录结果
        res[popIndex][0] = leftLessIndex;
        res[popIndex][1] = -1; // 右边没有比弹出元素小的元素
    }

    return res;
}


