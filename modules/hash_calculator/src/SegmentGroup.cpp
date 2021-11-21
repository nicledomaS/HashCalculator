#include "SegmentGroup.h"

namespace hash_calculator
{

SegmentGroup::SegmentGroup(std::vector<std::shared_ptr<Segment>> segments)
    : m_segments(std::move(segments))
{

}

void SegmentGroup::calcHash()
{

}

bool SegmentGroup::isReady() const
{
    return false;
}

std::string SegmentGroup::getHash() const
{
    return {};
}

} // hash_calculator