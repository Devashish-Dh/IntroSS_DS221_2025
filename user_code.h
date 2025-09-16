#ifndef USER_CODE_H
#define USER_CODE_H

// Feel free to include more library functions
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <random>
#include <chrono>
#include <climits>   
#include <functional> 
#include <queue>
#include <unordered_set>
#include <set>
#include <fstream>
#include <string>


using namespace std;


/* 
Complete this function
*/

//function to generate data using random number generator
vector<vector<int>> generateData(int rangeOfId, int rangeOfWeight, int numberOfParcelsWanted) {
    vector<vector<int>> randomlyGeneratedParcels;
    for (int i = 0; i < numberOfParcelsWanted; ++i) {
        //generate id randomly from 1 to rangeOfId
        int id = rand() % rangeOfId + 1;
    
        //generate weight randomly from 1 to rangeOfWeight
        int weight = rand() % rangeOfWeight + 1;

        randomlyGeneratedParcels.push_back({id, weight});
    }
    return randomlyGeneratedParcels;
}


// Function to sort parcels by their ID and weight
vector<vector<int>> sortByParcelId(const vector<vector<int>>& input_parcels) {

    vector<vector<int>> sorted_parcels = input_parcels;
    // sort using the 1st element of each vector (extend the std::sort() with custom comparison logic[lambda function])
    std::sort(sorted_parcels.begin(), sorted_parcels.end(), [](const vector<int>& a, const vector<int>& b) {
        
        //if ID is same then sort by weight
        if(a[0] == b[0]) {
            return a[1] < b[1];
        }
        else
        {return a[0] < b[0];}
    });

    //return vector (sorted by parcel ID and weight)
    return sorted_parcels;
}

//function that solves the problem
vector<vector<int>> sol(const vector<vector<int>>& parcels) {
    unordered_map<int, int> min_weights;
    unordered_map<int, int> counts;

    // Count occurrences and track min weights
    for (const auto& parcel : parcels) {
        int id = parcel[0], weight = parcel[1];
        counts[id]++;
        if (min_weights.count(id)) {
            min_weights[id] = min(min_weights[id], weight);
        } else {
            min_weights[id] = weight;
        }
    }

    // Debugging: print counts and min weights
    /*
    cout << "Counts and Min Weights:\n";
    for (auto& [id, cnt] : counts) {
        cout << "Id " << id << " Count " << cnt 
             << " MinWeight " << min_weights[id] << endl;
    }
    cout << endl;
    */

    vector<vector<int>> result;
    for (const auto& [id, min_weight] : min_weights) {
        if (counts[id] > 1) {
            result.push_back({id, min_weight});
        }
    }

    // Debugging: print unsorted result
    /*
    cout << "Unsorted Result:\n";
    for (auto& parcel : result) {
        cout << "Id " << parcel[0] << " MinWeight " << parcel[1] << endl;
    }
    cout << endl;
    */

    result = sortByParcelId(result);

    // Debugging: print final sorted result
    /*
    cout << "Sorted Result:\n";
    for (auto& parcel : result) {
        cout << "Id " << parcel[0] << " MinWeight " << parcel[1] << endl;
    }
    cout << endl;
    */

    return result;
}

vector<vector<int>> testing() {
    srand(42); // fixed seed for reproducibility

    //parameters for generating random data
    int rangeOfId = 100000000;        // IDs from 1 to rangeOfId
    int rangeOfWeight = 100000000;    // Weights from 1 to rangeOfWeight
    int numberOfParcelsWanted = 10;   // total parcels to generate

    // Generate test data
    vector<vector<int>> generatedParcels = generateData(rangeOfId, rangeOfWeight, numberOfParcelsWanted);

    //print parameters used for generating data
    cout << "Parameters for Data Generation:\n";
    cout << "Range of IDs: 1 to " << rangeOfId << endl;
    cout << "Range of Weights: 1 to " << rangeOfWeight << endl;
    cout << "Number of Parcels Generated: " << numberOfParcelsWanted << endl << endl;

    // cout << "Generated Parcels:\n";
    // for (auto& parcel : generatedParcels) {
    //     cout << "Id " << parcel[0] << " Weight " << parcel[1] << endl;
    // }
    // cout << endl;


    auto start = std::chrono::high_resolution_clock::now();

    // Run and time the solution
    vector<vector<int>> result = sol(generatedParcels);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    cout << "Time taken by q1 sol(): " << duration.count() << " microseconds" << endl;

    // cout << "Duplicate Parcels with Minimum Weight:\n";
    // for (auto& parcel : result) {
    //     cout << "Id " << parcel[0] << " MinWeight " << parcel[1] << endl;
    // }
    // cout << endl;

    return result; 
}






vector<vector<int>> question_one(const vector<vector<int>>& parcels) {
    // TODO: Implement function
    // parcels[i][0] is id of parcel i
    // parcels[i][1] is weight of parcel i
    // id might be repeated for different parcels

    //testing code
    //vector<vector<int>> testResult = testing();
    //return testResult;

    //std::cout << "\n----Q1 completed----\n" << std::endl;

    //actual code starts here, remember to comment out the testing code above before submission
    return sol(parcels);
}
















/* 
Complete this function
*/

//struct defn for tree node 
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

//recursive function to build tree from given preorder and inorder traversals (vector inputs)
TreeNode* buildTreeHelper(const vector<int>& preorder, int preStart, int preEnd,
                          const vector<int>& inorder, int inStart, int inEnd,
                          unordered_map<int, int>& inorderIndexMap) {
    if (preStart > preEnd || inStart > inEnd) {
        return nullptr;
    }   
    int rootVal = preorder[preStart];
    TreeNode* root = new TreeNode(rootVal);
    int inRootIndex = inorderIndexMap[rootVal];
    int numsLeft = inRootIndex - inStart;   
    root->left = buildTreeHelper(preorder, preStart + 1, preStart + numsLeft,
                                 inorder, inStart, inRootIndex - 1, inorderIndexMap);
    root->right = buildTreeHelper(preorder, preStart + numsLeft + 1, preEnd,
                                  inorder, inRootIndex + 1, inEnd, inorderIndexMap);
    return root;
}

//wrapper function to initiate tree building
TreeNode* buildTree(const vector<int>& preorder, const vector<int>& inorder) {
    unordered_map<int, int> inorderIndexMap;
    for (int i = 0; i < (int)inorder.size(); ++i) {
        inorderIndexMap[inorder[i]] = i;
    }
    return buildTreeHelper(preorder, 0, (int)preorder.size() - 1,
                           inorder, 0, (int)inorder.size() - 1, inorderIndexMap);
}

//recursive function for level order traversal of tree 
void levelOrderTraversal(TreeNode* root, unordered_map<int, int>& nodeLevels) {
    if (!root) return;
    vector<TreeNode*> queue = {root};
    int level = 0;
    while (!queue.empty()) {
        int size = queue.size();
        for (int i = 0; i < size; ++i) {
            TreeNode* node = queue[i];
            nodeLevels[node->val] = level;
            if (node->left) queue.push_back(node->left);
            if (node->right) queue.push_back(node->right);
        }
        queue.erase(queue.begin(), queue.begin() + size);
        level++;
    }
}

//function to return a vector of leaf nodes left to right
void getLeafNodes(TreeNode* root, vector<int>& leafNodes) {
    if (!root) return;
    if (!root->left && !root->right) {
        leafNodes.push_back(root->val);
        return;
    }
    getLeafNodes(root->left, leafNodes);
    getLeafNodes(root->right, leafNodes);
}   


//dfs to setup depth and up tables for LCA using Binary Lifting
void dfs_binary_lifting(TreeNode* node, TreeNode* parent,
                        const unordered_map<int,int>& valToIdx,
                        vector<int>& depth, vector<vector<int>>& up, int LOG) {
    if (!node) return;
    int idx = valToIdx.at(node->val);
    int parentIdx = parent ? valToIdx.at(parent->val) : -1;
    up[idx][0] = parentIdx;
    for (int j = 1; j < LOG; ++j) {
        if (up[idx][j-1] != -1) up[idx][j] = up[ up[idx][j-1] ][j-1];
        else up[idx][j] = -1;
    }
    if (parent) depth[idx] = depth[parentIdx] + 1;
    dfs_binary_lifting(node->left, node, valToIdx, depth, up, LOG);
    dfs_binary_lifting(node->right, node, valToIdx, depth, up, LOG);
}

// get LCA between two node-indices (compact) using up/depth
int getLCA_idx(int u, int v, const vector<int>& depth, const vector<vector<int>>& up, int LOG) {
    if (u == -1 || v == -1) return -1;
    int uu = u, vv = v;
    if (depth[uu] < depth[vv]) swap(uu, vv);
    int diff = depth[uu] - depth[vv];
    for (int j = 0; j < LOG; ++j) {
        if ((diff >> j) & 1) uu = up[uu][j];
    }
    if (uu == vv) return uu;
    for (int j = LOG - 1; j >= 0; --j) {
        if (up[uu][j] != up[vv][j]) {
            uu = up[uu][j];
            vv = up[vv][j];
        }
    }
    return up[uu][0];
}

// helper function: recursive
void buildPreorder(const vector<int>& inorder, int l, int r, 
                   vector<int>& preorder, mt19937& rng) {
    if (l > r) return;

    // choose a random root from inorder[l..r]
    uniform_int_distribution<int> dist(l, r);
    int rootIndex = dist(rng);

    preorder.push_back(inorder[rootIndex]);           // root
    buildPreorder(inorder, l, rootIndex - 1, preorder, rng); // left subtree
    buildPreorder(inorder, rootIndex + 1, r, preorder, rng); // right subtree
}

// generate random preorder traversal from given inorder traversal
vector<int> generateRandomPreorder(const vector<int>& inorder) {
    vector<int> preorder;
    random_device rd;
    mt19937 rng(rd()); // good RNG
    buildPreorder(inorder, 0, (int)inorder.size() - 1, preorder, rng);
    return preorder;
}

// to generate the inputs:
std::tuple< vector<int>, vector<int>, unordered_map<int, vector<int>>, vector<vector<int>> > generateRandomBinaryTree(int numNodes) {
    // Generate random preorder and inorder traversals
    vector<int> preorder, inorder;
    unordered_map<int, vector<int>> leafParcelMap;
    vector<vector<int>> queries;

    // TODO: Implement random tree generation and traversal population

    //generate a random in order traversal
    for (int i = 1; i <= numNodes; ++i) {
        inorder.push_back(i);
    }
    //shuffle inorder to get random order
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(inorder.begin(), inorder.end(), g);

    //generate preorder from inorder 
    preorder = generateRandomPreorder(inorder);

    //build tree from traversals to identify leaf nodes
    TreeNode* root = buildTree(preorder, inorder);

    //map for node to levels
    unordered_map<int, int> nodeLevels; //map to store node:level pairs
    levelOrderTraversal(root, nodeLevels);

    //print the tree level order traversal
    // cout<<"Level Order Traversal of the Generated Tree: "<<endl;
    // for (const auto& [node, level] : nodeLevels) {
    //     cout << "Node: " << node << ", Level: " << level << endl;
    // }
    // cout<<endl;

    //get leaf nodes
    vector<int> leafNodes;
    getLeafNodes(root, leafNodes);

    //make a map to track used parcel ids to avoid duplicates
    unordered_map<int, bool> usedParcelIds;

    //upper limit on parcel ids
    int parcelIdLimit = leafNodes.size() * 100; //arbitrary choice to have enough unique ids

    //assign k<=100 random parcels to each leaf node
    for (int leafNode : leafNodes) {
        int k = rand() % 100 + 1; //random k from 1 to 100
        //generate k unique parcel ids for this leaf node
        vector<int> parcels;
        while ((int)parcels.size() < k) {
            int parcelId = rand() % parcelIdLimit + 1; //random id from 1 to parcelIdLimit
            if (!usedParcelIds[parcelId]) {
                parcels.push_back(parcelId);
                usedParcelIds[parcelId] = true;
            }
        }
        leafParcelMap[leafNode] = parcels;
    }

    //print out leaf nodes and their parcels
    // cout<<"\n\nLeaf Nodes and their Parcels in Generated Tree: "<<endl;
    // for (const auto& [leafNode, parcels] : leafParcelMap) {
    //     cout << "Leaf Node: " << leafNode << ", Parcels: ";
    //     for (int parcel : parcels) {
    //         cout << parcel << " ";
    //     }
    //     cout << endl;
    // }
    // cout<<endl;

    //generate some random queries (each query is a list of parcel ids from usedParcelIds)
    int numQueries = 100000; //arbitrary choice of number of queries
    vector<int> usedParcelIdList;
    for (const auto& [parcelId, used] : usedParcelIds) {
        if (used) {
            usedParcelIdList.push_back(parcelId);
        }
    }
    int usedParcelIdCount = usedParcelIdList.size();
    for (int i = 0; i < numQueries; ++i) {
        int qSize = rand() % 5 + 1; //random query size from 1 to 5
        vector<int> query;
        for (int j = 0; j < qSize; ++j) {
            int parcelIndex = rand() % usedParcelIdCount;
            query.push_back(usedParcelIdList[parcelIndex]);
        }
        queries.push_back(query);
    }

    //print out generated queries
    // cout<<"Generated Queries: "<<endl;
    // for(int i = 0; i < queries.size(); ++i) {
    //     cout <<"Query " << i+1 << " Parcels: ";
    //     for(int parcel : queries[i]) {
    //         cout << parcel << " ";
    //     }
    //     cout << endl;
    // }   
    // cout<<endl;



    return std::make_tuple(preorder, inorder, leafParcelMap, queries);
}



//__________________________________
// preorder, inorder: traversals of the tree
// leafParcels: vector where leafParcels[i] are parcel ids assigned to the i-th leaf in level-order (wrapper will align them)
// query: vector of queries; each query is a vector of parcel ids
vector<int> sol(
    const vector<int>& preorder,
    const vector<int>& inorder,
    const vector<vector<int>>& leafParcels,
    const vector<vector<int>>& query
) {
    // Build the tree
    TreeNode* root = buildTree(preorder, inorder);

    // Get leaf nodes in left-to-right order
    vector<int> leafNodes;
    getLeafNodes(root, leafNodes);

    // Map each parcel to its leaf node
    unordered_map<int, int> parcelToLeaf; // parcelID -> leaf node value
    for (int i = 0; i < (int)leafNodes.size() && i < (int)leafParcels.size(); ++i) {
        for (int parcel : leafParcels[i])
            parcelToLeaf[parcel] = leafNodes[i];
    }

    // Build node value → index mapping for binary lifting
    unordered_map<int,int> valToIdx;
    vector<TreeNode*> stack = {root};
    int idx = 0;
    while (!stack.empty()) {
        TreeNode* node = stack.back(); stack.pop_back();
        if (!valToIdx.count(node->val)) valToIdx[node->val] = idx++;
        if (node->right) stack.push_back(node->right);
        if (node->left) stack.push_back(node->left);
    }

    int N = (int)valToIdx.size();
    const int LOG = 21;
    vector<int> depth(N, 0);
    vector<vector<int>> up(N, vector<int>(LOG, -1));

    // DFS to populate depth and up tables
    dfs_binary_lifting(root, nullptr, valToIdx, depth, up, LOG);

    // Answer queries
    vector<int> result;
    for (const auto& parcelsNeeded : query) {
        vector<int> leaves; // leaf node values
        for (int parcel : parcelsNeeded) {
            if (parcelToLeaf.count(parcel)) leaves.push_back(parcelToLeaf[parcel]);
        }

        if (leaves.empty()) {
            result.push_back(-1); // no parcels found
        } else if (leaves.size() == 1) {
            result.push_back(leaves[0]);
        } else {
            // Compute LCA index
            int lca_idx = valToIdx[leaves[0]];
            for (size_t i = 1; i < leaves.size(); ++i)
                lca_idx = getLCA_idx(lca_idx, valToIdx[leaves[i]], depth, up, LOG);

            // Convert back to node value
            int lca_val = -1;
            for (auto &[v,i] : valToIdx) if (i == lca_idx) { lca_val = v; break; }
            result.push_back(lca_val);
        }
    }

    return result;
}

//__________________________________

//code to generate random input for question two
//Generate random binary tree and its traversals
std::tuple<vector<int>, vector<int>, TreeNode*> generateTree(int numNodes) {
    // inorder = shuffled 1..numNodes
    vector<int> inorder(numNodes);
    iota(inorder.begin(), inorder.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(inorder.begin(), inorder.end(), g);

    // preorder generated randomly
    vector<int> preorder = generateRandomPreorder(inorder);

    // build tree
    TreeNode* root = buildTree(preorder, inorder);

    return {preorder, inorder, root};
}

//Assign parcels to leaf nodes
unordered_map<int, vector<int>> assignParcels(TreeNode* root) {
    vector<int> leafNodes;
    getLeafNodes(root, leafNodes);

    unordered_map<int, vector<int>> leafParcelMap;
    unordered_map<int, bool> usedParcelIds;

    int parcelIdLimit = (int)leafNodes.size() * 100; // enough unique IDs

    for (int leaf : leafNodes) {
        int k = rand() % 100 + 1; // 1–100 parcels
        vector<int> parcels;

        while ((int)parcels.size() < k) {
            int parcelId = rand() % parcelIdLimit + 1;
            if (!usedParcelIds[parcelId]) {
                parcels.push_back(parcelId);
                usedParcelIds[parcelId] = true;
            }
        }
        leafParcelMap[leaf] = parcels;
    }

    return leafParcelMap;
}

//Generate queries
vector<vector<int>> generateQueries(const unordered_map<int, vector<int>>& leafParcelMap, int numQueries) {
    vector<int> allParcels;
    for (const auto& [leaf, parcels] : leafParcelMap) {
        allParcels.insert(allParcels.end(), parcels.begin(), parcels.end());
    }

    vector<vector<int>> queries;
    int totalParcels = (int)allParcels.size();

    for (int i = 0; i < numQueries; i++) {
        int qSize = rand() % 5 + 1; // query size 1–5
        vector<int> query;
        for (int j = 0; j < qSize; j++) {
            int idx = rand() % totalParcels;
            query.push_back(allParcels[idx]);
        }
        queries.push_back(query);
    }

    return queries;
}

// Debug: print tree for verification
void printTree(TreeNode* root) {
    if (!root) return;
    cout << "Node " << root->val;
    if (root->left) cout << ", left -> " << root->left->val;
    if (root->right) cout << ", right -> " << root->right->val;
    cout << endl;
    printTree(root->left);
    printTree(root->right);
}

//Full generator wrapper
std::tuple<vector<int>, vector<int>, unordered_map<int, vector<int>>, vector<vector<int>>>
generateRandomBinaryTree(int numNodes, int numQueries) {
    auto [preorder, inorder, root] = generateTree(numNodes);
    
    // // Print the tree structure for verification
    // cout << "Generated Tree Structure:\n";  
    // printTree(root);

    auto leafParcelMap = assignParcels(root);
    // //print leaf nodes and their parcels
    // cout << "Leaf Nodes and their Parcels:\n";
    // for (const auto& [leaf, parcels] : leafParcelMap) {
    //     cout << "Leaf Node: " << leaf << ", Parcels: ";
    //     for (int p : parcels) cout << p << " ";
    //     cout << endl;
    // }
    // cout << endl;

    // //print the leaf nodes
    // cout << "Leaf Nodes:\n";
    // for (const auto& [leaf, parcels] : leafParcelMap) {
    //     cout << "Leaf Node: " << leaf << "\n";
    // }
    // cout << endl;

    auto queries = generateQueries(leafParcelMap, numQueries);
    // //print generated queries
    // cout << "\nGenerated Queries:\n";
    // for (const auto& query : queries) {
    //     for (int q : query) cout << q << " ";
    //     cout << endl;
    // }

    return {preorder, inorder, leafParcelMap, queries};
}


// Helper to print leaf nodes, their parcels, queries, and LCA results
void printQueryDebug(
    const vector<int>& leafNodes, 
    const vector<vector<int>>& leafParcels, 
    const vector<vector<int>>& queries,
    const vector<int>& results
) {
    cout << "\nQueries and their LCA Results:\n";
    for (size_t i = 0; i < queries.size(); ++i) {
        cout << "Query " << i+1 << " parcels: ";
        for (int p : queries[i])
            cout << p << " ";
        
        // show the leaf nodes containing these parcels
        cout << " -> Leaf nodes: ";
        for (int p : queries[i]) {
            for (size_t j = 0; j < leafNodes.size(); ++j) {
                if (find(leafParcels[j].begin(), leafParcels[j].end(), p) != leafParcels[j].end()) {
                    cout << leafNodes[j] << " ";
                    break; // only first matching leaf
                }
            }
        }

        cout << " -> LCA: " << results[i] << endl;
    }
    cout << endl;
}



//function to test the solution
vector<int> testing(int numNodes, int numQueries) {
    cout << "q2 Generating random binary tree with " << numNodes 
         << " nodes and " << numQueries << " queries..." << endl;

    // Generate random tree + parcels + queries
    auto [preorder, inorder, leafParcelMap, queries] = generateRandomBinaryTree(numNodes, numQueries);

    // Convert leafParcelMap to vector<vector<int>> in left-to-right order of leaves
    vector<int> leafNodes;
    getLeafNodes(buildTree(preorder, inorder), leafNodes);

    vector<vector<int>> leafParcels;
    for (int leaf : leafNodes) {
        if (leafParcelMap.count(leaf)) leafParcels.push_back(leafParcelMap[leaf]);
        else leafParcels.push_back({});
    }


    // Time the sol() call
    auto start = std::chrono::high_resolution_clock::now();
    vector<int> result = sol(preorder, inorder, leafParcels, queries);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    cout << "Execution Time on of q2_sol() on generated inputs: " << duration.count() << " µs\n";

    // // Optional debug print
    // cout << "Query results (LCA node values):\n";
    // for (size_t i = 0; i < result.size(); ++i) {
    //     cout << "Query " << i+1 << ": " << result[i] << "\n";
    // }
    // cout << endl;

    // Print detailed debug info
    //printQueryDebug(leafNodes, leafParcels, queries, result);

    return result;
}










vector<int> question_two(
    const vector<int>& preorder,
    const vector<int>& inorder,
    const vector<vector<int>>& leafParcels,
    const vector<vector<int>>& query
) {
    //TODO: Implement function

    //std::cout << "\n----Q2 started----\n" << std::endl;

    // Example testing / data generation
    // const int numNodes = 10; //variable                  1 ≤ n ≤ 10^6 → number of leaf nodes (loading junctions) so max leaves = 2* 10^6
    // const int numQueries = 100000;

    // vector<int> testResult = testing(numNodes, numQueries);
    
    // std::cout << "\n----Q2 completed----\n" << std::endl;
    // return testResult;
    



    //vector<int> result = sol(preorder, inorder, leafParcels, query);

    //print the result
    // std::cout << "LCA Results for given queries:\n";
    // for (size_t i = 0; i < result.size(); ++i) {
    //     std::cout << "Query " << i+1 << ": LCA Node Value = " << result[i] << "\n";
    // }
    // std::cout << std::endl;

    // std::cout << "\n----Q2 completed----\n" << std::endl;

    // return result;




    //actual code starts here, remember to comment out the testing code above before submission
    return sol(preorder, inorder, leafParcels, query);

}












//code to generate ranodm input for question three

int NUM_CITIES = 20;
int NUM_ROADS = 100;
int NUM_METRO = 4;
int MAX_WEIGHT = 100; // remember to keep it even (maximum weight generated will be 2*MAX_WEIGHT)
bool ENSURE_CONNECTED = false;
int RANDOM_SEED = 42;


// helper: random int in [l,r] using the engine
int randInt(int l, int r, mt19937 &rng) {
    uniform_int_distribution<int> dist(l, r);
    return dist(rng);
}

// generate edges
vector<vector<int>> generateEdges() {
    mt19937 rng(RANDOM_SEED); // fixed random engine
    vector<vector<int>> edges;
    set<pair<int,int>> existing;

    if (ENSURE_CONNECTED) {
        for (int i = 2; i <= NUM_CITIES; i++) {
            int u = i;
            int v = randInt(1, i-1, rng);
            int w = 2 * randInt(1, MAX_WEIGHT, rng); // even weight
            edges.push_back({u,v,w});
            existing.insert({min(u,v), max(u,v)});
        }
    }

    while ((int)edges.size() < NUM_ROADS) {
        int u = randInt(1, NUM_CITIES, rng);
        int v = randInt(1, NUM_CITIES, rng);
        if (u == v) continue;
        auto p = make_pair(min(u,v), max(u,v));
        if (existing.count(p)) continue;
        int w = 2 * randInt(1, MAX_WEIGHT, rng);
        edges.push_back({u,v,w});
        existing.insert(p);
    }

    return edges;
}

// generate metro cities
vector<int> generateMetroCities() {
    mt19937 rng(RANDOM_SEED); // fixed random engine
    vector<int> cities(NUM_CITIES);
    iota(cities.begin(), cities.end(), 1);
    shuffle(cities.begin(), cities.end(), rng); // shuffle with fixed seed
    cities.resize(NUM_METRO);
    sort(cities.begin(), cities.end());
    return cities;
}

//code for modified Dijkstra with boosters
// State = (node, refueledFlag)
// refueledFlag = 0 → not refueled yet
// refueledFlag = 1 → already refueled
struct State {
    int node;
    int refueled;
    long long dist;
};

// Min-heap comparator
struct CompareState {
    bool operator()(const State& a, const State& b) const {
        return a.dist > b.dist;
    }
};

// Struct to return both distance and parents
struct DijkstraResult {
    vector<vector<long long>> dist;
    vector<vector<int>> parent;
};

// Modified Dijkstra with boosters, also stores parent info
DijkstraResult runDijkstraWithBoosters(
    int n,
    vector<vector<pair<int,int>>>& graph,
    int start,
    const vector<bool>& isBooster
) {
    vector<vector<long long>> dist(n+1, vector<long long>(2, LLONG_MAX));
    vector<vector<int>> parent(n+1, vector<int>(2, -1));

    priority_queue<State, vector<State>, CompareState> pq;
    dist[start][0] = 0;
    pq.push({start, 0, 0});

    while (!pq.empty()) {
        State cur = pq.top(); pq.pop();
        int u = cur.node, s = cur.refueled;
        long long d = cur.dist;

        if (d > dist[u][s]) continue;

        // Option: refuel at booster
        if (s == 0 && isBooster[u]) {
            if (d < dist[u][1]) {
                dist[u][1] = d;
                parent[u][1] = u; // special marker: refueled here
                pq.push({u, 1, d});
            }
        }

        // Traverse edges
        for (auto [v, w] : graph[u]) {
            long long nd = d + (s == 1 ? w/2 : w);
            if (nd < dist[v][s]) {
                dist[v][s] = nd;
                parent[v][s] = u;
                pq.push({v, s, nd});
            }
        }
    }

    return {dist, parent};
}

// Reconstruct path from parent table
vector<int> reconstructPath(int start, int end, int state, vector<vector<int>>& parent) {
    vector<int> path;
    int cur = end, s = state;

    while (cur != -1) {
        path.push_back(cur);
        int p = parent[cur][s];

        // detect booster refuel marker
        if (p == cur && s == 1) {
            s = 0; // rewind state
            continue;
        }

        cur = p;
    }

    reverse(path.begin(), path.end());
    return path;
}




//function that solves question three
long long sol(
    const vector<vector<int>>& edges,
    const vector<int>& metro_cities
) {
    // Determine the number of nodes in the graph
    int n = 0;
    for (auto &e : edges) n = max(n, max(e[0], e[1]));
    for (int c : metro_cities) n = max(n, c);

    // Build the graph as an adjacency list
    vector<vector<pair<int,int>>> graph(n+1);
    for (auto &e : edges) {
        int u = e[0], v = e[1], w = e[2];
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    // Mark metro cities as boosters
    vector<bool> isBooster(n+1, false);
    for (int c : metro_cities) isBooster[c] = true;

    // Run the modified Dijkstra algorithm from both trucks
    auto res1 = runDijkstraWithBoosters(n, graph, 1, isBooster);
    auto res2 = runDijkstraWithBoosters(n, graph, n, isBooster);

    // Find the best city for the trucks to meet
    long long bestTime = LLONG_MAX;
    int meetCity = -1, state1 = -1, state2 = -1;

    for (int city = 1; city <= n; city++) {
        for (int s1 = 0; s1 < 2; s1++) {
            for (int s2 = 0; s2 < 2; s2++) {
                long long t1 = res1.dist[city][s1];
                long long t2 = res2.dist[city][s2];
                if (t1 == LLONG_MAX || t2 == LLONG_MAX) continue;
                long long meetTime = max(t1, t2);
                if (meetTime < bestTime) {
                    bestTime = meetTime;
                    meetCity = city;
                    state1 = s1;
                    state2 = s2;
                }
            }
        }
    }

    // If no meeting is possible, return -1
    if (bestTime == LLONG_MAX) return -1;

    // Optionally reconstruct paths (for debugging or verification)
    //auto pathTruck1 = reconstructPath(1, meetCity, state1, res1.parent);
    //auto pathTruck2 = reconstructPath(n, meetCity, state2, res2.parent);

    //print paths and travel times for debug
    
    // cout << "Truck 1 path: ";
    // for (int city : pathTruck1) cout << city << " ";
    // cout << " with travel time " << res1.dist[meetCity][state1] << "\n";    
    // cout << "Truck 2 path: ";
    // for (int city : pathTruck2) cout << city << " ";
    // cout << " with travel time " << res2.dist[meetCity][state2] << "\n";    
    // cout << "Meeting city: " << meetCity << " at time " << bestTime << "\n\n";
    
    

    return bestTime;
}


//function to test the solution
long long testing(int numCities, int numRoads, int numMetro, int maxWeight, bool ensureConnected = false) {
    cout << "q3 Generating random graph with " << numCities << " cities, "
         << numRoads << " roads, " << numMetro << " metro cities...\n";

    // save previous settings and override
    int old_NUM_CITIES = NUM_CITIES;
    int old_NUM_ROADS = NUM_ROADS;
    int old_NUM_METRO = NUM_METRO;
    int old_MAX_WEIGHT = MAX_WEIGHT;
    bool old_ENSURE_CONNECTED = ENSURE_CONNECTED;

    NUM_CITIES = numCities;
    NUM_ROADS = numRoads;
    NUM_METRO = numMetro;
    MAX_WEIGHT = maxWeight;
    ENSURE_CONNECTED = ensureConnected;

    // generate graph and metro cities
    vector<vector<int>> edges = generateEdges();
    vector<int> metroCities = generateMetroCities();

    // // print generated graph for debug
    // cout << "Edges (u, v, w):\n";
    // for (auto &e : edges) {
    //     cout << e[0] << " " << e[1] << " " << e[2] << "\n";
    // }
    // cout << "Metro cities: ";
    // for (int c : metroCities) cout << c << " ";
    // cout << "\n";

    // //print the generated graph
    // cout << "\nGenerated Graph:\n";
    // for (auto &e : edges) {
    //     cout << "City " << e[0] << " <--> City " << e[1] << " with travel time " << e[2] << "\n";
    // }
    // cout << "Metro Cities: ";
    // for (int c : metroCities) cout << c << " ";
    // cout << "\n\n"; 

    // time the sol() execution
    auto start = chrono::high_resolution_clock::now();
    long long minTime = sol(edges, metroCities); 
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Execution time for q3_sol() on generated random graph: " << duration.count() << " µs\n";
    cout << "Minimum meeting time returned by sol(): " << minTime << "\n";

    // restore previous settings
    NUM_CITIES = old_NUM_CITIES;
    NUM_ROADS = old_NUM_ROADS;
    NUM_METRO = old_NUM_METRO;
    MAX_WEIGHT = old_MAX_WEIGHT;
    ENSURE_CONNECTED = old_ENSURE_CONNECTED;

    return minTime;
}











/* 
Complete this function
*/
long long question_three(
    const vector<vector<int>>& edges,
    const vector<int>& metro_cities
) {
    // TODO: Implement function

    // std::cout << "\n----Q3 started----\n" << std::endl;

    // cout << "Testing the sol on generated random graph...\n"; 
    // int numCities = 20;   // adjust as needed    //2 ≤ n ≤ 2×10^5 → number of cities
    // int numRoads = 100;   // adjust as needed    //1 ≤ m ≤ 2×10^5 → number of roads
    // int numMetro = 4;     // adjust as needed    //1 ≤ k ≤ n → number of metro cities
    // int maxWeight = 100;  // adjust as needed    just pick positive even number


    // long long minTime = testing(numCities, numRoads, numMetro, maxWeight);

    // cout << "\n----Q3 completed----\n" << std::endl;

    // return minTime;

    //run on given input
    return sol(edges, metro_cities);


}


#endif // USER_CODE_H