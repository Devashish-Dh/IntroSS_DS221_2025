#ifndef USER_CODE_H
#define USER_CODE_H

// Feel free to include more library functions
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_map>

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



// to generate the inpuuts:

//function to generate preorder traversal from inorder traversal (helper for random tree generation)
void generatePreorder(const vector<int>& inorder, vector<int>& preorder) {
    if (inorder.empty()) return;
    int root = inorder[rand() % inorder.size()];
    preorder.push_back(root);
    vector<int> left, right;
    for (int val : inorder) {
        if (val < root) left.push_back(val);
        else if (val > root) right.push_back(val);
    }
    generatePreorder(left, preorder);
    generatePreorder(right, preorder);
}   

//function to generate random binary tree traversals with given number of nodes (also generate parcels on leafs and queries for testing)
pair<vector<int>, vector<int>> generateRandomBinaryTree(int numNodes) { 
    vector<int> preorder, inorder;
    //generate random unique values for nodes, let those be our inorder traversal
    vector<int> values(numNodes);
    for (int i = 0; i < numNodes; ++i) {
        values[i] = i + 1; //values from 1 to numNodes
    }
    random_shuffle(values.begin(), values.end());
    inorder = values;

    //generate preorder traversal from inorder traversal
    generatePreorder(inorder, preorder);
    
    //build tree from these traversals to decide on parcel input
    TreeNode* root = buildTree(preorder, inorder);
    vector<int> leafNodes;
    getLeafNodes(root, leafNodes);
    int numLeafs = leafNodes.size();

    return {preorder, inorder};
    };        







vector<int> question_two(
    const vector<int>& preorder,
    const vector<int>& inorder,
    const vector<vector<int>>& leafParcels,
    const vector<vector<int>>& query
) {
    // TODO: Implement function

    // preorder and inorder are the traversals of the binary tree
    // create the binary tree from these traversals
    TreeNode* root = buildTree(preorder, inorder); 

    //print the tree level order traversal
    // cout<<"Level Order Traversal of the Tree: "<<endl;
    // unordered_map<int, int> nodeLevels; //map to store node:level pairs
    // levelOrderTraversal(root, nodeLevels);
    // for (const auto& [node, level] : nodeLevels) {
    //     cout << "Node: " << node << ", Level: " << level << endl;
    // }
    // cout<<endl;

    // get the leaf nodes of the tree
    vector<int> leafNodes;
    getLeafNodes(root, leafNodes);      

    //map leafParcels[i] to the ith leaf node in level order traversal
    unordered_map<int, vector<int>> leafParcelMap; //map to store leafNode:parcels pairs
    for (int i = 0; i < leafNodes.size() && i < leafParcels.size(); ++i) {
        leafParcelMap[leafNodes[i]] = leafParcels[i];
    }       

    //print out what parcels are mapped to which leaf nodes
    // cout<<"Leaf Nodes and their Parcels: "<<endl;
    // for (const auto& [leafNode, parcels] : leafParcelMap) {
    //     cout << "Leaf Node: " << leafNode << ", Parcels: ";
    //     for (int parcel : parcels) {
    //         cout << parcel << " ";
    //     }
    //     cout << endl;
    // }
    // cout<<endl;  

    //setup for LCA (Lowest Common Ancestor) using Binary Lifting
    //constants for tree size
    const int MAX_NODES = 2000000;
    const int LOG = 21; // since 2^21 > 2000000
    vector<vector<int>> up(MAX_NODES, vector<int>(LOG, -1)); // up[v][j] is the 2^j-th ancestor of node v
    vector<int> depth(MAX_NODES, 0); // depth[v] is the depth of node v

    //run dfs to populate depth and up tables
    dfs(root, nullptr, depth, up, LOG); 

    //initialize result vector
    vector<int> result;

    //iterate through query 
    for (const auto& parcelsNeeded : query) {
        //for query[i], i is a list of parcels
        //make a vector of leaf nodes that contain these parcels (some parcels may belong to same leaf node)
        vector<int> leafNodesWithParcels;
        for (int parcel : parcelsNeeded) {
            for (const auto& [leafNode, parcels] : leafParcelMap) {
                if (find(parcels.begin(), parcels.end(), parcel) != parcels.end()) {
                    leafNodesWithParcels.push_back(leafNode);
                    break; //break inner loop to avoid duplicate leaf nodes for same parcel     
                }
            }
        }       

        //run LCA on the vector of leaf nodes to find the common ancestor
        if (leafNodesWithParcels.empty()) {
            result.push_back(-1); //if no leaf nodes found for the parcels, return -1
            continue;}
        else if (leafNodesWithParcels.size() == 1) {
            result.push_back(leafNodesWithParcels[0]); //if only one leaf node found, return that node
            continue;}
        else{
            int lca = leafNodesWithParcels[0];
            for (int i = 1; i < leafNodesWithParcels.size(); ++i) {
                lca = getLCA(lca, leafNodesWithParcels[i], depth, up, LOG);
            }
            result.push_back(lca); //push the LCA of all leaf nodes found for the parcels   
        }
    }

    //print out result
    cout<<"LCA Results for Queries: "<<endl;     
    for(int i = 0; i < result.size(); ++i) {
        cout <<"Query " << i+1 << " LCA Node " << result[i] << endl;
    }   
    cout<<endl;  


    return{};
}




/* 
Complete this function
*/
long long question_three(
    const vector<vector<int>>& edges,
    const vector<int>& metro_cities
) {
    // TODO: Implement function
    return 0;
}




#endif // USER_CODE_H