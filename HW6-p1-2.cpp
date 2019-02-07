#include <iostream>
#include "set-v3.h"

using namespace std;


int main() {

    set<int> s;
    s.insert(5);
    s.insert(4);
    s.insert(7);
    s.print(3);
    s.erase(7);
    cout<<endl;
    s.print(2);
    cout<<endl;
    cout<< s.count(7) <<endl;
    set<int>s2;
    s2 = s;
    s2.print(2);

    return 0;
}