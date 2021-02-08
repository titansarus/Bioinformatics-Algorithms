/* NOTE: This code is same as BA9C.  You don't need to input suffix array or LCP array,
 * it will generate them automatically, and use them in the alogorithm.
*/


#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct hash_pair_calculator {
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};


class Node {
public:
    int parent_number;
    vector<int> *children;

    Node(int parentNumber) : parent_number(parentNumber) {
        children = new vector<int>;
    }

    void add_children(int num) const {
        children->push_back(num);
    }

    void remove_children(int num) const {
        for (auto it = children->begin(); it != children->end();
             it++) {
            if (*it == num) {
                children->erase(it);
                break;;
            }
        }
    }


};

class Edge {
public:
    int start_index;
    int end_index;

    Edge(int startIndex, int endIndex) : start_index(startIndex), end_index(endIndex) {}
};

class problem_solver {

public:
    string input;
    vector<int> suffix_array;
    vector<int> lcp;
    vector<Node *> *nodes;
    unordered_map<pair<int, int>, Edge *, hash_pair_calculator> edges;

    problem_solver(const string &input, const vector<int> &suffix_array, const vector<int> &lcp) :
            input(input),
            suffix_array(suffix_array),
            lcp(lcp) {
        nodes = new vector<Node *>;
        Node *node = new Node(-1); // THIs IS ROOT
        nodes->push_back(node);
        unordered_map<pair<int, int>, Edge *, hash_pair_calculator> temp;
        edges = temp;
        make_tree();
    }

    void make_tree() {
        int i = 0;
        for (auto suff : suffix_array) {
            int insertion_parent;
            string insertion_suffix;

            string suffix = input.substr(suff, this->input.length() - suff + 1);
            this->insert_node(suffix, lcp[i], &insertion_parent, &insertion_suffix);

            this->nodes->push_back(new Node(insertion_parent));
            this->nodes->at(insertion_parent)->add_children(this->nodes->size() - 1);

            pair<int, int> key_pair = make_pair(insertion_parent, this->nodes->size() - 1);
            Edge *edge = new Edge(input.size() - insertion_suffix.size(), input.length());
            edges[key_pair] = edge;
            i++;
        }
    }

    void insert_node(const string& suffix, int lcp_distance, int *insertion_parent, string *insertion_suffix,
                     int current_node = 0) {
        if (lcp_distance == 0) {
            *insertion_parent = current_node;
            *insertion_suffix = suffix;
            return;
        }

        pair<int, int> index_pair = make_pair(current_node,
                                              ((this->nodes->at(
                                                      current_node)->children->at(
                                                      this->nodes->at(current_node)->children->size() - 1))));
        int dist = get_substring_from_edge(edges[index_pair]).size();
        if (dist <= lcp_distance) {
            string suff = suffix.substr(dist, suffix.length() - dist);
            int node_to_pass_to_function = this->nodes->at(
                    current_node)->children->at(
                    this->nodes->at(current_node)->children->size() - 1);

            insert_node(suff, lcp_distance - dist, insertion_parent, insertion_suffix, node_to_pass_to_function);
            return;
        } else {
            int node_to_pass_to_function = this->nodes->at(current_node)->children->at(
                    this->nodes->at(current_node)->children->size() - 1);
            *insertion_parent = split_edge(current_node, node_to_pass_to_function, lcp_distance);
            string suff = suffix.substr(lcp_distance, suffix.length() - lcp_distance);
            *insertion_suffix = suff;
            return;
        }

    }

    int split_edge(int parent_num, int child_num, int split) {

        int new_node_number = nodes->size();
        nodes->push_back(new Node(parent_num));
        nodes->at(new_node_number)->add_children(child_num);

        nodes->at(parent_num)->add_children(new_node_number);
        nodes->at(parent_num)->remove_children(child_num);

        nodes->at(child_num)->parent_number = new_node_number;

        pair<int, int> edge_index_old = make_pair(parent_num, child_num);
        pair<int, int> edge_index_new = make_pair(parent_num, new_node_number);
        pair<int, int> edge_index_new2 = make_pair(new_node_number, child_num);

        Edge *old_edge = edges[edge_index_old];
        edges[edge_index_new] = new Edge(old_edge->start_index, old_edge->start_index + split);
        edges[edge_index_new2] = new Edge(old_edge->start_index + split, old_edge->end_index);

        edges.erase(edge_index_old);

        return new_node_number;
    }

    string get_substring_from_edge(Edge *edge) const {
        if (edge == nullptr) {
            return "";
        }
        return input.substr(edge->start_index, edge->end_index - edge->start_index);
    }

};


//KASAI ALGORITHM for LCP ARRAY
//REFERENCE:
//Kasai, T.; Lee, G.; Arimura, H.; Arikawa, S.; Park, K. (2001).
// Linear-Time Longest-Common-Prefix Computation in Suffix Arrays and Its Applications.
// Proceedings of the 12th Annual Symposium on Combinatorial Pattern Matching.
// Lecture Notes in Computer Science. 2089. pp. 181–192. doi:10.1007/3-540-48194-X_17. ISBN 978-3-540-42271-6.
vector<int> lcp_kasai(string input, vector<int> suffix_array) {
    int len = input.size();
    vector<int> inverse_suffix(len);
    vector<int> answer(len);
    for (int i = 0; i < len; i++) {
        inverse_suffix[suffix_array[i]] = i;
    }
    int iter = 0;
    for (int i = 0; i < len; i++) {
        if (inverse_suffix[i] == len - 1) {
            iter = 0;
            continue;
        }

        int j = suffix_array[inverse_suffix[i] + 1];
        while (i + iter < len && j + iter < len && input.at(i + iter) == input.at(j + iter)) {
            iter++;
        }
        answer[inverse_suffix[i]] = iter;
        if (iter > 0) {
            iter--;
        }
    }
    answer.insert(answer.begin(), 0);
    answer.erase(answer.end() - 1);
    return answer;
}

vector<int> make_suffix_array(const string &input) {
    int len = input.size();
    int counter = 1;
    map<string, int> suffixes;
    for (int i = len - 1; i >= 0; i--) {
        string suffix = input.substr(i, counter);
        suffixes[suffix] = i;
        counter++;

    }
    vector<int> answer;
    for (const auto& ele : suffixes) {
        answer.push_back(ele.second);
    }
    return answer;
}

int main() {
    string input;
    cin >> input;
    vector<int> suffix_array = make_suffix_array(input);
    vector<int> lcp_array = lcp_kasai(input, suffix_array);
    auto *problemSolver = new problem_solver(input, suffix_array, lcp_array);

    vector<string> answer;
    for (auto edge : problemSolver->edges) {
        answer.push_back(problemSolver->get_substring_from_edge(edge.second));
    }
    for (const auto& str: answer) {
        cout << str << endl;
    }
    return 0;
}