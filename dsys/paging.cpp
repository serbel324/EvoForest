#include <dsys/paging.h>

#include <library/ext_math.h>

#include <cassert>

PageAllocator::PageAllocator(size_t pageCount)
    : _pageCount(pageCount)
    , _pages(pageCount, true)
{}

std::optional<PageIdx> PageAllocator::Allocate(size_t pagesToAllocate) {
    assert(pagesToAllocate > 0);

    size_t emptySequenceBegin = ExtMath::RandomInt(0, _pageCount);
    size_t emptySequenceIdx = emptySequenceBegin;
    size_t emptySequenceSize = 0;

    size_t pagesChecked = 0;

    while (pagesChecked < _pageCount) {
        if (!_pages[emptySequenceIdx]) {
            emptySequenceSize = 0;
            emptySequenceBegin = (emptySequenceIdx + 1) % _pageCount;
            emptySequenceIdx = emptySequenceBegin;
            ++pagesChecked;
            continue;
        }
        while (_pages[emptySequenceIdx]) {
            emptySequenceIdx = (emptySequenceIdx + 1) % _pageCount;
            ++pagesChecked;
            if (++emptySequenceSize == pagesToAllocate) {
                for (size_t idx = emptySequenceBegin, i = 0; i < pagesToAllocate;
                        idx = (emptySequenceIdx + 1) % _pageCount, ++i) {
                    _pages[idx] = false;
                }
                return {emptySequenceBegin};
            }
        }
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
