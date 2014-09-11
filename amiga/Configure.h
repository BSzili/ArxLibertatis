/* hand-crafted for AROS */

#ifndef ARX_CONFIGURE_H
#define ARX_CONFIGURE_H

//#define ARX_HAVE_WINAPI

// Threads
//#ifdef __amigaos4__
#define ARX_HAVE_PTHREADS
//#endif
//#define ARX_HAVE_PRCTL
#ifndef __amigaos4__
#define ARX_HAVE_PTHREAD_SETNAME_NP
#endif
//#define ARX_HAVE_PTHREAD_SET_NAME_NP

// Audio backend
#define ARX_HAVE_OPENAL
/*#define ARX_HAVE_OPENAL_EFX
#define ARX_HAVE_DSOUND*/

// Renderer backend
#define ARX_HAVE_OPENGL
//#define ARX_HAVE_D3D9

// Input backend
#define ARX_HAVE_SDL
//#define ARX_HAVE_DINPUT8

// Crash handler
/*#define ARX_HAVE_CRASHHANDLER_POSIX
#define ARX_HAVE_CRASHHANDLER_WINDOWS*/

// POSIX / Linux features
//#define ARX_HAVE_NANOSLEEP
//#define ARX_HAVE_CLOCK_GETTIME
//#define ARX_HAVE_FORK
//#define ARX_HAVE_READLINK
#define ARX_HAVE_DUP2 //unused
#define ARX_HAVE_EXECL
#define ARX_HAVE_EXECLP
#define ARX_HAVE_WAITPID //unused
//#define ARX_HAVE_BACKTRACE
//#define ARX_HAVE_ISATTY
//#define ARX_HAVE_WORDEXP_H
//#define ARX_HAVE_FPATHCONF
//#define ARX_HAVE_PATHCONF
//#define ARX_HAVE_PC_NAME_MAX
//#define ARX_HAVE_PC_CASE_SENSITIVE
//#define ARX_HAVE_NAME_MAX
//#define ARX_HAVE_SCHED_GETSCHEDULER
/*#define ARX_HAVE_UNAME // unused
#define ARX_HAVE_GETRUSAGE // unused
#define ARX_HAVE_POPEN // unused
#define ARX_HAVE_PCLOSE // unused
#define ARX_HAVE_SYSCONF*/
//#define ARX_HAVE_SIGACTION
/*#define ARX_HAVE_DIRFD
#define ARX_HAVE_FSTATAT*/
#define ARX_HAVE_GETCWD
/*#cmakedefine ARX_HAVE_GETEXECNAME
#cmakedefine ARX_HAVE_SYSCTL
#cmakedefine ARX_HAVE_SETENV*/

// Mac OS X features
//#define ARX_HAVE_MACH_CLOCK

// Arx components
/*#define BUILD_EDITOR
#define BUILD_EDIT_LOADSAVE*/

// Build system
#define UNITY_BUILD

#endif // ARX_CONFIGURE_H
