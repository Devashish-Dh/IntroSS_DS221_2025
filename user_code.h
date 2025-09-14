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

vector<vector<int>> question_one(const vector<vector<int>>& parcels) {
    // TODO: Implement function
    // parcels[i][0] is id of parcel i
    // parcels[i][1] is weight of parcel i
    // id might be repeated for different parcels

    //using unordered_map to store min weights and counts of each id
    unordered_map<int, int> min_weights;
    unordered_map<int, int> counts;
    int id, weight;

    vector<vector<int>> generatedParcels = generateData(100, 500, 1000);

    // iterate through parcels to populate the maps with min weights and counts
    for (const auto& parcel : generatedParcels) {
        id = parcel[0], weight = parcel[1];
        counts[id]++;
        if (min_weights.count(id)) {
            min_weights[id] = min(min_weights[id], weight);
        } else {
            min_weights[id] = weight;
        }
    }

    //initialize result vector
    vector<vector<int>> result;

    // iterate through min_weights to find duplicates and populate result
    for (const auto& [id, min_weight] : min_weights) {
        if (counts[id] > 1) {
            result.push_back({id, min_weight});
        }
    }

    //sort result by id before returning
    result = sortByParcelId(result);

    // // print out result
    // cout<<"Duplicate Parcels with Minimum Weight: "<<endl;
    // for(auto parcel : result) {
    //     cout <<"Id " << parcel[0] << " Min Weight " << parcel[1] << endl;
    // }   
    // cout<<endl; 


    return result;  
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
    for (int i = 0; i < inorder.size(); ++i) {
        inorderIndexMap[inorder[i]] = i;
    }
    return buildTreeHelper(preorder, 0, preorder.size() - 1,
                           inorder, 0, inorder.size() - 1, inorderIndexMap);
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
void dfs(TreeNode* node, TreeNode* parent, vector<int>& depth, vector<vector<int>>& up, int LOG) {
    if (!node) return;
    int nodeVal = node->val;
    int parentVal = parent ? parent->val : -1;
    up[nodeVal][0] = parentVal;
    for (int j = 1; j < LOG; ++j) {
        if (up[nodeVal][j - 1] != -1) {
            up[nodeVal][j] = up[up[nodeVal][j - 1]][j - 1];
        }
    }
    if (parent) {
        depth[nodeVal] = depth[parentVal] + 1;
    }
    dfs(node->left, node, depth, up, LOG);
    dfs(node->right, node, depth, up, LOG);
}   

//function to get LCA of two nodes u and v
int getLCA(int u, int v, const vector<int>& depth, const vector<vector<int>>& up, int LOG) {
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    for (int j = 0; j < LOG; ++j) {
        if ((diff >> j) & 1) {
            u = up[u][j];
        }
    }
    if (u == v) return u;
    for (int j = LOG - 1; j >= 0; --j) {
        if (up[u][j] != up[v][j]) {
            u = up[u][j];
            v = up[v][j];
        }
    }
    return up[u][0];
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
        while (parcels.size() < k) {
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
    int numQueries = 10; //arbitrary choice of number of queries
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

vector<int> question_two(
    const vector<int>& preorder,
    const vector<int>& inorder,
    const vector<vector<int>>& leafParcels,
    const vector<vector<int>>& query
    ) {
    // TODO: Implement function

    //generate data for testing
    const int numNodes = 5; //number of nodes in the tree
    
    //cout to check for segfault
    //cout<<"Generating Random Binary Tree with "<<numNodes<<" nodes for testing..."<<endl;

    //initialize std::tuple to hold generated data
    std::tuple< vector<int>, vector<int>, unordered_map<int, vector<int>>, vector<vector<int>> > incomingData;

    incomingData = generateRandomBinaryTree(numNodes);

    //unpack the tuple
    vector<int> genPreorder = std::get<0>(incomingData);
    vector<int> genInorder = std::get<1>(incomingData);
    unordered_map<int, vector<int>> genLeafParcelMap = std::get<2>(incomingData);
    vector<vector<int>> genQueries = std::get<3>(incomingData); 

    //cout to check for segfault
    //cout<<"Random Binary Tree Generated."<<endl<<endl;

    //convert map to vector for function input
    vector<vector<int>> genleafParcels;
    for (const auto& [leafNode, parcels] : genLeafParcelMap) {
        genleafParcels.push_back(parcels);
    }  

    //cout to check for segfault
    //cout<<"converted map to vector."<<endl<<endl;

    //all testing program here

    //cout to check for segfault
    //cout<<"Building Tree from Generated Traversals..."<<endl;

    //time this code block
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    TreeNode* testRoot = buildTree(genPreorder, genInorder); //need it

    //cout to check for segfault
    //cout<<"Tree Built."<<endl<<endl;


    vector<int> testLeafNodes;
    getLeafNodes(testRoot, testLeafNodes);
    //cout to check for segfault
    //cout<<"Got Leaf Nodes."<<endl<<endl;


    //map genleafParcels[i] to the ith leaf node in level order traversal
    unordered_map<int, vector<int>> testLeafParcelMap; //map to store leafNode:parcels pairs
    for (int i = 0; i < testLeafNodes.size() && i < genleafParcels.size(); ++i) {
        testLeafParcelMap[testLeafNodes[i]] = genleafParcels[i];
    }       
    //cout to check for segfault
    //cout<<"Mapped Leaf Nodes to Parcels."<<endl<<endl;


    //setup for LCA (Lowest Common Ancestor) using Binary Lifting
    //constants for tree size
    const int TESTMAX_NODES = 2000000;
    const int TESTLOG = 21; // since 2^21 > 2000000
    vector<vector<int>> testUp(TESTMAX_NODES, vector<int>(TESTLOG, -1)); // up[v][j] is the 2^j-th ancestor of node v   
    vector<int> testDepth(TESTMAX_NODES, 0); // depth[v] is the depth of node v

    //run dfs to populate depth and up tables
    dfs(testRoot, nullptr, testDepth, testUp, TESTLOG);

    //initialize result vector
    vector<int> testResult;

    //iterate through query 
    for (const auto& parcelsNeeded : genQueries) {
        //for query[i], i is a list of parcels
        //make a vector of leaf nodes that contain these parcels (some parcels may belong to same leaf node)
        vector<int> testleafNodesWithParcels;
        for (int parcel : parcelsNeeded) {
            for (const auto& [leafNode, parcels] : testLeafParcelMap) {
                if (find(parcels.begin(), parcels.end(), parcel) != parcels.end()) {
                    testleafNodesWithParcels.push_back(leafNode);
                    break; //break inner loop to avoid duplicate leaf nodes for same parcel     
                }
            }
        }
        //run LCA on the vector of leaf nodes to find the common ancestor
        if (testleafNodesWithParcels.empty()) {
            testResult.push_back(-1); //if no leaf nodes found for the parcels, return -1
            continue;}
        else if (testleafNodesWithParcels.size() == 1) {
            testResult.push_back(testleafNodesWithParcels[0]); //if only one leaf node found, return that node
            continue;}
        else{
            int lca = testleafNodesWithParcels[0];
            for (int i = 1; i < testleafNodesWithParcels.size(); ++i) {
                lca = getLCA(lca, testleafNodesWithParcels[i], testDepth, testUp, TESTLOG);
            }
            testResult.push_back(lca); //push the LCA of all leaf nodes found for the parcels   
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken for testing code: " << duration.count() << " microseconds" << endl;


    //print out result
    // cout<<"LCA Results for Generated Queries: "<<endl;     
    // for(int i = 0; i < testResult.size(); ++i) {
    //     cout <<"Query " << i+1 << " LCA Node " << testResult[i] << endl;
    // }   
    // cout<<endl; 




    
    // // actual program starts here, remember to comment out the testing code above before submission

    // // given preorder and inorder are the traversals of the binary tree
    // // create the binary tree from these traversals
    
    // TreeNode* root = buildTree(preorder, inorder); 

    // //print the tree level order traversal
    // // cout<<"Level Order Traversal of the Tree: "<<endl;
    // // unordered_map<int, int> nodeLevels; //map to store node:level pairs
    // // levelOrderTraversal(root, nodeLevels);
    // // for (const auto& [node, level] : nodeLevels) {
    // //     cout << "Node: " << node << ", Level: " << level << endl;
    // // }
    // // cout<<endl;

    // // get the leaf nodes of the tree
    // vector<int> leafNodes;
    // getLeafNodes(root, leafNodes);      

    // //map leafParcels[i] to the ith leaf node in level order traversal
    // unordered_map<int, vector<int>> leafParcelMap; //map to store leafNode:parcels pairs
    // for (int i = 0; i < leafNodes.size() && i < leafParcels.size(); ++i) {
    //     leafParcelMap[leafNodes[i]] = leafParcels[i];
    // }       

    // //print out what parcels are mapped to which leaf nodes
    // // cout<<"Leaf Nodes and their Parcels: "<<endl;
    // // for (const auto& [leafNode, parcels] : leafParcelMap) {
    // //     cout << "Leaf Node: " << leafNode << ", Parcels: ";
    // //     for (int parcel : parcels) {
    // //         cout << parcel << " ";
    // //     }
    // //     cout << endl;
    // // }
    // // cout<<endl;  

    // //setup for LCA (Lowest Common Ancestor) using Binary Lifting
    // //constants for tree size
    // const int MAX_NODES = 2000000;
    // const int LOG = 21; // since 2^21 > 2000000
    // vector<vector<int>> up(MAX_NODES, vector<int>(LOG, -1)); // up[v][j] is the 2^j-th ancestor of node v
    // vector<int> depth(MAX_NODES, 0); // depth[v] is the depth of node v

    // //run dfs to populate depth and up tables
    // dfs(root, nullptr, depth, up, LOG); 

    // //initialize result vector
    // vector<int> result;

    // //iterate through query 
    // for (const auto& parcelsNeeded : query) {
    //     //for query[i], i is a list of parcels
    //     //make a vector of leaf nodes that contain these parcels (some parcels may belong to same leaf node)
    //     vector<int> leafNodesWithParcels;
    //     for (int parcel : parcelsNeeded) {
    //         for (const auto& [leafNode, parcels] : leafParcelMap) {
    //             if (find(parcels.begin(), parcels.end(), parcel) != parcels.end()) {
    //                 leafNodesWithParcels.push_back(leafNode);
    //                 break; //break inner loop to avoid duplicate leaf nodes for same parcel     
    //             }
    //         }
    //     }       

    //     //run LCA on the vector of leaf nodes to find the common ancestor
    //     if (leafNodesWithParcels.empty()) {
    //         result.push_back(-1); //if no leaf nodes found for the parcels, return -1
    //         continue;}
    //     else if (leafNodesWithParcels.size() == 1) {
    //         result.push_back(leafNodesWithParcels[0]); //if only one leaf node found, return that node
    //         continue;}
    //     else{
    //         int lca = leafNodesWithParcels[0];
    //         for (int i = 1; i < leafNodesWithParcels.size(); ++i) {
    //             lca = getLCA(lca, leafNodesWithParcels[i], depth, up, LOG);
    //         }
    //         result.push_back(lca); //push the LCA of all leaf nodes found for the parcels   
    //     }
    // }

    // //print out result
    // cout<<"LCA Results for Queries: "<<endl;     
    // for(int i = 0; i < result.size(); ++i) {
    //     cout <<"Query " << i+1 << " LCA Node " << result[i] << endl;
    // }   
    // cout<<endl;  


    return{};
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



/* 
Complete this function
*/
long long question_three(
    const vector<vector<int>>& edges,
    const vector<int>& metro_cities
) {
    // TODO: Implement function

    //generate data for testing
    cout<<"Generating Random Graph with "<<NUM_CITIES<<" cities, "<<NUM_ROADS<<" roads, "<<NUM_METRO<<" metro cities for testing..."<<endl;

    //generate edgelist and metro cities
    vector<vector<int>> genEdges = generateEdges();
    vector<int> genMetroCities = generateMetroCities();

    cout<<"Random Graph Generated."<<endl<<endl;
    //print out generated graph
    cout<<"Generated Edges (u, v, w): "<<endl;
    for (const auto& e : genEdges) {
        cout << e[0] << " " << e[1] << " " << e[2] << endl;
    }
    cout<<endl;

    cout<<"Generated Metro Cities: "<<endl;
    for (int c : genMetroCities) {
        cout << c << " ";
    }
    cout<<endl<<endl;   

    //find number of nodes (cities) in the graph
    int genn = 0;
    for (auto &e : genEdges) {
        genn = max(genn, max(e[0], e[1])); //assuming cities are 1 to n inclusive
    }
    for (int c : genMetroCities) genn = max(genn, c);

    //build the graph as adjacency list
    vector<vector<pair<int,int>>> gengraph(genn+1);
    for (auto &e : genEdges) {
        int u = e[0], v = e[1], w = e[2];
        gengraph[u].push_back({v,w});
        gengraph[v].push_back({u,w});
    }


    // Mark metro cities as boosters from the input list
    vector<bool> genisBooster(genn+1, false);
    for (int c : genMetroCities) genisBooster[c] = true;

    // Run Dijkstra from both trucks
    auto genres1 = runDijkstraWithBoosters(genn, gengraph, 1, genisBooster);
    auto genres2 = runDijkstraWithBoosters(genn, gengraph, genn, genisBooster);

    long long genbestTime = LLONG_MAX;
    int genmeetCity = -1, state1 = -1, state2 = -1;

    // Find optimal meeting city
    for (int city = 1; city <= genn; city++) {
        for (int s1 = 0; s1 < 2; s1++) {
            for (int s2 = 0; s2 < 2; s2++) {
                long long t1 = genres1.dist[city][s1];
                long long t2 = genres2.dist[city][s2];
                if (t1 == LLONG_MAX || t2 == LLONG_MAX) continue;
                long long meetTime = max(t1, t2);
                if (meetTime < genbestTime) {
                    genbestTime = meetTime;
                    genmeetCity = city;
                    state1 = s1;
                    state2 = s2;
                }
            }
        }
    }

    if (genbestTime == LLONG_MAX) {
        cout << "No meeting possible\n";
        return -1;
    }

    // Reconstruct and print paths
    auto genpathTruck1 = reconstructPath(1, genmeetCity, state1, genres1.parent);
    auto genpathTruck2 = reconstructPath(genn, genmeetCity, state2, genres2.parent);

    cout << "Truck 1 path: ";
    for (int x : genpathTruck1) cout << x << " ";
    cout << "\nTruck 2 path: ";
    for (int x : genpathTruck2) cout << x << " ";
    cout << "\n";

    cout<<"Meeting City: " << genmeetCity << "\n";
    cout<<"Minimum Time to Meet: " << genbestTime << "\n";   
    
    
    cout<<"Testing code complete."<<endl<<endl;

    return genbestTime;

    //actual program starts here, remember to comment out the testing code above before submission

    // //find number of nodes (cities) in the graph
    // int n = 0;
    // for (auto &e : edges) {
    //     n = max(n, max(e[0], e[1]));
    // }
    // for (int c : metro_cities) n = max(n, c);

    // //build the graph as adjacency list
    // vector<vector<pair<int,int>>> graph(n+1);
    // for (auto &e : edges) {
    //     int u = e[0], v = e[1], w = e[2];
    //     graph[u].push_back({v,w});
    //     graph[v].push_back({u,w});
    // }


    // // Mark metro cities as boosters from the input list
    // vector<bool> isBooster(n+1, false);
    // for (int c : metro_cities) isBooster[c] = true;

    // // Run Dijkstra from both trucks
    // auto res1 = runDijkstraWithBoosters(n, graph, 1, isBooster);
    // auto res2 = runDijkstraWithBoosters(n, graph, n, isBooster);

    // long long bestTime = LLONG_MAX;
    // int meetCity = -1, state1 = -1, state2 = -1;

    // // Find optimal meeting city
    // for (int city = 1; city <= n; city++) {
    //     for (int s1 = 0; s1 < 2; s1++) {
    //         for (int s2 = 0; s2 < 2; s2++) {
    //             long long t1 = res1.dist[city][s1];
    //             long long t2 = res2.dist[city][s2];
    //             if (t1 == LLONG_MAX || t2 == LLONG_MAX) continue;
    //             long long meetTime = max(t1, t2);
    //             if (meetTime < bestTime) {
    //                 bestTime = meetTime;
    //                 meetCity = city;
    //                 state1 = s1;
    //                 state2 = s2;
    //             }
    //         }
    //     }
    // }

    // if (bestTime == LLONG_MAX) {
    //     cout << "No meeting possible\n";
    //     return -1;
    // }

    // // Reconstruct and print paths
    // auto pathTruck1 = reconstructPath(1, meetCity, state1, res1.parent);
    // auto pathTruck2 = reconstructPath(n, meetCity, state2, res2.parent);

    // cout << "Truck 1 path: ";
    // for (int x : pathTruck1) cout << x << " ";
    // cout << "\nTruck 2 path: ";
    // for (int x : pathTruck2) cout << x << " ";
    // cout << "\n";

    // cout<<"Meeting City: " << meetCity << "\n";
    // cout<<"Minimum Time to Meet: " << bestTime << "\n";

    //return bestTime;


}


#endif // USER_CODE_H