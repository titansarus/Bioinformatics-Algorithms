#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Node {
public:
    string value;
    list<Node *> adj;

    explicit Node(const string &value) : value(value) {
        list<Node *> temp;
        adj = temp;
    }
};

map<string, Node *> build_graph(string text, int k) {
    map<string, Node *> graph;
    if (text.size() < k) {
        return graph;
    }
    int length = k - 1;
    int i = 0;
    Node *first;
    Node *second;
    while (i + 1 + length <= text.size()) {
        string first_substr = text.substr(i, length);
        string second_substr = text.substr(i + 1, length);
        if (graph.find(first_substr) == graph.end()) {
            first = new Node(first_substr);
            graph[first_substr] = first;
        }
        if (graph.find(second_substr) == graph.end()) {
            second = new Node(second_substr);
            graph[second_substr] = second;
        }
        first = graph[first_substr];
        second = graph[second_substr];
        first->adj.push_back(second);
        i++;
    }
    return graph;
}

string answer;

int main() {
    int k;
    string text;
    cin >> k;
    cin >> text;
    auto graph = build_graph(text, k);
    for (auto &it : graph) {
        string str = it.first;
        Node *n = it.second;
        if (!n->adj.empty()) {
            answer += n->value + " -> ";
            int end_counter = 0;
            vector<string> connected_strings;
            for (auto sub_node : n->adj) {
                connected_strings.push_back(sub_node->value);
            }
            sort(connected_strings.begin(), connected_strings.end());

            for (auto s : connected_strings) {
                if (end_counter == n->adj.size() - 1) {
                    answer += s;
                } else {
                    answer += s + ",";
                }
                end_counter++;
            }
            answer += "\n";
        }

    }
    cout << answer;
    return 0;
}