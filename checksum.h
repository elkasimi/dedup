#pragma once

// TODO port to linux & windows

#include <CommonCrypto/CommonDigest.h>

#include <filesystem>
#include <fstream>

std::string sha256(const std::filesystem::path &path) {
  CC_SHA256_CTX ctx;
  CC_SHA256_Init(&ctx);

  std::ifstream in(path.c_str(), std::ios::binary | std::ios::in);
  char buf[1024];
  while (true) {
    in.read(buf, 1024);
    auto len = in.gcount();
    CC_SHA256_Update(&ctx, buf, len);
    if (!len) break;
  }

  unsigned char md[128];
  CC_SHA256_Final(md, &ctx);

  char out[128];
  for (auto i = 0u; i < 32; ++i) {
    std::sprintf(out + 2 * i, "%02x", md[i]);
  }
  out[64] = 0;

  return std::string(out);
}
