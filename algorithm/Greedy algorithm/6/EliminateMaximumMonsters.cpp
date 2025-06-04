#include <vector>
#include <algorithm>

// 消灭怪物的最大数量
// 你正在玩一款电子游戏，在游戏中你需要保护城市免受怪物侵袭
// 给定两个大小为n的整数数组dist、speed
// 其中dist[i]是第i个怪物与城市的初始距离
// 其中speed[i]是第i个怪物的速度
// 你有一种武器，一旦充满电，就可以消灭一个怪物，但是，武器需要1的时间才能充电完成
// 武器在游戏开始时是充满电的状态，怪物从0时刻开始移动，一旦任何怪物到达城市，就输掉了这场游戏
// 如果某个怪物恰好在某一分钟开始时到达城市，这也会被视为输掉游戏
// 返回在你输掉游戏前可以消灭的怪物的最大数量，如果消灭所有怪兽了返回n
// 测试链接 : https://leetcode.cn/problems/eliminate-maximum-number-of-monsters/

using namespace std;

int eliminateMaximum(vector<int>& dist, vector<int>& speed) {
    int n = dist.size();
    vector<int> time(n);
    for (int i = 0; i < n; i++) {
        // 计算到达时间（向上取整）
        time[i] = (dist[i] + speed[i] - 1) / speed[i];
    }
    sort(time.begin(), time.end());
    for (int i = 0; i < n; i++) {
        // 检查当前时刻能否击杀怪物
        if (time[i] <= i) {
            return i;
        }
    }
    return n;
}
