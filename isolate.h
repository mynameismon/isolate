/*
 *	Process Isolator
 *
 *	(c) 2012-2017 Martin Mares <mj@ucw.cz>
 *	(c) 2012-2014 Bernard Blackham <bernard@blackham.com.au>
 */

#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdbool.h>

#define NONRET __attribute__((noreturn))
#define UNUSED __attribute__((unused))
#define ARRAY_SIZE(a) (int)(sizeof(a)/sizeof(a[0]))

/* isolate.c */

void die(char *msg, ...) NONRET;
void NONRET __attribute__((format(printf,1,2))) err(char *msg, ...);
void __attribute__((format(printf,1,2))) msg(char *msg, ...);

extern int pass_environ;
extern int verbose;
extern int block_quota;
extern int inode_quota;
extern int cg_enable;
extern int cg_memory_limit;
extern int cg_timing;

extern int box_id;
extern uid_t box_uid, orig_uid;
extern gid_t box_gid, orig_gid;

/* util.c */

// malloc but dies on error
void *xmalloc(size_t size);

// strdup but dies on error
char *xstrdup(char *str);

// checks if directory exists
int dir_exists(char *path);

void rmtree(char *path);

// creates directory if does not exist
void make_dir(char *path);
void chowntree(char *path, uid_t uid, gid_t gid);
void close_all_fds(void);

void meta_open(const char *name);
void meta_close(void);
void __attribute__((format(printf,1,2))) meta_printf(const char *fmt, ...);

/* rules.c */

int set_env_action(char *a0);
char **setup_environment(void);

void init_dir_rules(void);
int set_dir_action(char *arg);
void apply_dir_rules(int with_defaults);

void set_quota(void);

/* cg.c */

void cg_init(void);
void cg_prepare(void);
void cg_enter(void);
int cg_get_run_time_ms(void);
void cg_stats(void);
void cg_remove(void);

/* config.c */

extern char *cf_box_root;
extern char *cf_cg_root;
extern char *cf_cg_parent;
extern int cf_first_uid;
extern int cf_first_gid;
extern int cf_num_boxes;

struct cf_per_box {
  struct cf_per_box *next;
  int box_id;
  char *cpus;
  char *mems;
};

struct options_t {
  int box_id;
  char* set_cwd;
  int cg_enable;
  char* dir_action;
  int no_default_dirs;
  int pass_env;
  char* environ;
  int fsize_limit;
  int stack_limit;
  int open_file_limit;
  char* redir_stdin; 
  int memory_limit;
  char* meta_options;
  char* redir_stdout;
  int max_processes;
  int block_quota;
  int inode_quota;
  char* redir_stderr;
  int redir_stderr_to_stdout;
  int silent;
  int timeout;
  int verbose;
  int wall_timeout;
  int extra_timeout;
  int mode;
  int cg_memory_limit;
  int cg_timing;
  int share_net;
  int inherit_fds;
  int tty_hack;
  char** program;
};

void cf_parse(void);
struct cf_per_box *cf_per_box(int box_id);
struct cf_per_box *cf_current_box(void);

// The main function inside an abstraction
int isolate_entry_main(int, char **);

void isolate_api_entry(struct options_t*);

enum ERR_CDS {
  ERR_DIR_ACTION_INVALID,
  ERR_ENV_VAR_INVALID,
  ERR_INVALID_NUMBER,
  ERR_NUM_OUT_OF_RANGE,
  ERR_CG_REQUIRED,
  ERR_ROOT_REQUIRED,
  ERR_ROOT_GRP_REQUIRED,
  ERR_PROGRAM_NOT_FOUND,
  ERR_MODE_NOT_FOUND
};

char* ERRORMSG;

int ERROR;

void init_options (struct options_t*);