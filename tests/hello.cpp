// int hello(int, int);

#include <iostream>
#include <string>
using namespace std;

extern "C"
{
    string hello(int a, int b)
    {
        cout << a + b << endl;
        return string("good morning");
    }
}
