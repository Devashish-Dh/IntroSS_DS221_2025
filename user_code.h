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

    // // approach in which the input is sorted first and then duplicates are found
    // cout<<"Input Parcels: "<<endl;
    // for(auto parcel : parcels) {
    //     cout <<"Id " << parcel[0] << " Weight " << parcel[1] << endl;
    // }   
    // cout<<endl;

    // cout<<"Sorted Parcels: "<<endl;
    // auto sorted = sortByParcelId(parcels);
    // for(auto parcel : sorted) {
    //     cout <<"Id " << parcel[0] << " Weight " << parcel[1] << endl;
    // }   
    // cout<<endl;

    // //return sorted;

    // //initialize result vector
    // vector<vector<int>> result; // {id, min_weight}
    // size_t n = sorted.size();      // for loop length
    // size_t i = 0;                  // iterator
    // size_t j = i;
    // int id, min_weight, count;
    // bool has_duplicate;         // flag to check if duplicate exists

    // while (i < n) {
    //         id = sorted[i][0];
    //         min_weight = sorted[i][1];
    //         j = i + 1;
    //         has_duplicate = false;

    //         while (j < n && sorted[j][0] == id) {
    //             min_weight = min(min_weight, sorted[j][1]);
    //             has_duplicate = true;
    //             j++;
    //         }

    //         if (has_duplicate) {
    //             result.push_back({id, min_weight});
    //         }

    //         i = j;
    //     }

    // // print out the result

    // cout<<"Duplicate Parcels with Minimum Weight: "<<endl;
    // for(auto parcel : result) {
    //     cout <<"Id " << parcel[0] << " Min Weight " << parcel[1] << endl;
    // }   
    // cout<<endl; 



    //approach which skips the sorting part and directly finds duplicates

    //using unordered_map to store min weights and counts of each id
    unordered_map<int, int> min_weights;
    unordered_map<int, int> counts;
    int id, weight;

    //sort the parcels to ensure ordering by ID in the result vector
    //this is optional, if ordering is not required, this can be skipped
    auto sorted = sortByParcelId(parcels); 

    for (const auto& parcel : sorted) {
        id = parcel[0], weight = parcel[1];
        counts[id]++;
        if (min_weights.count(id)) {
            min_weights[id] = min(min_weights[id], weight);
        } else {
            min_weights[id] = weight;
        }
    }

    vector<vector<int>> result;
    for (const auto& [id, min_weight] : min_weights) {
        if (counts[id] > 1) {
            result.push_back({id, min_weight});
        }
    }

    // print out the result

    cout<<"Duplicate Parcels with Minimum Weight: "<<endl;
    for(auto parcel : result) {
        cout <<"Id " << parcel[0] << " Min Weight " << parcel[1] << endl;
    }   
    cout<<endl; 


    return result;  



}





/* 
Complete this function
*/

vector<int> question_two(
    const vector<int>& preorder,
    const vector<int>& inorder,
    const vector<vector<int>>& leafParcels,
    const vector<vector<int>>& query
) {
    // TODO: Implement function
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