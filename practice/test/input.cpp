#include <bits/stdc++.h>
using namespace std;

#define PI 3.14
#define SQUARE(x) ((x)*(x))

int main() {

    int a = 2;
    string s = "hello world";

    cout << "// this is not a comment";
    cout << "/*this is not a comment*/";

    #ifdef PI
        cout << "Pi is defined";
    #endif
        cout << "Pi is not defined";
        
    // this is a comment

    return 0;
}

/*
this is a multi-line comment
*/