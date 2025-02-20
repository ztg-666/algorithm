// 定义一个用于维护中位数的类
class MedianHolder {
private:
    // 最大堆，存储较小的一半元素
    std::priority_queue<int> maxHeap;
    // 最小堆，存储较大的一半元素
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    // 调整两个堆的大小，确保它们的大小差不超过1
    void modifyTwoHeapsSize() {
        // 如果最大堆的大小比最小堆大2，则将最大堆的堆顶元素移到最小堆
        if (maxHeap.size() == minHeap.size() + 2) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
        // 如果最小堆的大小比最大堆大2，则将最小堆的堆顶元素移到最大堆
        if (minHeap.size() == maxHeap.size() + 2) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

public:
    // 添加一个新的数字到数据结构中
    void addNumber(int num) {
        // 如果最大堆为空或者新数字小于等于最大堆的堆顶元素，则将新数字加入最大堆
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            // 否则，将新数字加入最小堆
            minHeap.push(num);
        }
        // 调整两个堆的大小，确保它们的大小平衡
        modifyTwoHeapsSize();
    }

    // 获取当前数据结构中的中位数
    int getMedian() {
        int maxHeapSize = maxHeap.size();
        int minHeapSize = minHeap.size();
        // 如果没有添加任何数字，抛出异常
        if (maxHeapSize + minHeapSize == 0) {
            throw std::runtime_error("No numbers are added");
        }
        int maxHeapHead = maxHeap.top();
        int minHeapHead = minHeap.top();
        // 如果总元素个数是偶数，返回两个堆顶元素的平均值
        if (((maxHeapSize + minHeapSize) & 1) == 0) {
            return (maxHeapHead + minHeapHead) / 2;
        }
        // 如果总元素个数是奇数，返回较大的堆顶元素
        return maxHeapSize > minHeapSize ? maxHeapHead : minHeapHead;
    }
};
