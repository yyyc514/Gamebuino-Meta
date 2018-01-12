#ifndef FAKE_SD_H
#define FAKE_SD_H

#include "Arduino.h"
#include "File.h"
#include <stdio.h>
#include <unistd.h>
#include "tinyfiles.h"

class FakeSD {
public:
  FakeSD(const char* path) {
    printf("fake sd constructor\n");
    getcwd(cwd, sizeof(cwd));
    chdir(path, true);
    printf("dir is: %s\n", cwd);
  };

  char cwd[1000];
  char buf[2000];

  void buildFullPath(char *buffer, char *dir, char *path) {

    char slash = '/';
    strncpy(buffer, dir, sizeof(dir));
    if (buffer[strlen(buffer)-1] == '/') {
      strcat(buffer,path);
    } else {
      strcat(buffer,&slash);
      strcat(buffer,path);
    }
    printf("full path is: %s\n", buffer);
  }

  bool exists(const char* path) {
    buildFullPath(buf, cwd, (char *)path);
    struct stat   fstat;
    return (stat (buf, &fstat) == 0);
  }

  bool remove(const char* path) {
    buildFullPath(buf, cwd, (char *)path);
    // append paths
    return ::remove(buf) == 0;
  }

  File open(const char *path, uint8_t mode = FILE_READ) {
    File tmpFile;
    buildFullPath(buf, cwd, (char *)path);
    // append paths here
    tmpFile.open(buf, mode);
    return tmpFile;
  }

  // directories
  bool mkdir_p(const char* path, bool pFlag = true) {
    // TODO
  }

  bool mkdir(const char* path, bool pFlag = true) {
    if (pFlag) {
      return mkdir_p(path);
    }
    buildFullPath(buf, cwd, (char *)path);
    uint8_t r = mkdir(buf);
    return r == 0;
  }

  bool chdir(const char *path, bool set_cwd = false) {
    buildFullPath(buf, cwd, (char *)path);
    File f(buf);
    if (f.isDirectory()) {
        strncpy(cwd, buf, sizeof(cwd));
        return true;
    } else {
      return false;
    }
  }



};

typedef FakeSD SdFat;

#endif
