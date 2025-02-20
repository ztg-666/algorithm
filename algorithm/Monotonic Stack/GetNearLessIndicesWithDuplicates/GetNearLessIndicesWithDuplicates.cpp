
// 函数 getNearLess
// 功能：对于数组中的每个元素，找到其左边和右边第一个比它小的元素的索引，考虑重复值。
std::vector<std::vector<int>> getNearLess(const std::vector<int>& arr) {
    // 结果数组，每个元素是一个包含两个整数的向量，分别表示左边和右边第一个比它小的元素的索引
    std::vector<std::vector<int>> res(arr.size(), std::vector<int>(2, 0));
    // 栈，用于存储元素的索引，栈中的每个元素是一个向量，存储相同值的索引
    std::stack<std::vector<int>> stack;

    // 遍历数组中的每个元素
    for (int i = 0; i < arr.size(); i++) {
        // 当栈不为空且栈顶元素大于当前元素时，弹出栈顶元素
        while (!stack.empty() && arr[stack.top().back()] > arr[i]) {
            std::vector<int> popIs = stack.top(); // 弹出的元素索引向量
            stack.pop();
            // 计算左边第一个比弹出元素小的元素的索引
            int leftLessIndex = stack.empty() ? -1 : stack.top().back();
            // 记录结果
            for (int popi : popIs) {
                res[popi][0] = leftLessIndex;
                res[popi][1] = i; // 右边第一个比弹出元素小的元素的索引为当前元素索引
            }
        }
        // 如果栈不为空且栈顶元素等于当前元素，将当前元素索引添加到栈顶元素向量中
        if (!stack.empty() && arr[stack.top().back()] == arr[i]) {
            stack.top().push_back(i);
        } else {
            // 否则，创建一个新的向量，包含当前元素索引，并压入栈
            std::vector<int> list = {i};
            stack.push(list);
        }
    }

    // 处理栈中剩余的元素
    while (!stack.empty()) {
        std::vector<int> popIs = stack.top(); // 弹出的元素索引向量
        stack.pop();
        // 计算左边第一个比弹出元素小的元素的索引
        int leftLessIndex = stack.empty() ? -1 : stack.top().back();
        // 记录结果
        for (int popi : popIs) {
            res[popi][0] = leftLessIndex;
            res[popi][1] = -1; // 右边没有比弹出元素小的元素
        }
    }

    return res;
}
