#include "SegmentImpl.h"
#include "FileMapper.h"

#include <boost/algorithm/hex.hpp>
#include <boost/endian/conversion.hpp>

#include <iostream>
#include <vector>

namespace hash_calculator
{

namespace
{

std::string toString(const MD5::digest_type &digest)
{
    std::string result;
    if constexpr (boost::endian::order::native == boost::endian::order::big)
    {
        boost::algorithm::hex(digest, digest + sizeof(MD5::digest_type), std::back_inserter(result));
    }
    else
    {
        std::vector<unsigned int> transformResult;
        std::transform(
            digest,
            digest + 4,
            std::back_inserter(transformResult),
            [](const auto& val){
                return boost::endian::little_to_native(val);
            });
        boost::algorithm::hex(transformResult.begin(), transformResult.end(), std::back_inserter(result));
    }

    return result;
}

} // anonymous

SegmentImpl::SegmentImpl(
    const std::shared_ptr<FileMapper>& fileMapper,
    size_t startPosition,
    size_t endPosition,
    size_t offset)
    : m_fileMapper(fileMapper),
    m_currentPosition(startPosition),
    m_endPosition(endPosition),
    m_offset(offset)
{
}

void SegmentImpl::calcHash()
{
    if(m_offset == 0)
    {
        m_hash.get_digest(m_digest);
        m_isReady = true;
        return;
    }

    while(!m_isReady)
    {
        auto diff = m_endPosition - m_currentPosition;
        auto offset = diff < m_offset ? diff : m_offset;
        auto ptr = m_fileMapper->getPtr(m_currentPosition, offset);
        if(ptr == nullptr)
        {
            m_isReady = true;
            break;
        }
      
        m_hash.process_bytes(ptr, offset);
        m_currentPosition += offset;
        m_isReady = m_currentPosition == m_endPosition;

        if(!m_isReady)
        {
            auto newDiff = m_endPosition - m_currentPosition;
            auto newOffset = newDiff < m_offset ? newDiff : m_offset;
            m_fileMapper->remapping(m_currentPosition, newOffset);
        }
    }

    m_hash.get_digest(m_digest);
}

bool SegmentImpl::isReady() const
{
    return m_isReady;
}

std::string SegmentImpl::getHash() const
{
    if(!m_isReady)
    {
        throw std::runtime_error("Hash did not call");
    }

    return toString(m_digest);
}

} // hash_calculator