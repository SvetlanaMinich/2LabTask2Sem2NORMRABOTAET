#include <iostream>
#include "string.h"
String a = "1230";


int main()
{
    a.push_back(1);
    a += "1230\n";
   
    std::cout << a.get_size();

}