#include <iostream>
#include<list>
#include <deque>
#include <string>
#include <algorithm>
#include <vector>

#define MAX_NUMBER_OF_NODES 100000
using namespace std;


class Node {
public:
    int value;
    list<Node *> *adj;

    Node(int value) : value(value) {
        adj = new list<Node *>;
    }
};

int in_edge_count[MAX_NUMBER_OF_NODES];
int out_edge_count[MAX_NUMBER_OF_NODES];
Node *graph_nodes[MAX_NUMBER_OF_NODES];
int max_node = -1;


void tokenize_by_arrow(string input, string *first_part, string *second_part) {
    string delim = " -> ";
    size_t finder = input.find(delim);
    *first_part = input.substr(0, finder);
    input.erase(0, finder + delim.size());
    *second_part = input;
}

vector<int> tokenize_by_comma(string input) {
    vector<int> answer;
    string delimiter = ",";
    size_t finder;
    while ((finder = input.find(delimiter)) != input.npos) {
        answer.push_back(stoi(input.substr(0, finder)));
        input.erase(0, finder + delimiter.length());
    }

    answer.push_back(stoi(input));
    return answer;


}

void build_graph(vector<string> inputs) {
    for (string input : inputs) {
        string first_part, second_part;
        tokenize_by_arrow(input, &first_part, &second_part);
        int first = stoi(first_part);
        if (graph_nodes[first] == NULL) {
            graph_nodes[first] = new Node(first);
        }
        if (first > max_node) {
            max_node = first;
        }
        for (int number : tokenize_by_comma(second_part)) {
            if (graph_nodes[number] == NULL) {
                graph_nodes[number] = new Node(number);
            }
            graph_nodes[first]->adj->push_back(graph_nodes[number]);
            in_edge_count[number]++;
            out_edge_count[first]++;
            if (number > max_node) {
                max_node = number;
            }
        }

    }
}

void find_start_and_end_edge_number(int *start, int *end) {
    for (int i = 0; i <= max_node; i++) {
        if (in_edge_count[i] == out_edge_count[i]) {
            continue;
        } else if (in_edge_count[i] > out_edge_count[i]) {
            *end = i;
        } else {
            *start = i;
        }
    }
}

vector<int> euler_path(int start) {
    vector<int> answer;
    deque<Node *> dq;
    dq.push_back(graph_nodes[start]);
    Node *current_node = graph_nodes[start];
    while (!dq.empty()) {
        if (current_node->adj->empty()) {
            answer.push_back(current_node->value);
            current_node = dq.back();
            dq.pop_back();
        } else {
            Node *new_node = current_node->adj->back();
            current_node->adj->pop_back();
            dq.push_back(current_node);
            current_node = new_node;
        }


    }
    reverse(answer.begin(), answer.end());
    return answer;
}

//WE WILL USE END AS SIGNAL FOR END OF INPUTS
//SO WE MUST ADD "END" TO THE LAST LINE OF INPUT
int main() {

    string input;
    vector<string> inputs;
    while (true) {
        getline(cin, input);
        if (input == "END") {
            break;
        }
        inputs.push_back(input);
    }
    build_graph(inputs);
    int start;
    int end;
    find_start_and_end_edge_number(&start, &end);

    auto answer = euler_path(start);
    for (int i = 0; i < answer.size() - 1; i++) {
        cout << answer[i] << "->";
    }
    cout << answer[answer.size() - 1];
    return 0;
}