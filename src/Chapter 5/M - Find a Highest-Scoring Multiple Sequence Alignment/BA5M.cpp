#include <iostream>
#include <algorithm>
#include <vector>

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

void alignment_match_sigma_mu_without_delta(string &a, string &b, string &c) {
    long long int dp_matrix[a.length() + 1][b.length() + 1][c.length() + 1];
    int arrow_matrix[a.length() + 1][b.length() + 1][c.length() + 1];
    dp_matrix[0][0][0] = 0.0;
    arrow_matrix[0][0][0] = 8;

    //0: i-- ; 1: j-- ; 2: k-- ; 3: i-- j-- ; 4: i-- k--; 5 j-- k--; 6: i-- j-- k--

    for (int i = 0; i <= a.length(); i++) {
        for (int j = 0; j <= b.length(); j++) {
            for (int k = 0; k<= c.length(); k++) {
                dp_matrix[i][j][k] = 0;
            }
        }
    }

    for (int i = 1; i <= a.length(); i++) {
        for (int j = 0; j <= b.length(); j++) {
            dp_matrix[i][j][0] = 0;
            arrow_matrix[i][j][0] = 1;
        }
    }

    for (int i = 1; i <= a.length(); i++) {
        for (int k = 0; k <= c.length(); k++) {
            dp_matrix[i][0][k] = 0;
            arrow_matrix[i][0][k] = 0;
        }
    }

    for (int j = 1; j <= b.length(); j++) {
        for (int k = 0; k <= c.length(); k++) {
            dp_matrix[0][j][k] = 0;
            arrow_matrix[0][j][k] = 2;

        }
    }

    for (int i = 0; i <= a.length(); i++) {
        arrow_matrix[i][0][0] = 0;
    }
    for (int i = 0; i <= b.length(); i++) {
        arrow_matrix[0][i][0] = 1;
    }
    for (int i = 0; i <= c.length(); i++) {
        arrow_matrix[0][0][i] = 2;
    }


    for (int i = 1; i <= a.length(); i++) {
        for (int j = 1; j <= b.length(); j++) {
            for (int k = 1; k <= c.length(); k++) {
                char a_char = a[i - 1];
                char b_char = b[j - 1];
                char c_char = c[k - 1];
                bool is_match = (a_char == b_char) && (b_char == c_char);

                dp_matrix[i][j][k] =
                        MAX(dp_matrix[i][j][k - 1], dp_matrix[i][j - 1][k], dp_matrix[i][j - 1][k - 1],
                            dp_matrix[i - 1][j][k],
                            dp_matrix[i - 1][j][k - 1], dp_matrix[i - 1][j - 1][k],
                            (is_match ? dp_matrix[i - 1][j - 1][k - 1] + 1 : dp_matrix[i - 1][j - 1][k - 1]));


                if (dp_matrix[i][j][k] == dp_matrix[i - 1][j][k]) {
                    arrow_matrix[i][j][k] = 0;
                } else if (dp_matrix[i][j][k] == dp_matrix[i][j - 1][k]) {
                    arrow_matrix[i][j][k] = 1;
                } else if (dp_matrix[i][j][k] == dp_matrix[i][j][k - 1]) {
                    arrow_matrix[i][j][k] = 2;
                } else if (dp_matrix[i][j][k] == dp_matrix[i][j - 1][k - 1]) {
                    arrow_matrix[i][j][k] = 5;
                } else if (dp_matrix[i][j][k] == dp_matrix[i - 1][j - 1][k]) {
                    arrow_matrix[i][j][k] = 3;
                } else if (dp_matrix[i][j][k] == dp_matrix[i - 1][j][k - 1]) {
                    arrow_matrix[i][j][k] = 4;
                } else if (dp_matrix[i][j][k] == (dp_matrix[i - 1][j - 1][k - 1]) ||
                           (dp_matrix[i][j][k]) == dp_matrix[i - 1][j - 1][k - 1] + 1) {
                    arrow_matrix[i][j][k] = 6;
                }


            }
        }
    }


    vector<char> str_i;
    vector<char> str_j;
    vector<char> str_k;

    int i, j, k;
    i = a.length();
    j = b.length();
    k = c.length();

    //0: i-- ; 1: j-- ; 2: k-- ; 3: i-- j-- ; 4: i-- k--; 5 j-- k--; 6: i-- j-- k--
    while (i != 0 || j != 0 || k != 0) {
        if (arrow_matrix[i][j][k] == 6) {
            str_i.push_back(a[i - 1]);
            str_j.push_back(b[j - 1]);
            str_k.push_back(c[k - 1]);
            i--;
            j--;
            k--;
        } else if (arrow_matrix[i][j][k] == 0) {
            str_i.push_back(a[i - 1]);
            str_j.push_back('-');
            str_k.push_back('-');
            i--;
        } else if (arrow_matrix[i][j][k] == 1) {
            str_j.push_back(b[j - 1]);
            str_i.push_back('-');
            str_k.push_back('-');
            j--;
        } else if (arrow_matrix[i][j][k] == 2) {
            str_k.push_back(c[k - 1]);
            str_j.push_back('-');
            str_i.push_back('-');
            k--;
        } else if (arrow_matrix[i][j][k] == 3) {
            str_i.push_back(a[i - 1]);
            str_j.push_back(b[j - 1]);
            str_k.push_back('-');
            i--;
            j--;
        } else if (arrow_matrix[i][j][k] == 4) {
            str_i.push_back(a[i - 1]);
            str_j.push_back('-');
            str_k.push_back(c[k - 1]);
            i--;
            k--;

        } else if (arrow_matrix[i][j][k] == 5) {
            str_i.push_back('-');
            str_j.push_back(b[j - 1]);
            str_k.push_back(c[k - 1]);
            j--;
            k--;
        }
    }


    reverse(str_i.begin(), str_i.end());
    reverse(str_j.begin(), str_j.end());
    reverse(str_k.begin(), str_k.end());

    cout << dp_matrix[a.length()][b.length()][c.length()] << endl;
    for (int i = 0; i < str_i.size(); i++) {
        cout << str_i[i];
    }
    cout << "\n";
    for (int i = 0; i < str_j.size(); i++) {
        cout << str_j[i];
    }
    cout << "\n";
    for (int i = 0; i < str_k.size(); i++) {
        cout << str_k[i];
    }


}


int main() {
    string a, b, c;
    cin >> a;
    cin >> b;
    cin >> c;

    alignment_match_sigma_mu_without_delta(a, b, c);
    return 0;
}