#ifndef _SHELL_H
#define _SHELL_H


extern void help(void);
extern void initialize_memory(void);
extern void format(void);
extern void cd(char tmp[9]);
extern void mkdir(char tmp[9],int type);
extern void cat(char tmp[9],int type);
extern void rmdir(char tmp[9]);
extern void del(char tmp[9]);
extern void open_file(char tmp[9]);
extern void close_file(char tmp[9]);
extern void read_file(char tmp[9]);
extern void write_file(char tmp[9]);
extern void write_file_(char tmp[9]);
extern void write_file_111(char tmp[9]);
extern void ls(void);
extern void check_disk(void);
extern int ext2_load(void);
extern int reset_password(void);
extern void help(void);
extern void chmod(char tmp[9],char mode[4]);

#endif
