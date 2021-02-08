#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>


using namespace std;


unordered_map<char, int> nucleotide_to_number{{'A', 0},
                                              {'C', 1},
                                              {'G', 2},
                                              {'T', 3}};


float probability_calc(string text, vector<vector<float>> &prob_mat) {
    float probability = 1.0;
    for (int i = 0; i < text.size(); i++) {
        probability = probability * prob_mat[nucleotide_to_number[text[i]]][i];
    }
    return probability;
}


vector<vector<int>> count_matrix_maker(vector<string> &motifs, bool add_one = true) {
    vector<vector<int>> answer(4, vector<int>(motifs[0].length(), 1 * (add_one)));
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

int score_of_motifs(vector<string> motifs, int k) {
    auto count_mat = count_matrix_maker(motifs, false);
    int full_sum = 0;
    for (int j = 0; j < 4; j++) {
        full_sum += count_mat[j][0];
    }
    int answer = 0;
    for (int i = 0; i < k; i++) {
        int max_occur = 0;
        for (int j = 0; j < 4; j++) {
            if (count_mat[j][i] > max_occur) {
                max_occur = count_mat[j][i];
            }
        }
        answer += (motifs.size() - 1 * max_occur);
    }
    return answer;
}

vector<string> all_kmers(const string &dna, int k) {
    vector<string> answer;
    for (int i = 0; i < dna.size() - k + 1; i++) {
        answer.push_back(dna.substr(i, k));
    }
    return answer;
}


vector<string> motifs_generator(vector<string> &dnas, vector<vector<float>> &probs, int k) {
    vector<string> answer;
    for (const auto &dna : dnas) {
        float max_prob = -0.1;
        vector<string> all_kmer = all_kmers(dna, k);
        string most_prob_kmer;
        for (const auto &kmer : all_kmer) {
            float prob = probability_calc(kmer, probs);
            if (max_prob < prob) {
                max_prob = prob;
                most_prob_kmer = kmer;
            }
        }
        answer.push_back(most_prob_kmer);
    }
    return answer;
}


string random_kmer(const string &text, int k) {

    int index = (rand() % (text.size() - k + 1));
    return text.substr(index, k);
}

vector<string> drop_one_string(vector<string> &strings, int i) {
    vector<string> answer;
    for (int j = 0; j < strings.size(); j++) {
        if (i != j) {
            answer.push_back(strings[j]);
        }
    }
    return answer;
}

vector<float> cdf(vector<float> pmf) {
    vector<float> answer;
    float total = 0.0;
    for (float prob : pmf) {
        total += prob;
        answer.push_back(total);
    }
    return answer;
}

int generate_random_index(vector<float> pmf) {
    auto cumulative = cdf(pmf);
    float number = cumulative[cumulative.size() - 1];
    float rnd_0_1 = 1.00 * rand() / RAND_MAX;
    number = number * rnd_0_1;
    int i = 0;
    while (cumulative[i] <= number) {
        i++;
    }
    return i;
}

vector<string> gibbs_sampler(vector<string> &dnas, int k, int t, int N, int *score) {
    vector<string> motifs;
    for (int i = 0; i < t; i++) {
        string temp = random_kmer(dnas[i], k);
        motifs.push_back(temp);
    }
    vector<string> best_motifs = motifs;
    int best_score = INT_MAX;
    for (int j = 0; j < N; j++) {
        int index = rand() % (t);
        auto drop_one = drop_one_string(motifs, index);
        auto count_mat = count_matrix_maker(drop_one);
        auto prob_mat = prob_matrix_from_count_matrix(count_mat, t - 1);
        auto kmers = all_kmers(dnas[index], k);
        vector<float> probs;
        for (auto kmer : kmers) {
            float prob_temp = probability_calc(kmer, prob_mat);
            probs.push_back(prob_temp);
        }
        auto motif_index_gen = generate_random_index(probs);
        string new_motif = dnas[index].substr(motif_index_gen, k);
        motifs[index] = new_motif;
        int score_new = score_of_motifs(motifs, k);
        if (score_new < best_score) {
            best_score = score_new;
            best_motifs = motifs;
        }
    }
    *score = best_score;
    return best_motifs;

}




int main() {

    srand((unsigned) time(NULL));
    int k, t , N;
    cin >> k >> t >> N;
    vector<string> dnas;


    for (int i = 0; i < t; i++) {
        string temp;
        cin >> temp;
        dnas.push_back(temp);
    }


    int NUMBER_OF_ITERS = 20;
    int best_score = INT_MAX;
    vector<string> best;
    for (int i = 0; i < NUMBER_OF_ITERS; i++) {
        int score;
        auto motif = gibbs_sampler(dnas, k, t , N, &score);
        if (score < best_score) {
            best = motif;
            best_score = score;
        }
    }

    for (const auto &str : best) {
        cout << str << endl;
    }


//    auto answer = greedy_motif(dnas, k, t);
//    for (auto str : answer) {
//        cout << str << endl;
//    }
    return 0;
}