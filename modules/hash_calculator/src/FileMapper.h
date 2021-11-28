#pragma once

#include <gsl/gsl>

#include <boost/noncopyable.hpp>

namespace hash_calculator
{

class FileMapper : private boost::noncopyable
{
public:
    FileMapper(int fd, size_t startPosition, size_t maxBufsize);
    ~FileMapper();

    void* getPtr(size_t currentPosition, size_t offset) const;
    void mapping();
    void remapping(size_t startPosition, size_t offset);

private:
    int m_fd;
    size_t m_startPosition;
    size_t m_maxBufsize;
    void* m_ptr;
};

} // hash_calculator
