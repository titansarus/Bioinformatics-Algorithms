#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

#define MAX(...) std::max({__VA_ARGS__})


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
                        -2, -2, -3, -2, 3, -3, 2, -1, -2, -1, -1, -2, -3, -1, -2, -2, -2, -1, 2, 7,};


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
                                const vector<vector<string>> &arrow_matrix_A,
                                const vector<vector<string>> &arrow_matrix_M,
                                const vector<vector<string>> &arrow_matrix_B) {
    int i, j, layer;
    layer = 0;//0: M; 1 : A; 2 : B;
    i = a.length();
    j = b.length();
    while (i != -1 && j != -1) {
        if (layer == 0) {
            str_i.push_back(a[i]);
            str_j.push_back(b[j]);

            if (arrow_matrix_M[i][j] == "0") {
                layer = 0;
            } else if (arrow_matrix_M[i][j] == "1") {
                layer = 1;
            } else if (arrow_matrix_M[i][j] == "2") {
                layer = 2;
            }
            i--;
            j--;
        } else if (layer == 1) {
            str_i.push_back('-');
            str_j.push_back(b[j]);
            if (arrow_matrix_A[i][j] == "0") {
                layer = 0;
            } else if (arrow_matrix_A[i][j] == "1") {
                layer = 1;
            } else if (arrow_matrix_A[i][j] == "2") {
                layer = 2;
            }
            j--;
        } else {
            str_i.push_back(a[i]);
            str_j.push_back('-');
            if (arrow_matrix_B[i][j] == "0") {
                layer = 0;
            } else if (arrow_matrix_B[i][j] == "1") {
                layer = 1;
            } else if (arrow_matrix_B[i][j] == "2") {
                layer = 2;
            }
            i--;
        }
    }
    while (i != -1) {
        str_i.push_back(a[i]);
        str_j.push_back('-');
        i--;
    }
    while (j != -1) {
        str_i.push_back('-');
        str_j.push_back(b[j]);
        j--;
    }

    reverse(str_i.begin(), str_i.end());
    reverse(str_j.begin(), str_j.end());
}

int calculate_score(vector<char> &str_a, vector<char> &str_b, int gap_start, int gap_extend) {
    bool is_gap = 0;
    int score = 0;
    for (int i = 0; i < str_a.size() - 1; i++) {
        if (str_a[i] == '-' || str_b[i] == '-') {
            if (!is_gap) {
                score -= gap_start;
                is_gap = true;
            } else {
                score -= gap_extend;
            }
        } else {
            is_gap = false;
            score += BLOSUM62[aminoacid_to_number[str_a[i]]][aminoacid_to_number[str_b[i]]];
        }
    }
    return score;
}

void alignment_with_matrix_and_sigma(string &a, string &b, int gap_start, int gap_extend) {
    vector<vector<int>> dp_matrix_M(a.length() + 1);
    vector<vector<int>> dp_matrix_A(a.length() + 1);
    vector<vector<int>> dp_matrix_B(a.length() + 1);

    vector<vector<string>> arrow_matrix_M(a.length() + 1);
    vector<vector<string>> arrow_matrix_A(a.length() + 1);
    vector<vector<string>> arrow_matrix_B(a.length() + 1);
    for (int i = 0; i <= a.length(); i++) {
        vector<string> temp(b.length() + 1);
        vector<string> temp5(b.length() + 1);
        vector<string> temp6(b.length() + 1);
        vector<int> temp2(b.length() + 1);
        vector<int> temp3(b.length() + 1);
        vector<int> temp4(b.length() + 1);
        arrow_matrix_A[i] = temp;
        arrow_matrix_M[i] = temp5;
        arrow_matrix_B[i] = temp6;
        dp_matrix_M[i] = temp2;
        dp_matrix_A[i] = temp3;
        dp_matrix_B[i] = temp4;
    }


    for (int i = 0; i <= a.length(); i++) {
        dp_matrix_M[i][0] = -1000000;
        dp_matrix_A[i][0] = -1000000;
        dp_matrix_B[i][0] = -gap_start - (i - 1) * gap_extend;
    }
    for (int i = 0; i <= b.length(); i++) {
        dp_matrix_M[0][i] = -1000000;
        dp_matrix_B[0][i] = -1000000;
        dp_matrix_A[0][i] = -gap_start - (i - 1) * gap_extend;

    }
    dp_matrix_M[0][0] = dp_matrix_A[0][0] = dp_matrix_B[0][0] = 0;

    for (int i = 1; i <= a.length(); i++) {
        for (int j = 1; j <= b.length(); j++) {
            char a_char = a[i];
            char b_char = b[j];
            int score = BLOSUM62[aminoacid_to_number[a_char]][aminoacid_to_number[b_char]];
            {
                dp_matrix_M[i][j] =
                        score + MAX(dp_matrix_M[i - 1][j - 1], dp_matrix_B[i - 1][j - 1], dp_matrix_A[i - 1][j - 1]);
                if (dp_matrix_M[i][j] == score + dp_matrix_M[i - 1][j - 1]) {
                    arrow_matrix_M[i][j] = "0";
                } else if (dp_matrix_M[i][j] == score + dp_matrix_A[i - 1][j - 1]) {
                    arrow_matrix_M[i][j] = "1";
                } else if (dp_matrix_M[i][j] == score + dp_matrix_B[i - 1][j - 1]) {
                    arrow_matrix_M[i][j] = "2";
                }
            }
            {
                dp_matrix_A[i][j] = MAX(-gap_start + dp_matrix_M[i][j - 1], -gap_start + dp_matrix_B[i][j - 1],
                                        -gap_extend + dp_matrix_A[i][j - 1]);
                if (dp_matrix_A[i][j] == -gap_start + dp_matrix_M[i][j - 1]) {
                    arrow_matrix_A[i][j] = "0";
                } else if (dp_matrix_A[i][j] == -gap_extend + dp_matrix_A[i][j - 1]) {
                    arrow_matrix_A[i][j] = "1";
                } else if (dp_matrix_A[i][j] == -gap_start + dp_matrix_B[i][j - 1]) {
                    arrow_matrix_A[i][j] = "2";
                }
            }
            {
                dp_matrix_B[i][j] = MAX(-gap_start + dp_matrix_M[i - 1][j], -gap_extend + dp_matrix_B[i - 1][j],
                                        -gap_start + dp_matrix_A[i - 1][j]);
                if (dp_matrix_B[i][j] == -gap_start + dp_matrix_M[i - 1][j]) {
                    arrow_matrix_B[i][j] = "0";
                } else if (dp_matrix_B[i][j] == -gap_start + dp_matrix_A[i - 1][j]) {
                    arrow_matrix_B[i][j] = "1";
                } else if (dp_matrix_B[i][j] == -gap_extend + dp_matrix_B[i - 1][j]) {
                    arrow_matrix_B[i][j] = "2";
                }
            }


        }


    }
    vector<char> str_i;
    vector<char> str_j;
    make_aligned_string_vector(str_i, str_j, a, b, arrow_matrix_A, arrow_matrix_M, arrow_matrix_B);
    cout << calculate_score(str_i, str_j, gap_start, gap_extend);
    cout << endl;

    for (int i = 0; i < str_i.size(); i++) {
        cout << str_i[i];
    }
    cout << endl;
    for (int i = 0; i < str_j.size(); i++) {
        cout << str_j[i];
    }
    cout << endl;


}


int main() {
    string a, b;
    cin >> a;
    cin >> b;
    alignment_with_matrix_and_sigma(a, b, 11, 1);
    return 0;
}