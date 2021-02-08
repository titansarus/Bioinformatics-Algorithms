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
    for (int i = 0; i < dna.size() - length + 1; i++) {
        string ss = dna.substr(i, length);
        float prob = probability_calc(ss, prob_mat);
        if (prob > prob_max) {
            prob_max = prob;
            answer = ss;
        }
    }
    return answer;
}

vector<string> get_initial_motifs(vector<string> &dnas, int start_index, int length) {
    vector<string> answer;
    answer.reserve(dnas.size());
    for (int i = 0; i < dnas.size(); i++) {
        answer.push_back(dnas[i].substr(start_index, length));
    }
    return answer;
}

vector<vector<int>> count_matrix_maker(vector<string> &motifs) {
    vector<vector<int>> answer(4, vector<int>(motifs[0].length(), 0));
    for (auto motif : motifs) {
        for (int i = 0; i < motif.size(); i++) {
            int index = nucleotide_to_number[motif[i]];
            answer[index][i] += 1;
        }
    }
    return answer;
}

vector<vector<float>> prob_matrix_from_count_matrix(vector<vector<int>> &count_matrix, int t) {
    vector<vector<float>> answer(4, vector<float>(count_matrix[0].size(), 0));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < count_matrix[0].size(); j++) {
            answer[i][j] = 1.000 * count_matrix[i][j] / t;
        }
    }
    return answer;
}

int score_of_motifs(vector<string> motifs,int k) {
    auto count_mat = count_matrix_maker(motifs);
    int answer = 0;
    for (int i = 0; i < k; i++) {
        int max_occur = 0;
        for (int j = 0; j < 4; j++) {
            if (count_mat[j][i] > max_occur) {
                max_occur = count_mat[j][i];
            }
        }
        answer += (4- max_occur);
    }
    return answer;
}

vector<string> all_kmers(string dna, int k) {
    vector<string> answer;
    for (int i = 0; i < dna.size() - k+1; i++) {
        answer.push_back(dna.substr(i, k));
    }
    return answer;
}

vector<string> greedy_motif(vector<string> dnas, int k, int t) {
    vector<string> best_motifs = get_initial_motifs(dnas, 0, k);
    auto all_first_kmers = all_kmers(dnas[0], k);

    for (auto kmer : all_first_kmers) {
        vector<string> motifs;
        motifs.clear();
        motifs.push_back(kmer);
        for (int i = 1; i < t; i++) {
            auto count_mat = count_matrix_maker(motifs);
            auto prob_mat = prob_matrix_from_count_matrix(count_mat, motifs.size());
            string most_probable = most_probable_string(dnas[i], prob_mat, k);
            motifs.push_back(most_probable);
        }
        int motif_score = score_of_motifs(motifs,k);
        int best_motif_score = score_of_motifs(best_motifs,k);


        if (motif_score < best_motif_score) {
            best_motifs = motifs;
        }
    }
    return best_motifs;

}

int main() {
    int k, t;
    cin >> k >> t;
    vector<string> dnas;
    for (int i = 0; i < t; i++) {
        string temp;
        cin >> temp;
        dnas.push_back(temp);
    }
    auto answer = greedy_motif(dnas, k, t);
    for (auto str : answer) {
        cout << str << endl;
    }
    return 0;
}