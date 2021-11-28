#include "SegmentGroup.h"
#include "FileMapper.h"

#include <future>
#include <iostream>

namespace hash_calculator
{

SegmentGroup::SegmentGroup(
    int cpuCount,
    std::vector<std::shared_ptr<Segment>> segments,
    std::vector<std::shared_ptr<FileMapper>> mappers)
    : m_cpuCount(cpuCount),
    m_segments(std::move(segments)),
    m_mappers(std::move(mappers))
{
}

void SegmentGroup::calcHash()
{
    for(const auto& mapper : m_mappers)
    {
        mapper->mapping();
    }

    size_t nextIndex = 0;
    std::vector<std::future<void>> futures;

    while(nextIndex < m_segments.size() || !futures.empty())
    {
        if(futures.size() < m_cpuCount && nextIndex < m_segments.size())
        {
            const auto& segment = m_segments[nextIndex++];
            auto feature = m_segments.size() == 1 ?
                std::async(&Segment::calcHash, segment) :
                std::async(std::launch::async, &Segment::calcHash, segment);
            futures.push_back(std::move(feature));
        }

        auto it = futures.begin();
        while(it !=  futures.end())
        {
            using namespace std::literals::chrono_literals;
            if(it->wait_for(0s) == std::future_status::ready)
            {
                it->get();
                it = futures.erase(it);
                continue;
            }
            
            ++it;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool SegmentGroup::isReady() const
{
    return std::all_of(m_segments.begin(), m_segments.end(), [](const auto& segment){
        return segment->isReady();
    });
}

std::string SegmentGroup::getHash() const
{
    std::string result;
    for(const auto& segment : m_segments)
    {
        auto hash = result.empty() ? segment->getHash() : "\n" + segment->getHash();
        result.append(hash);
    }
    
    return result;
}

} // hash_calculator