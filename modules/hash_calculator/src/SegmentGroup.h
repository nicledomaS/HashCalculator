#pragma once

#include "Segment.h"

#include <string>
#include <vector>
#include <memory>
#include <future>

namespace hash_calculator
{

class SegmentGroup final : public Segment
{
public:
    SegmentGroup(std::vector<std::shared_ptr<Segment>> segments);

    void calcHash() override;
    bool isReady() const override;
    std::string getHash() const override;

private:
    std::vector<std::shared_ptr<Segment>> m_segments;
    std::vector<std::future<void>> m_futures;
};

} // hash_calculator