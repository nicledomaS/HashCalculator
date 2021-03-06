#include "FileMapper.h"

#include <iostream>

#include <sys/mman.h>

namespace hash_calculator
{

namespace
{

void* map(int fd, size_t startPosition, size_t maxBufsize)
{
    return mmap(nullptr, maxBufsize, PROT_READ, MAP_PRIVATE, fd, startPosition);
}

bool unmap(void** ptr, size_t size)
{
    auto res = munmap(*ptr, size) == 0;
    *ptr = nullptr;
    return res;
}

} // anonymous

FileMapper::FileMapper(int fd, size_t startPosition, size_t maxBufsize)
    : m_fd(fd),
    m_startPosition(startPosition),
    m_maxBufsize(maxBufsize),
    m_ptr(nullptr)
{
}

FileMapper::~FileMapper()
{
    unmap(&m_ptr, m_maxBufsize);
}

void* FileMapper::getPtr(size_t currentPosition, size_t offset) const
{
    if(currentPosition >= m_startPosition && (currentPosition + offset) <= (m_startPosition + m_maxBufsize))
    {
        auto pos = currentPosition - m_startPosition;
        return m_ptr + pos;
    }

    throw std::overflow_error("The requested memory area is not available");
}

void FileMapper::mapping()
{
    if(!m_ptr)
    {
        std::cout << "Mapping" << std::endl;
        std::cout << "Start position: " << m_startPosition << std::endl;
        std::cout << "Max Bufsize: " << m_maxBufsize << std::endl;
        m_ptr = map(m_fd, m_startPosition, m_maxBufsize);
        if(m_ptr == MAP_FAILED && m_maxBufsize)
        {
            throw std::runtime_error("Mapping is failed");
        }
    }
    else
    {
        throw std::runtime_error("Memory has already mapped");
    }
}

void FileMapper::remapping(size_t startPosition, size_t offset)
{
    if(m_ptr && unmap(&m_ptr, m_maxBufsize))
    {
        m_startPosition = startPosition;
        m_maxBufsize = offset;

        mapping();
    }
    else
    {
        throw std::runtime_error("Remapping is failed");
    }
}

} // hash_calculator