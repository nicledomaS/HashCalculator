#include "HashCalculator.h"
#include "FileMapper.h"
#include "SegmentImpl.h"
#include "SegmentGroup.h"

#include <iostream>
#include <fstream>

namespace hash_calculator
{

HashCalculator::HashCalculator(
    std::string inFilePath,
    std::string outFilePath,
    size_t blockSize)
    : m_inFilePath(std::move(inFilePath)),
    m_outFilePath(std::move(outFilePath)),
    m_blockSize(blockSize),
    m_desc(fileOpen(m_inFilePath)),
    m_segments(prepareSegments(m_desc, m_blockSize))
{
}

void HashCalculator::calc()
{
    std::cout << "File: " << m_inFilePath << std::endl;
    std::cout << "File size: " << m_desc.sb.st_size / 1024 / 1024 << "Mb" << std::endl;
    std::cout << "Segment size: " << m_blockSize << "Mb" << std::endl;
    std::cout << "Created segments: " << m_segments.size() << std::endl;

    for(const auto& segment : m_segments)
    {
        segment->calcHash();

        if(segment->isReady())
        {
            std::cout << "Hash was calculate" << std::endl;
        }
    }
}

void HashCalculator::saveResult()
{
    std::cout << "Save hashes to file: " << std::endl;
    std::ofstream ostrm(m_outFilePath, std::ios::out | std::ios::trunc);

    for(const auto& segment : m_segments)
    {
        auto hash = segment->getHash();
        ostrm << hash << std::endl;
        std::cout << hash << std::endl;
    }
}

} // hash_calculator
