#include <boost/range/adaptors.hpp>

#include <cstdlib>
#include <iostream>

int main()
{
    const std::vector< int > nums{ 1, 2, 3, 4, 5, 6, 7, 8 };

    for ( const auto n : nums | boost::adaptors::filtered( []( auto i ) { return i % 2 == 0; } ) )
    {
        std::cout << n << std::endl;
    }

    return EXIT_SUCCESS;
}
