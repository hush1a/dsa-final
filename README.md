# DSA Final - Practice Projects

This repository contains comprehensive implementations of data structures and algorithms through real-world application scenarios. Each project demonstrates multiple advanced DSA concepts integrated into practical systems.

## Projects Overview

### 1. Hospital Management System (`hospital_management_class_based.cpp`)
A complete hospital management system implementing multiple data structures using object-oriented design:

**Data Structures & Algorithms:**
- **Tree Structure**: Hierarchical organization of hospital departments and work units
- **Balanced BST**: Patient record storage using `std::map` for O(log n) operations
- **Graph with Dijkstra's Algorithm**: Doctor referral network with optimal pathfinding
- **DFS (Depth-First Search)**: Treatment combination analysis using recursive backtracking

**Classes:**
- `HospitalStructure`: Manages department-unit hierarchy
- `PatientRecordSystem`: Efficient patient record management
- `ReferralSystem`: Doctor referral network with shortest path finding
- `TreatmentPlanner`: Generates all possible treatment combinations

### 2. Content Moderation System (`content_moderation_system.cpp`)
An advanced content moderation platform for social media using multiple sophisticated algorithms:

**Data Structures & Algorithms:**
- **AVL Tree**: Self-balancing BST for banned words storage with O(log n) search
- **Graph**: Social network analysis for bot detection based on connectivity
- **KMP Algorithm**: Efficient string pattern matching for banned phrase detection
- **Priority Queue**: Severity-based content flagging system

**Features:**
- Automatic balancing AVL tree with rotations
- Advanced string matching with failure function computation
- Social graph analysis for isolated account detection
- Multi-factor severity scoring system

### 3. Food Delivery & Logistics Application (`food_delivery_and_logistics_application.cpp`)
A comprehensive delivery optimization system integrating multiple algorithms:

**Data Structures & Algorithms:**
- **Kruskal's MST**: Minimum spanning tree for optimal delivery network construction
- **Union-Find (Disjoint Set)**: Efficient cycle detection with path compression
- **Max Heap**: Priority-based order processing system
- **KMP String Matching**: Menu recommendation system

**Features:**
- Network cost optimization with promotional route discounts
- Priority queue for urgent order handling
- Efficient menu search with pattern matching
- Union by rank optimization for MST construction

### 4. E-Learning Platform (`e_learning.cpp`)
A complete e-learning management system using linked data structures:

**Data Structures & Algorithms:**
- **Linked Lists**: Course and topic management
- **Queue**: Multi-level assignment review system
- **Stack**: Rollback functionality for submission states
- **Student Progress Tracking**: Linked list-based completion tracking

**Features:**
- Dynamic course content management
- Three-tier review process (first → mid → final)
- Complete rollback system for submission states
- Student progress visualization

## Key DSA Concepts Demonstrated

### Trees & Graphs
- AVL tree with automatic balancing
- Graph traversal algorithms (Dijkstra, DFS)
- Minimum spanning tree construction
- Social network analysis

### Advanced Algorithms
- KMP string matching with failure function
- Union-Find with path compression and union by rank
- Priority queue implementations
- Backtracking and recursive algorithms

### Data Structure Integration
- Multiple data structures working together in single systems
- Object-oriented design patterns
- Efficient memory management
- Time complexity optimization

## Compilation & Running

Each file can be compiled independently:

```bash
g++ -o hospital hospital_management_class_based.cpp
g++ -o content_mod content_moderation_system.cpp
g++ -o delivery food_delivery_and_logistics_application.cpp
g++ -o elearning e_learning.cpp
```

## Complexity Analysis

All implementations focus on optimal time and space complexity:
- Search operations: O(log n) using balanced trees
- Graph algorithms: O(E log V) for Dijkstra, O(E log E) for Kruskal
- String matching: O(n + m) using KMP algorithm
- Priority operations: O(log n) using heap structures

These projects showcase practical applications of advanced data structures and algorithms in real-world scenarios, demonstrating both theoretical understanding and implementation skills.

