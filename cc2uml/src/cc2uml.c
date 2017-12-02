//#ifdef _MSC_VER
#ifndef __GNUC__
#error This project must be built use gcc toolchain, not others.
#endif

#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
#include <tchar.h>
#endif

#ifndef _MSC_VER
#include <unistd.h>
#include <dirent.h>
#ifdef __MINGW64_VERSION_MINOR
#include <minwindef.h>
#include <minmax.h>
#endif
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
#include <io.h> // 在 mingw-w64 中，已包含在 unistd.h 中
#endif

#if defined(__linux__)
#include <sys/param.h> // for MIN
#include <malloc.h> // for malloc_trim
#define UNLIKELY(x)     __builtin_expect(!!(x),0)
#define LIKELY(x)       __builtin_expect(!!(x),1)
#include <sys/mman.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <fcntl.h>


#if defined(__linux__)
static void searchdir(const char *restrict dirname, FILE *restrict fout);
#endif

#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
static void searchdir2(const char *restrict dirname, FILE *restrict fout);
#endif

static void work(const char * restrict filepath, unsigned int filesize, 
    int filepathlength, const char *restrict cur_cwd, FILE *restrict fout);
/* 返回找到的类的个数 */
static int dowork(const char *restrict content, unsigned int contentlength,
    FILE *restrict fout);
static inline const char *skip_space_and_comment(const char *restrict content);
static inline const char *skipword(const char *restrict content);

/* 去掉首尾空白类字符，中间的连续空白类字符用一个空格替代 
 * _用\_代替 */
static inline int strcpy_trim(char *restrict to, int tosize, const char *restrict from, int fromlength);

/* 出错返回 -1，函数返回 0，字段返回 1 */
static inline int function_or_field(const char *restrict token, int length);

/* 写类, parent_or_me: 0 表示classname是自己，
 * 1 表示 classname 是自己的父类 */
static inline void print_class(FILE *restrict fout, const char *restrict classname, int len, 
    int parent_or_me);

/* 写函数或方法。func_or_field 取自 function_or_field 的返回值 */
static inline void print_field_or_operation(FILE *restrict fout, const char *restrict name, int len,
    const char *restrict access, int accesslen, int func_or_field);

static __attribute__((noreturn)) void usage(void);

#if 0
static struct {
  const char *name;
  int length;
} splitter [] = {{"class", 5}, {"{", 1}, {":", 1}};
#endif


#ifdef _MSC_VER
#define S_ISDIR(mode) ((mode & _S_IFREG) != _S_IFREG)
#define S_ISREG(mode) ((mode & _S_IFREG) == _S_IFREG)
#endif

int main(int argc, const char *argv[], const char *env[])
{
  struct stat st;
  FILE *fp;
  const char *outpath;
  struct timeb tb_start, tb_end;

  ftime(&tb_start);
  if(*++argv) {
    if(strcmp(*argv, "-o") != 0)
      usage();
  }
  else
    usage();

  if((outpath = *++argv) == NULL)
    usage();

  if((fp = fopen(outpath, "w")) == NULL) {
    fprintf(stderr, "fopen %s error: %s", outpath, strerror(errno));
    usage();
  }

  if(!*(argv + 1)) {
    fclose(fp);
    remove(*argv);
    fputs("没有指定输入文件。\n", stderr);
    usage();
  }

#if 0
#if defined(__MINGW64_VERSION_MINOR) 
  printf(" PATH_MAX = %d 定义在 limits.h，在 mingw-w64 中，"
      "stdlib.h 包含了 limits.h\n"
      " MAX_PATH = %d 定义在 minwindef.h\n"
      "_MAX_PATH = %d 定义在 stdlib.h\n", 
      PATH_MAX, MAX_PATH, _MAX_PATH);
#elif defined(__linux__)
  printf("PATH_MAX = %d 定义在 limits.h，在 linux 中，"
      "dirent.h应包含了与 limits.h 相同的头文件。\n", PATH_MAX);
#endif
#endif
  while(*++argv) {
    if(stat(*argv, &st) == -1) {
      fprintf(stderr, "stat %s error: %s\n", *argv, strerror(errno));
      continue;
    }

    if(S_ISDIR(st.st_mode))
#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
      searchdir2(*argv, fp);
#elif defined(__linux__)
    searchdir(*argv, fp);
#endif
    else if(S_ISREG(st.st_mode)) {
#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
      char cur_cwd[_MAX_PATH];
#elif defined(__linux__)
      char cur_cwd[PATH_MAX];
#endif
      memset(cur_cwd, 0, sizeof(cur_cwd));
      getcwd(cur_cwd, sizeof(cur_cwd) / sizeof(cur_cwd[0]));
      work(*argv, st.st_size, strlen(*argv), cur_cwd, fp);
    }
  }

  fclose(fp);
  if(stat(outpath, &st) == 0) {
    if(st.st_size == 0L)
      unlink(outpath);
  }
  else
    fprintf(stderr, "stat %s error: %s", outpath, strerror(errno));
  work(NULL, 0U, 0, NULL, NULL);
  ftime(&tb_end);
  printf("time used: %ld (ms).\n", tb_end.time * 1000 + tb_end.millitm
      - tb_start.time * 1000 - tb_start.millitm);
  return 0;
}

#if defined(__linux__)
static void searchdir(const char *restrict dirname, FILE *restrict fout)
{
  DIR *dir;
  struct dirent *dirent;
  struct stat st;
  //char cwd[MAX_PATH];
  //这几个值在windows下都是260
#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
  char cwd[_MAX_PATH], cur_cwd[_MAX_PATH];
#elif defined(__linux__)
  char cwd[PATH_MAX], cur_cwd[PATH_MAX];
#endif

  if((dir = opendir(dirname)) == NULL) {
    fprintf(stderr, "opendir [%s] error: %s\n", 
        dirname, strerror(errno));
    exit(-1);
  } 

  memset(cwd, 0, sizeof(cwd));
  getcwd(cwd, sizeof(cwd) / sizeof(cwd[0]));

  if(chdir(dirname) == -1)
    fprintf(stderr, "warning: chdir %s error: %s\n", 
        dirname, strerror(errno));

  memset(cur_cwd, 0, sizeof(cur_cwd));
  getcwd(cur_cwd, sizeof(cur_cwd) / sizeof(cur_cwd[0]));

  while((dirent = readdir(dir)) != NULL) {
    if(strcmp(dirent->d_name, ".") == 0 
        || strcmp(dirent->d_name, "..") == 0)
      continue;
    if(stat(dirent->d_name, &st) == -1) {
      fprintf(stderr, "warning: stat %s: %s\n", dirent->d_name,
          strerror(errno));
      continue;
    }
    if(S_ISDIR(st.st_mode))
      searchdir(dirent->d_name, fout);
    else if(S_ISREG(st.st_mode))
#if defined(__MINGW64_VERSION_MINOR) 
      work(dirent->d_name, st.st_size, dirent->d_namlen, cur_cwd, fout);
#elif defined(__linux__)
    work(dirent->d_name, st.st_size, strlen(dirent->d_name), cur_cwd, fout);
#endif
  }
  (void)closedir(dir);
  chdir(cwd);
}
#endif

#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
static void searchdir2(const char *restrict dirname, FILE *restrict fout)
{
  int handle;
  struct _finddata_t fileinfo;
  char cwd[_MAX_PATH], cur_cwd[_MAX_PATH];

  memset(&fileinfo, 0, sizeof(fileinfo));

  memset(cwd, 0, sizeof(cwd));
  getcwd(cwd, sizeof(cwd) / sizeof(cwd[0]));

  chdir(dirname);
  memset(cur_cwd, 0, sizeof(cur_cwd));
  getcwd(cur_cwd, sizeof(cur_cwd) / sizeof(cur_cwd[0]));

  if((handle = _findfirst("*.*", &fileinfo)) != -1)
    do {
      if(strcmp(fileinfo.name, ".") == 0 
          || strcmp(fileinfo.name, "..") == 0)
        continue;
      if((fileinfo.attrib & _A_SUBDIR) == _A_SUBDIR)
        searchdir2(fileinfo.name, fout);
      else if((fileinfo.attrib & _A_ARCH) == _A_ARCH
          || (fileinfo.attrib & _A_NORMAL) == _A_NORMAL)
        work(fileinfo.name, fileinfo.size, strlen(fileinfo.name), cur_cwd, fout);
    } while(_findnext(handle, &fileinfo) != -1);

  _findclose(handle);
  chdir(cwd);
}
#endif

#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
/* 其实这个函数在 Linux 下也可以用 */
static void work(const char * restrict filepath, unsigned int filesize, 
    int filepathlength, const char *restrict cur_cwd, FILE *restrict fout)
{
  unsigned int bytes_read;
  int class_num;
  FILE *fp;

  static int unsigned max_bytes;
  static char *buf;

  if(filepath == NULL || filesize == 0U 
      || filepathlength == 0 || cur_cwd == NULL
      || fout == NULL) {
    max_bytes = 0u;
    free(buf);
#if defined(__linux__)
    malloc_trim(0LU);
#endif
    return;
  }
#if defined(__linux__)
  if(strcasecmp(&filepath[filepathlength - 2], ".h") != 0
      && strcasecmp(&filepath[filepathlength - 4], ".hpp") != 0
      && strcasecmp(&filepath[filepathlength - 4], ".hxx") != 0)
#elif defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
    /* 只要 .h、.hpp 和 .hxx 文件 */ 
    if(stricmp(&filepath[filepathlength - 2], ".h") != 0
        && stricmp(&filepath[filepathlength - 4], ".hpp") != 0
        && stricmp(&filepath[filepathlength - 4], ".hxx") != 0)
#endif
      return;

#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
  if((fp = fopen(filepath, "rb")) == NULL)
#else
    if((fp = fopen(filepath, "r")) == NULL)
#endif
    {
      fprintf(stderr, "warning: open %s error: %s\n", filepath,
          strerror(errno));
      return;
    }
  if(max_bytes < filesize) {
    max_bytes = filesize;
    free(buf);
#if defined(__linux__)
    malloc_trim(0UL);
#endif
//#define __SIZE_TYPE__ long unsigned int
    if((buf = malloc(max_bytes + 1)) == NULL) {
      fclose(fp);
      fprintf(stderr, "warning: malloc %u bytes error: %s\n", filesize,
          strerror(errno));
      return;
    }
  }
  memset(buf, 0, filesize + 1);
  if((bytes_read = fread(buf, 1LU, filesize, fp)) != filesize)
    fprintf(stderr, "warning: fread %u(%u) bytes error: %s\n", filesize,
        bytes_read, strerror(errno));
#if 0
  printf("start %s =====", filepath);
  printf("\nmax_bytes = %6u, bytes_read = %6u, filesize = %6u, length = %6u\n",
      max_bytes, bytes_read, filesize, strlen(buf));
#endif
  fprintf(fout, "%%convert from [%s/%s] start\n", cur_cwd, filepath);
  class_num = dowork(buf, bytes_read, fout);
#if 0
  printf("==== end %s\n", filepath);
#endif
  if(class_num > 0)  {
#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
    fseek(fout, -2L, SEEK_CUR);
#else
    fseek(fout, -1L, SEEK_CUR);
#endif
  }
  fprintf(fout, "%% Total class: %d \n%%convert from [%s/%s] end\n\n", 
      class_num, cur_cwd, filepath);
  fclose(fp);
}
#else
static void work(const char * restrict filepath, unsigned int filesize, 
    int filepathlength, const char *restrict cur_cwd, FILE *restrict fout)
{
  int class_num, fd;
  const char *buf;

  if(UNLIKELY(filepath == NULL || filesize == 0U 
      || filepathlength == 0 || cur_cwd == NULL
      || fout == NULL))
    return;

#if defined(__linux__)
  if(strcasecmp(&filepath[filepathlength - 2], ".h") != 0
      && strcasecmp(&filepath[filepathlength - 4], ".hpp") != 0
      && strcasecmp(&filepath[filepathlength - 4], ".hxx") != 0)
#elif defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
    /* 只要 .h、.hpp 和 .hxx 文件 */ 
    if(stricmp(&filepath[filepathlength - 2], ".h") != 0
        && stricmp(&filepath[filepathlength - 4], ".hpp") != 0
        && stricmp(&filepath[filepathlength - 4], ".hxx") != 0)
#endif
      return;

  if(UNLIKELY((fd = open(filepath, O_RDONLY)) == -1)) {
    fprintf(stderr, "open %s error: %s\n", filepath, strerror(errno));
    return;
  }
  if(UNLIKELY((buf = (const char *)mmap(NULL, filesize, PROT_READ, 
          MAP_PRIVATE | MAP_LOCKED, fd, 0L)) == NULL)) {
    fprintf(stderr, "mmap %d error: %s\n", fd, strerror(errno));
    return;
  }
  close(fd);
  fprintf(fout, "%%convert from [%s/%s] start\n", cur_cwd, filepath);
  class_num = dowork(buf, filesize, fout);
  if(LIKELY(class_num > 0))  {
#if defined(__MINGW64_VERSION_MINOR) || defined(_MSC_VER)
    fseek(fout, -2L, SEEK_CUR);
#else
    fseek(fout, -1L, SEEK_CUR);
#endif
  }
  fprintf(fout, "%% Total class: %d \n%%convert from [%s/%s] end\n\n", 
      class_num, cur_cwd, filepath);
  munmap((void *)buf, filesize);
}
#endif
static int dowork(const char *restrict content, 
    unsigned int contentlength, FILE * restrict fout)
{
  const char *pc, *tokenstart, *pstatment;
  int classdef = 0, parentclass = 0, inclass = 0, 
      funcdef = 0, fof, isaccesstype = 0, classnum1 = 0, classnum2 = 0;
  int open_curly_braces_count = 0, idlen, accesstypelen = 0, mynamelen = 0;
  char id[BUFSIZ], accesstype[BUFSIZ], myname[BUFSIZ];

  /* 生成 Latex 文件 */
  /* 调试 */
  tokenstart = pstatment = NULL;
  pc = content;
  while(*(pc = skip_space_and_comment(pc))) {

    if(open_curly_braces_count == 0 && strncmp(pc, "class", 5) == 0) {
      classdef = 1;
      inclass = 1;
      /* 默认访问类型 */
      strcpy(accesstype, "private");
      accesstypelen = 7;
    }

    if(open_curly_braces_count == 0 && strncmp(pc, "struct", 6) == 0) {
      classdef = 1;
      inclass = 1;
      /* 默认访问类型 */
      strcpy(accesstype, "public");
      accesstypelen = 6;
    }

    if(!inclass)  {
      pc++;
      continue;
    }

    if(*pc == '}')
      --open_curly_braces_count;

    if(open_curly_braces_count > 1) {
      pc++;
      continue;
    }

    /* 测试性质的代码 */
    if(open_curly_braces_count < 0) {
      pc++;
      open_curly_braces_count = 0;
      continue;
    }

    if(isalpha(*pc) || *pc == '_' || *pc == '~') {
      tokenstart = pc;
      if(funcdef == 1 && pstatment == NULL)
        pstatment = pc;
      if(funcdef == 2) {
        funcdef = 1;
        pstatment = pc;
      }
      pc = skipword(pc);
    }
    else {
      switch(*pc) {
        case ':':
          idlen = strcpy_trim(id, sizeof(id), tokenstart, 
              pc - tokenstart);
          if(classdef) {
            /* 冒号前面的类名，肯定是自己的名字 */
            while(!isalnum(id[idlen - 1]) && id[idlen - 1] != '_')
              idlen--;
            id[idlen] = '\0';
            print_class(fout, id, idlen, 0);
            classnum1++;
            strcpy(myname, id);
            mynamelen = idlen;
            /* 在冒号之后的下一个类，是该类的父类 */
            parentclass = 1;
          }
          else if(isaccesstype && (strcmp(id, "public") == 0
                || strcmp(id, "private") == 0
                || strcmp(id, "protected") == 0)) {
            strcpy(accesstype, id);
            accesstypelen = idlen;
            /* 2 表示函数/方法和字段的定义要重新开始*/
            funcdef = 2;
          }
          break;
        case ',':
          if(classdef && mynamelen > 0) { // 是类的定义，而且已有自己
            idlen = strcpy_trim(id, sizeof(id), tokenstart, 
                pc - tokenstart);
            /* 用逗号分割的类名，肯定是自己的父类 */
            while(!isalnum(id[idlen - 1]) && id[idlen - 1] != '_')
              idlen--;
            id[idlen] = '\0';
            print_class(fout, id, idlen, 1);
          }
          break;
        case '{':
          if(open_curly_braces_count == 0) {
            idlen = strcpy_trim(id, sizeof(id), tokenstart, 
                pc - tokenstart);
            /* 类名之后是'{'，是自己还是是自己的父类，
             * 看前面有没有':'*/
            while(!isalnum(id[idlen - 1]) && id[idlen - 1] != '_')
              idlen--;
            id[idlen] = '\0';
            print_class(fout, id, idlen, parentclass);
            if(parentclass == 0) {
              strcpy(myname, id);
              mynamelen = idlen;
              classnum1++;
            }
          }
          open_curly_braces_count++;
          if(open_curly_braces_count == 1) {
            parentclass = 0;
            classdef = 0;
            funcdef = 1;
            isaccesstype = 1;
          }
          else if(pstatment) {
            idlen = strcpy_trim(id, sizeof(id), pstatment, 
                pc - pstatment);
            fof = function_or_field(id, idlen);
            print_field_or_operation(fout, id, idlen, accesstype, 
                accesstypelen, fof);
            pstatment = NULL;
          }
          break;
        case ';': case '}':
          if(open_curly_braces_count == 0) {
            inclass = 0;
            funcdef = 0;
            if(mynamelen > 0) {
              fprintf(fout, "\\end{class} %%%.*s\n\n", mynamelen, myname);
              mynamelen = 0;
              pstatment = NULL;
              classnum2++;
            }
          }
          if(pstatment) {
            idlen = strcpy_trim(id, sizeof(id), pstatment, 
                pc - pstatment);
            fof = function_or_field(id, idlen);
            print_field_or_operation(fout, id, idlen, accesstype, 
                accesstypelen, fof);
            pstatment = NULL;
          }
          isaccesstype = 1;
          break;
        case ')': // 类初始化列表
          isaccesstype = 0;
          break;
      }
      pc++;
    }
  }
  if(classnum1 != classnum2)
    printf("waring: %d != %d, someting error\n", classnum1, classnum2);
#if 0
  printf("pc - content = %ld\n", pc - content);
#endif
  return classnum1;
}

static inline const char *skip_space_and_comment(const char *restrict content)
{
  const char *pc;
  pc = content;

  while(*pc && (isspace(*pc) || *pc == '/' || *pc == '#')) {
    /* // 注释开始 */
    if(*pc == '/' && *(pc + 1) == '/') {
      do {
        pc++;
      } while(*pc && *pc != '\r' && *pc != '\n');
      if(*pc)
        pc++;
    }
    else if(*pc == '/' && *(pc + 1) == '*') { /* / * 注释开始 */ 
      do {
        pc++;
      } while(*pc && (*pc != '*' || *(pc + 1) != '/'));
      if(*pc)
        pc += 2;
    }
    else 
      if(*pc == '#') { // 宏定义
      do {
        pc++;
      } while(*pc && *pc != '\r' && *pc != '\n');
    }
    else 
      pc++;
  } 
  return pc;
}


static inline const char *skipword(const char *restrict content)
{
  const char *pc;
  pc = content;

  while(isalpha(*pc) || *pc == '_' || *pc == '~') pc++;
  return pc;

}

static inline int strcpy_trim(char *restrict to, int tosize, const char *restrict from, int fromlength)
{
  char *pend;
  int len, cnt, inword, inlinecomment, inblockcomment;

  memset(to, 0, tosize);

  /* 不要开头的空白类字符 */
  while(*from && isspace(*from)) {
    from++;
    fromlength--;
  }

#if defined(__MINGW64_VERSION_MINOR)
  len = min(fromlength, tosize - 1);
#elif  defined(__linux__)
  len = MIN(fromlength, tosize - 1);
#endif

  pend = to;
  cnt = inword = inlinecomment = inblockcomment = 0;
  while(*from && cnt < len) {
    if((*from == '/' && *(from + 1) == '/') || *from == '#')
      inlinecomment = 1;
    if(*from == '/' && *(from + 1) == '*')
      inblockcomment = 1;

    if(inlinecomment) {
      if(*from == '\r' || *from == '\n')
        inlinecomment = 0;
     len--;
    }
    else if(inblockcomment) {
      if(*from == '*' && *(from + 1) == '/') {
        from++;
        inblockcomment = 0;
        len--;
      }
     len--;
    }
    else { 
      if(isspace(*from) && (*from != '\r' || *from != '\n')) {
        if(inword) {
          *pend = ' ';
          inword = 0;
          pend++;
        }
      }
      else {
        if(inword == 0)
          inword = 1;
        if(*from == '&')
          *pend++ = '\\';
        if(*from == '_')
          pend += snprintf(pend, tosize - (pend - to), "%s",
              "{\\textunderscore}");
        if(*from == '~') {
          strcpy(pend, "{\\textasciitilde}");
          pend += 17;
        }
        if(*from != '_' && *from != '~') {
          *pend = *from;
          pend++;
        }
      }
      cnt++;
    }
    from++;
  }
  if(isspace(*(pend - 1)))
    --pend;
  *pend = '\0';
  return pend - to;
}

static inline int function_or_field(const char *restrict token, int length)
{
  const char *p;

  if(token == NULL || length <= 0)
    return -1;

  if((p = strchr(token, '(')) != NULL)  {
    /* 可能是函数，也可能是函数指针。目前只识别如下形式的函数指针：
     * int (*func)()
     * 由于 strcpy_trim 的作用，在本程序中，函数指针也可能是如下形
     * 式：
     * int ( * func)()
     * int (* func)()
     * int ( *func)()
     */
    if((*(p + 1) && *(p + 1) == '*') || (*(p + 2) && *(p + 2) == '*')) 
      return 1;
    return 0;
  }
  return 1;
}

static __attribute__((noreturn)) void usage(void)
{
  fprintf(stderr, "usage:\ncc2uml <-o outfile> [input file or dir]\n");
  exit(-1);
}

static inline void print_class(FILE *restrict fout, const char *restrict classname, 
    int len, int parent_or_me)
{
  if(parent_or_me == 0) // 自己
    fprintf(fout, "\\begin{class}[text width = 5cm]{%.*s}{0, 0}\n",
        len, (char *)classname);
  else // 自己的父类
    fprintf(fout, "\\inherit{%.*s}\n", len, (char *)classname);
}

static inline void print_field_or_operation(FILE *restrict fout, const char *restrict name, int len,
    const char *restrict access, int accesslen, int func_or_field)
{
  char *p;

  if(func_or_field == 0) { // 函数
    if((p = strstr(name, "virtual")) != NULL) {
      if(*(p + 7) && isspace(*(p + 7))) // 这是个 virtual 函数
        fprintf(fout, "\\%.*svirtualoperation{%.*s}\n", accesslen,
            (char *)access, len, (char*)name);
    }
    else if((p = strstr(name, "static")) != NULL) {
      if(*(p + 7) && isspace(*(p + 6))) // 这是个 static 函数
        fprintf(fout, "\\%.*sstaticoperation{%.*s}\n", accesslen,
            (char *)access, len, (char*)name);
    }
    else 
      fprintf(fout, "\\%.*soperation{%.*s}\n", accesslen,
          (char *)access, len, (char*)name);
  }
  else if(func_or_field == 1)  {// 字段
    if((p = strstr(name, "static")) != NULL) {
      if(*(p + 7) && isspace(*(p + 6))) // 这是个 static 字段
        fprintf(fout, "\\%.*sstaticattribute{%.*s}\n", accesslen,
            (char *)access, len, (char*)name);
    }
    else
      fprintf(fout, "\\%.*sattribute{%.*s}\n", accesslen,
          (char *)access, len, (char*)name);
  }
}
