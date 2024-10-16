#include "memoryAllocation.hpp"
#include <cstdint>

typedef struct {
  uint32_t size;
  uint32_t nextBlock;
} blockHeader;

void allocator::init(uint64_t addr, uint64_t _size) {
  address = addr;
  size = _size;

  blockHeader *base = (blockHeader *)addr;
  base->size = 0;
  base->nextBlock = 0;
  baseBlock = 0;
}

uint64_t allocator::getFreeMem(void *_block) {
  blockHeader *block = (blockHeader *)_block;

  if (block->nextBlock != 0) { // get free memory between two block
    blockHeader *nextBlock = (blockHeader *)(address + block->nextBlock);
    return (uint64_t)(nextBlock) -
           ((uint64_t)block + sizeof(blockHeader) + block->size);
  }

  // get remain memory
  return ((uint64_t)address + size) -
         ((uint64_t)block + sizeof(blockHeader) + block->size);
}

void *allocator::alloc(uint64_t memSize) {
  if (memSize == 0) {
    return nullptr;
  }
  blockHeader *block = (blockHeader *)(baseBlock + address);
  // if base block has size = 0
  if (block->size == 0) {
    // alloc at base
    if (memSize <= size) {
      block->size = memSize;
      return (void *)(address + sizeof(blockHeader));
    } else {
      return nullptr;
    }
  }

  // alloc when number of blocks >= 1
  // find suitable free memory block
  while (block != nullptr) {
    if (getFreeMem(block) >= (memSize + sizeof(blockHeader))) { // allocate
      blockHeader *tmp =
          (blockHeader *)((uint64_t)block + sizeof(blockHeader) + block->size);

      tmp->nextBlock = block->nextBlock;
      tmp->size = memSize;

      block->nextBlock = (uint64_t)tmp - address;
      return (void *)((uint64_t)tmp + sizeof(blockHeader));
    }

    if (block->nextBlock == 0) {
      block = nullptr;
    } else
      block = (blockHeader *)(address + block->nextBlock);
  }

  return nullptr;
}

void allocator::free(void *ptr) {
  uint64_t ptrAddr = (uint64_t)ptr;
  blockHeader *block = (blockHeader *)(ptrAddr - sizeof(blockHeader));
  // if free the base block
  if ((ptrAddr - address - sizeof(blockHeader)) == baseBlock) {
    // get NextBlock
    uint32_t nextBlock = block->nextBlock;
    baseBlock = nextBlock;

    // if there is only one block
    if (nextBlock == 0) {
      block->size = 0;
    }
    return;
  }

  // free block that is not the base block
  block = (blockHeader *)(address + baseBlock);
  uint32_t blockID = ptrAddr - address - sizeof(blockHeader);

  // find block that point to blockID
  while (block->nextBlock != blockID) {

    // avoid set block as null
    if (block->nextBlock == 0) {
      return;
    }

    block = (blockHeader *)(address + block->nextBlock);
  }

  // dealloc
  blockHeader *freeBlock = (blockHeader *)(ptrAddr - sizeof(blockHeader));
  block->nextBlock = freeBlock->nextBlock;
}