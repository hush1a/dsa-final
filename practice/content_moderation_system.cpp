#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

// ----------- AVL Tree Implementation for Banned Words Storage -----------
/**
 * AVL Tree Node structure for storing banned words efficiently
 * Provides O(log n) search, insert operations with automatic balancing
 */
struct AVLNode {
    string key;          // The banned word
    AVLNode* left;       // Left child pointer
    AVLNode* right;      // Right child pointer
    int height;          // Height of node for balancing calculations

    AVLNode(string k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

/**
 * Get height of AVL node (0 for null nodes)
 * Used for balance factor calculation
 */
int getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

/**
 * Calculate balance factor of a node
 * Balance factor = height(left subtree) - height(right subtree)
 * AVL property: balance factor must be in range [-1, 1]
 */
int getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

/**
 * Right rotation for AVL tree balancing
 * Used when left subtree is heavier (left-left or left-right case after left rotation)
 * Time Complexity: O(1)
 */
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;      // x becomes new root
    AVLNode* T2 = x->right;    // Store x's right subtree

    // Perform rotation
    x->right = y;              // y becomes right child of x
    y->left = T2;              // T2 becomes left child of y

    // Update heights (y first, then x as x is now parent)
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x; // Return new root
}

/**
 * Left rotation for AVL tree balancing
 * Used when right subtree is heavier (right-right or right-left case after right rotation)
 * Time Complexity: O(1)
 */
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;     // y becomes new root
    AVLNode* T2 = y->left;     // Store y's left subtree

    // Perform rotation
    y->left = x;               // x becomes left child of y
    x->right = T2;             // T2 becomes right child of x

    // Update heights (x first, then y as y is now parent)
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y; // Return new root
}

/**
 * Insert a banned word into AVL tree with automatic balancing
 * Maintains AVL property through rotations when needed
 * Time Complexity: O(log n)
 */
AVLNode* insertAVL(AVLNode* node, string key) {
    // Step 1: Perform standard BST insertion
    if (!node) return new AVLNode(key);

    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else
        return node; // Duplicate keys not allowed

    // Step 2: Update height of current node
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    // Step 3: Get balance factor to check if rebalancing is needed
    int balance = getBalance(node);

    // Step 4: Perform rotations if tree becomes unbalanced
    // Left Left Case: balance > 1 and key inserted in left subtree of left child
    if (balance > 1 && key < node->left->key) 
        return rightRotate(node);

    // Right Right Case: balance < -1 and key inserted in right subtree of right child
    if (balance < -1 && key > node->right->key) 
        return leftRotate(node);

    // Left Right Case: balance > 1 and key inserted in right subtree of left child
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);   // First left rotation
        return rightRotate(node);              // Then right rotation
    }

    // Right Left Case: balance < -1 and key inserted in left subtree of right child
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right); // First right rotation
        return leftRotate(node);                // Then left rotation
    }

    // Return unchanged node if balanced
    return node;
}

/**
 * Search for a banned word in AVL tree
 * Efficient O(log n) search due to balanced nature of AVL tree
 */
bool searchAVL(AVLNode* node, const string& key) {
    if (!node) return false;           // Word not found
    if (key == node->key) return true; // Word found
    // Recursively search in appropriate subtree based on comparison
    return key < node->key ? searchAVL(node->left, key) : searchAVL(node->right, key);
}

// ----------- Graph Implementation for Social Network Analysis -----------
/**
 * Undirected graph to represent social connections between users
 * Used for bot detection based on social connectivity patterns
 */
class Graph {
    map<string, vector<string>> adj; // Adjacency list representation
public:
    /**
     * Add bidirectional edge between two users
     * Represents mutual social connection
     */
    void addEdge(string u, string v) {
        adj[u].push_back(v); // u is connected to v
        adj[v].push_back(u); // v is connected to u (undirected)
    }

    /**
     * Get degree (number of connections) of a user
     * Users with degree 0 are potential bots (isolated accounts)
     * Time Complexity: O(1)
     */
    int degree(string u) {
        return adj[u].size();
    }
};

// ----------- Content Moderation Data Structures -----------
/**
 * Structure to store information about flagged posts
 * Contains all relevant data for moderation decision making
 */
struct FlaggedPost {
    string username;    // User who posted the content
    string content;     // The actual post content
    int severity;       // Calculated severity score (higher = more problematic)
    string priority;    // Priority level based on severity (HIGH/MEDIUM/LOW)
    bool isBot;         // Whether user is detected as potential bot
    int reputation;     // User's reputation score
};

/**
 * Convert numerical severity score to priority level
 * Used for categorizing flagged content by urgency
 */
string getPriority(int score) {
    if (score >= 6) return "HIGH";    // Immediate attention required
    if (score >= 3) return "MEDIUM";  // Moderate priority
    return "LOW";                     // Low priority review
}

/**
 * Display detailed information about a flagged post
 * Provides comprehensive view for content moderators
 */
void printFlaggedPost(const FlaggedPost& post) {
    cout << "User: " << post.username << "\n";
    cout << "Reputation: " << post.reputation << "\n";
    cout << "Bot Detected: " << (post.isBot ? "Yes" : "No") << "\n";
    cout << "Severity Score: " << post.severity << "\n";
    cout << "Priority: " << post.priority << "\n";
    cout << "Content: " << post.content << "\n\n";
}

// ----------- KMP String Matching Algorithm for Advanced Pattern Detection -----------
/**
 * KMP (Knuth-Morris-Pratt) Algorithm for efficient string pattern matching
 * Provides O(n + m) time complexity for finding patterns in text
 * Useful for detecting banned phrases, not just individual words
 */
class KMPStringMatcher {
public:
    /**
     * Compute Longest Proper Prefix which is also Suffix (LPS) array
     * Used to avoid redundant character comparisons in KMP algorithm
     * Time Complexity: O(m) where m is pattern length
     */
    static vector<int> computeLPS(const string& pattern) {
        int m = pattern.length();
        vector<int> lps(m, 0);
        int len = 0; // Length of previous longest prefix suffix
        int i = 1;

        while (i < m) {
            if (pattern[i] == pattern[len]) {
                lps[i++] = ++len; // Character matches, extend LPS
            } else {
                if (len != 0) len = lps[len - 1]; // Mismatch: use previous LPS value
                else lps[i++] = 0; // No proper prefix suffix exists
            }
        }
        return lps;
    }

    /**
     * KMP pattern matching algorithm
     * Searches for pattern occurrences in text using preprocessed LPS array
     * Time Complexity: O(n + m) where n = text length, m = pattern length
     */
    static vector<int> findPatternOccurrences(const string& text, const string& pattern) {
        vector<int> occurrences;
        int n = text.length(), m = pattern.length();
        
        if (m == 0) return occurrences;
        
        vector<int> lps = computeLPS(pattern);
        int i = 0, j = 0; // i: text index, j: pattern index

        while (i < n) {
            if (pattern[j] == text[i]) {
                i++; j++; // Characters match, advance both pointers
            }
            
            if (j == m) {
                occurrences.push_back(i - j); // Pattern found at position (i-j)
                j = lps[j - 1]; // Continue searching for more occurrences
            } else if (i < n && pattern[j] != text[i]) {
                // Mismatch: use LPS to skip characters intelligently
                if (j != 0) j = lps[j - 1]; // Jump to previous border
                else i++; // No border exists, advance text pointer
            }
        }
        return occurrences;
    }

    /**
     * Check if text contains pattern (boolean version)
     * More efficient when only existence check is needed
     */
    static bool containsPattern(const string& text, const string& pattern) {
        return !findPatternOccurrences(text, pattern).empty();
    }
};

// ----------- Main Content Moderation System -----------
int main() {
    // Step 1: Initialize banned words database using AVL tree
    // AVL tree provides O(log n) search performance for banned word detection
    AVLNode* bannedWords = nullptr;
    vector<string> words = {"spam", "fake", "scam", "hate"};
    cout << "Building banned words database...\n";
    for (const string& word : words) {
        bannedWords = insertAVL(bannedWords, word);
        cout << "Added banned word: " << word << "\n";
    }

    // Step 2: Initialize user reputation system
    // Higher reputation users get lower severity penalties
    map<string, int> userReputation;
    userReputation["alice"] = 10;   // High reputation user
    userReputation["bob"] = 3;      // Low reputation user  
    userReputation["charlie"] = 5;  // Medium reputation user
    cout << "\nUser reputation system initialized.\n";

    // Step 3: Build social network graph for bot detection
    // Users with no social connections are flagged as potential bots
    Graph socialGraph;
    socialGraph.addEdge("alice", "bob");
    socialGraph.addEdge("bob", "charlie");
    // Note: "botuser" intentionally has no connections (isolated = potential bot)
    cout << "Social network graph constructed.\n";

    // Step 4: Process incoming content posts
    vector<pair<string, string>> posts = {
        {"alice", "this is a great product"},    // Clean content from high-rep user
        {"bob", "this is a scam"},              // Contains banned word "scam"
        {"botuser", "check out this link"},     // From potential bot user
        {"charlie", "I hate this"}              // Contains banned word "hate"
    };

    // Step 5: Priority queue for processing flagged content by severity
    // Higher severity content gets processed first (max heap behavior)
    priority_queue<pair<int, FlaggedPost>> flaggedQueue;

    cout << "\nAnalyzing posts for violations...\n";
    
    // Step 6: Enhanced content analysis with KMP string matching
    // Add banned phrases for more sophisticated detection
    vector<string> bannedPhrases = {"click here", "free money", "urgent action", "limited time"};
    
    for (auto& post : posts) {
        string user = post.first;
        string content = post.second;
        int severity = 0;

        // Method 1: Individual banned word detection using AVL tree search
        string word;
        stringstream ss(content);
        while (ss >> word) {
            // Convert to lowercase for case-insensitive matching
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            // Check if word exists in banned words AVL tree
            if (searchAVL(bannedWords, word)) {
                severity++; // Increment severity for each banned word found
                cout << "Banned word '" << word << "' detected in post by " << user << "\n";
            }
        }

        // Method 2: Advanced pattern detection using KMP algorithm
        string lowerContent = content;
        transform(lowerContent.begin(), lowerContent.end(), lowerContent.begin(), ::tolower);
        
        for (const string& phrase : bannedPhrases) {
            if (KMPStringMatcher::containsPattern(lowerContent, phrase)) {
                severity += 2; // Higher penalty for banned phrases
                cout << "Banned phrase '" << phrase << "' detected in post by " << user << "\n";
            }
        }

        // Get user reputation (default 0 for unknown users)
        int rep = userReputation.count(user) ? userReputation[user] : 0;
        
        // Bot detection based on social graph connectivity
        bool isBot = (socialGraph.degree(user) == 0);
        if (isBot) {
            severity += 2; // Significant penalty for potential bots
            cout << "Potential bot detected: " << user << " (no social connections)\n";
        }
        
        // Low reputation penalty
        if (rep < 5) {
            severity++; // Additional penalty for low reputation users
            cout << "Low reputation penalty applied to " << user << "\n";
        }

        // Add to flagged queue if any violations detected
        if (severity > 0) {
            FlaggedPost fp = {user, content, severity, getPriority(severity), isBot, rep};
            flaggedQueue.push({severity, fp}); // Priority queue orders by severity
            cout << "Post flagged with severity " << severity << "\n";
        }
    }

    // Step 7: Process flagged content in priority order
    cout << "\n=== FLAGGED CONTENT REPORT (Ordered by Severity) ===\n\n";
    while (!flaggedQueue.empty()) {
        // Extract highest severity post first (max heap property)
        printFlaggedPost(flaggedQueue.top().second);
        flaggedQueue.pop();
    }

    return 0;
}
