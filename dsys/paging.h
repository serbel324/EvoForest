#pragma once

#include <optional>
#include <vector>

#include <dsys/arch.h>
#include <dsys/literals.h>

using PageIdx = uint32_t;

class PageAllocator {
public:
    PageAllocator(size_t pageCount);

    std::optional<PageIdx> Allocate(size_t pagesToAllocate);
    void Free(PageIdx firstPage, size_t pagesToFree);

private:
    // TODO : better allocator, e.g. based on Cartesian tree
    size_t _pageCount;
    std::vector<bool> _pages;
};