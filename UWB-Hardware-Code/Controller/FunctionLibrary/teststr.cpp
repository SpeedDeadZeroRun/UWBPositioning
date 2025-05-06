#include<iostream>
#include<string>
using namespace std;

int main() {
    string s = "00:01 1450 -78.25,00:02 2560 -98.65,00:03 1563 -56.41";
    const char *a = "00:0A";
    int head = 0;
    int tail = 0;
    string temp;
    while(tail < s.length()) {
        if(s[tail] == ',') {
            temp = temp+a+' '+s.substr(head, tail-head+1);
            head = tail+1;
        }
        tail = tail+1;
    }
    temp = temp+a+' '+s.substr(head, s.length());
    cout << s << '\n';
    cout << temp;

    return 0;
}