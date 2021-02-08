#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <iomanip>
#include <cmath>


using namespace std;

class Node;

class Edge;

class UndirectedGraph;

const vector<int> NULL_VECTOR_CONSTANT;

class Node {
public:
    int number;
    vector<Edge *> *edges;

    explicit Node(int number) : number(number) {
        edges = new vector<Edge *>;
    }
};

class Edge {
public:
    Node *start;
    Node *end;
    float weight = 0;

    Edge(Node *start, Node *end, float weight) : start(start), end(end), weight(weight) {}


};


class UndirectedGraph {
public:
    vector<Node *> *nodes;
    vector<Edge *> *edges;

    UndirectedGraph() {
        nodes = new vector<Node *>;
        edges = new vector<Edge *>;
    }

    explicit UndirectedGraph(int initial_size) {
        nodes = new vector<Node *>;
        edges = new vector<Edge *>;
        for (int i = 0; i < initial_size; i++) {
            add_node(i);
        }
    }

    Node *add_node(int number) {
        if (find_node(number) == nullptr) {
            Node *node = new Node(number);
            nodes->push_back(node);
            return node;
        }
    }

    Edge *add_edge(Node *src, Node *dest, float weight) {
        Edge *edge = new Edge(src, dest, weight);
        Edge *edge2 = new Edge(dest, src, weight);
        edges->push_back(edge);
        edges->push_back(edge2);
        src->edges->push_back(edge);
        dest->edges->push_back(edge2);
        return edge;
    }

    Node *find_node(int number) {
        for (int i = 0; i < nodes->size(); i++) {
            if (nodes->at(i)->number == number) {
                return nodes->at(i);
            }
        }
        return nullptr;
    }


    void add_edge(int src, int dest, float weight) {

        Node *src_node = find_node(src);
        Node *end_node = find_node(dest);
        if (src_node == nullptr) {
            src_node = add_node(src);
        }
        if (end_node == nullptr) {
            end_node = add_node(dest);
        }
        add_edge(src_node, end_node, weight);
    }

    vector<float> traverse(int start, int end, bool *is_found, vector<float> path) {
        if (path.empty()) {
            path.push_back(start);
        }

        Node *start_node = find_node(start);
        for (auto edge : *(start_node->edges)) {
            if (find(path.begin(), path.end(), edge->end->number) != path.end()) {
                continue;
            }
            vector<float> path2(path);

            path2.push_back(edge->end->number);
            if (edge->end->number == end) {
                *is_found = true;
                return path2;
            } else {
                auto answer = traverse(edge->end->number, end, is_found, path2);
                if (*is_found) {
                    return answer;
                }
            }

        }
        *is_found = false;
        return path;

    }

    void print_adj_list() const {
        cout << setprecision(3) << fixed;
        for (auto node : *nodes) {
            for (auto edge : *node->edges) {
                cout << node->number << "->" << edge->end->number << ":" << edge->weight << endl;
            }
        }
    }

};


void remove_from_vector(vector<int> &list_of_nodes, int node_index_i);

vector<vector<float>> remove_row_col_from_matrix(vector<vector<float>> &mat, int index) {
    vector<vector<float>> answer;
    for (int i = 0; i < mat.size(); i++) {
        if (i != index) {
            vector<float> temp;
            for (int j = 0; j < mat.size(); j++) {
                if (j != index) {
                    temp.push_back(mat[i][j]);
                }
            }
            answer.push_back(temp);

        }
    }


    return answer;
}

vector<vector<float>> dprime_maker(vector<vector<float>> &mat, vector<float> &total_dist, int n) {
    vector<vector<float>> answer;
    answer.resize(n, vector<float>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            answer[i][j] = (n - 2.0) * mat[i][j] - total_dist[i] - total_dist[j];
            answer[j][i] = answer[i][j];
        }
    }
    return answer;
}

void find_i_j_min_diag(vector<vector<float>> &mat, int n, int *i_index, int *j_index, float *min_diag) {

    *min_diag = +10000000000;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (mat[i][j] < *min_diag) {
                *min_diag = mat[i][j];
                *i_index = i;
                *j_index = j;
            }

        }

    }
}

vector<vector<float>> delta_maker(vector<float> total_dist, int n) {
    vector<vector<float>> answer;
    answer.resize(n, vector<float>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            answer[i][j] = (1.0 * total_dist[i] - total_dist[j]) / (n - 2.0);
            answer[j][i] = 1.0 * answer[i][j];
        }
    }
    return answer;
}

vector<float> sum_of_row_maker(vector<vector<float>> &mat) {
    vector<float> answer;
    for (int i = 0; i < mat.size(); i++) {
        float temp = 0.0;
        for (int j = 0; j < mat[i].size(); j++) {
            temp += mat[i][j];
        }
        answer.push_back(temp);

    }
    return answer;
}


UndirectedGraph NeighbourJoin(vector<vector<float>> &D, int n, vector<int> list_of_nodes) {

    if (list_of_nodes == NULL_VECTOR_CONSTANT) {
        for (int i = 0; i < n; i++) {
            list_of_nodes.push_back(i);
        }
    }

    if (n == 2) {
        UndirectedGraph G = UndirectedGraph(n);
        G.add_edge(list_of_nodes[0], list_of_nodes[1], D[0][1]);
        return G;
    }

    vector<float> total_dist = sum_of_row_maker(D);
    auto dprime_mat = dprime_maker(D, total_dist, n);
    int i_index, j_index;
    float min_diag;
    find_i_j_min_diag(dprime_mat, n, &i_index, &j_index, &min_diag);
    auto delta = delta_maker(total_dist, n);
    float limb_i = (1.0 * D[i_index][j_index] + delta[i_index][j_index]) / 2.000;
    float limb_j = (1.0 * D[i_index][j_index] - delta[i_index][j_index]) / 2.000;
    vector<float> row;
    for (int i = 0; i < n; i++) {
        float temp = (1.00 * D[i][i_index] + D[i][j_index] - D[i_index][j_index]) * 1.000 / 2.00;
        row.push_back(temp);
    }
    row.push_back(0.00);

    for (int i = 0; i < n; i++) {
        D[i].push_back(row[i]);
    }
    D.push_back(row);

    int new_node_index = list_of_nodes.at(list_of_nodes.size() - 1) + 1;
    list_of_nodes.push_back(new_node_index);
    D = remove_row_col_from_matrix(D, (i_index > j_index ? i_index : j_index));
    D = remove_row_col_from_matrix(D, (i_index > j_index ? j_index : i_index));
    int node_index_i = list_of_nodes[i_index];
    int node_index_j = list_of_nodes[j_index];
    remove_from_vector(list_of_nodes, node_index_i);
    remove_from_vector(list_of_nodes, node_index_j);
    auto G = NeighbourJoin(D, n - 1, list_of_nodes);

    G.add_edge(new_node_index, node_index_i, limb_i);
    G.add_edge(new_node_index, node_index_j, limb_j);
    return G;
}

void remove_from_vector(vector<int> &list_of_nodes, int node_index_i) {
    for (auto it = list_of_nodes.begin(); it < list_of_nodes.end(); it++) {
        if (*it == node_index_i) {
            list_of_nodes.erase(it);
            break;
        }
    }
}

int main() {
    int n;
    cin >> n;
    vector<vector<float>> mat;
    mat.resize(n, vector<float>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    auto g = NeighbourJoin(mat, n, NULL_VECTOR_CONSTANT);
    g.print_adj_list();
}