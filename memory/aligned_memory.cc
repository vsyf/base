/*
 * aligned_memory.cc
 * Copyright (C) 2023 youfa <vsyfar@gmail.com>
 *
 * Distributed under terms of the GPLv2 license.
 */

#include "aligned_memory.h"

#include <stdlib.h>  // for free, malloc
#include <string.h>  // for memcpy

#include "base/checks.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <stdint.h>
#endif

// Reference on memory alignment:
// http://stackoverflow.com/questions/227897/solve-the-memory-alignment-in-c-interview-question-that-stumped-me
namespace ave {
namespace base {

uintptr_t GetRightAlign(uintptr_t start_pos, size_t alignment) {
  // The pointer should be aligned with `alignment` bytes. The - 1 guarantees
  // that it is aligned towards the closest higher (right) address.
  return (start_pos + alignment - 1) & ~(alignment - 1);
}

// Alignment must be an integer power of two.
bool ValidAlignment(size_t alignment) {
  if (!alignment) {
    return false;
  }
  return (alignment & (alignment - 1)) == 0;
}

void* GetRightAlign(const void* pointer, size_t alignment) {
  if (!pointer) {
    return NULL;
  }
  if (!ValidAlignment(alignment)) {
    return NULL;
  }
  uintptr_t start_pos = reinterpret_cast<uintptr_t>(pointer);
  return reinterpret_cast<void*>(GetRightAlign(start_pos, alignment));
}

void* AlignedMalloc(size_t size, size_t alignment) {
  if (size == 0) {
    return NULL;
  }
  if (!ValidAlignment(alignment)) {
    return NULL;
  }

  // The memory is aligned towards the lowest address that so only
  // alignment - 1 bytes needs to be allocated.
  // A pointer to the start of the memory must be stored so that it can be
  // retreived for deletion, ergo the sizeof(uintptr_t).
  void* memory_pointer = malloc(size + sizeof(uintptr_t) + alignment - 1);
  AVE_CHECK(memory_pointer) << "Couldn't allocate memory in AlignedMalloc";

  // Aligning after the sizeof(uintptr_t) bytes will leave room for the header
  // in the same memory block.
  uintptr_t align_start_pos = reinterpret_cast<uintptr_t>(memory_pointer);
  align_start_pos += sizeof(uintptr_t);
  uintptr_t aligned_pos = GetRightAlign(align_start_pos, alignment);
  void* aligned_pointer = reinterpret_cast<void*>(aligned_pos);

  // Store the address to the beginning of the memory just before the aligned
  // memory.
  uintptr_t header_pos = aligned_pos - sizeof(uintptr_t);
  void* header_pointer = reinterpret_cast<void*>(header_pos);
  uintptr_t memory_start = reinterpret_cast<uintptr_t>(memory_pointer);
  memcpy(header_pointer, &memory_start, sizeof(uintptr_t));

  return aligned_pointer;
}

void AlignedFree(void* mem_block) {
  if (mem_block == NULL) {
    return;
  }
  uintptr_t aligned_pos = reinterpret_cast<uintptr_t>(mem_block);
  uintptr_t header_pos = aligned_pos - sizeof(uintptr_t);

  // Read out the address of the AlignedMemory struct from the header.
  uintptr_t memory_start_pos = *reinterpret_cast<uintptr_t*>(header_pos);
  void* memory_start = reinterpret_cast<void*>(memory_start_pos);
  free(memory_start);
}
}  // namespace base
}  // namespace ave
