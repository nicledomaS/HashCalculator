#include "HashCalculator.h"

namespace hash_calculator
{

HashCalculator::HashCalculator(
    std::string inFilePath,
    std::string outFilePath,
    size_t blockSize)
    : m_inFilePath(std::move(inFilePath)),
    m_outFilePath(std::move(outFilePath)),
    m_blockSize(blockSize)
{}

void HashCalculator::calc()
{

}

void HashCalculator::saveResult()
{
    
}

} // hash_calculator
