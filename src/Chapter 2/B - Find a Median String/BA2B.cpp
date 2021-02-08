#include <iostream>
#include <vector>

using namespace std;

char nucleotides[4] = {'A', 'T', 'C', 'G'};

int hamming_distance(const string &a, const string &b) {
    int answer = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            answer++;
        }
    }
    return answer;
}

int least_hamming_distance_in_text(const string &text, const string &pattern) {
    int min = INT_MAX;
    for (int i = 0; i < text.size() - pattern.size() + 1; i++) {
        string ss = text.substr(i, pattern.size());
        int hamm_dist = hamming_distance(pattern, ss);
        if (hamm_dist < min) {
            min = hamm_dist;
        }
    }
    return min;
}

int distance_of_all_dna(vector<string> &dnas, const string &pattern) {
    int answer = 0;
    for (string dna : dnas) {
        answer += least_hamming_distance_in_text(dna, pattern);
    }
    return answer;
}

string pattern_generator(int pattern_length, int number) {
    string pattern = "";
    int counter = 0;
    while (counter != pattern_length) {
        int char_index = number % 4;
        pattern += nucleotides[char_index];
        number = number / 4;
        counter++;
    }
    return pattern;
}


string median_string(vector<string> &dnas, int pattern_length) {

    string best_pattern;
    int min = INT_MAX;
    for (int i = 0; i < (1 << pattern_length * 2); i++) {
        string patt = pattern_generator(pattern_length, i);
        int dist = distance_of_all_dna(dnas, patt);
        if (dist < min) {
            min = dist;
            best_pattern = patt;
        }

    }
    return best_pattern;

}


int main() {

    int n;
    cin >> n;
    vector<string> dnas;
    //you must input END at the end of strings to stop getting inputs.
    while (true) {
        string temp;
        cin >> temp;
        if (temp == "END") {
            break;
        }
        dnas.push_back(temp);
    }

    string answer = median_string(dnas, n);
    cout << answer;

    return 0;
}