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


void
find_closest_clusters(unordered_map<Node *, vector<Node *>> &clusters, vector<vector<float>> &D, UndirectedGraph &G,
                      Node **first_node, Node **second_node, float *best_distance) {

    *best_distance = (float) +10000000000;

    for (int i = 0; i < D.size(); i++) {
        for (int j = 0; j < i; j++) {
            if (i != j) {
                auto Node_i = G.find_node(i);
                auto Node_j = G.find_node(j);
                if (clusters.find(Node_i) != clusters.end() && clusters.find(Node_j) != clusters.end()) {
                    if (D[Node_i->number][Node_j->number] < *best_distance) {
                        *first_node = Node_i;
                        *second_node = Node_j;
                        *best_distance = (D[Node_i->number][Node_j->number]);
                    }
                }
            }
        }
    }


}

float
distance(unordered_map<Node *, vector<Node *>> &clusters, vector<vector<float>> &D, UndirectedGraph &G, int i, int j) {

    auto Node_i = G.find_node(i);
    auto Node_j = G.find_node(j);
    float answer = +1000000000;
    if (clusters.find(Node_i) != clusters.end() && clusters.find(Node_j) != clusters.end()) {
        answer = 0;
        for (auto ni : clusters[Node_i]) {
            for (auto nj : clusters[Node_j]) {
                answer = answer + D[ni->number][nj->number];
            }
        }
    } else {
        return answer;
    }
    return answer / (clusters[Node_j].size() * clusters[Node_i].size() * 1.00);

}

UndirectedGraph UPGMA(vector<vector<float>> D, int n, int initial_size) {

    auto G = UndirectedGraph(n);
    unordered_map<Node *, float> age;

    unordered_map<Node *, vector<Node *>> clusters;

    for (auto node : *G.nodes) {
        vector<Node *> temp;
        temp.push_back(node);
        clusters[node] = temp;
    }

    for (auto node : *G.nodes) {
        age[node] = 0;
    }


    while (clusters.size() > 1) {
        Node *inode;
        Node *jnode;
        float best_distance;
        find_closest_clusters(clusters, D, G, &inode, &jnode, &best_distance);
        int next_node_number = G.nodes->size();
        Node *next_node = G.add_node(next_node_number);
        G.add_edge(next_node_number, inode->number, 1);
        G.add_edge(next_node_number, jnode->number, 1);
        vector<Node *> temp;
        for (auto nd : clusters[inode]) {
            temp.push_back(nd);
        }
        for (auto nd : clusters[jnode]) {
            temp.push_back(nd);
        }

        clusters[next_node] = temp;

        age[next_node] = D[inode->number][jnode->number] / 2.000;

        clusters.erase(inode);
        clusters.erase(jnode);

        vector<float> row;
        for (int i = 0; i < D.size(); i++) {
            float number = distance(clusters, D, G, i, next_node_number);
            row.push_back(number);
        }
        row.push_back(0);
        for (int i = 0; i < D.size(); i++) {
            D[i].push_back(row[i]);
        }
        D.push_back(row);
    }

    for (auto edge : *G.edges) {
        edge->weight = fabs(age[edge->start] - age[edge->end]);
    }
    return G;
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
    auto g = UPGMA(mat, n, n);
    g.print_adj_list();

}