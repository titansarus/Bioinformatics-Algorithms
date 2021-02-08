#include <iostream>
#include <map>
#include <vector>
#include <string>


using namespace std;

int main() {

    string input;
    cin >> input;
    int len = input.size();
    int counter = 1;
    map<string, int> suffixes;
    for (int i = len - 1; i >= 0; i--) {
        string suffix = input.substr(i, counter);
        suffixes[suffix] = i;
        counter++;

    }

    counter =0;

    for (auto ele : suffixes) {
        cout<< ele.second << ((counter++ !=suffixes.size()-1)?(", "):(" "));
    }

    return 0;
}