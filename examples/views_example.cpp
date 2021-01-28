#include <algorithm>
#include <cassert>
#include <list>
#include <vector>

#include "vector_view.hpp"
#include "list_view.hpp"
#include "map_view.hpp"

struct Complex {
    int group;
    int data[4];
    char key;
};

int main() {
    /* This example shows the source data being stored as a list
     * and being accessed via the vector view */
    {
        std::list<int> data{1, 2, 3, 4, 5}; // data is a list

        rviews::vector_view view{data}; // view it as a vector
        
        for(auto& elem : view) {
            std::cout << elem << std::endl;
        }
        
        view.push_back(6); // lets add 2 more elemts via the view
        view.push_back(7);
        
        assert(data.size() == view.size()); // check that real data was updated
        assert(data.size() == 7);
        assert(view.size() == 7);

        // lets check that view shows exactly the same elements that are in the data
        int& data_obj = *data.begin();
        int& view_obj = view[0];
        assert(&data_obj == &view_obj); // it is the same object
    }
    /* This example shows the source data being stored as a vector
     * and being accessed via the map view */
    {
        std::vector<Complex> data;
        data.push_back({1, {1, 1, 1, 1}, 'a'});
        data.push_back({1, {2, 2, 2, 2}, 'b'});
        data.push_back({1, {3, 3, 3, 3}, 'c'});
        data.push_back({2, {4, 4, 4, 4}, 'd'});
        data.push_back({2, {5, 5, 5, 5}, 'e'});
        data.push_back({2, {6, 6, 6, 6}, 'f'});
        data.push_back({3, {7, 7, 7, 7}, 'g'});

        rviews::map_view view{data, &Complex::key};

        std::cout << "'d' element by vector:\n";
        std::cout << view['d'].group << std::endl; // O(log N)

        std::cout << "'d' element by map view:\n";
        std::cout << 
            std::find_if(data.begin(), data.end(), 
                [](auto& complex) { return complex.key == 'd'; }
            )->group 
            << std::endl; // O(N)
    }
}