
/* there are LOT of magic syscalls in freebsd...ill really have to look on it */
/* jail, jail_attach, utrace, kqueue->EV_FILTER_PROC->NOTE_FORK, nfsclnt */
/* syscall-freebsd */
struct syscall_t syscalls_freebsd_x86[] = {
  { "syscall", 0, 4 },
  { "exit", 1, 1 },
  { "fork", 2, 0 },
  { "read", 3, 3 },
  { "write", 4, 3 },
  { "open", 5, 3 },
  { "close", 6, 1 },
  { "wait4", 7, 3 },
  { "old_creat", 8, 2 },
  { "link", 9, 2 },
  { "unlink", 10, 1 },
  { "old_execve", 11, 3},
  { "chdir", 12, 1},
  { "fchdir", 13, 1},
  { "mknod", 14, 1},
  { "chmod", 15, 1},
  { "chown", 16, 1},
  { "break", 17, 1},
  { "old_fsstat", 18, 1},
  { "old_lseek", 19, 1},
  { "getpid", 20, 0},
  { "mount", 21, 0},
  { "unmount", 22, 0},
  { "setuid", 23, 1},
  { "getuid", 24, 0},
  { "ptrace", 26, 4},
  { "recvmsg", 27, 4},
  { "sendmsg", 28, 4},
  { "recvfrom", 29, 4},
  { "accept", 30, 4},
  { "getpeername", 31, 3},
  { "getsockname", 32, 3},
  { "access", 33, 2},
  { "chflags", 34, 2},
  { "fchflags", 35, 2},
  { "sync", 36, 0},
  { "kill", 37, 2},
  { "dup", 41, 2},
  { "pipe", 42, 1},
  { "getepid", 43, 0},
  { "profil", 44, 4}, /* LOLSOME! THIS IS LIKE linux oprofile! we need to hack on it! */
  { "ktrace", 45, 4},
  { "getgid", 47, 0},
  { "signal", 48, 2},
  { "getlogin", 49, 0}, /* like getuid but returns a string */
  { "setlogin", 50, 1}, /* like setuid but passing a string */
  { "acct", 51, 1}, /* given a string for file */
  { "sigaltstack", 53, 2}, /* alterate stack context for signals */
  { "ioctl", 54, 3 },
  { "reboot", 55, 1},
  { "revoke", 56, 1}, /* given a string invalidates the access to a file */
  { "symlink", 57, 2},
  { "readlink", 58, 1},
  { "execve", 59, 2},
  { "umask", 60, 1},
  { "chroot", 61, 1},
  { "mmap", 90, 6},
  { "munmap", 91, 1},
  { "socketcall", 102, 2 },
  { "sigreturn", 119, 1 },
  { "clone", 120, 4 },
  { "mprotect", 125, 3},
  { "rt_sigaction", 174, 3},
  { "rt_sigprocmask", 175, 3},
  { "sysctl", 149, 1 },
  { "mmap2", 192, 6},
  { "fstat64", 197, 2},
  { "fcntl64", 221, 3},
  { "gettid", 224, 0},
  { "set_thread_area", 243, 2},
  { "get_thread_area", 244, 2},
  { "exit_group", 252, 1},
  { NULL, 0, 0 }
};

