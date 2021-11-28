#include "HashCalculator.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <string>
#include <memory>

namespace program_opt = boost::program_options;

constexpr auto Help = "help";
constexpr auto Input = "input";
constexpr auto Output = "output";
constexpr auto Size = "size";
constexpr auto DefaultSize = 1;

int main (int argc, char** argv)
{
    try
    {
        program_opt::options_description desc("Allowed options");
        desc.add_options()
        (Help, "produce help message")
        (Input, program_opt::value<std::string>(), "set input file")
        (Output, program_opt::value<std::string>(), "set output file")
        (Size, program_opt::value<size_t>(), "set segment size");

        program_opt::variables_map varMap;
        program_opt::store(program_opt::parse_command_line(argc, argv, desc), varMap);
        program_opt::notify(varMap);

        if (varMap.count(Help) || !varMap.count(Input) || !varMap.count(Output)) {
            std::cout << desc << "\n";
            return 0;
        }

        auto input = varMap[Input].as<std::string>();
        auto output = varMap[Output].as<std::string>();
        auto segmentSize = varMap.count(Size) ? varMap[Size].as<size_t>() : DefaultSize;

        auto hashCalc = std::make_unique<hash_calculator::HashCalculator>(
            std::move(input), std::move(output), segmentSize);
        hashCalc->calc();
        hashCalc->saveResult();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Exception of unknown type!" << std::endl;
    }

    return 0;
}