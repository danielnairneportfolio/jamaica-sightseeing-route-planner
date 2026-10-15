// PLEASE USE C++ 17 TO MAKE THIS CODE RUN PROPERLY
// PLEASE USE C++ 17 TO MAKE THIS CODE RUN PROPERLY
// PLEASE USE C++ 17 TO MAKE THIS CODE RUN PROPERLY

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <algorithm>
#include <cfloat>
#include <climits>
#include <set>

using namespace std;

string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" ");
    size_t end = str.find_last_not_of(" ");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

class SightseeingMap {
private:
    unordered_map<string, vector<pair<string, int>>> adjacencyList;

public:

    void addEdge(const string& capital1, const string& capital2, int distance) {
        string c1 = toLower(capital1);
        string c2 = toLower(capital2);
        adjacencyList[c1].emplace_back(c2, distance);
        adjacencyList[c2].emplace_back(c1, distance); 
    }

    bool isCapitalValid(const string& capital) const {
        return adjacencyList.find(toLower(trim(capital))) != adjacencyList.end();
    }

    void suggestAdjacent(const string& capital) const {
        string key = toLower(trim(capital));
        if (!isCapitalValid(capital)) {
            cout << "Capital " << capital << " Not found in the map.\n";
            return;
        }

        cout << "Adjacent capitals to " << capital << ":\n";
        for (const auto& neighbor : adjacencyList.at(key)) {
            cout << "- " << neighbor.first << " (" << neighbor.second << " km)\n";
        }
    }

    const unordered_map<string, vector<pair<string, int>>>& getMap() const {
        return adjacencyList;
    }

    void printAdjacencyList() const {
        cout << "\n--- Adjacency List ---\n";
        for (const auto& [capital, neighbors] : adjacencyList) {
            cout << capital << ": ";
            for (const auto& [neighbor, distance] : neighbors) {
                cout << "(" << neighbor << ", " << distance << " km) ";
            }
            cout << endl;
        }
        cout << "----------------------\n";
    }
};

pair<vector<string>, int> findShortestPath(
    const unordered_map<string, vector<pair<string, int>>>& graph,
    const string& start, const string& destination) {
    unordered_map<string, int> distances;
    unordered_map<string, string> previous;
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;

    string lowerStart = toLower(trim(start));
    string lowerDestination = toLower(trim(destination));

    if (graph.find(lowerStart) == graph.end() || graph.find(lowerDestination) == graph.end()) {
        return {{}, -1};
    }

    for (const auto& node : graph) {
        distances[node.first] = numeric_limits<int>::max();
    }
    distances[lowerStart] = 0;
    pq.emplace(0, lowerStart);

    while (!pq.empty()) {
        auto [currentDistance, current] = pq.top();
        pq.pop();

        if (current == lowerDestination) break;

        for (const auto& [neighbor, weight] : graph.at(current)) {
            int newDistance = currentDistance + weight;
            if (newDistance < distances[neighbor]) {
                distances[neighbor] = newDistance;
                previous[neighbor] = current;
                pq.emplace(newDistance, neighbor);
            }
        }
    }

    vector<string> path;
    for (string at = lowerDestination; !at.empty(); at = previous[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    return (path.front() == lowerStart) ? make_pair(path, distances[lowerDestination]) : make_pair(vector<string>{}, -1);
}

void greedy(const unordered_map<string, vector<pair<string, int>>>& capitalMap, const string& startCapital) {
    string currentCapital = toLower(trim(startCapital));

    if (capitalMap.find(currentCapital) == capitalMap.end()) {
        cout << "Starting capital " << startCapital << " Not found in the map.\n";
        return;
    }

    unordered_map<string, bool> visited;
    vector<string> sightseeingOrder;

    while (visited.size() < capitalMap.size()) {
        sightseeingOrder.push_back(currentCapital);
        visited[currentCapital] = true;

        string nextCapital = "";
        int minConnections = INT_MAX;

        for (const auto& connectedNode : capitalMap.at(currentCapital)) {
            if (!visited[connectedNode.first]) {
                int connections = capitalMap.at(connectedNode.first).size();
                if (connections < minConnections) {
                    minConnections = connections;
                    nextCapital = connectedNode.first;
                }
            }
        }

        if (nextCapital.empty()) break;
        currentCapital = nextCapital;
    }

    cout << "The Greedy algorithm design sightseeing order:\n";
    for (const string& capital : sightseeingOrder) {
        cout << capital << " ";
    }
    cout << endl;
}

int main() {
    SightseeingMap map;

    map.addEdge("Kingston", "Spanish Town", 25);
    map.addEdge("Spanish Town", "May Pen", 40);
    map.addEdge("May Pen", "Mandeville", 40);
    map.addEdge("Mandeville", "Black River", 60);
    map.addEdge("Black River", "Savanna-la-Mar", 47);
    map.addEdge("Savanna-la-Mar", "Montego Bay", 50);
    map.addEdge("Montego Bay", "Falmouth", 34);
    map.addEdge("Falmouth", "St Ann's Bay", 58);
    map.addEdge("St Ann's Bay", "Port Maria", 43);
    map.addEdge("Port Maria", "Port Antonio", 70);
    map.addEdge("Port Antonio", "Kingston", 89);

    int choice = 0;

    do {
        cout << "\n--- Welcome to the Jamaica Sightseeing App! ---\n";
        cout << "1. Find shortest path between two capitals\n";
        cout << "2. Suggest adjacent capitals\n";
        cout << "3. Exit\n";
        cout << "4. Debug: Print the adjacency list\n";
        cout << "5. Greedy sightseeing\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                string start, destination;
                cout << "Enter the starting capital: ";
                cin.ignore();
                getline(cin, start);
                cout << "Enter the destination capital: ";
                getline(cin, destination);

                auto [path, distance] = findShortestPath(map.getMap(), start, destination);
                if (path.empty()) {
                    cout << "No path found between " << start << " and " << destination << ".\n";
                } else {
                    cout << "Shortest path: ";
                    for (const auto& capital : path) {
                        cout << capital << " ";
                    }
                    cout << "\nTotal distance: " << distance << " km\n";
                }
                break;
            }
            case 2: {
                string capital;
                cout << "Enter the capital: ";
                cin.ignore();
                getline(cin, capital);
                map.suggestAdjacent(capital);
                break;
            }
            case 3:
                cout << "Thank you for using the Jamaica Sightseeing App. See you later!\n";
                break;
            case 4:
                map.printAdjacencyList();
                break;
            case 5: {
                string startCapital;
                cout << "Enter the starting capital for sightseeing: ";
                cin.ignore();
                getline(cin, startCapital);
                greedy(map.getMap(), startCapital);
                break;
            }
            default:
                cout << "Invalid choice. Please select an option from the menu.\n";
        }
    } while (choice != 3);

    return 0;
}
