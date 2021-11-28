#include "Utils.h"
#include "SegmentImpl.h"
#include "SegmentGroup.h"
#include "FileMapper.h"

#include <iostream>

#include <fcntl.h>
#include <sched.h> 

namespace hash_calculator
{

namespace
{

constexpr auto MaxSingleBufSize = 200; // MB
constexpr auto MaxPartBufSize = 50; // MB
constexpr auto MaxBigSegmentCount = 4;
constexpr auto MaxCpuCount = 4;

static cpu_set_t mask;

void assignCores(int cpuCount)
{
    CPU_ZERO(&mask);
    int currentCpuId = 0;
    while(currentCpuId < cpuCount)
    {
        CPU_SET(currentCpuId++, &mask);
    }

    if(sched_setaffinity(0, sizeof(mask), &mask) == 0)
    {
        std::cout << "Assigned " << cpuCount << " first CPUs" << std::endl;
    }
    else
    {
        std::cerr << "CPUs did not assigned" << std::endl;
    }
}

std::vector<std::shared_ptr<Segment>> segmentsWithMinBlock(
    const FileDesc& desc, long blockSize, long size, int cpuCount, size_t segmentCount)
{
    std::vector<std::shared_ptr<Segment>> groups;
    std::vector<std::shared_ptr<Segment>> segments;
    std::vector<std::shared_ptr<FileMapper>> mappers;

    auto pos = 0;
    while(pos < desc.sb.st_size)
    {
        auto diff = desc.sb.st_size - pos;
        auto fileMapOffset = diff < size ? diff : size;
        auto fileMapper = std::make_shared<FileMapper>(desc.fd, pos, fileMapOffset);

        auto startPos = pos;

        for(size_t i = 0; i < segmentCount && startPos < desc.sb.st_size; ++i)
        {
            auto diff = desc.sb.st_size - startPos;
            auto offset = diff < blockSize ? diff : blockSize;

            auto segment = std::make_shared<SegmentImpl>(fileMapper, startPos, startPos + offset, offset);
            segments.push_back(std::move(segment));
            startPos += offset;
        }

        mappers.push_back(std::move(fileMapper));

        auto groupSegment = std::make_shared<SegmentGroup>(cpuCount, std::move(segments), std::move(mappers));
        groups.push_back(std::move(groupSegment));

        pos += fileMapOffset;
    }

    return groups;
}

std::vector<std::shared_ptr<Segment>> segmentsWithMaxBlock(
    const FileDesc& desc, long blockSize, long size, int cpuCount, size_t segmentCount = MaxBigSegmentCount)
{
    std::vector<std::shared_ptr<Segment>> groups;
    std::vector<std::shared_ptr<Segment>> segments;
    std::vector<std::shared_ptr<FileMapper>> mappers;
    
    auto pos = 0;
    while(pos < desc.sb.st_size)
    {
        auto diff = desc.sb.st_size - pos;
        auto fileMapOffset = diff < size ? diff : size;
        auto fileMapper = std::make_shared<FileMapper>(desc.fd, pos, fileMapOffset);
        
        auto segmentOffset = diff < blockSize ? diff : blockSize;
        auto segment = std::make_shared<SegmentImpl>(fileMapper, pos, pos + segmentOffset, fileMapOffset);
        segments.push_back(std::move(segment));
        mappers.push_back(std::move(fileMapper));

        pos += segmentOffset;

        if(segments.size() == segmentCount || pos == desc.sb.st_size)
        {
            auto segment = std::make_shared<SegmentGroup>(cpuCount, std::move(segments), std::move(mappers));
            groups.push_back(std::move(segment));
        }
    }

    return groups;
}

} // anonymous

FileDesc fileOpen(const std::string& inFilePath)
{
    int fd = open(inFilePath.c_str(), O_RDONLY);
    if (fd == -1)
    {
        throw std::runtime_error("File did not open");
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        throw std::runtime_error("File information did not get");
    }

    return {fd, sb};
}

std::vector<std::shared_ptr<Segment>> prepareSegments(const FileDesc& desc, size_t blockSize)
{
    auto allCpuCount = std::thread::hardware_concurrency();
    auto cpuCount = allCpuCount > MaxCpuCount ? MaxCpuCount : allCpuCount;
    assignCores(cpuCount);

    if(desc.sb.st_size == 0)
    {
        std::vector<std::shared_ptr<FileMapper>> mappers;
        std::vector<std::shared_ptr<Segment>> segments;
        auto fileMapper = std::make_shared<FileMapper>(desc.fd, 0, 0);
        segments.push_back(std::make_shared<SegmentImpl>(fileMapper, 0, 0, 0));
        mappers.push_back(std::move(fileMapper));
        auto segmentGroup = std::make_shared<SegmentGroup>(cpuCount, std::move(segments), std::move(mappers));
        return { segmentGroup };
    }

    auto blockSizeInBytes = toBytes(blockSize);
    auto segmentCount = (MaxSingleBufSize / blockSize);
    auto size = segmentCount > 0 ? segmentCount * blockSizeInBytes : toBytes(MaxPartBufSize);
    
    return segmentCount > 1 ?
        segmentsWithMinBlock(desc, blockSizeInBytes, size, cpuCount, segmentCount) :
        segmentsWithMaxBlock(desc, blockSizeInBytes, size, cpuCount);
}

} // hash_calculator