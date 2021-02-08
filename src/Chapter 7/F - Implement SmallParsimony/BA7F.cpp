#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include <regex>
#include <random>

#ifndef BIG_NUMBER
#define BIG_NUMBER 100000000
#endif


class Graph;

class LabeledGraph;

using namespace std;

template<typename T>
bool is_in_vector(vector<T> &vec, T t) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == t) {
            return true;
        }
    }
    return false;
}

class Graph {
public:
    vector<int> nodes;
    map<int, vector<pair<int, float>>> edges;
    bool undirected;
    int N;

    explicit Graph(int n = 1, bool undirected = true) : undirected(undirected), N(n) {
        vector<int> temp;

        for (int i = 0; i < n; i++) {
            temp.push_back(i);
        }
        nodes = temp;
        map<int, vector<pair<int, float>>> edges_temp;
        edges = edges_temp;
    }

    void make_one_way_edge(int src, int end, float weight = 1.0) {
        if (!is_in_vector(nodes, src)) {
            nodes.push_back(src);
        }
        if (edges.find(src) != edges.end()) { //Update Weight If Edge Exist
            vector<pair<int, float>> temp = edges[src];
            vector<pair<int, float>> new_vec;
            for (auto &i : temp) {
                if (i.first != end) {
                    new_vec.push_back(i);
                }
            }
            auto p = make_pair(end, weight);
            new_vec.push_back(p);
            edges[src] = new_vec;
        } else {
            vector<pair<int, float>> new_vec;
            auto p = make_pair(end, weight);
            new_vec.push_back(p);
            edges[src] = new_vec;
        }
    }

    void make_edge(int src, int end, float weight = 1.0) {
        this->make_one_way_edge(src, end, weight);
        if (undirected) {
            this->make_one_way_edge(end, src, weight);
        }
    }


    void make_graph_from_another_graph(Graph T) {
        for (auto node : T.nodes) {
            if (!is_in_vector(nodes, node)) {
                nodes.push_back(node);
                if (T.edges.find(node) != T.edges.end()) {
                    for (auto edge : T.edges[node]) {
                        make_edge(node, edge.first, edge.second);
                    }
                }
            }
        }

    }
};

class LabeledGraph : public Graph {
public:
    map<int, string> labels;
    vector<int> leaves;

    explicit LabeledGraph(int N = 1, bool bidirectional = true) : Graph(N, bidirectional) {
        map<int, string> temp;
        labels = temp;
        vector<int> temp2;
        leaves = temp2;

    }

    bool is_leaf(int index) {
        return is_in_vector(leaves, index);
    }

    void set_label(int node, string label) {
        if (!is_in_vector(nodes, node)) {
            nodes.push_back(node);
        }
        labels[node] = label;
    }

    static int get_leaf(const string &label_str, LabeledGraph &T) {
        for (auto p : T.labels) {
            if (p.second == label_str) {
                return p.first;
            }
        }
        return -1;
    }

    static LabeledGraph
    make_graph_from_string_vector(int N, const vector<string> &input_lines, string alphabet = "ATCG",
                                  bool undirected = true) {
        LabeledGraph T(-1, undirected);
        regex reg(make_regex_pattern(alphabet));

        for (const auto &input : input_lines) {
            smatch match;
            if (regex_search(input, match, reg)) {
                int index;
                if (match.str(2).empty()) {
                    index = get_leaf(match.str(3), T);
                    if (index == -1) {
                        index = T.labels.size();
                    }
                    T.set_label(index, match.str(3));
                    if (!is_in_vector(T.leaves, index)) {
                        T.leaves.push_back(index);
                    }
                }

                if (match.str(3).empty()) {
                    index = stoi(match.str(2));
                }

                if (match.str(5).empty()) {
                    int index_2 = get_leaf(match.str(6), T);
                    if (index_2 == -1) {
                        index_2 = T.labels.size();
                        if (!is_in_vector(T.leaves, index_2)) {
                            T.leaves.push_back(index_2);
                        }
                        T.set_label(index_2, match.str(6));
                    }
                    T.make_edge(index, index_2);

                }

                if (match.str(6).empty()) {
                    auto index2 = stoi(match.str(5));
                    T.make_edge(index, index2);
                }
            }


        }
        return T;
    }

    void make_graph_tree_from_another(const LabeledGraph &T) {
        make_graph_from_another_graph(T);
        for (const auto &label : T.labels) {
            set_label(label.first, label.second);
        }
    }

    static string make_regex_pattern(const string &alphabet) {
        string pattern =
                "(([0-9]+)|(["
                + alphabet
                + "]+))->(([0-9]+)|(["
                + alphabet
                + "]+))";
        return pattern;
    }
};


void small_parsimony(LabeledGraph &T, LabeledGraph *final_assignment, float *answer, string alphabet = "ATCG") {

    auto assignments = LabeledGraph(T.N);
    assignments.make_graph_tree_from_another(T);

    auto small_parsimony_util_for_characters = [&assignments, &T, &alphabet](vector<char> &character) {
        map<int, bool> processed;
        map<int, vector<float>> s;

        auto update_assignment = [&assignments, &alphabet](int v, vector<float> s) {
            if (assignments.labels.find(v) == assignments.labels.end()) {
                assignments.labels[v] = "";
            }
            int index = 0;
            float min_s = BIG_NUMBER;
            for (int i = 0; i < s.size(); i++) {
                if (s[i] < min_s) {
                    min_s = s[i];
                    index = i;
                }
            }
            string new_label = assignments.labels[v] + alphabet[index];
            assignments.set_label(v, new_label);
            return alphabet[index];
        };

        function<void(int,
                      char)> label_assigner_backtrack = [&assignments, &alphabet, &T, &s, &label_assigner_backtrack](
                int v,
                char current_assignment) {

            for (auto edge : T.edges[v]) {
                int v_next = edge.first;
                if (T.is_leaf(edge.first)) {
                    continue;
                }
                if (assignments.labels.find(v_next) == assignments.labels.end()) {
                    assignments.labels[v_next] = "";
                }
                float min_score = BIG_NUMBER;
                for (int i = 0; i < alphabet.length(); i++) {
                    if (s[v_next][i] < min_score) {
                        min_score = s[v_next][i];
                    }
                }
                vector<int> indices;
                for (int i = 0; i < alphabet.length(); i++) {
                    if (s[v_next][i] == min_score) {
                        indices.push_back(i);
                    }
                }
                bool matched = false;
                for (auto i : indices) {
                    if (alphabet[i] == current_assignment) {
                        matched = true;
                        assignments.set_label(v_next, assignments.labels[v_next] + current_assignment);
                        label_assigner_backtrack(v_next, current_assignment);
                    }
                }
                if (!matched) {
                    default_random_engine rng;
                    int lower_bound = 0;
                    int upper_bound = indices.size() - 1;
                    uniform_int_distribution<int> distribution(lower_bound, upper_bound);
                    int next_assignment_index = distribution(rng);
                    char next_assignment_char = alphabet[indices[next_assignment_index]];
                    assignments.set_label(v_next, assignments.labels[v_next] + next_assignment_char);
                    label_assigner_backtrack(v_next, next_assignment_char);
                }
            }
        };

        auto compute_score = [&s, &T, &alphabet](char symbol, int v) mutable {

            auto delta_symbol = [&symbol, &alphabet](int i) {
                return symbol != alphabet[i] ? 1.0 : 0.0;
            };

            auto get_min_in_s = [delta_symbol, &alphabet, &s](int e) {
                float answer = BIG_NUMBER;
                for (int i = 0; i < alphabet.length(); i++) {
                    if (s[e][i] + delta_symbol(i) < answer) {
                        answer = s[e][i] + delta_symbol(i);
                    }
                }
                return answer;
            };

            float sum = 0.0;
            for (auto e : T.edges[v]) {
                sum += get_min_in_s(e.first);
            }
            return sum;
        };


        auto find_ripe_node = [&T, &processed]() {
            for (auto vertex : T.nodes) {
                if (processed.find(vertex) != processed.end() && !processed[vertex] &&
                    T.edges.find(vertex) != T.edges.end()) {
                    for (auto e : T.edges[vertex]) {
                        if (e.first > vertex) {
                            continue;
                        }
                        if (processed.find(e.first) != processed.end() && !processed[e.first]) {
                            break;
                        }
                    }
                    return vertex;
                }

            }
            return -BIG_NUMBER;
        };

        for (auto v : T.nodes) {
            if (T.is_leaf(v)) {
                processed[v] = true;
                vector<float> s_v;
                for (int i = 0; i < alphabet.size(); i++) {
                    char symbol = alphabet[i];
                    if (symbol == character[v]) {
                        s_v.push_back(0);
                    } else {
                        s_v.push_back(BIG_NUMBER);
                    }
                }
                s[v] = s_v;
            } else {
                processed[v] = false;
            }
        }

        auto v = find_ripe_node();
        int v_last;
        while (v != -BIG_NUMBER) {
            processed[v] = true;
            vector<float> s_v_new;
            for (auto symbol : alphabet) {
                float temp = compute_score(symbol, v);
                s_v_new.push_back(temp);
            }
            s[v] = s_v_new;
            v_last = v;
            v = find_ripe_node();
        }

        label_assigner_backtrack(v_last, update_assignment(v_last, s[v_last]));
        float min_retval = BIG_NUMBER;
        for (int i = 0; i < alphabet.size(); i++) {
            if (s[v_last][i] < min_retval) {
                min_retval = s[v_last][i];
            }
        }
        return min_retval;


    };


    float sum_answer = 0;
    for (int i = 0; i < T.labels[0].size(); i++) {
        vector<char> characters;
        characters.clear();
        for (auto label : T.labels) {
            characters.push_back(label.second[i]);
        }
        sum_answer += small_parsimony_util_for_characters(characters);
    }
    *answer = sum_answer;
    *final_assignment = assignments;
}

int hamming_distance(string &a, string &b) {
    int answer = 0;
    for (int i = 0; i < a.length(); i++) {
        if (a[i] != b[i]) {
            answer++;
        }
    }
    return answer;
}

void print_answer(LabeledGraph &T) {
    string answer = "";
    sort(T.nodes.begin(), T.nodes.end());
    for (auto node : T.nodes) {
        for (auto edge : T.edges[node]) {
            int end_node = edge.first;
            if (T.labels.find(node) != T.labels.end() && T.labels.find(end_node) != T.labels.end()) {
                int hamming_dist = hamming_distance(T.labels[node], T.labels[end_node]);
                answer += T.labels[node] + "->" + T.labels[end_node]
                          + ":" + to_string(hamming_dist) + "\n";
            }
        }
    }
    cout << answer;
}

int main() {

    int n;
    cin >> n;

    vector<string> inputs;

    while (true) { //END is used to identify end of inputs. If program doesn't get end as input, it will not run.
        string temp;
        cin >> temp;
        if (temp == "END") { break; }
        inputs.push_back(temp);
    }


    auto T = LabeledGraph::make_graph_from_string_vector(n, inputs, "ATCG", false);
    LabeledGraph new_tree;
    float score_answer;

    small_parsimony(T, &new_tree, &score_answer);
    cout << score_answer << endl;
    print_answer(new_tree);

    return 0;
}