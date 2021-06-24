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

#include "memcpy.h"
#include "memcpy.inl.h"

extern "C" void* memcpy(void*, const void*, size_t);
extern "C" void memcpy_glibc_wrapper(char* __restrict dst,
                                     const char* __restrict src, size_t count) {
  memcpy(dst, src, count);
}

extern "C" void* __memcpy_aarch64_simd(void*, const void*, size_t);
extern "C" void memcpy_aor_wrapper(char* __restrict dst,
                                   const char* __restrict src, size_t count) {
  __memcpy_aarch64_simd(dst, src, count);
}

extern "C" void memcpy_I4_O32_B8_Linf_noopt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                return CopyAlignedBlocks<8>(dst, src, count);  // [   32, +inf]
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.192802
extern "C" void memcpy_I4_O32_B8_Linf_opt(char* __restrict dst,
                                          const char* __restrict src,
                                          size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      return CopyAlignedBlocks<8>(dst, src, count);  // [   32, +inf]
    }
  }
}

extern "C" void memcpy_I4_O64_B8_Linf_noopt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  return CopyAlignedBlocks<8>(dst, src,
                                              count);  // [   64, +inf]
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.280604
extern "C" void memcpy_I4_O64_B8_Linf_opt(char* __restrict dst,
                                          const char* __restrict src,
                                          size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64) {
        return CopyAlignedBlocks<8>(dst, src, count);  // [   64, +inf]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      }
    }
  }
}

extern "C" void memcpy_I4_O64_B16_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  return CopyAlignedBlocks<16>(dst, src,
                                               count);  // [   64, +inf]
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.280604
extern "C" void memcpy_I4_O64_B16_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64) {
        return CopyAlignedBlocks<16>(dst, src, count);  // [   64, +inf]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      }
    }
  }
}

extern "C" void memcpy_I4_O128_B8_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    return CopyAlignedBlocks<8>(dst, src,
                                                count);  // [  128, +inf]
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.316722
extern "C" void memcpy_I4_O128_B8_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        return CopyAlignedBlocks<8>(dst, src, count);  // [  128, +inf]
      }
    }
  }
}

extern "C" void memcpy_I4_O128_B16_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    return CopyAlignedBlocks<16>(dst, src,
                                                 count);  // [  128, +inf]
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.316722
extern "C" void memcpy_I4_O128_B16_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        return CopyAlignedBlocks<16>(dst, src, count);  // [  128, +inf]
      }
    }
  }
}

extern "C" void memcpy_I4_O128_B32_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    return CopyAlignedBlocks<32>(dst, src,
                                                 count);  // [  128, +inf]
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.316722
extern "C" void memcpy_I4_O128_B32_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        return CopyAlignedBlocks<32>(dst, src, count);  // [  128, +inf]
      }
    }
  }
}

extern "C" void memcpy_I4_O256_B8_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    if (count < 256) {
                      return CopyBlockOverlap<128>(dst, src,
                                                   count);  // [  128,  255]
                    } else if (count >= 256) {
                      return CopyAlignedBlocks<8>(dst, src,
                                                  count);  // [  256, +inf]
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.331715
extern "C" void memcpy_I4_O256_B8_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<8>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I4_O256_B16_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    if (count < 256) {
                      return CopyBlockOverlap<128>(dst, src,
                                                   count);  // [  128,  255]
                    } else if (count >= 256) {
                      return CopyAlignedBlocks<16>(dst, src,
                                                   count);  // [  256, +inf]
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.331715
extern "C" void memcpy_I4_O256_B16_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<16>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I4_O256_B32_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    if (count < 256) {
                      return CopyBlockOverlap<128>(dst, src,
                                                   count);  // [  128,  255]
                    } else if (count >= 256) {
                      return CopyAlignedBlocks<32>(dst, src,
                                                   count);  // [  256, +inf]
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.331715
extern "C" void memcpy_I4_O256_B32_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<32>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I4_O256_B64_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count < 8) {
            return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
          } else if (count >= 8) {
            if (count < 16) {
              return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
            } else if (count >= 16) {
              if (count < 32) {
                return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
              } else if (count >= 32) {
                if (count < 64) {
                  return CopyBlockOverlap<32>(dst, src, count);  // [   32, 63]
                } else if (count >= 64) {
                  if (count < 128) {
                    return CopyBlockOverlap<64>(dst, src,
                                                count);  // [   64,  127]
                  } else if (count >= 128) {
                    if (count < 256) {
                      return CopyBlockOverlap<128>(dst, src,
                                                   count);  // [  128,  255]
                    } else if (count >= 256) {
                      return CopyAlignedBlocks<64>(dst, src,
                                                   count);  // [  256, +inf]
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.331715
extern "C" void memcpy_I4_O256_B64_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count >= 4) {
      return CopyBlockOverlap<4>(dst, src, count);  // [    4,    7]
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<64>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I8_O32_B8_Linf_noopt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      return CopyAlignedBlocks<8>(dst, src,
                                                  count);  // [   32, +inf]
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.317890
extern "C" void memcpy_I8_O32_B8_Linf_opt(char* __restrict dst,
                                          const char* __restrict src,
                                          size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      return CopyAlignedBlocks<8>(dst, src, count);  // [   32, +inf]
    }
  }
}

extern "C" void memcpy_I8_O64_B8_Linf_noopt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        return CopyAlignedBlocks<8>(dst, src,
                                                    count);  // [   64, +inf]
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.405692
extern "C" void memcpy_I8_O64_B8_Linf_opt(char* __restrict dst,
                                          const char* __restrict src,
                                          size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64) {
        return CopyAlignedBlocks<8>(dst, src, count);  // [   64, +inf]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      }
    }
  }
}

extern "C" void memcpy_I8_O64_B16_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        return CopyAlignedBlocks<16>(dst, src,
                                                     count);  // [   64, +inf]
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.405692
extern "C" void memcpy_I8_O64_B16_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64) {
        return CopyAlignedBlocks<16>(dst, src, count);  // [   64, +inf]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      }
    }
  }
}

extern "C" void memcpy_I8_O128_B8_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          return CopyAlignedBlocks<8>(dst, src,
                                                      count);  // [  128, +inf]
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.441810
extern "C" void memcpy_I8_O128_B8_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        return CopyAlignedBlocks<8>(dst, src, count);  // [  128, +inf]
      }
    }
  }
}

extern "C" void memcpy_I8_O128_B16_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          return CopyAlignedBlocks<16>(dst, src,
                                                       count);  // [  128, +inf]
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.441810
extern "C" void memcpy_I8_O128_B16_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        return CopyAlignedBlocks<16>(dst, src, count);  // [  128, +inf]
      }
    }
  }
}

extern "C" void memcpy_I8_O128_B32_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          return CopyAlignedBlocks<32>(dst, src,
                                                       count);  // [  128, +inf]
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.441810
extern "C" void memcpy_I8_O128_B32_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        return CopyAlignedBlocks<32>(dst, src, count);  // [  128, +inf]
      }
    }
  }
}

extern "C" void memcpy_I8_O256_B8_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          if (count < 256) {
                            return CopyBlockOverlap<128>(
                                dst, src, count);  // [  128,  255]
                          } else if (count >= 256) {
                            return CopyAlignedBlocks<8>(
                                dst, src, count);  // [  256, +inf]
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.456802
extern "C" void memcpy_I8_O256_B8_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<8>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I8_O256_B16_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          if (count < 256) {
                            return CopyBlockOverlap<128>(
                                dst, src, count);  // [  128,  255]
                          } else if (count >= 256) {
                            return CopyAlignedBlocks<16>(
                                dst, src, count);  // [  256, +inf]
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.456802
extern "C" void memcpy_I8_O256_B16_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<16>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I8_O256_B32_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          if (count < 256) {
                            return CopyBlockOverlap<128>(
                                dst, src, count);  // [  128,  255]
                          } else if (count >= 256) {
                            return CopyAlignedBlocks<32>(
                                dst, src, count);  // [  256, +inf]
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.456802
extern "C" void memcpy_I8_O256_B32_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<32>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I8_O256_B64_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count < 16) {
                    return CopyBlockOverlap<8>(dst, src, count);  // [    8, 15]
                  } else if (count >= 16) {
                    if (count < 32) {
                      return CopyBlockOverlap<16>(dst, src,
                                                  count);  // [   16,   31]
                    } else if (count >= 32) {
                      if (count < 64) {
                        return CopyBlockOverlap<32>(dst, src,
                                                    count);  // [   32,   63]
                      } else if (count >= 64) {
                        if (count < 128) {
                          return CopyBlockOverlap<64>(dst, src,
                                                      count);  // [   64,  127]
                        } else if (count >= 128) {
                          if (count < 256) {
                            return CopyBlockOverlap<128>(
                                dst, src, count);  // [  128,  255]
                          } else if (count >= 256) {
                            return CopyAlignedBlocks<64>(
                                dst, src, count);  // [  256, +inf]
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.456802
extern "C" void memcpy_I8_O256_B64_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 8 && count < 16) {
    return CopyBlockOverlap<8>(dst, src, count);  // [    8,   15]
  } else if (count < 8) {
    if (count == 4) {
      return CopyBlock<4>(dst, src);  // 4
    } else if (count < 4) {
      if (count == 1) {
        return CopyBlock<1>(dst, src);  // 1
      } else if (count < 1) {
        if (count == 0) {
          return;  // 0
        }
      } else if (count >= 2) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count < 3) {
          if (count == 2) {
            return CopyBlock<2>(dst, src);  // 2
          }
        }
      }
    } else if (count >= 5) {
      if (count == 6) {
        return CopyBlock<6>(dst, src);  // 6
      } else if (count < 6) {
        if (count == 5) {
          return CopyBlock<5>(dst, src);  // 5
        }
      } else if (count >= 7) {
        if (count == 7) {
          return CopyBlock<7>(dst, src);  // 7
        }
      }
    }
  } else if (count >= 16) {
    if (count < 32) {
      return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
    } else if (count >= 32) {
      if (count >= 64 && count < 128) {
        return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
      } else if (count < 64) {
        return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
      } else if (count >= 128) {
        if (count < 256) {
          return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
        } else if (count >= 256) {
          return CopyAlignedBlocks<64>(dst, src, count);  // [  256, +inf]
        }
      }
    }
  }
}

extern "C" void memcpy_I16_O32_B8_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    return CopyAlignedBlocks<8>(
                                        dst, src, count);  // [   32, +inf]
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.800651
extern "C" void memcpy_I16_O32_B8_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    return CopyAlignedBlocks<8>(dst, src, count);  // [   32, +inf]
  }
}

extern "C" void memcpy_I16_O64_B8_Linf_noopt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      return CopyAlignedBlocks<8>(
                                          dst, src, count);  // [   64, +inf]
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.888453
extern "C" void memcpy_I16_O64_B8_Linf_opt(char* __restrict dst,
                                           const char* __restrict src,
                                           size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64) {
      return CopyAlignedBlocks<8>(dst, src, count);  // [   64, +inf]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    }
  }
}

extern "C" void memcpy_I16_O64_B16_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      return CopyAlignedBlocks<16>(
                                          dst, src, count);  // [   64, +inf]
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.888453
extern "C" void memcpy_I16_O64_B16_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64) {
      return CopyAlignedBlocks<16>(dst, src, count);  // [   64, +inf]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    }
  }
}

extern "C" void memcpy_I16_O128_B8_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        return CopyAlignedBlocks<8>(
                                            dst, src, count);  // [  128, +inf]
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.924571
extern "C" void memcpy_I16_O128_B8_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      return CopyAlignedBlocks<8>(dst, src, count);  // [  128, +inf]
    }
  }
}

extern "C" void memcpy_I16_O128_B16_Linf_noopt(char* __restrict dst,
                                               const char* __restrict src,
                                               size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        return CopyAlignedBlocks<16>(
                                            dst, src, count);  // [  128, +inf]
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.924571
extern "C" void memcpy_I16_O128_B16_Linf_opt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      return CopyAlignedBlocks<16>(dst, src, count);  // [  128, +inf]
    }
  }
}

extern "C" void memcpy_I16_O128_B32_Linf_noopt(char* __restrict dst,
                                               const char* __restrict src,
                                               size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        return CopyAlignedBlocks<32>(
                                            dst, src, count);  // [  128, +inf]
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.924571
extern "C" void memcpy_I16_O128_B32_Linf_opt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      return CopyAlignedBlocks<32>(dst, src, count);  // [  128, +inf]
    }
  }
}

extern "C" void memcpy_I16_O256_B8_Linf_noopt(char* __restrict dst,
                                              const char* __restrict src,
                                              size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        if (count < 256) {
                                          return CopyBlockOverlap<128>(
                                              dst, src, count);  // [  128, 255]
                                        } else if (count >= 256) {
                                          return CopyAlignedBlocks<8>(
                                              dst, src,
                                              count);  // [  256, +inf]
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.939564
extern "C" void memcpy_I16_O256_B8_Linf_opt(char* __restrict dst,
                                            const char* __restrict src,
                                            size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      if (count < 256) {
        return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
      } else if (count >= 256) {
        return CopyAlignedBlocks<8>(dst, src, count);  // [  256, +inf]
      }
    }
  }
}

extern "C" void memcpy_I16_O256_B16_Linf_noopt(char* __restrict dst,
                                               const char* __restrict src,
                                               size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        if (count < 256) {
                                          return CopyBlockOverlap<128>(
                                              dst, src, count);  // [  128, 255]
                                        } else if (count >= 256) {
                                          return CopyAlignedBlocks<16>(
                                              dst, src,
                                              count);  // [  256, +inf]
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.939564
extern "C" void memcpy_I16_O256_B16_Linf_opt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      if (count < 256) {
        return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
      } else if (count >= 256) {
        return CopyAlignedBlocks<16>(dst, src, count);  // [  256, +inf]
      }
    }
  }
}

extern "C" void memcpy_I16_O256_B32_Linf_noopt(char* __restrict dst,
                                               const char* __restrict src,
                                               size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        if (count < 256) {
                                          return CopyBlockOverlap<128>(
                                              dst, src, count);  // [  128, 255]
                                        } else if (count >= 256) {
                                          return CopyAlignedBlocks<32>(
                                              dst, src,
                                              count);  // [  256, +inf]
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.939564
extern "C" void memcpy_I16_O256_B32_Linf_opt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      if (count < 256) {
        return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
      } else if (count >= 256) {
        return CopyAlignedBlocks<32>(dst, src, count);  // [  256, +inf]
      }
    }
  }
}

extern "C" void memcpy_I16_O256_B64_Linf_noopt(char* __restrict dst,
                                               const char* __restrict src,
                                               size_t count) {
  if (count == 0) {
    return;  // 0
  } else if (count >= 1) {
    if (count == 1) {
      return CopyBlock<1>(dst, src);  // 1
    } else if (count >= 2) {
      if (count == 2) {
        return CopyBlock<2>(dst, src);  // 2
      } else if (count >= 3) {
        if (count == 3) {
          return CopyBlock<3>(dst, src);  // 3
        } else if (count >= 4) {
          if (count == 4) {
            return CopyBlock<4>(dst, src);  // 4
          } else if (count >= 5) {
            if (count == 5) {
              return CopyBlock<5>(dst, src);  // 5
            } else if (count >= 6) {
              if (count == 6) {
                return CopyBlock<6>(dst, src);  // 6
              } else if (count >= 7) {
                if (count == 7) {
                  return CopyBlock<7>(dst, src);  // 7
                } else if (count >= 8) {
                  if (count == 8) {
                    return CopyBlock<8>(dst, src);  // 8
                  } else if (count >= 9) {
                    if (count == 9) {
                      return CopyBlock<9>(dst, src);  // 9
                    } else if (count >= 10) {
                      if (count == 10) {
                        return CopyBlock<10>(dst, src);  // 10
                      } else if (count >= 11) {
                        if (count == 11) {
                          return CopyBlock<11>(dst, src);  // 11
                        } else if (count >= 12) {
                          if (count == 12) {
                            return CopyBlock<12>(dst, src);  // 12
                          } else if (count >= 13) {
                            if (count == 13) {
                              return CopyBlock<13>(dst, src);  // 13
                            } else if (count >= 14) {
                              if (count == 14) {
                                return CopyBlock<14>(dst, src);  // 14
                              } else if (count >= 15) {
                                if (count == 15) {
                                  return CopyBlock<15>(dst, src);  // 15
                                } else if (count >= 16) {
                                  if (count < 32) {
                                    return CopyBlockOverlap<16>(
                                        dst, src, count);  // [   16,   31]
                                  } else if (count >= 32) {
                                    if (count < 64) {
                                      return CopyBlockOverlap<32>(
                                          dst, src, count);  // [   32,   63]
                                    } else if (count >= 64) {
                                      if (count < 128) {
                                        return CopyBlockOverlap<64>(
                                            dst, src, count);  // [   64,  127]
                                      } else if (count >= 128) {
                                        if (count < 256) {
                                          return CopyBlockOverlap<128>(
                                              dst, src, count);  // [  128, 255]
                                        } else if (count >= 256) {
                                          return CopyAlignedBlocks<64>(
                                              dst, src,
                                              count);  // [  256, +inf]
                                        }
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Average path length 2.939564
extern "C" void memcpy_I16_O256_B64_Linf_opt(char* __restrict dst,
                                             const char* __restrict src,
                                             size_t count) {
  if (count >= 16 && count < 32) {
    return CopyBlockOverlap<16>(dst, src, count);  // [   16,   31]
  } else if (count < 16) {
    if (count == 8) {
      return CopyBlock<8>(dst, src);  // 8
    } else if (count < 8) {
      if (count == 4) {
        return CopyBlock<4>(dst, src);  // 4
      } else if (count < 4) {
        if (count == 1) {
          return CopyBlock<1>(dst, src);  // 1
        } else if (count < 1) {
          if (count == 0) {
            return;  // 0
          }
        } else if (count >= 2) {
          if (count == 3) {
            return CopyBlock<3>(dst, src);  // 3
          } else if (count < 3) {
            if (count == 2) {
              return CopyBlock<2>(dst, src);  // 2
            }
          }
        }
      } else if (count >= 5) {
        if (count == 6) {
          return CopyBlock<6>(dst, src);  // 6
        } else if (count < 6) {
          if (count == 5) {
            return CopyBlock<5>(dst, src);  // 5
          }
        } else if (count >= 7) {
          if (count == 7) {
            return CopyBlock<7>(dst, src);  // 7
          }
        }
      }
    } else if (count >= 9) {
      if (count == 11) {
        return CopyBlock<11>(dst, src);  // 11
      } else if (count < 11) {
        if (count == 10) {
          return CopyBlock<10>(dst, src);  // 10
        } else if (count < 10) {
          if (count == 9) {
            return CopyBlock<9>(dst, src);  // 9
          }
        }
      } else if (count >= 12) {
        if (count == 13) {
          return CopyBlock<13>(dst, src);  // 13
        } else if (count < 13) {
          if (count == 12) {
            return CopyBlock<12>(dst, src);  // 12
          }
        } else if (count >= 14) {
          if (count == 14) {
            return CopyBlock<14>(dst, src);  // 14
          } else if (count >= 15) {
            if (count == 15) {
              return CopyBlock<15>(dst, src);  // 15
            }
          }
        }
      }
    }
  } else if (count >= 32) {
    if (count >= 64 && count < 128) {
      return CopyBlockOverlap<64>(dst, src, count);  // [   64,  127]
    } else if (count < 64) {
      return CopyBlockOverlap<32>(dst, src, count);  // [   32,   63]
    } else if (count >= 128) {
      if (count < 256) {
        return CopyBlockOverlap<128>(dst, src, count);  // [  128,  255]
      } else if (count >= 256) {
        return CopyAlignedBlocks<64>(dst, src, count);  // [  256, +inf]
      }
    }
  }
}

llvm::ArrayRef<const MemcpyFunction> GetMemoryFunctions() {
  return {(MemcpyFunction){.name = "memcpy_I4_O32_B8_Linf_noopt",
                           .function = memcpy_I4_O32_B8_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 32ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O32_B8_Linf_opt",
                           .function = memcpy_I4_O32_B8_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 32ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O64_B8_Linf_noopt",
                           .function = memcpy_I4_O64_B8_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O64_B8_Linf_opt",
                           .function = memcpy_I4_O64_B8_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O64_B16_Linf_noopt",
                           .function = memcpy_I4_O64_B16_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O64_B16_Linf_opt",
                           .function = memcpy_I4_O64_B16_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O128_B8_Linf_noopt",
                           .function = memcpy_I4_O128_B8_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O128_B8_Linf_opt",
                           .function = memcpy_I4_O128_B8_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O128_B16_Linf_noopt",
                           .function = memcpy_I4_O128_B16_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O128_B16_Linf_opt",
                           .function = memcpy_I4_O128_B16_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O128_B32_Linf_noopt",
                           .function = memcpy_I4_O128_B32_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O128_B32_Linf_opt",
                           .function = memcpy_I4_O128_B32_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O256_B8_Linf_noopt",
                           .function = memcpy_I4_O256_B8_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O256_B8_Linf_opt",
                           .function = memcpy_I4_O256_B8_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O256_B16_Linf_noopt",
                           .function = memcpy_I4_O256_B16_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O256_B16_Linf_opt",
                           .function = memcpy_I4_O256_B16_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O256_B32_Linf_noopt",
                           .function = memcpy_I4_O256_B32_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O256_B32_Linf_opt",
                           .function = memcpy_I4_O256_B32_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I4_O256_B64_Linf_noopt",
                           .function = memcpy_I4_O256_B64_Linf_noopt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 64ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I4_O256_B64_Linf_opt",
                           .function = memcpy_I4_O256_B64_Linf_opt,
                           .overlap_point = 4ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 64ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O32_B8_Linf_noopt",
                           .function = memcpy_I8_O32_B8_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 32ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O32_B8_Linf_opt",
                           .function = memcpy_I8_O32_B8_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 32ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O64_B8_Linf_noopt",
                           .function = memcpy_I8_O64_B8_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O64_B8_Linf_opt",
                           .function = memcpy_I8_O64_B8_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O64_B16_Linf_noopt",
                           .function = memcpy_I8_O64_B16_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O64_B16_Linf_opt",
                           .function = memcpy_I8_O64_B16_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O128_B8_Linf_noopt",
                           .function = memcpy_I8_O128_B8_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O128_B8_Linf_opt",
                           .function = memcpy_I8_O128_B8_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O128_B16_Linf_noopt",
                           .function = memcpy_I8_O128_B16_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O128_B16_Linf_opt",
                           .function = memcpy_I8_O128_B16_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O128_B32_Linf_noopt",
                           .function = memcpy_I8_O128_B32_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O128_B32_Linf_opt",
                           .function = memcpy_I8_O128_B32_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O256_B8_Linf_noopt",
                           .function = memcpy_I8_O256_B8_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O256_B8_Linf_opt",
                           .function = memcpy_I8_O256_B8_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O256_B16_Linf_noopt",
                           .function = memcpy_I8_O256_B16_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O256_B16_Linf_opt",
                           .function = memcpy_I8_O256_B16_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O256_B32_Linf_noopt",
                           .function = memcpy_I8_O256_B32_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O256_B32_Linf_opt",
                           .function = memcpy_I8_O256_B32_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I8_O256_B64_Linf_noopt",
                           .function = memcpy_I8_O256_B64_Linf_noopt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 64ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I8_O256_B64_Linf_opt",
                           .function = memcpy_I8_O256_B64_Linf_opt,
                           .overlap_point = 8ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 64ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O32_B8_Linf_noopt",
                           .function = memcpy_I16_O32_B8_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 32ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O32_B8_Linf_opt",
                           .function = memcpy_I16_O32_B8_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 32ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O64_B8_Linf_noopt",
                           .function = memcpy_I16_O64_B8_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O64_B8_Linf_opt",
                           .function = memcpy_I16_O64_B8_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O64_B16_Linf_noopt",
                           .function = memcpy_I16_O64_B16_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O64_B16_Linf_opt",
                           .function = memcpy_I16_O64_B16_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 64ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O128_B8_Linf_noopt",
                           .function = memcpy_I16_O128_B8_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O128_B8_Linf_opt",
                           .function = memcpy_I16_O128_B8_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O128_B16_Linf_noopt",
                           .function = memcpy_I16_O128_B16_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O128_B16_Linf_opt",
                           .function = memcpy_I16_O128_B16_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O128_B32_Linf_noopt",
                           .function = memcpy_I16_O128_B32_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O128_B32_Linf_opt",
                           .function = memcpy_I16_O128_B32_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 128ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O256_B8_Linf_noopt",
                           .function = memcpy_I16_O256_B8_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O256_B8_Linf_opt",
                           .function = memcpy_I16_O256_B8_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 8ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O256_B16_Linf_noopt",
                           .function = memcpy_I16_O256_B16_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O256_B16_Linf_opt",
                           .function = memcpy_I16_O256_B16_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 16ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O256_B32_Linf_noopt",
                           .function = memcpy_I16_O256_B32_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O256_B32_Linf_opt",
                           .function = memcpy_I16_O256_B32_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 32ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "memcpy_I16_O256_B64_Linf_noopt",
                           .function = memcpy_I16_O256_B64_Linf_noopt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 64ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "memcpy_I16_O256_B64_Linf_opt",
                           .function = memcpy_I16_O256_B64_Linf_opt,
                           .overlap_point = 16ULL,
                           .loop_point = 256ULL,
                           .loop_block_size = 64ULL,
                           .accelerator_point = 18446744073709551615ULL,
                           .optimized = true},
          (MemcpyFunction){.name = "glibc",
                           .function = memcpy_glibc_wrapper,
                           .overlap_point = 0ULL,
                           .loop_point = 0ULL,
                           .loop_block_size = 0ULL,
                           .accelerator_point = 0ULL,
                           .optimized = false},
          (MemcpyFunction){.name = "aor",
                           .function = memcpy_aor_wrapper,
                           .overlap_point = 0ULL,
                           .loop_point = 0ULL,
                           .loop_block_size = 0ULL,
                           .accelerator_point = 0ULL,
                           .optimized = false}};
}

// Generated 60 configurations
