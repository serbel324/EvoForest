#include <dsys/paging.h>

#include <cassert>

PageAllocator::PageAllocator(size_t pageCount)
    : _pageCount(pageCount)
    , _pages(pageCount, true)
{}

std::optional<PageIdx> PageAllocator::Allocate(size_t pagesToAllocate) {
    assert(pagesToAllocate > 0);

    size_t intervalBegin = 0;
    size_t intervalEnd = 0;

    while (intervalBegin < _pageCount) {
        if (!_pages[intervalBegin]) {
            intervalEnd = ++intervalBegin;
            continue;
        }
        while (intervalEnd < _pageCount && _pages[intervalEnd]) {
            ++intervalEnd;
            if (intervalEnd - intervalBegin == pagesToAllocate) {
                for (size_t i = intervalBegin; i < intervalEnd; ++i) {
                    _pages[i] = false;
                }
                return {intervalBegin};
            }
        }
        intervalBegin = intervalEnd;
    }
    return {};
}

void PageAllocator::Free(PageIdx firstPage, size_t pagesToFree) {
    while (pagesToFree > 0) {
        assert(!_pages[firstPage]);
        _pages[firstPage++] = true;
        pagesToFree--;
    }
}
