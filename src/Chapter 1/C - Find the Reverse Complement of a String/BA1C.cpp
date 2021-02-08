#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <map>
#include <future>


using namespace std;
using namespace std::chrono;


unordered_map<char, char> base_complement({{'A', 'T'},
                                           {'T', 'A'},
                                           {'C', 'G'},
                                           {'G', 'C'}});

char symbols[4] = {'A', 'T', 'C', 'G'};


string reverse_complement(string nucleotide) {//From the other problem
    stringstream ss;
    string reverseString = move(nucleotide);
    reverse(reverseString.begin(), reverseString.end());
    int len = reverseString.length();
    for (int i = 0; i < len; i++) {
        ss << base_complement[reverseString.at(i)];
    }
    return ss.str();

}

unordered_set<string> generate_different_states_one_string(const string &str, int index) {
    unordered_set<string> answer({str});
    for (char symbol : symbols) {
        string newString = str;
        newString[index] = symbol;
        answer.insert(newString);
    }
    return answer;
}

unordered_set<string> generate_different_states_multiple_strings(const unordered_set<string> &strings, int index) {
    unordered_set<string> answer;
    for (const string &str: strings) {
        unordered_set<string> temp = generate_different_states_one_string(str, index);
        answer.insert(temp.begin(), temp.end());
    }
    return answer;
}

vector<vector<int> *> combinations(int n, int k) {
    vector<vector<int> *> answer;
    vector<bool> bitmask(n);
    fill(bitmask.end() - k, bitmask.end(), true);
    do {
        auto *element_positions = new vector<int>();
        answer.push_back(element_positions);
        for (int i = 0; i < n; i++) {
            if (bitmask[i]) {
                element_positions->push_back(i);
            }
        }


    } while (next_permutation(bitmask.begin(), bitmask.end()));
    return answer;
}

unordered_set<string> find_all_mismatch_strings(const string &str, int valid_mismatch_count) {
    int len = str.length();
    vector<vector<int> *> index_container = combinations(len, valid_mismatch_count);
    unordered_set<string> answer;
    for (vector<int> *indexes : index_container) {
        auto *base_strings = new unordered_set<string>;
        base_strings->clear();
        base_strings->insert(str);
        for (int index : *indexes) {
            auto temp = generate_different_states_multiple_strings(*base_strings, index);
            base_strings->insert(temp.begin(), temp.end());
        }
        answer.insert(base_strings->begin(), base_strings->end());
    }
    return answer;
}

unordered_map<string, int> find_count_map(const string &str, int n, int k) {
    unordered_map<string, int> answer;
    int len = str.length();
    for (int i = 0; i < len - n + 1; i++) {
        string substring = str.substr(i, n);
        unordered_set<string> mismatch_strings = find_all_mismatch_strings(substring, k);
        for (const string &s : mismatch_strings) {
            auto searched = answer.find(s);
            if (searched != answer.end()) {
                searched->second++;
            } else {
                answer[s] = 1;
            }
        }
    }
    return answer;
}

void make_or_update(unordered_map<string, int> &answer, const pair<const string, int> &element) {
    auto searched = answer.find(element.first);
    if (searched != answer.end()) {
        searched->second += element.second;
    } else {
        answer[element.first] = element.second;
    }
}


unordered_map<string, int>
combine_two_count_map(const unordered_map<string, int> &count_map1, const unordered_map<string, int> &count_map2) {
    unordered_map<string, int> answer;
    for (const auto &element : count_map1) {
        make_or_update(answer, element);
    }
    for (const auto &element : count_map2) {
        make_or_update(answer, element);
    }
    return answer;
}


vector<string> find_max_kmer_from_count_map(const unordered_map<string, int> &count_map) {
    vector<string> answer;
    int max = 0;
    for (const auto &element : count_map) {
        if (element.second < max) {
            continue;
        } else if (element.second > max) {
            answer.clear();
            max = element.second;
        }
        answer.push_back(element.first);
    }
    return answer;
}


int main() {
    string text;
    int k, d;
    cin >> text;
    cin >> k >> d;

    //multithreading
    auto future = async(find_count_map, text, k, d);
    auto future2 = async(find_count_map, reverse_complement(text), k, d);
    auto count_map1 = future.get();
    auto count_map2 = future2.get();


    auto combined_map = combine_two_count_map(count_map1, count_map2);
    vector<string> answer = find_max_kmer_from_count_map(combined_map);


    for (const string &str : answer) {
        cout << str << " ";
    }
}