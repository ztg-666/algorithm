
auto getNextArray(string ms) {
    if(ms.length() == 1) {
        return vector{-1};
    }
    vector next(ms.length(),-1);
    next[0] = -1;
    next[1] = 0;
    int i = 2;
    int cn = 0;
    while(i < ms.length()) {
        if(ms[i - 1] == ms[cn]) {//使用i-1位置上的信息推i位置上的信息
            next[i++] = ++cn;
        }else if(cn > 0) {
            cn = next[cn];
            // 当前跳到cn位置的字符，和i-1位置的字符配不上
        }else {
            next[i++] = 0;
        }
    }
    return next;
}
int getIndexOf(string* s,string* m) {
    if(s == nullptr || m == nullptr || m->length() < 1 || s->length() < m->length()) {
        return -1;
    }
    string str1 = *s;
    string str2 = *m;
    int i1 = 0;
    int i2 = 0;
    vector<int> next = getNextArray(str2);
    while(i1 < str1.length() && i2 < str2.length()) {
        if(str1[i1] == str2[i2]) {
            i1++;
            i2++;
        }else if(next[i2] == -1) { // i2 == 0 比对内容已经无法往前跳了
            i1++;
        }else {
            i2 = next[i2];
        }
    }
    return i2 == str2.length() ? i1 - i2 : -1;
}
