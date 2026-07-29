#pragma once

#include "../debugging/Logger.h"

#include <fcntl.h>
#include <string_view>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

class MemoryMappedFile {
private:
  int fileDescriptor = -1;
  void *data = MAP_FAILED;
  size_t length = 0;

public:
  explicit MemoryMappedFile(const char *filePath) {
    fileDescriptor = open(filePath, O_RDONLY);
    if (fileDescriptor == -1) {
      Logger::error("Failed to open file");
      return;
    }

    struct stat fileMetadata;
    if (fstat(fileDescriptor, &fileMetadata) == -1 || fileMetadata.st_size <= 0) {
      Logger::error("Failed to get file metadata");
      return;
    }

    length = static_cast<size_t>(fileMetadata.st_size);

    void *mappingAddress = nullptr; // Kernel auto decides
    size_t lengthOfFileToRead = length;
    int memoryProtectionMode = PROT_READ;
    int mappingBehaviour = MAP_PRIVATE;
    off_t offset = 0;
    data = mmap(mappingAddress, lengthOfFileToRead, memoryProtectionMode, mappingBehaviour, fileDescriptor, offset);
    if (data == MAP_FAILED) {
      Logger::error("Failed to memory map file");
      return;
    }
  }

  ~MemoryMappedFile() {
    if (data != MAP_FAILED)
      munmap(data, length);
    if (fileDescriptor != -1)
      close(fileDescriptor);
  }

  std::string_view peekIn() const {
    if (data == MAP_FAILED)
      return {};
    return std::string_view(static_cast<char *>(data), length);
  }

  bool isValid() const { return data != MAP_FAILED; }
};

void setupProjectConfigAtPath(std::string configFilePath);