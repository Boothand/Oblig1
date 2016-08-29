#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    ifstream stream;
    string str;
    stream.open("test.txt");
    
    if (stream.is_open())
    {
        getline(stream, str);
        cout << str << endl;
    }
    return 0;
}
