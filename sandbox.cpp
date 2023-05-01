#include <iostream>
#include <vector>
#include <regex>
#include <string>

int main() {
    // Range based for loop
    std::vector<int> m_peer {10, 20, 30};
    std::cout << m_peer.size() << std::endl;
    for(int chr: m_peer) {
        std::cout << chr << std::endl;
    }

    // Find in string
    /*std::string partition = "242342";
    auto designation = std::to_string(3);
    if (partition.find(designation) != std::string::npos)
        std::cout << "Found" << std::endl;
    else
        std::cout << "Not found" << std::endl;
    */

    // Regex
    /*std::string phase = ":3454:43535";
    std::regex pattern("\\d+");
    std::sregex_iterator it(phase.begin(), phase.end(), pattern);
    std::sregex_iterator end;

    int partition_num = 1;
    while (it != end) {
        std::smatch partition = *it;

        std::cout << "Partition #" << partition_num << ": " << partition.str() << std::endl;
        it++;
        partition_num++;
    }*/

    // Range-based for loop
    /*int array[] = {322, 342343, 3534, 555};

    for (auto itr: array) {
        std::cout << itr << std::endl;
    }*/
    return 0;
}