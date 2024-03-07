#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

struct Edge {
    int destination, weight;
};

using Graph = vector<vector<Edge>>;
vector<int> dijkstra(const Graph& graph, int source) {
    int n = graph.size();
    vector<int> distance(n, numeric_limits<int>::max());
    distance[source] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, source});
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        for (const Edge& edge : graph[u]) {
            int v = edge.destination, w = edge.weight;
            if (distance[u] + w < distance[v]) {
                distance[v] = distance[u] + w;
                pq.push({distance[v], v});
            }
        }
    }
    return distance;
}

struct Activity {
    string name;
    int priority, time;
    Activity* left = nullptr;
    Activity* right = nullptr;
    Activity(const string& n, int p, int t) : name(n), priority(p), time(t) {}
};

void insertActivity(Activity*& root, const string& name, int priority, int time) {
    if (!root) {
        root = new Activity(name, priority, time);
    } else {
        insertActivity(priority < root->priority ? root->left : root->right, name, priority, time);
    }
}

void inOrderTraversal(Activity* root) {
    if (root) {
        inOrderTraversal(root->left);
        cout << "Activity: " << root->name << ", Priority: " << root->priority << ", Time: " << root->time << endl;
        inOrderTraversal(root->right);
    }
}

struct ItineraryNode {
    string activityName;
    ItineraryNode* next = nullptr;
    ItineraryNode(const string& name) : activityName(name) {}
};

void addToItinerary(ItineraryNode*& head, const string& activityName) {
    auto newNode = new ItineraryNode(activityName);
    newNode->next = head;
    head = newNode;
}

void displayItinerary(const ItineraryNode* head) {
    for (const ItineraryNode* current = head; current; current = current->next) {
        cout << "->" << current->activityName;
    }
    cout << endl;
}

struct Expense {
    string description;
    int amount;
    Expense(const string& desc, int amt) : description(desc), amount(amt) {}
    bool operator>(const Expense& other) const { return amount > other.amount; }
};

void manageBudget(priority_queue<Expense, vector<Expense>, greater<>>& budgetQueue, int budget) {
    int totalSpent = 0;
    while (!budgetQueue.empty() && totalSpent < budget) {
        Expense expense = budgetQueue.top();
        budgetQueue.pop();
        if (totalSpent + expense.amount <= budget) {
            totalSpent += expense.amount;
            cout << "Spent $" << expense.amount << " on: " << expense.description << endl;
        }
    }
    cout << "Total spent: $" << totalSpent << endl;
}

int main() {
    Graph graph(5);
    graph[0] = {{1, 10}, {2, 15}};
    graph[1] = {{3, 12}};
    graph[2] = {{3, 10}};
    graph[3] = {{4, 5}};

    int source, destination;
    cout << "Enter source destination: ";
    cin >> source >> destination;
    vector<int> distances = dijkstra(graph, source);
    cout << "Shortest path distance from " << source << " to " << destination << ": " << distances[destination] << endl;

    Activity* scheduleTree = nullptr;
    insertActivity(scheduleTree, "Visit Museum", 2, 3);
    insertActivity(scheduleTree, "Explore Park", 1, 4);
    insertActivity(scheduleTree, "Dinner at Restaurant", 3, 2);
    cout << "\nActivity Schedule: \n";
    inOrderTraversal(scheduleTree);

    ItineraryNode* itineraryHead = nullptr;
    addToItinerary(itineraryHead, "Visit Museum");
    addToItinerary(itineraryHead, "Explore Park");
    addToItinerary(itineraryHead, "Dinner at Restaurant");
    cout << "\nItinerary: \n";
    displayItinerary(itineraryHead);

    priority_queue<Expense, vector<Expense>, greater<>> budgetQueue;
    budgetQueue.push({"Museum Ticket", 20});
    budgetQueue.push({"Park Entry", 15});
    budgetQueue.push({"Dinner", 30});
    int budget;
    cout << "\nEnter your budget: $";
    cin >> budget;
    cout << "\nBudget Management: \n";
    manageBudget(budgetQueue, budget);
    return 0;
}
