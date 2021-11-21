#include "SegmentImpl.h"

namespace hash_calculator
{

SegmentImpl::SegmentImpl(
    std::shared_ptr<FileMapper> fileMapper,
    int startPosition,
    size_t offset)
    : m_fileMapper(std::move(fileMapper)),
    m_startPosition(startPosition),
    m_currentPosition(startPosition),
    m_offset(offset)
{

}

void SegmentImpl::calcHash()
{
}

bool SegmentImpl::isReady() const
{
    return false;
}

std::string SegmentImpl::getHash() const
{
    return {};
}

} // hash_calculator