#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int count_arr[256];


vector<int> make_suffix_array(const string &input) {
    int len = input.size();
    int counter = 1;
    map<string, int> suffixes;
    for (int i = len - 1; i >= 0; i--) {
        string suffix = input.substr(i, counter);
        suffixes[suffix] = i;
        counter++;

    }
    vector<int> answer;
    for (const auto &ele : suffixes) {
        answer.push_back(ele.second);
    }
    return answer;
}


string sort_string(const string &input) {
    string output = input;
    sort(output.begin(), output.end());
    return output;
}

void make_count_array(const string &sorted) {
    int a = sorted.length();
    int i = a - 1;
    a--;
    while (i > 0) {
        if (sorted.at(i) == sorted.at(i - 1)) {
            a--;
        } else {
            count_arr[int(sorted.at(i))] = a--;
        }
        i--;
    }
}


vector<int> last_to_first(const string &last_col, map<char, int> &char_to_int_map, int **occ) {
    vector<int> lf;
    for (int i = 0; i < last_col.size(); i++) {
        lf.push_back(count_arr[int(last_col.at(i))] + occ[i][char_to_int_map[last_col.at(i)]] -1);
    }
    return lf;
}

int **make_occ(string last_column, map<char, int> char_to_int_map) {
    int **occ = (int **) malloc(last_column.size() * sizeof(int *));
    for (int i = 0; i < last_column.size(); i++) {
        occ[i] = (int *) malloc(char_to_int_map.size() * sizeof(int));
    }
    for (int i = 0; i < last_column.size(); i++) {
        for (int j = 0; j < char_to_int_map.size(); j++) {
            occ[i][j] = 0;
        }
    }

    for (int i = 0; i < last_column.size(); i++) {
        if (i != 0) {
            memcpy(occ[i], occ[i - 1], char_to_int_map.size() * sizeof(int));
        }
        occ[i][char_to_int_map[last_column[i]]]++;
    }
    return occ;
}

int main() {
    map<char, int> char_to_int_map;
    string last_column;
    cin >> last_column;
    int a;
    cin >> a;
    string first_column = sort_string(last_column);
    cout << first_column << endl;
    make_count_array(first_column);

    int counter = 0;
    for (int i = 0; i < 256; i++) {
        if (count_arr[i] != 0 || char(i) == '$') {
            char_to_int_map[char(i)] = counter++;
        }
    }


    int **occ = make_occ(last_column, char_to_int_map);
    auto lf = last_to_first(last_column, char_to_int_map, occ);

    for (int i = 0; i < lf.size(); i++) {
        cout << i << "\t";
    }
    cout << endl;
    for (int i = 0; i < lf.size(); i++) {
        cout << lf[i] << "\t";
    }
    cout << endl;

    cout << lf[a];

    return 0;
}