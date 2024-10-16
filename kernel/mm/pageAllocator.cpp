#include "kernel/error/error.hpp"
#include "kernel/mm/bitmap.hpp"
#include "limine/limine.h"
#include <cstdint>
#include <kernel/arch/x86_64/memory/memory.hpp>
#include <kernel/essential.hpp>
#include <kernel/kprintf.hpp>

/*
PAGE FRAME ALLOCATOR
using bitmap allocation, allocate by physical address
*/

// in byte
uint64_t memorySize;
uint64_t bitmapSize;
uint64_t baseBitmapAddress;
uint64_t baseBitmapFreeSize;
uint64_t freeMemory;

uint64_t *pageBitmap;
bitmap _bitmap;

void initPageFrameAllocator() {
  // calculate memory size
  limine_memmap_response *mmp = memoryMap;
  memorySize = 0;
  freeMemory = 0;

  for (uint64_t i = 0; i < mmp->entry_count; i++) {
    if (mmp->entries[i]->type != LIMINE_MEMMAP_RESERVED &&
        mmp->entries[i]->type != LIMINE_MEMMAP_BAD_MEMORY &&
        mmp->entries[i]->type != LIMINE_MEMMAP_FRAMEBUFFER) {
      memorySize += mmp->entries[i]->length;
    }

    if (mmp->entries[i]->type == LIMINE_MEMMAP_USABLE) {
      freeMemory += mmp->entries[i]->length;
    }
  }

  // calculate bitmap size
  bitmapSize = memorySize / 1024; // to KB
  bitmapSize /= 4;                // 4KB page
  bitmapSize /= 8;                // 8 bit = 1 byte

  // find suitable address for bitmap
  baseBitmapAddress = 0;
  baseBitmapFreeSize = 0;
  for (uint64_t i = 0; i < mmp->entry_count; i++) {
    if (mmp->entries[i]->type == LIMINE_MEMMAP_USABLE &&
        mmp->entries[i]->length >= bitmapSize) {

      // check if entries[i].length smaller than baseBitmapFreeSize
      if (baseBitmapFreeSize >= bitmapSize &&
          baseBitmapFreeSize < mmp->entries[i]->length) {
        baseBitmapAddress = mmp->entries[i]->base;
        baseBitmapFreeSize = mmp->entries[i]->length;
      } else if (baseBitmapFreeSize <
                 bitmapSize) { // if baseBitmapFreeSize is 0
        baseBitmapAddress = mmp->entries[i]->base;
        baseBitmapFreeSize = mmp->entries[i]->length;
      }
    }
  }

  // check again
  if (baseBitmapFreeSize <= bitmapSize) {
    kprintf("Error: out of memory\n");
    halt();
  }

  // place the bitmap at baseBitmapAddress
  kprintf("SET 0x%x as bitmap\n", baseBitmapAddress);
  pageBitmap = (uint64_t *)(baseBitmapAddress + virtualAddressOffset);

  for (uint64_t i = 0; i < memorySize / 64; i++) {

    pageBitmap[i] = 0;
  }

  _bitmap.arr = pageBitmap;
  _bitmap.size = bitmapSize;

  // check for usable memory again
  for (uint64_t i = 0; i < mmp->entry_count; i++) {
    if (mmp->entries[i]->type == LIMINE_MEMMAP_USABLE) {

      uint64_t baseIndex = 0;
      uint64_t nPage = 0;

      // check if address isnt a begin address of a page
      if (mmp->entries[i]->base % 4096 != 0) {
        baseIndex = mmp->entries[i]->base / 4096 + 1; // use next page
        nPage = (uint64_t)(mmp->entries[i]->length / 4096 - 1);
      } else {
        baseIndex = mmp->entries[i]->base / 4096;
        nPage = (uint64_t)(mmp->entries[i]->length / 4096);
      }

      _bitmap.set(baseIndex, nPage, 1); // available
    }
  }

  // mark bitmap page isnt available
  uint64_t baseIndex = baseBitmapAddress / 4096;
  _bitmap.set(baseIndex, bitmapSize / 4096 + 1,
              0); // mark one page more for fewer bugs

  kprintf("initPageFrameAllocator() success!\n");
}

bool checkPage(uint64_t address) {

  uint64_t index =
      (uint64_t)((uint64_t)(getPhysicalAddress(address)) / 1024 / 4);

  if (index >= bitmapSize * 8) {
    kprintf("check page failed\n");
    return 0;
  }

  return _bitmap.get(index);
}

uint64_t allocPage(uint64_t nPage) {
  if (nPage == 0) {
    return 0;
  }

  if (nPage * 4096 > freeMemory) {
    kprintf("Error: out of memory\n");
    return 0;
  }

  // looping throught uint64_t array and checking for bit that not equal to
  // zero
  for (uint64_t i = 0; i < _bitmap.size / 8; i++) {
    uint64_t c = 0;
    uint64_t j = 0;
    uint64_t iOffset = i * 64;
    uint64_t bPage = 0;
    if (_bitmap.arr[i] != 0) {
      // begin searching with block i
      // searching for empty bit
      while ((j + iOffset) < _bitmap.size && (_bitmap.get(iOffset + j) != 1)) {
        j++;
      }
      bPage = j;
      // searching for empty page
      while (c < nPage) {
        if (_bitmap.get(iOffset + j) == 1) {
          c++;
          j++;
        } else {
          break;
        }
      }
    }

    if (c == nPage) {
      freeMemory -= 4096 * nPage;

      // marking as used
      for (uint64_t k = 0; k < c; k++) {
        _bitmap.set(iOffset + bPage + k, 0);
      }

      return virtualAddressOffset + (iOffset + bPage) * 4096;
    }
  }

  fireError(OUT_OF_MEMORY);
  return 0;
}

void deallocPage(uint64_t address, uint64_t nPage) {
  // getting index
  uint64_t index = getPhysicalAddress(address) / 4096;

  // mark as available
  _bitmap.set(index, nPage, 1);

  freeMemory += 4096;
}