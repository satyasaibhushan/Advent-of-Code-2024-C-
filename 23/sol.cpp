#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include <map>
#include <sstream>
#include <chrono>
#include <set>
#include <algorithm>

using namespace std;

// Find the largest group where every node is directly connected to every other node
string findLargestClique(const set<string> &nodes, const map<string, set<string>> &connections)
{
    // Start with just one node, then try to expand
    set<string> bestClique;

    for (const auto &startNode : nodes)
    {
        set<string> potentialClique = {startNode};
        set<string> candidates = connections.at(startNode); // All neighbors of the start node

        // Keep expanding with nodes that are connected to all current nodes
        while (!candidates.empty())
        {
            // Find a node that connects to all nodes in the current clique
            bool foundCandidate = false;
            string nextNode;

            for (const auto &candidate : candidates)
            {
                bool connectsToAll = true;

                // Check if this candidate connects to all nodes in the current clique
                for (const auto &cliqueNode : potentialClique)
                {
                    if (connections.at(cliqueNode).find(candidate) == connections.at(cliqueNode).end())
                    {
                        connectsToAll = false;
                        break;
                    }
                }

                if (connectsToAll)
                {
                    nextNode = candidate;
                    foundCandidate = true;
                    break;
                }
            }

            if (foundCandidate)
            {
                // Add the node to the clique
                potentialClique.insert(nextNode);

                // Remove it from candidates
                candidates.erase(nextNode);

                // Filter candidates to only those connected to the new node
                set<string> newCandidates;
                for (const auto &candidate : candidates)
                {
                    if (connections.at(nextNode).find(candidate) != connections.at(nextNode).end())
                    {
                        newCandidates.insert(candidate);
                    }
                }
                candidates = newCandidates;
            }
            else
            {
                // No more candidates can be added
                break;
            }
        }

        // Check if this clique is larger than our best so far
        if (potentialClique.size() > bestClique.size())
        {
            bestClique = potentialClique;
        }
    }

    // Convert to sorted vector and format as string
    vector<string> cliqueVec(bestClique.begin(), bestClique.end());
    sort(cliqueVec.begin(), cliqueVec.end());

    string result;
    for (size_t i = 0; i < cliqueVec.size(); i++)
    {
        if (i > 0)
            result += ",";
        result += cliqueVec[i];
    }

    cout << "Found largest clique with " << bestClique.size() << " nodes" << endl;
    return result;
}

long long solve(vector<pair<string, string>> A)
{
    // Implementation for Part 1 (counting triangles with a 't' node)
    map<string, vector<string>> graph;
    set<string> nodes;
    set<tuple<string, string, string>> triples;
    long long ans = 0;

    for (auto &[a, b] : A)
    {
        graph[a].push_back(b);
        graph[b].push_back(a);
        nodes.insert(a);
        nodes.insert(b);
    }

    // Find all triangles (cycles of length 3)
    for (auto &node : nodes)
    {
        vector<string> neighbors = graph[node];
        for (size_t i = 0; i < neighbors.size(); i++)
        {
            for (size_t j = i + 1; j < neighbors.size(); j++)
            {
                string neighbor1 = neighbors[i];
                string neighbor2 = neighbors[j];

                // Check if neighbor1 and neighbor2 are connected
                if (find(graph[neighbor1].begin(), graph[neighbor1].end(), neighbor2) != graph[neighbor1].end())
                {
                    vector<string> triple = {node, neighbor1, neighbor2};
                    sort(triple.begin(), triple.end());

                    // Create a tuple for the sorted triple
                    tuple<string, string, string> triplet(triple[0], triple[1], triple[2]);

                    // Only count if not already seen
                    if (triples.find(triplet) == triples.end())
                    {
                        triples.insert(triplet);

                        // Count if at least one node starts with 't'
                        if (triple[0][0] == 't' || triple[1][0] == 't' || triple[2][0] == 't')
                        {
                            ans++;
                        }
                    }
                }
            }
        }
    }

    return ans;
}

string solve2(vector<pair<string, string>> A)
{
    set<string> nodes;
    map<string, set<string>> connections;

    for (auto &[a, b] : A)
    {
        nodes.insert(a);
        nodes.insert(b);
        connections[a].insert(b);
        connections[b].insert(a);
    }
    return findLargestClique(nodes, connections);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = chrono::high_resolution_clock::now();

    ifstream file("./question");
    string line;
    vector<pair<string, string>> A;

    cout << "Reading input file..." << endl;
    while (getline(file, line))
    {
        A.push_back(make_pair(line.substr(0, 2), line.substr(3, 2)));
    }
    cout << "Read " << A.size() << " edges" << endl;

    long long ans = solve(A);
    string ans2 = solve2(A);

    cout << ans << endl;
    cout << ans2 << endl;

    cout << "Run time - "
         << chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;

    return 0;
}
