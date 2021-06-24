// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MEMCPY_INL_H
#define MEMCPY_INL_H
#include <stddef.h>  // size_t
#include <stdint.h>  // intptr_t / uintptr_t

// Return whether `value` is zero or a power of two.
static constexpr bool is_power2_or_zero(size_t value) {
  return (value & (value - 1U)) == 0;
}

// Return whether `value` is a power of two.
static constexpr bool is_power2(size_t value) {
  return value && is_power2_or_zero(value);
}

// Compile time version of log2 that handles 0.
static constexpr size_t log2(size_t value) {
  return (value == 0 || value == 1) ? 0 : 1 + log2(value / 2);
}

// Returns the first power of two preceding value or value if it is already a
// power of two (or 0 when value is 0).
static constexpr size_t le_power2(size_t value) {
  return value == 0 ? value : 1ULL << log2(value);
}

// Returns the first power of two following value or value if it is already a
// power of two (or 0 when value is 0).
static constexpr size_t ge_power2(size_t value) {
  return is_power2_or_zero(value) ? value : 1ULL << (log2(value) + 1);
}

template <size_t alignment>
intptr_t offset_from_last_aligned(const void *ptr) {
  static_assert(is_power2(alignment), "alignment must be a power of 2");
  return reinterpret_cast<uintptr_t>(ptr) & (alignment - 1U);
}

template <size_t alignment>
intptr_t offset_to_next_aligned(const void *ptr) {
  static_assert(is_power2(alignment), "alignment must be a power of 2");
  // The logic is not straightforward and involves unsigned modulo arithmetic
  // but the generated code is as fast as it can be.
  return -reinterpret_cast<uintptr_t>(ptr) & (alignment - 1U);
}

// Returns the offset from `ptr` to the next cache line.
static inline intptr_t offset_to_next_cache_line(const void *ptr) {
  return offset_to_next_aligned<64>(ptr);
}

// Copies `kBlockSize` bytes from `src` to `dst`.
template <size_t kBlockSize>
static void CopyBlock(char *__restrict dst, const char *__restrict src) {
  __builtin_memcpy_inline(dst, src, kBlockSize);
}

// Copies `kBlockSize` bytes from `src + count - kBlockSize` to
// `dst + count - kBlockSize`.
// Precondition: `count >= kBlockSize`.
template <size_t kBlockSize>
static void CopyLastBlock(char *__restrict dst, const char *__restrict src,
                          size_t count) {
  const size_t offset = count - kBlockSize;
  CopyBlock<kBlockSize>(dst + offset, src + offset);
}

// Copies `kBlockSize` bytes twice with an overlap between the two.
//
// [1234567812345678123]
// [__XXXXXXXXXXXXXX___]
// [__XXXXXXXX_________]
// [________XXXXXXXX___]
//
// Precondition: `count >= kBlockSize && count <= kBlockSize`.
template <size_t kBlockSize>
static void CopyBlockOverlap(char *__restrict dst, const char *__restrict src,
                             size_t count) {
  CopyBlock<kBlockSize>(dst, src);
  CopyLastBlock<kBlockSize>(dst, src, count);
}

// Copies `count` bytes by blocks of `kBlockSize` bytes.
// Copies at the start and end of the buffer are unaligned.
// Copies in the middle of the buffer are aligned to `kBlockSize`.
//
// e.g. with
// [12345678123456781234567812345678]
// [__XXXXXXXXXXXXXXXXXXXXXXXXXXX___]
// [__XXXXXXXX______________________]
// [________XXXXXXXX________________]
// [________________XXXXXXXX________]
// [_____________________XXXXXXXX___]
//
// Precondition: `count > 2 * kBlockSize` for efficiency.
//               `count >= kBlockSize` for correctness.
template <size_t kBlockSize>
static void CopyAlignedBlocks(char *__restrict dst, const char *__restrict src,
                              size_t count) {
  CopyBlock<kBlockSize>(dst, src);  // Copy first block

  // Copy aligned blocks
  size_t offset = kBlockSize - offset_from_last_aligned<kBlockSize>(dst);
  for (; offset + kBlockSize < count; offset += kBlockSize)
    CopyBlock<kBlockSize>(dst + offset, src + offset);

  CopyLastBlock<kBlockSize>(dst, src, count);  // Copy last block
}

#ifdef __x86_64__
static void CopyRepMovsb(char *__restrict dst, const char *__restrict src,
                         size_t count) {
  asm volatile("rep movsb" : "+D"(dst), "+S"(src), "+c"(count) : : "memory");
}
#endif


#endif // MEMCPY_INL_H
