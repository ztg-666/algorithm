
vector<int> getMaxWindow(const std::vector<int>& arr, int w) {
    // 如果输入数组为空，或者窗口大小小于1，或者数组长度小于窗口大小，则返回空结果
    if (arr.empty() || w < 1 || arr.size() < w) {
        return {};
    }

    // 双端队列，用于存储数组元素的索引，队列中的索引对应的数组元素是递减的
    std::deque<int> qmax;
    // 结果数组，存储每个窗口的最大值
    std::vector<int> res(arr.size() - w + 1);
    // 结果数组的索引
    int index = 0;

    // 遍历数组中的每个元素
    for (int i = 0; i < arr.size(); i++) {
        // 移除队列中所有小于当前元素的索引，保持队列的递减性质
        while (!qmax.empty() && arr[qmax.back()] <= arr[i]) {
            qmax.pop_back();
        }
        // 将当前元素的索引加入队列
        qmax.push_back(i);

        // 如果队列头部的索引已经不在当前窗口范围内，则移除它
        if (qmax.front() == i - w) {
            qmax.pop_front();
        }

        // 当遍历到第w-1个元素时，开始记录每个窗口的最大值
        if (i >= w - 1) {
            res[index++] = arr[qmax.front()];
        }
    }

    // 返回结果数组
    return res;
}
