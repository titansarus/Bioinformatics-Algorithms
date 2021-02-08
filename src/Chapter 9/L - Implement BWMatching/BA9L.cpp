#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>

using namespace std;


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

int *make_count_array(const string &sorted) {
    int *count_arr = (int *) malloc(256 * sizeof(int));
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
    return count_arr;
}


vector<int> last_to_first(const string &last_col, map<char, int> &char_to_int_map, int **occ, int *count_arr) {
    vector<int> lf;
    for (int i = 0; i < last_col.size(); i++) {
        if (last_col.at(i) == '$') {
            lf.push_back(0);
        } else {
            lf.push_back(count_arr[int(last_col.at(i))] + occ[i][char_to_int_map[last_col.at(i)]] - 1);
        }
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

map<char, int> make_char_to_int_map(int *count_arr) {
    int counter = 0;
    map<char, int> char_to_int_map;
    for (int i = 0; i < 256; i++) {
        if (count_arr[i] != 0 || char(i) == '$') {
            char_to_int_map[char(i)] = counter++;
        }
    }
    return char_to_int_map;
}

vector<string> tokenize_by_space(const string &i) {
    string input = i;
    vector<string> answer;
    string delimiter = " ";
    size_t finder;
    while ((finder = input.find(delimiter)) != input.npos) {
        answer.push_back((input.substr(0, finder)));
        input.erase(0, finder + delimiter.length());
    }

    answer.push_back((input));
    return answer;


}

bool does_contain(const string &last_column, char symbol, int top, int bottom) {
    for (int i = top; i <= bottom; i++) {
        if (last_column[i] == symbol) {
            return true;
        }
    }
    return false;
}

int top_index(const string &last_column, char symbol, int top, int bottom) {
    for (int i = top; i <= bottom; i++) {
        if (last_column[i] == symbol) {
            return i;
        }
    }
    return -1;
}

int bottom_index(const string &last_column, char symbol, int top, int bottom) {
    for (int i = bottom; i >= top; i--) {
        if (last_column[i] == symbol) {
            return i;
        }
    }
    return -1;
}

int bwmatching(const string &last_column, string pattern, const vector<int> &lf) {
    int top = 0;
    int bottom = last_column.size() - 1;
    while (top <= bottom) {
        if (!pattern.empty()) {
            char symbol = pattern[pattern.length() - 1];
            pattern.pop_back();
            if (does_contain(last_column, symbol, top, bottom)) {
                int ti = top_index(last_column, symbol, top, bottom);
                int bi = bottom_index(last_column, symbol, top, bottom);
                top = lf[ti];
                bottom = lf[bi];
            } else {
                return 0;
            }


        } else {
            return bottom - top + 1;
        }
    }
    return -1;

}

int main() {


    string last_column;
    getline(cin, last_column);
    string temp;
    getline(cin, temp);
    istringstream is(temp);
    vector<string> patterns{istream_iterator<string>{is}, istream_iterator<string>{}};


    string first_column = sort_string(last_column);
    int *count_arr = make_count_array(first_column);
    map<char, int> char_to_int_map = make_char_to_int_map(count_arr);
    int **occ = make_occ(last_column, char_to_int_map);
    vector<int> lf = last_to_first(last_column, char_to_int_map, occ, count_arr);


    for (auto pattern : patterns) {
        cout << bwmatching(last_column, pattern, lf) <<" ";
    }

    return 0;
}