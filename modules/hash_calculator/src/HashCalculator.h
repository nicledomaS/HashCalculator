#pragma once

#include "Utils.h"

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include <memory>

namespace hash_calculator
{

class Segment;
class FileMapper;

class HashCalculator : private boost::noncopyable
{
public:
    HashCalculator(
        std::string inFilePath,
        std::string outFilePath,
        size_t blockSize);

    void calc();
    void saveResult();

private:
    std::string m_inFilePath;
    std::string m_outFilePath;
    size_t m_blockSize;
    FileDesc m_desc;
    std::vector<std::shared_ptr<Segment>> m_segments;
};

} // hash_calculator
