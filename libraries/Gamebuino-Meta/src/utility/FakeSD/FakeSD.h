#ifndef FAKE_SD_H
#define FAKE_SD_H

#include "Arduino.h"
#include "File.h"
#include <stdio.h>
#include <unistd.h>
#include "tinyfiles.h"

#define MAX_FILE_LEN 2000

class FakeSD {
public:
  FakeSD(const char* path) {
    getcwd(cwd, sizeof(cwd));
    chdir(path, true);
    chroot();
  };

  bool begin(uint8_t port) { return true; };

  char cwd[MAX_FILE_LEN];
  char buf[MAX_FILE_LEN];

  void buildFullPath(char *buffer, char *dir, char *path) {
    if (path[0] == '/') { path++; };

    char slash[2] = "/";
    strncpy(buffer, dir, MAX_FILE_LEN);
    if (buffer[strlen(buffer)-1] == '/') {
      strcat(buffer,path);
    } else {
      strcat(buffer,slash);
      strcat(buffer,path);
    }
    printf("full path is: %s\n", buffer);
    fflush(stdout);
  }

  bool exists(const char* path) {
    struct stat   fstat;

    buildFullPath(buf, cwd, (char *)path);
    if (withinJail(buf)) {
      return (stat (buf, &fstat) == 0);
    } else {
      return false;
    }
  }

  bool remove(const char* path) {
    buildFullPath(buf, cwd, (char *)path);

    if (withinJail(buf)) {
      return ::remove(buf) == 0;
    } else {
      return false;
    }
  }

  // TODO: jail
  File open(const char *path, uint8_t mode = FILE_READ) {
    File tmpFile;
    buildFullPath(buf, cwd, (char *)path);
    tmpFile.open(buf, mode);
    return tmpFile;
  }

  // directories
  bool mkdir_p(const char* path, bool pFlag = true) {
    char tmp[MAX_FILE_LEN];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",path);
    len = strlen(tmp);
    if(tmp[len - 1] == '/')
            tmp[len - 1] = 0;
    for(p = tmp + 1; *p; p++)
            if(*p == '/') {
                    *p = 0;
                    if (!mkdir(tmp, false)) {
                      return false;
                    }
                    *p = '/';
            }
    return mkdir(tmp, false);
  }

  bool mkdir(const char* path, bool pFlag = true) {
    if (pFlag) { return mkdir_p(path); }

    buildFullPath(buf, cwd, (char *)path);
    if (withinJail(buf)) {
      uint8_t r = mkdir(buf);
      return r == 0; }
    else {
      return false;
    }
  }

  void chroot() {
    _jailed = true;
    strcpy(_jail, cwd);
  }

  // /Users/jgoebel/secret/rooted
  // /Users/jgoebel/sec

  bool chdir(const char *path, bool set_cwd = false) {
    char canonical[MAX_FILE_LEN];

    buildFullPath(buf, cwd, (char *)path);
    printf("looking for %s\n", buf);
    File f(buf);
    if (f.isDirectory()) {
      printf("dir found\n");
      realpath(buf, canonical);
      strcat(canonical, "/");
      printf("canonical: %s\n", canonical);
      if (withinJail(canonical)) {
        strncpy(cwd, canonical, sizeof(cwd));
        return true;
      } else {
        return false;
      }
    } else {
      printf("not a dir\n");
      return false;
    }
  }

private:

  bool withinJail(char * path) {
    if (!_jail) { return true; }
    // make sure jail has / as last character
    if (_jail[strlen(_jail)-1] != '/') {
      printf("Your jail doesn't seem to have a / at the end.");
      return false;
    }

    char canonical[MAX_FILE_LEN];
    realpath(path, canonical);

    // JAIL: /home/jim/fs/
    // then  /home/jim/fs/* is consider safe
    return memcmp(canonical, _jail, strlen(_jail)) == 0;
  }

  bool _jailed = false;
  char _jail[MAX_FILE_LEN];


};

typedef FakeSD SdFat;

#endif
