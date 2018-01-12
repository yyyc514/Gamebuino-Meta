#ifndef FAKE_FILE_H
#define FAKE_FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include "tinyfiles.h"

// use the gnu style oflag in open()
/** open() oflag for reading */
uint8_t const O_READ = 0X01;
/** open() oflag - same as O_IN */
uint8_t const O_RDONLY = O_READ;
/** open() oflag for write */
uint8_t const O_WRITE = 0X02;
/** open() oflag - same as O_WRITE */
uint8_t const O_WRONLY = O_WRITE;
/** open() oflag for reading and writing */
uint8_t const O_RDWR = (O_READ | O_WRITE);
/** open() oflag mask for access modes */
uint8_t const O_ACCMODE = (O_READ | O_WRITE);
/** The file offset shall be set to the end of the file prior to each write. */
uint8_t const O_APPEND = 0X04;
/** synchronous writes - call sync() after each write */
uint8_t const O_SYNC = 0X08;
/** truncate the file to zero length */
uint8_t const O_TRUNC = 0X10;
/** set the initial position at the end of the file */
uint8_t const O_AT_END = 0X20;
/** create the file if nonexistent */
uint8_t const O_CREAT = 0X40;
/** If O_CREAT and O_EXCL are set, open() shall fail if the file exists */
uint8_t const O_EXCL = 0X80;


#define FILE_READ O_READ
/** Arduino SD.h style flag for open at EOF for read/write with create. */
#define FILE_WRITE (O_RDWR | O_CREAT | O_AT_END)


class File {
  public:
    File() {};
    File(char *full_name) {
      _init(full_name);
    }

    char filename[1000];
    char full_path[1000];

    operator bool() {
      return isOpen();
    }
    void _init(char * path) {
      strncpy(full_path, path, sizeof(full_path));
      char *basename;
      basename = ::basename(path);
      strcpy(filename, basename);
      stat(full_path, &_stat);
    }

    bool getName(char* name, size_t size) {
      strncpy(name, filename, size);
    }

    void modeFromFlags(char *mode, uint8_t flags) {
      mode[0] = 0;
      switch(flags) {
        case O_READ:
          strcat(mode, "r");
          break;
        case O_WRITE:
          strcat(mode, "r+"); // read/update
          break;
        case FILE_WRITE:
          strcat(mode, "r+"); // read/update
          break;
        case O_RDWR|O_CREAT|O_TRUNC:
          strcat(mode, "w+"); // write/update
          break;
        break;
      }
      if (flags & O_APPEND) {
        mode[0] = 'a'; // append mode
      }
      if (flags & O_SYNC) {
        _always_flush = true;
      }
      // always binary mode
      strcat(mode, "b");
    }

    void _before_open() {
      // if file does not exist and we are asked to create it
      if ((_flags & O_CREAT) && stat(full_path, &_stat) == -1) {
        f = fopen(full_path, "w");
        close();
        // need to do this again since we're past init now
        stat(full_path, &_stat);
      }
    }

    bool open(char* path, uint8_t oflag = O_READ) {
      char mode[5];

      _flags = oflag;
      modeFromFlags(mode, oflag);

      _init(path);
      _before_open();
      f = fopen(path, mode);
      if (f == NULL) {
        return false;
      } else {
        _open = true;
        if (isDirectory()) {
          ::tfDirOpen(&_dir, path);
        }
        if (isFile() && (_flags & O_AT_END)) {
          seekEnd(0);
        }
        return true;
      }
    }


    bool isOpen() {
      return _open;
    }

    bool isFile() {
      return S_ISREG(_stat.st_mode);
    }

    bool isDirectory() {
      return S_ISDIR(_stat.st_mode);
    }

    int fileSize() {
      stat(full_path, &_stat);
      return _stat.st_size;
    }

    int available() {
      return isFile() ? fileSize() - position() : 0;
    }

    int write(const char* str) {
      return write(str, strlen(str));
    }
    size_t write(uint8_t b) {
      return write(&b, 1);
    }
    size_t write(const void* buf, size_t nbyte) {
      return write((uint8_t*)buf, nbyte);
    }
    size_t write(const uint8_t *buf, size_t size) {
      return fwrite(buf, 1, size, f);
    };


    // seeking
    uint32_t position() { return ftell(f); };
    void rewind() { ::rewind(f); };
    bool seekSet(uint32_t pos) {
      return isFile() ? fseek(f, pos, SEEK_SET) : false;
    }
    bool seekCur(int32_t offset) {
      return isFile() ? seekSet(position() + offset) : false;
    }
    bool seekEnd(int32_t offset = 0) {
      return isFile() ? seekSet(fileSize() + offset) : false;
    }
    bool seek(uint32_t pos) {
      return isFile() ? seekSet(pos) : false;
    }



    // read
    int read() {
      uint8_t b;
      return read(&b, 1) == 1 ? b : -1;
    };
    int read(void* buf, size_t nbyte) {
      return fread(buf, 1, nbyte, f);
    }

    // close
    bool close() {
      if (!_open) {
        return false; }

      if (fclose(f)==0) {
        _open = false;
        return true;
      } else {
        return false;
      }
    };

    File openNextFile(uint8_t mode = O_READ) {
      tfFILE tmp;
      File tmpFile;
      if (_dir.has_next) {
        tfReadFile(&_dir, &tmp);
        ::tfDirNext(&_dir);
        tmpFile.open(tmp.path, mode);
        return tmpFile;
      } else {
        return NULL;
      }
    }

    bool truncate(uint32_t length){
      return ::truncate(full_path, length) == 0;
    }
    void flush() {
      fflush(f);
    }

    FILE *f;
  private:


    bool _open = false;

    tfDIR _dir;
    bool _always_flush = false;
    uint8_t _flags = 0;
    struct stat _stat;

};

#endif
