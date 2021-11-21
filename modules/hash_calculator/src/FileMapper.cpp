#include "FileMapper.h"

namespace hash_calculator
{

FileMapper::FileMapper(int fd, size_t maxBufsize)
    : m_fd(fd),
    m_data(maxBufsize, 0)
{
}

void* FileMapper::getPtr(int currentPosition, size_t offset) const
{
    return nullptr;
}

void FileMapper::mapping(int currentPosition, size_t offset)
{

}

void FileMapper::remapping(int currentPosition, size_t offset)
{

}

} // hash_calculator