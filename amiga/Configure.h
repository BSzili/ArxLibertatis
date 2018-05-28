
#ifndef ARX_CONFIGURE_H
#define ARX_CONFIGURE_H

// Aligned allocation
// C11
#define ARX_HAVE_ALIGNED_ALLOC 1
// Windows
#define ARX_HAVE_ALIGNED_MALLOC 0
#define ARX_HAVE_ALIGNED_FREE 0
// POSIX
#define ARX_HAVE_POSIX_MEMALIGN 0
#if defined(__AROS__) || defined(__amigaos4__)
// TODO remove this
#define aligned_alloc(alignment, size) malloc((size))
#endif

// Thread & process control
#define ARX_HAVE_PTHREADS 1
#define ARX_HAVE_PRCTL 0
#ifdef __amigaos4__
#define ARX_HAVE_PTHREAD_SETNAME_NP 0
#else
#define ARX_HAVE_PTHREAD_SETNAME_NP 1
#endif
#define ARX_HAVE_PTHREAD_SET_NAME_NP 0
#define ARX_HAVE_SCHED_GETSCHEDULER 0
#define ARX_HAVE_NANOSLEEP 0
#define ARX_HAVE_FORK 0
#define ARX_HAVE_SETPGID 0
#define ARX_HAVE_EXECVP 0
#define ARX_HAVE_POSIX_SPAWNP 0
#define ARX_HAVE_WAITPID 0
#define ARX_HAVE_SIGACTION 1
#define ARX_HAVE_GETPID 1
#define ARX_HAVE_GETUID 1
#define ARX_HAVE_GETGID 1
#define ARX_HAVE_KILL 0
#define ARX_HAVE_SETRLIMIT 1

// Time
#define ARX_HAVE_CLOCK_GETTIME 0
#define ARX_HAVE_MACH_CLOCK 0

// Filesystem & I/O
#define ARX_HAVE_READLINK 0
#define ARX_HAVE_OPEN 1
#define ARX_HAVE_O_CLOEXEC 0
#define ARX_HAVE_FCNTL 1
#define ARX_HAVE_DUP2 1
#define ARX_HAVE_PIPE 0
#define ARX_HAVE_READ 1
#define ARX_HAVE_CLOSE 1
#define ARX_HAVE_ISATTY 0
#define ARX_HAVE_THREADSAFE_READDIR 1
#define ARX_HAVE_FPATHCONF 0
#define ARX_HAVE_PATHCONF 0
#define ARX_HAVE_NAME_MAX 1
#define ARX_HAVE_PC_NAME_MAX 0
#define ARX_HAVE_PC_CASE_SENSITIVE 0
#define ARX_HAVE_DIRFD 1
#define ARX_HAVE_FSTATAT 0
#define ARX_HAVE_CHDIR 1

// Video backend
#define ARX_HAVE_EPOXY 0
#ifdef __AROS__
#define ARX_HAVE_GLEW 1
#else
#define ARX_HAVE_GLEW 0
#endif
#define ARX_HAVE_GLX 0
#define ARX_HAVE_EGL 0
#define ARX_HAVE_GL_STATIC 0

// Audio backend
#define ARX_HAVE_OPENAL 1
#define ARX_HAVE_OPENAL_EFX 0
#define ARX_HAVE_OPENAL_HRTF 0

// Input backend
#define ARX_HAVE_SDL1 1
#define ARX_HAVE_SDL2 0

// Crash handler
#define ARX_HAVE_CRASHHANDLER_POSIX 0
#define ARX_HAVE_CRASHHANDLER_WINDOWS 0

// Endianness
#define ARX_HAVE_BUILTIN_BSWAP16 0
#define ARX_HAVE_BUILTIN_BSWAP32 0
#define ARX_HAVE_BUILTIN_BSWAP64 0
#define ARX_HAVE_BSWAP_16 0
#define ARX_HAVE_BSWAP_32 0
#define ARX_HAVE_BSWAP_64 0

// Misc POSIX / Linux features
#define ARX_HAVE_DLSYM 1
#define ARX_HAVE_BACKTRACE 0
#define ARX_HAVE_WORDEXP 0
#define ARX_HAVE_UNAME 0
#define ARX_HAVE_GETRUSAGE 1
#define ARX_HAVE_SYSCONF 1
#define ARX_HAVE_GETEXECNAME 0
#define ARX_HAVE_SYSCTL 0
#define ARX_HAVE_SYSCTLBYNAME 0
#define ARX_HAVE_SETENV 0
#define ARX_HAVE_UNSETENV 0
#define ARX_HAVE_CONFSTR 1

// Architure-specific features
#define ARX_HAVE_GET_CPUID 0
#define ARX_HAVE_GET_CPUID_MAX 0
#define ARX_HAVE_XMMINTRIN 0
#define ARX_HAVE_PMMINTRIN 0
#define ARX_HAVE_BUILTIN_IA32_FXSAVE 0

// Build system
#define UNITY_BUILD 1

// Defaults
#define ARX_DEBUG_GL 0

#endif // ARX_CONFIGURE_H
