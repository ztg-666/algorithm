#include<bits/stdc++.h>
using namespace std;

// 定义一个类 MedianFinder，用于动态查找中位数
class MedianFinder {
private:
    std::priority_queue<int> maxHeap; // 大顶堆，存储较小的一半数字
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap; // 小顶堆，存储较大的一半数字

public:
    MedianFinder() {
        // 构造函数，初始化两个堆
    }

    // 向数据流中添加一个数字
    void addNum(int num) {
        // 根据当前数字与大顶堆堆顶元素的关系，决定将数字放入哪个堆
        if (maxHeap.empty() || maxHeap.top() >= num) {
            maxHeap.push(num); // 如果大顶堆为空或数字小于等于大顶堆堆顶，则放入大顶堆
        } else {
            minHeap.push(num); // 否则放入小顶堆
        }
        balance(); // 调整堆平衡
    }

    // 查找当前数据流的中位数
    double findMedian() {
        // 如果两个堆大小相等，则中位数为两堆堆顶元素的平均值
        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        }
        // 否则，中位数为较大堆的堆顶元素
        else {
            return maxHeap.size() > minHeap.size() ? maxHeap.top() : minHeap.top();
        }
    }

private:
    // 调整两个堆的平衡，确保它们的大小差不超过1
    void balance() {
        if (std::abs((int)maxHeap.size() - (int)minHeap.size()) == 2) { // 如果大小差为2，则需要调整
            if (maxHeap.size() > minHeap.size()) { // 如果大顶堆较大
                minHeap.push(maxHeap.top()); // 将大顶堆堆顶元素移至小顶堆
                maxHeap.pop();
            } else { // 如果小顶堆较大
                maxHeap.push(minHeap.top()); // 将小顶堆堆顶元素移至大顶堆
                minHeap.pop();
            }
        }
    }
};

int main() {
    MedianFinder medianFinder;
    medianFinder.addNum(1);
    medianFinder.addNum(2);
    cout << "Current median: " << medianFinder.findMedian() << endl; // 输出 1.5
    medianFinder.addNum(3);
    cout << "Current median: " << medianFinder.findMedian() << endl; // 输出 2
    medianFinder.addNum(4);
    cout << "Current median: " << medianFinder.findMedian() << endl; // 输出 2.5
    medianFinder.addNum(5);
    cout << "Current median: " << medianFinder.findMedian() << endl; // 输出 3
    return 0;
}