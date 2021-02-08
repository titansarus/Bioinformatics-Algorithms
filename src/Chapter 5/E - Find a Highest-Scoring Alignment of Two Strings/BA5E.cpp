#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

#define MAX(...) std::max({__VA_ARGS__})


const float tol = 0.000001;


void print_lcs_from_aligned(vector<char> a, vector<char> b) {
    cout << "\n";
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == b[i] && a[i] != '-') {

            cout << a[i];

        }
    }
    cout << "\n";
}

int BLOSUM62[20][20] = {4, 0, -2, -1, -2, 0, -2, -1, -1, -1, -1, -2, -1, -1, -1, 1, 0, 0, -3, -2,
                        0, 9, -3, -4, -2, -3, -3, -1, -3, -1, -1, -3, -3, -3, -3, -1, -1, -1, -2, -2,
                        -2, -3, 6, 2, -3, -1, -1, -3, -1, -4, -3, 1, -1, 0, -2, 0, -1, -3, -4, -3,
                        -1, -4, 2, 5, -3, -2, 0, -3, 1, -3, -2, 0, -1, 2, 0, 0, -1, -2, -3, -2,
                        -2, -2, -3, -3, 6, -3, -1, 0, -3, 0, 0, -3, -4, -3, -3, -2, -2, -1, 1, 3,
                        0, -3, -1, -2, -3, 6, -2, -4, -2, -4, -3, 0, -2, -2, -2, 0, -2, -3, -2, -3,
                        -2, -3, -1, 0, -1, -2, 8, -3, -1, -3, -2, 1, -2, 0, 0, -1, -2, -3, -2, 2,
                        -1, -1, -3, -3, 0, -4, -3, 4, -3, 2, 1, -3, -3, -3, -3, -2, -1, 3, -3, -1,
                        -1, -3, -1, 1, -3, -2, -1, -3, 5, -2, -1, 0, -1, 1, 2, 0, -1, -2, -3, -2,
                        -1, -1, -4, -3, 0, -4, -3, 2, -2, 4, 2, -3, -3, -2, -2, -2, -1, 1, -2, -1,
                        -1, -1, -3, -2, 0, -3, -2, 1, -1, 2, 5, -2, -2, 0, -1, -1, -1, 1, -1, -1,
                        -2, -3, 1, 0, -3, 0, 1, -3, 0, -3, -2, 6, -2, 0, 0, 1, 0, -3, -4, -2,
                        -1, -3, -1, -1, -4, -2, -2, -3, -1, -3, -2, -2, 7, -1, -2, -1, -1, -2, -4, -3,
                        -1, -3, 0, 2, -3, -2, 0, -3, 1, -2, 0, 0, -1, 5, 1, 0, -1, -2, -2, -1,
                        -1, -3, -2, 0, -3, -2, 0, -3, 2, -2, -1, 0, -2, 1, 5, -1, -1, -3, -3, -2,
                        1, -1, 0, 0, -2, 0, -1, -2, 0, -2, -1, 1, -1, 0, -1, 4, 1, -2, -3, -2,
                        0, -1, -1, -1, -2, -2, -2, -1, -1, -1, -1, 0, -1, -1, -1, 1, 5, 0, -2, -2,
                        0, -1, -3, -2, -1, -3, -3, 3, -2, 1, 1, -3, -2, -2, -3, -2, 0, 4, -3, -1,
                        -3, -2, -4, -3, 1, -2, -2, -3, -3, -2, -1, -4, -4, -2, -3, -3, -2, -3, 11, 2,
                        -2, -2, -3, -2, 3, -3, 2, -1, -2, -1, -1, -2, -3, -1, -2, -2, -2, -1, 2, 7};


unordered_map<char, int> aminoacid_to_number({{'A', 0},
                                              {'C', 1},
                                              {'D', 2},
                                              {'E', 3},
                                              {'F', 4},
                                              {'G', 5},
                                              {'H', 6},
                                              {'I', 7},
                                              {'K', 8},
                                              {'L', 9},
                                              {'M', 10},
                                              {'N', 11},
                                              {'P', 12},
                                              {'Q', 13},
                                              {'R', 14},
                                              {'S', 15},
                                              {'T', 16},
                                              {'V', 17},
                                              {'W', 18},
                                              {'Y', 19}});


void make_aligned_string_vector(vector<char> &str_i, vector<char> &str_j, const string &a, const string &b,
                                const vector<vector<string>> &arrow_matrix) {
    int i, j;
    i = a.length();
    j = b.length();
    while (i != 0 || j != 0) {
// ← ↖ ↑
        if (arrow_matrix[i][j] == "↑") {
            str_i.push_back(a[i - 1]);
            str_j.push_back('-');
            i--;
        } else if (arrow_matrix[i][j] == "←") {
            str_i.push_back('-');
            str_j.push_back(b[j - 1]);
            j--;
        } else if (arrow_matrix[i][j] == "↖") {
            str_i.push_back(a[i - 1]);
            str_j.push_back(b[j - 1]);
            i--;
            j--;
        }
    }

    reverse(str_i.begin(), str_i.end());
    reverse(str_j.begin(), str_j.end());
}

void print_output(const vector<char> &str_i, const vector<char> &str_j, int max_score) {
    cout << max_score << endl;
    for (int i = 0; i < str_i.size(); i++) {
        cout << str_i[i];
    }
    cout << "\n";
    for (int i = 0; i < str_j.size(); i++) {
        cout << str_j[i];
    }

}

void alignment_with_matrix_and_sigma(string &a, string &b, int sigma) {
    vector<vector<int>> dp_matrix(a.length() + 1);

    vector<vector<string>> arrow_matrix(a.length() + 1);
    for (int i = 0; i <= a.length(); i++) {
        vector<string> temp(b.length() + 1);
        vector<int> temp2(b.length() + 1);
        arrow_matrix[i] = temp;
        dp_matrix[i] = temp2;
    }
    dp_matrix[0][0] = 0;
    arrow_matrix[0][0] = "   ";

    for (int i = 1; i <= b.length(); i++) {
        dp_matrix[0][i] = dp_matrix[0][i - 1] - sigma;
        arrow_matrix[0][i] = "←";
    }

    for (int i = 1; i <= a.length(); i++) {
        dp_matrix[i][0] = dp_matrix[i - 1][0] - sigma;
        arrow_matrix[i][0] = "↑";

    }


    for (int i = 1; i <= a.length(); i++) {
        for (int j = 1; j <= b.length(); j++) {
            char a_char = a[i - 1];
            char b_char = b[j - 1];

            dp_matrix[i][j] = MAX(dp_matrix[i - 1][j] - sigma, dp_matrix[i][j - 1] - sigma, ((
                    dp_matrix[i - 1][j - 1] + BLOSUM62[aminoacid_to_number[a_char]][aminoacid_to_number[b_char]])));

            if ((
                    abs(dp_matrix[i][j] - (dp_matrix[i - 1][j - 1] +
                                           BLOSUM62[aminoacid_to_number[a_char]][aminoacid_to_number[b_char]])) <
                    tol)) {
                arrow_matrix[i][j] = "↖";
            } else if ((
                    abs(dp_matrix[i][j]
                        - (dp_matrix[i - 1][j] - sigma)) < tol)) {
                arrow_matrix[i][j] = "↑";
            } else if ((
                    abs(dp_matrix[i][j]
                        - (dp_matrix[i][j - 1] - sigma)) < tol)) {
                arrow_matrix[i][j] = "←";
            }
        }
    }

    vector<char> str_i;
    vector<char> str_j;
    make_aligned_string_vector(str_i, str_j, a, b, arrow_matrix);
    print_output(str_i, str_j, dp_matrix[a.length()][b.length()]);
}


int main() {
    string a, b;
    cin >> a;
    cin >> b;
    alignment_with_matrix_and_sigma(a, b, 5);
    return 0;
}