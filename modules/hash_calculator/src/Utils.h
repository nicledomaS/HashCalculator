#pragma once

#include <string>
#include <vector>
#include <memory>

#include <sys/stat.h>

namespace hash_calculator
{

class Segment;

struct FileDesc
{
    int fd;
    struct stat sb;
};

inline size_t toBytes(size_t mbs)
{
    return mbs << 20;
}

FileDesc fileOpen(const std::string& inFilePath);
std::vector<std::shared_ptr<Segment>> prepareSegments(const FileDesc& desc, size_t blockSize);

} // hash_calculator