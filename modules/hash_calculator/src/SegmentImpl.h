#pragma once

#include "Segment.h"

#include <boost/uuid/detail/md5.hpp>

#include <string>
#include <memory>

using MD5 = boost::uuids::detail::md5;

namespace hash_calculator
{

class FileMapper;

class SegmentImpl final : public Segment
{
public:
    SegmentImpl(
        const std::shared_ptr<FileMapper>& fileMapper,
        size_t startPosition,
        size_t endPosition,
        size_t offset);

    void calcHash() override;
    bool isReady() const override;
    std::string getHash() const override;

private:
    std::shared_ptr<FileMapper> m_fileMapper;
    size_t m_currentPosition;
    size_t m_endPosition;
    size_t m_offset;
    MD5 m_hash;
    MD5::digest_type m_digest;
    bool m_isReady = false;
};

} // hash_calculator