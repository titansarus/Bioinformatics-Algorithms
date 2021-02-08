#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

#define MAX(...) std::max({__VA_ARGS__})


const float tol = 0.000001;


void print_lcs_recursive(string &a, vector<vector<string>> arrow_matrix, int i, int j) {
    vector<char> answer;
    while (i != 0 && j != 0) {

        if (arrow_matrix[i][j] == "↖") {
            answer.push_back(a[i - 1]);
            i--;
            j--;
        } else if (arrow_matrix[i][j] == "←") {
            j--;
        } else if (arrow_matrix[i][j] == "↑") {
            i--;
        }
    }
    for (int i = answer.size() - 1; i >= 0; i--) {
        cout << answer[i];
    }

}


void solve_lcs(string &a, string &b) {
    vector<vector<int>> dp_matrix(a.length() + 1);

    vector<vector<string>> arrow_matrix(a.length() + 1);
    for (int i = 0; i <= a.length(); i++) {
        vector<string> temp(b.length() + 1);
        vector<int> temp2(b.length() + 1);
        arrow_matrix[i] = temp;
        dp_matrix[i] = temp2;
    }
    dp_matrix[0][0] = 0.0;
    arrow_matrix[0][0] = "   ";

    for (int i = 1; i <= b.length(); i++) {
        dp_matrix[0][i] = 0;
        arrow_matrix[0][i] = "←";
    }

    for (int i = 1; i <= a.length(); i++) {
        dp_matrix[i][0] = 0;
        arrow_matrix[i][0] = "↑";

    }


    for (int i = 1; i <= a.length(); i++) {
        for (int j = 1; j <= b.length(); j++) {
            char a_char = a[i - 1];
            char b_char = b[j - 1];
            bool is_match = (a_char == b_char);

            dp_matrix[i][j] = MAX(dp_matrix[i - 1][j], dp_matrix[i][j - 1], (is_match ? (
                    dp_matrix[i - 1][j - 1] + 1) : (dp_matrix[i - 1][j - 1])));

            if ((abs(dp_matrix[i][j] - (dp_matrix[i - 1][j - 1] + 1)) < tol) && is_match) {
                arrow_matrix[i][j] = "↖";
            } else if ((abs(dp_matrix[i][j] - (dp_matrix[i - 1][j])) < tol)) {
                arrow_matrix[i][j] = "↑";
            } else if ((abs(dp_matrix[i][j] - (dp_matrix[i][j - 1])) < tol)) {
                arrow_matrix[i][j] = "←";
            } else if ((abs(dp_matrix[i][j] - (dp_matrix[i - 1][j - 1])) < tol) && !is_match) {
                arrow_matrix[i][j] = "↖";
            }
        }
    }

    print_lcs_recursive(a, arrow_matrix, a.length(), b.length());
}


int main() {
    string a, b;
    cin >> a;
    cin >> b;

    solve_lcs(a, b);
    return 0;
}