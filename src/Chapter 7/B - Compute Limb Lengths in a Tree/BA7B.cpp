#include <iostream>
#include <vector>

using namespace std;

double limb_length(int j, const vector<vector<int>> &distance_matrix) {
    double limb_length = +2147483647.0;
    int n = distance_matrix.size();
    for (int i = 0; i < n; i++) {
        if (i == j) {
            continue;
        }
        for (int k = 0; k < n; k++) {
            if (k == j) {
                continue;
            }

            double dist = (1.0 * distance_matrix[i][j] + distance_matrix[k][j] - distance_matrix[i][k]) / 2.0;
            if (dist < limb_length) {
                limb_length = dist;
            }

        }
    }
    return limb_length;
}

int main() {
    int n, index;
    cin >> n >> index;

    vector<vector<int>> distance_matrix;
    distance_matrix.resize(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> distance_matrix[i][j];
        }
    }

    cout << limb_length(index, distance_matrix);


}