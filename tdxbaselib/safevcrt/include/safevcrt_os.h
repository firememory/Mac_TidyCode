//////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2006 - All Rights Reserved
//  模块名称: SafeVCRT(系统兼容设置)
//  创建日期: 2006/07/28
//  代码编写: zhanglijun
//  功能说明: 安全的VC运行库
//            当前库主要封装VCRT/STD_C++库的常用操作,为了应对广泛采用中的带来的交叉关联问题
//            本模块抽象系统相关环境,构造一个统一的C++编程接口
//            模块支持以下平台:WIN32/WIN64/LINUX32/LINUX64/UNIX32/UNIX64/MAXOSX/ANDROID/IOS
//            模块支持以下架构:_ARCH_IA32_/_ARCH_AMD64_/_ARCH_ARM_
//  引用示例: 
//  修改记录:
//////////////////////////////////////////////////////////////////////////
#ifndef __SAFE_VCRT_OS__H__
#define __SAFE_VCRT_OS__H__
#if !defined(__SAFE_VCRT__H__)
	#error safevcrt.h must be included
#endif

#if defined(WIN32)
	#define PLATFORM_WINDOWS
	#if defined(_WIN64)
		#define _ARCH_AMD64_
	#else
		#define _ARCH_IA32_
	#endif
	#define _COMPILE_BY_MSC_
#endif
#if defined(LINUX)
	#define PLATFORM_LINUX
	#if defined(LINUX64)
		#define _ARCH_AMD64_
	#else
		#define _ARCH_IA32_
	#endif
	#define _COMPILE_BY_GCC_
#endif
#if defined(UNIX)
	#define PLATFORM_UNIX
	#if defined(UNIX64)
		#define _ARCH_AMD64_
	#else
		#define _ARCH_IA32_
	#endif
	#define _COMPILE_BY_GCC_
#endif
#if defined(MACOSX)
	#define PLATFORM_MACOSX
	#if defined(MACOSX64)
		#define _ARCH_AMD64_
	#else
		#define _ARCH_IA32_
	#endif
	#define _COMPILE_BY_GCC_
#endif

#if defined(IOS)
	#define PLATFORM_IOS
	#if defined(USE_SIMULATOR)
		#define _ARCH_IA32_
	#else
		#define _ARCH_ARM_
	#endif
	#define _COMPILE_BY_GCC_
#endif
#if defined(ANDROID)
	#define PLATFORM_ANDROID
	#define _ARCH_ARM_
	#define _COMPILE_BY_GCC_
#endif




//////////////////////////////////////////////////////////////////////////
// 系统头文件
#if defined(WIN32)
	#if defined(SAFEVCRT_LIB) && defined(SAFEVCRT_DYNAMIC)
		#pragma pack(8)
		// 移植到VS2005
		#define WINVER 0X0501
		#define _WIN32_WINNT 0x0501
		#if !defined(_WIN64) && !defined(_USE_32BIT_TIME_T)
		#define _USE_32BIT_TIME_T
		#endif

		#define VC_EXTRALEAN
		#include <afx.h>
		#include <afxwin.h>
		#include <afxext.h>
		#ifndef _AFX_NO_OLE_SUPPORT
		#include <afxole.h>
		#include <afxodlgs.h>
		#include <afxdisp.h>
		#include <dde.h>
		#endif
		#include <afxdtctl.h>
		#ifndef _AFX_NO_AFXCMN_SUPPORT
			#include <afxcmn.h>
		#endif
		#pragma pack()
	#endif

	// 移植到VS2005
	#ifndef WINVER
		#define WINVER 0X0501
	#endif
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501
	#endif
	#if !defined(_WIN64) && !defined(_USE_32BIT_TIME_T)
		#define _USE_32BIT_TIME_T
	#endif
	#include <winsock2.h>
	#include <mswsock.h>
	#include <ws2tcpip.h>
	#include <string.h>
	#include <malloc.h>
	#include <share.h>
	#include <sys/types.h>
	#include <io.h>
	#include <math.h>
	#include <ctype.h>
	#include <dos.h>
	#include <time.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <fcntl.h>
	#include <direct.h>
	#include <process.h>
	#include <tchar.h>
	#include <limits.h>
	#include <wtypes.h>
	#include <sys/stat.h>
	#include <sys/locking.h>
	#include <new.h>
	#include <signal.h>
	#include <commctrl.h>


	// 内部容许的出错信息
	#pragma warning(disable: 4127)  // constant expression for TRACE/ASSERT
	#pragma warning(disable: 4512)  // private operator= are good to have
	#pragma warning(disable: 4710)  // private constructors are disallowed
	// 由于优化选项可能导致的警告(参考MFC,全部屏蔽)
	#ifndef _DEBUG
	#pragma warning(disable: 4702)  // unreachable code caused by optimizations
	#endif
	// 内部允许的错误信息(VS2005)
	#pragma warning(disable: 4996)  // 老规范CRT运行库调用
	#pragma warning(disable: 4430)  // 模板类警告
	#pragma warning(disable: 4819)  // 代码允许非UNICODE格式
	#pragma warning(disable: 4351)  // 数组的构造函数警告
#endif

#if defined(_COMPILE_BY_GCC_)
	#include <stdio.h>
	#include <math.h>
	#include <string.h>
	#include <sys/stat.h>
	#include <stdarg.h>
	#include <errno.h>
	#include <signal.h>
	#include <ctype.h>
	#include <stdlib.h>
	#include <fcntl.h>
	#include <syslog.h>
	#include <stddef.h>
	
	#include <unistd.h>
	#include <pthread.h>
	#include <dirent.h>
	#include <fnmatch.h>
	#include <termios.h>
	#include <netdb.h>
	#include <getopt.h>
	#include <sys/mman.h>
	#include <sys/wait.h>
	#include <sys/ioctl.h>
	#include <sys/uio.h>
	#include <sys/un.h>
	#include <sys/socket.h>
	#include <sys/param.h>
	#include <sys/utsname.h>
	#include <sys/file.h>
	#include <sys/types.h>
	#include <sys/time.h>
	#include <sys/times.h>
	#include <net/if.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <utime.h>
	#include <wchar.h>
	#include <dlfcn.h>
	#include <zlib.h>

	#if defined(PLATFORM_LINUX) || defined(PLATFORM_ANDROID) || defined(PLATFORM_UNIX)
		#include <new>
	#endif

	#if defined(PLATFORM_LINUX) || defined(PLATFORM_ANDROID)
		#include <sys/epoll.h>
		#include <linux/keyboard.h>
		#include <linux/fb.h>
		#include <netinet/ip_icmp.h>
	#endif

	#if defined(PLATFORM_LINUX)
		#include <ulimit.h>
		#include <sysexits.h>
		#include <sys/kd.h>
		#include <sys/io.h>
		#include <arpa/tftp.h>
	#endif
#endif



#endif

