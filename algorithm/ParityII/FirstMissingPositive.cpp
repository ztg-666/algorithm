#include <vector>
#include <iostream>

// 缺失的第一个正数
// 给你一个未排序的整数数组 nums ，请你找出其中没有出现的最小的正整数。
// 请你实现时间复杂度为 O(n) 并且只使用常数级别额外空间的解决方案。
// 测试链接 : https://leetcode.cn/problems/first-missing-positive/

class FirstMissingPositive {
public:
    // 时间复杂度O(n)，额外空间复杂度O(1)
    static int firstMissingPositive(std::vector<int>& arr) {
        // l的左边，都是做到i位置上放着i+1的区域
        // 永远盯着l位置的数字看，看能不能扩充(l++)
        int l = 0;
        // [r....]垃圾区
        // 最好的状况下，认为1~r是可以收集全的，每个数字收集1个，不能有垃圾
        // 有垃圾呢？预期就会变差(r--)
        int r = arr.size();
        while (l < r) {
            if (arr[l] == l + 1) {
                // 如果当前位置的值正确，则移动左指针
                l++;
            } else if (arr[l] <= l || arr[l] > r || arr[arr[l] - 1] == arr[l]) {
                // 如果当前位置的值不在有效范围内或重复，则将该值移到垃圾区
                swap(arr, l, --r);
            } else {
                // 将当前位置的值放到正确的位置
                swap(arr, l, arr[l] - 1);
            }
        }
        // 返回缺失的第一个正整数
        return l + 1;
    }

private:
    // 交换数组中的两个元素
    static void swap(std::vector<int>& arr, int i, int j) {
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
};

int main() {
    // 示例输入
    std::vector<int> nums = {3, 4, -1, 1};

    // 调用firstMissingPositive函数并输出结果
    int result = FirstMissingPositive::firstMissingPositive(nums);
    std::cout << "The first missing positive integer is: " << result << std::endl;

    return 0;
}
