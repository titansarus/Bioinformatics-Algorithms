#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Node;

class Edge;

class UndirectedGraph;

int limb_length(int j, const vector<vector<int>> &distance_matrix) {
    int limb_length = +2147483647.0;
    int n = distance_matrix.size();
    for (int i = 0; i < n; i++) {
        if (i == j) {
            continue;
        }
        for (int k = 0; k < n; k++) {
            if (k == j) {
                continue;
            }

            int dist = (distance_matrix[i][j] + distance_matrix[k][j] - distance_matrix[i][k]) / 2;
            if (dist < limb_length) {
                limb_length = dist;
            }

        }
    }
    return limb_length;
}

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
    int weight = 0;

    Edge(Node *start, Node *end, int weight) : start(start), end(end), weight(weight) {}
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

    Edge *add_edge(Node *src, Node *dest, int weight) {
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


    void remove_edge(Node *src, Node *dest) {
        for (auto it = src->edges->begin(); it < src->edges->end(); it++) {
            if ((*it)->end->number == dest->number) {
                src->edges->erase(it);
                it--;
            }
        }

        for (auto it = dest->edges->begin(); it < dest->edges->end(); it++) {
            if ((*it)->end->number == src->number) {
                dest->edges->erase(it);
                it--;
            }
        }

        for (auto it = edges->begin(); it < edges->end(); it++) {
            if ((*it)->start->number == src->number && (*it)->end->number == dest->number) {
                edges->erase(it);
                it--;
            } else if ((*it)->start->number == dest->number && (*it)->end->number == src->number) {
                edges->erase(it);
                it--;
            }
        }
    }

    void remove_edge(int src, int dest) {
        Node *srcNode = find_node(src);
        Node *destNode = find_node(dest);
        remove_edge(srcNode, destNode);
    }


    void add_edge(int src, int dest, int weight) {

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

    vector<int> traverse(int start, int end, bool *is_found, vector<int> path) {
        if (path.empty()) {
            path.push_back(start);
        }

        Node *start_node = find_node(start);
        for (auto edge : *(start_node->edges)) {
            if (find(path.begin(), path.end(), edge->end->number) != path.end()) {
                continue;
            }
            vector<int> path2(path);

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


    vector<Edge *> edge_from_path(vector<int> path) {
        vector<Edge *> answer;
        for (int i = 0; i < path.size() - 1; i++) {
            int node_number = path[i];
            Node *node = find_node(node_number);
            for (auto edge :*node->edges) {
                if (edge->end->number == path[i + 1]) {
                    answer.push_back(edge);
                    break;
                }
            }

        }
        return answer;

    }

    void find_good_node_2(vector<int> path, int number, bool *is_found, int *weight, int *weight_prev, int *node,
                          int *node_prev) {
        vector<Edge *> path_edges = edge_from_path(path);

        vector<pair<int, int>> pairs_traversal;
        pairs_traversal.emplace_back(path.at(0), 0);
        for (int i = 1; i < path.size(); i++) {
            pairs_traversal.emplace_back(path.at(i), path_edges.at(i - 1)->weight);
        }

        *weight = 0;
        for (auto p : pairs_traversal) {
            *weight_prev = *weight;
            *weight += p.second;
            if (*weight == number) {
                *is_found = true;
                *node = p.first;
                *node_prev = p.first;
                return;
            } else if (*weight > number) {
                *is_found = false;
                *node = p.first;
                return;
            }
            *node_prev = p.first;
        }

        *is_found = false;
    }

    void print_adj_list() const {
        for (auto node : *nodes) {
            for (auto edge : *node->edges) {
                cout << node->number << "->" << edge->end->number << ":" << edge->weight << endl;
            }
        }
    }

};

void find_ikn(vector<vector<int>> matrix, int *i_ans, int *k_ans, int *n_ans, int *weight) {
    *n_ans = matrix.size() - 1;
    for (int i = 0; i <= *n_ans; i++) {
        for (int k = 0; k <= *n_ans; k++) {
            if (i != k) {
                if (matrix[i][k] == matrix[i][*n_ans] + matrix[*n_ans][k]) {
                    *i_ans = i;
                    *k_ans = k;
                    *weight = matrix[i][*n_ans];
                    return;
                }
            }
        }
    }

}

UndirectedGraph AdditivePhylogeny(vector<vector<int>> matrix, int n, int initial_size) {
    if (n == 2) {
        auto G = UndirectedGraph(initial_size);
        G.add_edge(0, 1, matrix[0][1]);
        return G;
    }
    int limb = limb_length(n - 1, matrix);

    vector<vector<int>> bald_matrix;
    for (int i = 0; i < matrix.size(); i++) {
        vector<int> d(matrix.size());
        bald_matrix.push_back(d);
    }

    for (int i = 0; i < matrix.size() - 1; i++) {
        for (int j = 0; j < matrix.size() - 1; j++) {
            bald_matrix[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < matrix.size() - 1; i++) {

        bald_matrix[matrix.size() - 1][i] = matrix[matrix.size() - 1][i] - limb;
        bald_matrix[i][matrix.size() - 1] = bald_matrix[matrix.size() - 1][i];
    }
    int i_ans;
    int k_ans;
    int n_ans;
    int x;

    find_ikn(bald_matrix, &i_ans, &k_ans, &n_ans, &x);

    vector<vector<int>> cut_matrix;
    for (int i = 0; i < matrix.size() - 1; i++) {
        vector<int> t(matrix.size() - 1);
        cut_matrix.push_back(t);
    }
    for (int i = 0; i < matrix.size() - 1; i++) {
        for (int j = 0; j < matrix.size() - 1; j++) {
            cut_matrix[i][j] = matrix[i][j];
        }
    }

    auto G = AdditivePhylogeny(cut_matrix, n - 1, initial_size);

    bool is_found;
    vector<int> path;
    path = G.traverse(i_ans, k_ans, &is_found, path);
    auto edges_path = G.edge_from_path(path);

    bool is_found2;

    int node_prev, node, weight, weight_prev;

    int v;
    G.find_good_node_2(path, x, &is_found2, &weight_prev, &weight, &node, &node_prev);
    if (is_found2) {
        v = node_prev;
        G.add_edge(n_ans, v, limb);
    } else {
        v = G.nodes->size();
        G.add_node(v);
        G.remove_edge(node_prev, node);
        G.add_edge(v, node_prev, x - weight);
        G.add_edge(v, node, weight_prev - x);
    }
    G.add_edge(n_ans, v, limb);

    return G;
}

int main() {

    int n;
    cin >> n;

    vector<vector<int>> mat;
    mat.resize(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {
            cin >> mat[i][j];
        }
    }
    auto g = AdditivePhylogeny(mat, n, n);
    g.print_adj_list();

}