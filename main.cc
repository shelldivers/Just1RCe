#include <iostream>
using namespace std;

int main() {
    int x = 42, y = 0;
    int result = x / y;  // Intentional division by zero to trigger cppcheck warning

    cout<<"Result is: "<<result<<endl;  // No space around operators (cpplint issue)

    int* ptr = new int(5);
    return 0;  // Memory leak: `ptr` is not deleted (cppcheck warning)
}
