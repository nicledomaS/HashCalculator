#pragma once

#include <boost/noncopyable.hpp>

#include <vector>

namespace hash_calculator
{

class FileMapper : private boost::noncopyable
{
public:
    FileMapper(int fd, size_t maxBufsize);

    void* getPtr(int currentPosition, size_t offset) const;
    void mapping(int currentPosition, size_t offset);
    void remapping(int currentPosition, size_t offset);

private:
    int m_fd;
    std::vector<unsigned char> m_data;
};

} // hash_calculator
