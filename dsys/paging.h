#pragma once

#include <optional>
#include <vector>

#include <dsys/arch.h>
#include <dsys/literals.h>

const uint32_t PageSize = 1_KB;
const uint32_t PageCells = PageSize / sizeof(RamCellType);

using PageIdx = uint32_t;
using Page = std::array<RamCellType, PageCells>;

class PageAllocator {
public:
    PageAllocator(size_t pageCount);

    std::optional<PageIdx> Allocate(size_t pagesToAllocate);
    void Free(PageIdx firstPage, size_t pagesToFree);

private:
    // TODO : better allocator, e.g. based on Descartes-tree
    size_t _pageCount;
    std::vector<bool> _pages;
};