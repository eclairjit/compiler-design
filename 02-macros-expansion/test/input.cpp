#include <iostream>

// Define a macro
#define PI 3.14159

// Define a functional macro
#define SQUARE(x) ((x) * (x))

// Conditional compilation


int main() {
    std::cout << "Value of PI: " << PI << std::endl;
    std::cout << "Square of 5: " << SQUARE(5) << std::endl;

#ifdef PI
    std::cout << "Advanced features are enabled." << std::endl;
#else
    std::cout << "Advanced features are not enabled." << std::endl;
#endif

    return 0;
}