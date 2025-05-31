/*
 * Hospital Management System - Class-Based Implementation
 * This file implements a comprehensive hospital management system using object-oriented design:
 * - HospitalStructure class: Tree-based organizational hierarchy management
 * - PatientRecordSystem class: Balanced BST for patient data storage
 * - ReferralSystem class: Graph-based doctor referral network with pathfinding
 * - TreatmentPlanner class: DFS-based treatment combination analysis
 */

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>
#include <set>
#include <algorithm>

using namespace std;

// ============================
// Class: HospitalStructure (Tree for Departments and Work Units)
// ============================

// Manages the hierarchical organizational structure of the hospital
// Uses a tree-like structure where departments contain multiple work units
class HospitalStructure {
public:
    // Represents a work unit (team) within a department
    struct WorkUnit {
        string name;               // Name of the work unit (e.g., "ICU Team A")
        vector<string> members;    // List of staff members in this unit
    };

    // Represents a hospital department containing multiple work units
    struct Department {
        string name;               // Department name (e.g., "Cardiology")
        vector<WorkUnit> units;    // Collection of work units within department
    };

    // Map storing all departments using balanced BST for efficient access
    // Key: department name, Value: department object with all its units
    map<string, Department> departments;

    // Adds a new work unit to a specified department
    // Creates the department if it doesn't exist
    // Time complexity: O(log n) for department lookup + O(1) for unit addition
    void addWorkUnit(string deptName, string unitName, vector<string> members) {
        WorkUnit unit = {unitName, members};
        departments[deptName].name = deptName;  // Ensure department exists and is named
        departments[deptName].units.push_back(unit);  // Add unit to department
    }

    // Displays the complete hospital organizational structure
    // Iterates through all departments and their work units
    void displayStructure() {
        for (const auto& [deptName, dept] : departments) {
            cout << "Department: " << deptName << "\n";
            // Display all units within this department
            for (const auto& unit : dept.units) {
                cout << "  Unit: " << unit.name << " Members: ";
                // Display all members of this unit
                for (const auto& m : unit.members) cout << m << " ";
                cout << endl;
            }
        }
    }
};

// ============================
// Class: PatientRecordSystem (Balanced BST using std::map)
// ============================

// Manages patient records using a balanced binary search tree for efficient operations
// Provides O(log n) search, insert, and delete operations
class PatientRecordSystem {
private:
    // Balanced BST storing patient records
    // Key: patient ID, Value: patient record information
    map<string, string> records;

public:
    // Adds a new patient record to the system
    // If patient ID already exists, updates the existing record
    // Time complexity: O(log n)
    void addRecord(string id, string record) {
        records[id] = record;
    }

    // Searches for a patient record by ID
    // Returns the record if found, otherwise returns error message
    // Time complexity: O(log n)
    string searchRecord(string id) {
        if (records.find(id) != records.end())
            return records[id];
        return "Record not found";
    }
};

// ============================
// Class: ReferralSystem (Graph with Adjacency List)
// ============================

// Manages the doctor referral network using graph data structure
// Implements Dijkstra's algorithm to find optimal referral paths
class ReferralSystem {
private:
    // Adjacency list representation of the referral network
    // Key: doctor name, Value: vector of (referred_doctor, referral_cost) pairs
    unordered_map<string, vector<pair<string, int>>> graph;

public:
    // Adds a referral relationship between two doctors
    // 'from' doctor can refer patients to 'to' doctor with specified cost/time
    void addReferral(string from, string to, int cost) {
        graph[from].push_back({to, cost});
    }

    // Finds the fastest/cheapest referral path between two doctors using Dijkstra's algorithm
    // Useful for determining optimal patient referral chains in complex hospital networks
    void findFastestPath(string start, string end) {
        // Distance map: shortest distance from start to each doctor
        unordered_map<string, int> dist;
        // Previous node map: used for reconstructing the optimal path
        unordered_map<string, string> prev;
        // Priority queue: (distance, doctor) pairs ordered by distance
        set<pair<int, string>> pq;

        // Initialize all distances to infinity
        for (auto& p : graph)
            dist[p.first] = numeric_limits<int>::max();

        // Set starting doctor distance to 0 and add to priority queue
        dist[start] = 0;
        pq.insert({0, start});

        // Main Dijkstra's algorithm loop
        while (!pq.empty()) {
            auto [d, u] = *pq.begin();  // Extract doctor with minimum distance
            pq.erase(pq.begin());

            // Examine all doctors that current doctor can refer to
            for (auto& [v, cost] : graph[u]) {
                // If we found a shorter path to doctor v through u
                if (dist[v] > dist[u] + cost) {
                    pq.erase({dist[v], v});     // Remove old distance entry
                    dist[v] = dist[u] + cost;   // Update with shorter distance
                    prev[v] = u;                // Record path for reconstruction
                    pq.insert({dist[v], v});    // Insert updated distance
                }
            }
        }

        // Check if destination doctor is reachable
        if (dist[end] == numeric_limits<int>::max()) {
            cout << "No path found.\n";
            return;
        }

        // Reconstruct and display the optimal referral path
        vector<string> path;
        for (string at = end; at != ""; at = prev[at])
            path.push_back(at);

        reverse(path.begin(), path.end());
        cout << "Fastest referral path (cost = " << dist[end] << "): ";
        for (auto& doc : path) cout << doc << " ";
        cout << endl;
    }
};

// ============================
// Class: TreatmentPlanner (DFS for Combinations)
// ============================

// Analyzes different treatment combinations using Depth-First Search
// Generates all possible subsets of available treatments for comprehensive planning
class TreatmentPlanner {
public:
    // Generates all possible combinations of treatments using recursive DFS
    // Implements powerset algorithm to explore all treatment possibilities
    // Parameters:
    //   treatments: vector of available treatments
    //   current: current combination being built
    //   idx: current index in treatments vector
    void dfsCombinations(vector<string>& treatments, vector<string>& current, int idx) {
        // Base case: processed all treatments, output current combination
        if (idx == treatments.size()) {
            cout << "Combination: ";
            for (const auto& t : current) cout << t << " ";
            cout << endl;
            return;
        }
        
        // Recursive case 1: Include current treatment in combination
        current.push_back(treatments[idx]);
        dfsCombinations(treatments, current, idx + 1);
        current.pop_back();  // Backtrack to explore other possibilities
        
        // Recursive case 2: Exclude current treatment from combination
        dfsCombinations(treatments, current, idx + 1);
    }
};

// ============================
// Main Function to Demonstrate Classes
// ============================

int main() {
    // Demonstrate hospital organizational structure management
    HospitalStructure hs;
    hs.addWorkUnit("Cardiology", "HeartTeamA", {"Dr. A", "Nurse B"});
    hs.addWorkUnit("Neurology", "BrainUnitX", {"Dr. C", "Nurse D"});
    hs.displayStructure();

    // Demonstrate patient record management system
    PatientRecordSystem prs;
    prs.addRecord("P123", "John Doe - Checkup 2023");
    prs.addRecord("P456", "Jane Smith - Surgery 2022");
    cout << "Search record P123: " << prs.searchRecord("P123") << endl;

    // Demonstrate referral network and optimal pathfinding
    ReferralSystem rs;
    rs.addReferral("Dr. A", "Dr. B", 5);    // Direct referral with cost 5
    rs.addReferral("Dr. B", "Dr. C", 3);    // Continuation of referral chain
    rs.addReferral("Dr. A", "Dr. D", 10);   // Alternative path with higher cost
    rs.addReferral("Dr. D", "Dr. C", 1);    // Cheaper final step
    rs.findFastestPath("Dr. A", "Dr. C");   // Find optimal A->C path

    // Demonstrate treatment combination analysis
    TreatmentPlanner tp;
    vector<string> treatments = {"Med1", "TherapyA", "SurgeryX"};
    vector<string> path;
    tp.dfsCombinations(treatments, path, 0);  // Generate all treatment combinations

    return 0;
}
