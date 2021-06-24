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

#ifndef LLVM_AUTOMEMCPY_SUPPLEMENTARY_MATERIALS_CODE_MEMCPY_H
#define LLVM_AUTOMEMCPY_SUPPLEMENTARY_MATERIALS_CODE_MEMCPY_H

#include <cstddef>
#include <string_view>

#include "llvm/ADT/ArrayRef.h"

using memcpy_function = void (*)(char* __restrict dst,
                                 const char* __restrict src, size_t count);
struct MemcpyFunction {
  std::string_view name;
  memcpy_function function;
  size_t overlap_point;
  size_t loop_point;
  size_t loop_block_size;
  size_t accelerator_point;
  bool optimized;
};

llvm::ArrayRef<const MemcpyFunction> GetMemoryFunctions();

#endif // LLVM_AUTOMEMCPY_SUPPLEMENTARY_MATERIALS_CODE_MEMCPY_H
