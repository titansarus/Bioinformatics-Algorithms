#include <iostream>
#include <string>
#include <exception>
#include <vector>
using namespace std;

int hamming_distance(string str1, string str2) {
    int len = str1.length();
    if (len != str2.length()) {
        throw bad_exception();
    }
    int answer = 0;
    for (int i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            answer++;
        }
    }
    return answer;
}

bool check_match(string pattern, string subtext, int valid_mismatches) {
    if (hamming_distance(pattern, subtext) <= valid_mismatches) {
        return true;
    }
    return false;
}

int main() {

    vector<int> indexes;


    string pattern;
    string text;
    int valid_mismatches;

    cin >> pattern;
    cin >> text;
    cin >> valid_mismatches;

    int pattern_length = pattern.length();
    for (int i =0;i<text.length() - pattern_length+1;i++)
    {
        if (check_match(pattern,text.substr(i,pattern_length), valid_mismatches))
        {
            indexes.push_back(i);
        }
    }
    cout<<"\n";
    for (int index : indexes)
    {

        cout<<index<<" ";
    }
    cout<<"\n";


    return 0;
}