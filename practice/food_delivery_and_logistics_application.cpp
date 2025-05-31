#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// ----------- Edge Class -----------
/**
 * Edge class represents a weighted edge in the delivery network graph
 * Used for Kruskal's MST algorithm - stores source, destination, and weight
 */
class Edge {
public:
    int u, v, weight;  // u: source node, v: destination node, weight: delivery cost
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
    
    // Operator overloading for sorting edges by weight (ascending order)
    // Time Complexity: O(1)
    bool operator<(const Edge& other) const {
        return this->weight < other.weight;
    }
};

// ----------- Disjoint Set (Union-Find) -----------
/**
 * Disjoint Set data structure for Kruskal's MST algorithm
 * Implements path compression and union by rank for optimal performance
 * Time Complexity: O(α(n)) where α is inverse Ackermann function (nearly constant)
 */
class DisjointSet {
private:
    vector<int> parent, rank;  // parent[i]: parent of node i, rank[i]: depth of tree rooted at i
public:
    // Initialize each node as its own parent with rank 0
    DisjointSet(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i)
            parent[i] = i;  // Each node is initially its own parent
    }
    
    // Find operation with path compression optimization
    // Recursively finds root and compresses path for future queries
    int find(int u) {
        if (u != parent[u])
            parent[u] = find(parent[u]);  // Path compression: point directly to root
        return parent[u];
    }
    
    // Union operation with union by rank optimization
    // Merges two sets by attaching smaller tree under root of larger tree
    void unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            // Union by rank: attach smaller tree under larger tree
            if (rank[rootU] < rank[rootV]) parent[rootU] = rootV;
            else if (rank[rootU] > rank[rootV]) parent[rootV] = rootU;
            else {
                parent[rootV] = rootU;
                rank[rootU]++;  // Increase rank only when trees have equal rank
            }
        }
    }
};

// ----------- Order Class -----------
/**
 * Order class represents a food delivery order with priority
 * Higher priority value means more urgent delivery
 */
class Order {
public:
    int id;       // Unique order identifier
    int priority; // Priority level (higher = more urgent)
    Order(int id, int priority) : id(id), priority(priority) {}
};

// ----------- Max Heap for Orders -----------
/**
 * Max Heap implementation for priority queue of orders
 * Processes orders based on priority (highest priority first)
 * Time Complexity: Insert O(log n), Extract O(log n)
 */
class MaxHeap {
private:
    vector<Order> heap;  // Dynamic array to store heap elements

    // Heapify up: Restore heap property after insertion
    // Moves newly inserted element up until heap property is satisfied
    void heapifyUp(int index) {
        while (index > 0 && heap[index].priority > heap[(index - 1) / 2].priority) {
            swap(heap[index], heap[(index - 1) / 2]);  // Swap with parent
            index = (index - 1) / 2;  // Move to parent position
        }
    }

    // Heapify down: Restore heap property after extraction
    // Moves root element down until heap property is satisfied
    void heapifyDown(int index) {
        int size = heap.size();
        while (2 * index + 1 < size) {  // While node has at least left child
            int largest = index;
            int left = 2 * index + 1;
            int right = 2 * index + 2;

            // Find largest among node and its children
            if (left < size && heap[left].priority > heap[largest].priority)
                largest = left;
            if (right < size && heap[right].priority > heap[largest].priority)
                largest = right;

            // If largest is not current node, swap and continue
            if (largest != index) {
                swap(heap[index], heap[largest]);
                index = largest;
            } else break;  // Heap property satisfied
        }
    }

public:
    // Insert order into heap and maintain max heap property
    void insert(Order o) {
        heap.push_back(o);           // Add to end of array
        heapifyUp(heap.size() - 1);  // Restore heap property
    }

    // Extract order with maximum priority
    Order extractMax() {
        if (heap.empty()) return Order(-1, -1);  // Return invalid order if empty
        Order top = heap[0];         // Store maximum element
        heap[0] = heap.back();       // Move last element to root
        heap.pop_back();             // Remove last element
        heapifyDown(0);              // Restore heap property from root
        return top;
    }

    bool isEmpty() {
        return heap.empty();
    }
};

// ----------- KMP String Matching Algorithm -----------
/**
 * Menu Recommender using KMP (Knuth-Morris-Pratt) algorithm
 * Efficiently searches for keywords in menu items
 * Time Complexity: O(n + m) where n = text length, m = pattern length
 */
class MenuRecommender {
public:
    // Compute Longest Proper Prefix which is also Suffix (LPS) array
    // Used to avoid redundant character comparisons in KMP algorithm
    static vector<int> computeLPS(string pattern) {
        int m = pattern.length();
        vector<int> lps(m, 0);  // LPS array initialization
        int len = 0, i = 1;     // len: length of previous longest prefix suffix
        
        while (i < m) {
            if (pattern[i] == pattern[len]) {
                lps[i++] = ++len;  // Character matches, extend LPS
            } else {
                if (len != 0) len = lps[len - 1];  // Mismatch: use previous LPS value
                else lps[i++] = 0;  // No proper prefix suffix exists
            }
        }
        return lps;
    }

    // KMP pattern matching algorithm
    // Searches for pattern in text using preprocessed LPS array
    static bool containsKeyword(string text, string pattern) {
        int n = text.length(), m = pattern.length();
        vector<int> lps = computeLPS(pattern);  // Preprocess pattern
        int i = 0, j = 0;  // i: text index, j: pattern index
        
        while (i < n) {
            if (pattern[j] == text[i]) {
                i++; j++;  // Characters match, advance both pointers
            }
            if (j == m) return true;  // Complete pattern found
            else if (i < n && pattern[j] != text[i]) {
                // Mismatch: use LPS to skip characters intelligently
                if (j != 0) j = lps[j - 1];  // Jump to previous border
                else i++;  // No border exists, advance text pointer
            }
        }
        return false;  // Pattern not found
    }
};

// ----------- Core Delivery Network System -----------
/**
 * Main system integrating all algorithms:
 * 1. Kruskal's MST for minimum cost network construction
 * 2. Max Heap for priority-based order processing
 * 3. KMP for intelligent menu recommendation
 */
class DeliveryNetwork {
private:
    int numNodes;                    // Number of delivery locations
    vector<Edge> edges;              // All possible delivery routes
    MaxHeap orderHeap;              // Priority queue for order management
    vector<string> menuItems;       // Available menu items for recommendation

public:
    DeliveryNetwork(int numNodes) : numNodes(numNodes) {}

    // Add delivery route with optional promotional discount
    // Promotional routes get 20% cost reduction
    void addRoute(int u, int v, int cost, bool hasPromo = false) {
        if (hasPromo) cost = cost * 0.8;  // Apply 20% discount for promotional routes
        edges.push_back(Edge(u, v, cost));
    }

    // Add order to priority queue for processing
    void addOrder(int id, int priority) {
        orderHeap.insert(Order(id, priority));
    }

    // Process all orders in priority order (highest priority first)
    // Uses max heap to ensure optimal order processing sequence
    void processOrders() {
        cout << "\nProcessing Orders by Priority:\n";
        while (!orderHeap.isEmpty()) {
            Order o = orderHeap.extractMax();  // Get highest priority order
            cout << "Order ID: " << o.id << ", Priority: " << o.priority << "\n";
        }
    }

    // Add menu item to recommendation system
    void addMenuItem(string item) {
        menuItems.push_back(item);
    }

    // Recommend menu items containing specified keyword
    // Uses KMP algorithm for efficient string matching
    void recommendMenus(string keyword) {
        cout << "\nMenu Recommendations for: " << keyword << "\n";
        for (const string& item : menuItems) {
            // Use KMP algorithm to check if menu item contains keyword
            if (MenuRecommender::containsKeyword(item, keyword))
                cout << "- " << item << "\n";
        }
    }

    // Build minimum spanning tree using Kruskal's algorithm
    // Returns minimum cost to connect all delivery locations
    // Time Complexity: O(E log E) where E is number of edges
    int buildMinimumCostNetwork() {
        // Step 1: Sort all edges by weight in ascending order
        sort(edges.begin(), edges.end());
        
        // Step 2: Initialize disjoint set for cycle detection
        DisjointSet ds(numNodes);

        int totalCost = 0;
        cout << "\nSelected Routes in Minimum Cost Network:\n";
        
        // Step 3: Process edges in sorted order (Kruskal's algorithm)
        for (Edge e : edges) {
            // Check if adding this edge creates a cycle
            if (ds.find(e.u) != ds.find(e.v)) {
                ds.unite(e.u, e.v);          // Add edge to MST
                totalCost += e.weight;        // Add cost to total
                cout << "Route: " << e.u << " <-> " << e.v << " | Cost: " << e.weight << "\n";
            }
            // If edge creates cycle, skip it (greedy choice)
        }
        return totalCost;
    }
};

// ----------- Main Driver -----------
/**
 * Demonstration of the complete food delivery and logistics system
 * Shows integration of MST, Priority Queue, and String Matching algorithms
 */
int main() {
    // Initialize delivery network with 6 locations (nodes 0-5)
    DeliveryNetwork dn(6);

    // Add delivery routes between locations
    // Format: addRoute(source, destination, cost, hasPromo)
    dn.addRoute(0, 1, 4);
    dn.addRoute(0, 2, 4);
    dn.addRoute(1, 2, 2, true);  // Promotional route with discount
    dn.addRoute(1, 3, 5);
    dn.addRoute(2, 3, 8);
    dn.addRoute(2, 4, 10);
    dn.addRoute(3, 4, 2);
    dn.addRoute(3, 5, 6, true); // Promotional route with discount
    dn.addRoute(4, 5, 3);

    // Add orders with different priorities for demonstration
    // Format: addOrder(orderID, priority)
    dn.addOrder(101, 2);  // Low priority
    dn.addOrder(102, 5);  // High priority (processed first)
    dn.addOrder(103, 3);  // Medium priority

    // Add menu items for recommendation system
    dn.addMenuItem("Spicy Chicken Rice");
    dn.addMenuItem("Sweet and Sour Pork");
    dn.addMenuItem("Vegetarian Salad");
    dn.addMenuItem("Grilled Chicken Wrap");

    // Execute system operations
    dn.processOrders();           // Process orders by priority using max heap
    dn.recommendMenus("Chicken"); // Find menu items containing "Chicken" using KMP
    
    // Build minimum cost network using Kruskal's MST algorithm
    int minCost = dn.buildMinimumCostNetwork();
    cout << "\nTotal Minimum Cost to Build Network: " << minCost << "\n";

    return 0;
}
