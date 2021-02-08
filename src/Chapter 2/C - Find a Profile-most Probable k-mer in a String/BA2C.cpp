#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

unordered_map<char, int> nucleotide_to_number{{'A', 0},
                                              {'C', 1},
                                              {'G', 2},
                                              {'T', 3}};

unordered_map<int, char> number_to_nucleotide{{0, 'A'},
                                              {1, 'C'},
                                              {2, 'G'},
                                              {3, 'T'}};


float probability_calc(string text, vector<vector<float>> &prob_mat) {
    float probability = 1.0;
    for (int i = 0; i < text.size(); i++) {
        probability = probability * prob_mat[nucleotide_to_number[text[i]]][i];
    }
    return probability;
}

string most_probable_string(string dna, vector<vector<float>> &prob_mat, int length) {
    string answer;
    float prob_max = -1;
    for (int i = 0; i < dna.size() - length; i++) {
        string ss = dna.substr(i, length);
        float prob = probability_calc(ss, prob_mat);
        if (prob > prob_max) {
            prob_max = prob;
            answer = ss;
        }
    }
    return answer;
}

int main() {
    string dna;
    cin >> dna;
    int n;
    cin >> n;
    vector<vector<float>> matrix(4, vector<float>(n, 0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    cout << most_probable_string(dna, matrix, n);
    return 0;
}