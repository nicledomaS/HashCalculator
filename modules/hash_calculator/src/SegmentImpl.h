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
        std::shared_ptr<FileMapper> fileMapper,
        int startPosition,
        size_t offset);

    void calcHash() override;
    bool isReady() const override;
    std::string getHash() const override;

private:
    std::shared_ptr<FileMapper> m_fileMapper;
    int m_startPosition;
    int m_currentPosition;
    size_t m_offset;
    MD5 m_hash;
};

} // hash_calculator