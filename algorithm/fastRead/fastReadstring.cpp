#include<bits/stdc++.h>
using namespace std;
char s[1000005];

void fast_read() {
    int p = 0;
    char c = getchar();
    while (!isgraph(c)) c = getchar(); // 跳过空白符
    while (isgraph(c)) { // 读取到非可打印字符停止
        s[p++] = c;
        c = getchar();
    }
    s[p] = '\0';
}
