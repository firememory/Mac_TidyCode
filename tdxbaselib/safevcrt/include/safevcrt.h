//////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2006 - All Rights Reserved
//  模块名称: SafeVCRT
//  创建日期: 2006/07/28
//  代码编写: zhanglijun
//  功能说明: 安全的VC运行库
//            当前库主要封装VCRT/STD_C++库的常用操作,为了应对广泛采用中的带来的交叉关联问题
//            当前库采用静态库方式,当前可以被移植到linux上
//  引用示例: 
//  修改记录:
//////////////////////////////////////////////////////////////////////////
#ifndef __SAFE_VCRT__H__
#define __SAFE_VCRT__H__


// 兼容模式,兼容旧版
#if defined(COMPATIBLE_5X_SAFEVCRT)
#include "safevcrt_5x.h"
#else
#define WITH_SAFEVCRT


#if !defined(SAFEVCRT_LIB)

	// 默认使用精巧版SafeVCRT(备注:对象不可用于接口)
	#if !defined(MINI_SAFEVCRT) && !defined(FULL_SAFEVCRT) && !defined(DYNA_SAFEVCRT)
		#define MINI_SAFEVCRT
	#endif

	// 精巧模式(完整模式,静态库)
	#if defined(MINI_SAFEVCRT)
		#define SAFEVCRT_STATIC
		#define SAFEVCRT_MINIMODE

	// 完整模式(完整模式,静态库)
	#elif defined(FULL_SAFEVCRT)
		#define SAFEVCRT_STATIC
		#define SAFEVCRT_FULLMODE

	// 动态模式(完整模式,静态动态自动选择)
	#elif defined(DYNA_SAFEVCRT)
		#if defined(WIN32)
			#define SAFEVCRT_DYNAMIC
		#else
			#define SAFEVCRT_STATIC
		#endif
		#define SAFEVCRT_FULLMODE

	// 模式未定义
	#else
		#error SafeVCRT.h must be use one mode:  DYNA_SAFEVCRT,MINI_SAFEVCRT,FULL_SAFEVCRT
	#endif
#endif


// 嵌入其他模式支持
#include "safevcrt_os.h"
#include "safevcrt_types.h"

// 接口导出
#ifdef PLATFORM_WINDOWS
	#define DLLEXPORT __declspec(dllexport)
	#define DLLIMPORT __declspec(dllimport)
	#define LIBEXPORT
	#define LIBIMPORT
#else
	#define DLLEXPORT
	#define DLLIMPORT
	#define LIBEXPORT
	#define LIBIMPORT
#endif

#ifndef SAFEVCRT_DYNAMIC
	#ifdef SAFEVCRT_LIB
		#define SAFEVCRT_IMP LIBEXPORT
	#else
		#define SAFEVCRT_IMP LIBIMPORT
	#endif
#else
	#ifdef SAFEVCRT_LIB
		#define SAFEVCRT_IMP DLLEXPORT
	#else
		#define SAFEVCRT_IMP DLLIMPORT
	#endif
#endif



//////////////////////////////////////////////////////////////////////////
// 调用方式
#if defined(PLATFORM_WINDOWS)
	// 扩充调用方式
	#define CLIBAPI __stdcall
	#define CLIBCDECL __cdecl
	#define CLIBFASTCALL __fastcall
#else
	// 扩充调用方式
	#define CLIBAPI
	#define CLIBCDECL
	#define CLIBFASTCALL	
#endif






	


#if defined(_ARCH_AMD64_)
	#if defined(PLATFORM_WINDOWS)
		#define UNALIGNED	__unaligned
	#else
		#define UNALIGNED
	#endif
#else
	#define UNALIGNED
#endif

// INLINE宏定义
#define CLIB_INLINE	inline

// 禁制赋值
#define NO_ASSIGN_OPERATOR(CLASS)						\
	private:											\
		const CLASS& operator=(const CLASS& Src);		\


// 信号处理(注意信号处理上WINDOWS和LINUX存在区别)
//		WINDOWS常用信号
//			SIGINT          2       用户中断程序运行
//			SIGILL          4       非法指令(非法过程镜像)
//			SIGFPE          8       浮点计算异常
//			SIGSEGV         11      地址(段)冲突
//			SIGTERM         15      外部强制中断
//			SIGBREAK(WIN)   21      CTRL+BREAK中断程序运行
//			SIGABRT         22      程序调用abort函数准备异常退出
//		LINUX常用信号
//			SIGHUP			1		如果终端界面检测到一个连接断开，则将此信号送给与该终端相关的控制进程(对
//									话期首进程)。参见图9.11，此信号送给session结构中s-leader字段所指向的进程。
//									仅当终端的CLOCAL标志没有设置时，在上述条件下才产生此信号。(如果所连接的
//									终端是本地的，才设置该终端的CLOCAL标志。它告诉终端驱动程序忽略所有调制解
//									调器的状态行。在第十一章将说明如何设置此标志)注意，接到此信号的对话期首进
//									程可能在后台，作为一个例子见图97。这区别于通常由终端产生的信号(中断、退
//									出和挂起)，这些信号总是传递给前台进程组。如果对话期前进程终止，则也产生此
//									信号。在这种情况，此信号送给前台进程组中的每一个进程。通常用此信号通知精
//									灵进程(第十三章)以再读它们的配置文件。为此选用SIGHUP的理由是，因为一个精灵
//									进程不会有一个控制终端，而且通常决不会接收到这种信号。SIGKILL 此信号指示进
//									程已执行一条非法硬件指令。4.3BSD由abort函数产生此信号。SIGABRT现在用于这种
//									情况。
//			SIGINT(WIN)  	2		当用户按中断键(常常是DELETE或Control-C)时，终端驱动程序产生此信号  
//									并送至前台进程组中的每一个进程(见图98)。当一个进程在运行时失控，特别  
//									是它正在屏幕上产生大量不需要的输出时，常用此信号终止它。
//			SIGQUIT			3		当用户在终端上按退出键(常常是Control-＼)时，产生此信号，并送至前台进程  
//									组中的所有进程(见图9.8)。此信号不仅终止前台进程组(如SIGINT所做的那样),  
//									它也产生一个core文件。
//			SIGILL(WIN)  	4		非法指令(非法过程镜像)
//			SIGTRAP			5		这指示一个实现定义的硬件故障。
//									此信号名来自于PPP-11的TRAP指令。  
//			SIGABRT(WIN)  	6		调用abort函数时(1017节)产生此信号。该进程异常终止。
//			SIGBUS			7		这指示一个实现定义的硬件故障。
//			SIGFPE(WIN)  	8		此信号表示一个算术运算异常，例如除以0，浮点溢出等。
//			SIGKILL			9		这是两个不能被捕捉或忽略信号中的一个。它向系统管理员提供了一种可以消灭  
//									任一进程的可靠方法。
//			SIGUSR1			10		这是一个用户定义的信号，可用于应用程序。
//			SIGSEGV(WIN)  	11		此信号指示进程进行了一次无效的存储访问。名字SEG表示"段违例"
//									("segmentation violation")。
//			SIGUSR2			12		这是一个用户定义的信号，可用于应用程序。
//			SIGPIPE			13		如果在读进程已终止时写管道，则产生信号SIGPIPE。在14.2节中将说明管道。  
//									当套接口的一端已经终止时，一个进程写该插口也产生此信号。
//			SIGALRM			14		调用alarm函数时设置的时间已经超过.详细情况见10.10节.看由setitimer(2)
//									函数设置的间隔时间已经过时，那么也产生此信号。
//			SIGTERM(WIN)  	15		这是由kill(1)命令发送的系统默认终止信号。
//			SIGSTKFLT		16
//			SIGCHLD			17		在一个进程终止或停止时，SIGCHLD信号被送给其父进程。按系统默认，将忽略  
//									此信号。如果父进程希望了解其子进程的这种状态改变，则应捕捉此信号。信  
//									号捕捉函数中通常要调用wait函数以取得子进程ID和其终止状态。系统V的早期  
//									版本有一个名为SIGCLD(无H)的类似信号。这一信号具有非标准的语义，在SVR2  
//									的手册页就警告在新的程序中尽量小要使用这种信号。应用程序应当使用标准的  
//									SIGCHLD信号。在10.7节中讨论这两个信号。
//			SIGCONT			18		此作业控制信号送给需要其继续运行的处于停止状态的进程。如果接收到此信号  
//									的进程处于停止状态，则系统默认动作是使该进程继续运行，否则默认动作是忽  
//									略此信号。例如，vi编辑程序在捕捉到此信号后，重画终端屏幕。关于进一步的  
//									情况见1020节。
//			SIGSTOP			19		这是一个作业控制信号，它停止一个进程。它类似于交互停止信号(SIGTSTP)，  
//									但是SIGSTOP不能被捕促或忽略。
//			SIGTSTP			20		SIGTSTP 这是交互停止信号，当用户在终端上按挂起键*(常常是Control-Z)时，终端
//									驱动程序产生此信号。
//			SIGTTIN			21		当一个后台进程组进程试图读其控制终端时，终端驱动程序产生此信号。(请参见9
//									8节中对此问题的讨论)。在下列例外情形下，不产生此信号，此时读操作出错返回，
//									errno设置为EIO：(a)读进程忽略或阻塞此信号，式(b)读进程所属的进程组是孤儿进
//									程组。
//			SIGTTOU			22		当一个后台进程组进程试图写其控制终端时产生此信号(请参见9.8节对此问题的讨论。)
//									与上面所述的SIGTTIN信号不同，一个进程可以选择为允许后台进程写控制终端。在第
//									十一章中将讨论如何更改此选择项。如果不允许后台进程写，则与SIGTTIN相似也有两种
//									特殊情况：(a)写进程忽略或阻塞此信号，式(b)写进程所属进程组是孤儿进程组。在这两
//									种情况下不产生此信号，写操作出错返回，errno设置为EIO。不论是否允许后台进程写，
//									某些除写以外的下列终端操作也能产生此信号：tcsetatlr,tcsendbreak,tcdrain,tcflush,
//									tcflow以及tcsetpgrp。在第十一章将说明这些终端操作。
//			SIGURG			23		此信号通知进程已经发生一个紧急情况。在网络连接上，接到非规定波特率的数据时，此
//									信号是可选择地产生的。
//			SIGXCPU			24		SVR4和43+BSD支持资源限制的概念(见711节)。如果进程超过了其软CPU时间限制，则
//									产生SIGXCPU信号。
//			SIGXFSZ			25		如果进程超过了其软文件长度限制(见711节)，则SVR4和43+BSD产生此信号。
//			SIGVTALRM		26		当一个由setitimer(2)函数设置的虚拟间隔时间已经超过时产生此信号。
//			SIGPROF			27		将setitimer(2)函数设置的梗概统计 间隔时间已经超过时产生此信号。
//			SIGWINCH		28		SVR4和43+BSD系统核保持与每个终端或伪终端相关联的*不幸的是语术停止(stop)有不同的
//									意义。在讨论作业控制和信号时我们需提及停止(stopping)和继续作业。但是终端驱动程序一
//									直用术语停止表示用Control-S和Control-Q字符停止和起动终输出。因此，终端驱动程序将产
//									生交互停止信号和字符称之为挂起字符(suspend)而非停止字符。
//									窗口的大小、一个进程可以用ioctl函数(见11,12节)得到或设置窗口的大小。如果一个进程用
//									ioctl的设置-窗口-大小命令更改了窗口大小，则系统核将SIGWINCH信号送至在前台进程组。
//			SIGIO			29		此信号指示一个异步I／O事件。在12.6.2中将对此进行讨论。在图10.1中,对SIGIO的系统默认
//									动作是终止或忽略.不幸的是,这依赖于系统.在SVR4中,SIGIO与SIGPOLL相同，其默认动作是终
//									止此进程。在43+BSD中(此信号起源于4.2BSD)，其默认动作是忽略它。
//			SIGPOLL			SIGIO	这是一种SVR4信号，当在一个可轮询设备上发生一特定事件时产生此信号。在12.5.2节中将说
//									明poll函数和此信号.它与4.3+BSD的SIGIO和SIGURG信号相接近.
// 在WINDOWS程序下需要对特殊信号进行一些处理,因为这些信号并不支持
#if defined(PLATFORM_WINDOWS)
#define SIGALRM	14
#define SIGPIPE 13
#define SIGUSR2	12
#define SIGCHLD 17
#endif
typedef void (* SIGNAL_ROUTINE)(int nSignalNo);
SIGNAL_ROUTINE SAFEVCRT_IMP SetSignalRoutine(int nSignalNo,SIGNAL_ROUTINE pfnSignalRoutine);



//////////////////////////////////////////////////////////////////////////
// 线程处理
#if defined(PLATFORM_WINDOWS)
	typedef HANDLE HTHREAD;
	typedef UINT THREADID;
	typedef UINT HRTHREAD;
	#define HTHREAD_NULL		NULL
	#define HRTHREAD_EXCEPTION	0XFFFFFFFF
#else
	typedef pthread_t HTHREAD;
	typedef pthread_t THREADID;
	typedef VOID* HRTHREAD;
	#define HTHREAD_NULL		0
	#define HRTHREAD_EXCEPTION	(LPVOID)(~0)
#endif
typedef HRTHREAD (WINAPI * LPTHREAD_ENTRY_ROUTE)(LPVOID);
SAFEVCRT_IMP HTHREAD clibCreateThread(LPTHREAD_ENTRY_ROUTE lpStartAddress,LPVOID lpParameter,THREADID* pnThreadId);
SAFEVCRT_IMP THREADID clibGetThreadId();
SAFEVCRT_IMP BOOL clibJoinThread(HTHREAD hThread,HRTHREAD* pnExitCode);
SAFEVCRT_IMP VOID clibExitThread(HRTHREAD nExitCode);
SAFEVCRT_IMP BOOL clibTerminateThread(HTHREAD hThread,HRTHREAD nExitCode);
SAFEVCRT_IMP VOID clibExitProcess();




#if defined(_COMPILE_BY_GCC_)
	// 循环移位
	inline unsigned long _lrotl(unsigned long value, int shift)
		{	unsigned long nSeg1 = value>>(size_of(unsigned long)-shift);
			unsigned long nSeg2 = value<<shift;
			return nSeg1+nSeg2;
		}
	inline unsigned long _lrotr(unsigned long value, int shift)
		{	unsigned long nSeg1 = value>>shift;
			unsigned long nSeg2 = value<<(size_of(unsigned long)-shift);
			return nSeg1+nSeg2;
		}
		inline unsigned int _rotl(unsigned int value, int shift)
		{	unsigned int nSeg1 = value>>(size_of(unsigned int)-shift);
			unsigned int nSeg2 = value<<shift;
			return nSeg1+nSeg2;
		}
		inline unsigned int _rotr(unsigned int value, int shift)
		{	unsigned int nSeg1 = value>>shift;
			unsigned int nSeg2 = value<<(size_of(unsigned int)-shift);
			return nSeg1+nSeg2;
		}



	// WINDOWS的替代函数
	#define _alloca alloca
	#define strcmpi stricmp
	#define	_vsnprintf vsnprintf
	#define	_snprintf snprintf
	#define	wcsnicmp wcsncasecmp
	#define	wcsicmp wcscasecmp
	#define	strnicmp strncasecmp
	#define	stricmp strcasecmp
	#define lstrlenW wcslen
	#define lstrlenA strlen
	#define stricmp strcasecmp
	#define _snprintf snprintf
	#define _vsnprintf vsnprintf
	#define memicmp strnicmp
	// 字符串处理
	#ifdef UNICODE
		#define _vstprintf	vswprintf
		#define _stprintf   swprintf
		#define _vsntprintf	_vsnwprintf
		#define _sntprintf	_snwprintf

		#define lstrlen		lstrlenW
		#define _tcspbrk	wcspbrk
		#define _tcsspn     wcsspn
		#define _tcscspn	wcscspn
		#define _tcsstr		wcsstr
		#define _tcschr		wcschr
		#define _tcsrchr	wcsrchr
		#define _tcslen		wcslen
		#define _tcsncmp	wcsncmp
		#define _tcsdup		wcsdup
		#define _ttoi       _wtoi
		#define _istspace   iswspace
		#define _istdigit   iswdigit
		#define _tcsupr		wcsupr
		#define _tcslwr		wcslwr
		#define _tcsrev		wcsrev
		#define _tcsinc		wcsinc
	#else
		#define _vstprintf	vsprintf
		#define _stprintf	sprintf
		#define _vsntprintf	_vsnprintf
		#define _sntprintf	_snprintf

		#define lstrlen		lstrlenA
		#define _tcspbrk	strpbrk
		#define _tcsspn     strspn
		#define _tcscspn	strcspn
		#define _tcsstr		strstr
		#define _tcschr		strchr
		#define _tcsrchr	strrchr
		#define _tcslen		strlen
		#define _tcsncmp	strncmp
		#define _tcsdup		strdup
		#define _ttoi		atoi
		#define _istspace	isspace
		#define _istdigit	isdigit
		#define _tcsupr		strupr
		#define _tcslwr		strlwr
		#define _tcsrev		strrev
		#define _tcsinc		strinc
	#endif
	#if defined(_ARCH_AMD64_)
		#define _atoi64		atol
	#else
		#define _atoi64		atoll
	#endif
	#include "gnu2win.h"
	#include "profile.h"
#endif

// X32和X64处理
#ifndef strlen32
	#define strlen32(p)	((unsigned int)strlen(p))
#endif

#if defined(PLATFORM_WINDOWS) && defined(_ARCH_AMD64_)
	#define time32_t	long
	#define time32(p)	((long)time(p))
#elif defined(_ARCH_AMD64_)
	#define time32_t	LONG
	#define time32(p)	((LONG)time(p))
#else
	#define time32_t	time_t
	#define time32(p)	(time(p))
#endif




//////////////////////////////////////////////////////////////////////////
// 智能指针
#define DECLARE_SMART_CLASS(SmartClassName,RefClassName)																						\
	class SmartClassName																														\
	{																																			\
	public:																																		\
		SmartClassName(RefClassName* pInterface) { m_pInterface=pInterface; }																	\
		SmartClassName(const SmartClassName& Ref) { m_pInterface=Ref.m_pInterface;  if(m_pInterface!=NULL) m_pInterface->QueryInterface(); }	\
		virtual~SmartClassName() { if(m_pInterface!=NULL) m_pInterface->ReleaseInterface(); }													\
		const SmartClassName& operator=(const SmartClassName& Ref)																				\
			{	if(m_pInterface!=NULL) m_pInterface->ReleaseInterface();																		\
				m_pInterface=Ref.m_pInterface;																									\
				if(m_pInterface!=NULL) m_pInterface->QueryInterface();																			\
				return *this;																													\
			}																																	\
		VOID SetHandler(RefClassName* pInterface)																								\
			{	if(m_pInterface!=NULL) m_pInterface->ReleaseInterface();																		\
				m_pInterface=pInterface;																										\
			}																																	\
		BOOL IsValid() { return m_pInterface!=NULL; }																							\
		operator RefClassName* () const { return m_pInterface; }																				\
		RefClassName* operator->() { VERIFY(m_pInterface!=NULL); return m_pInterface; }															\
	protected:																																	\
		RefClassName* m_pInterface;																												\
	};																																			\


#define DECLARE_INTERFACE_CLASS(RefClassName)																									\
	RefClassName* QueryInterface()																												\
	{	InterlockedIncrement(&m_nRefCount);																										\
		return this;																															\
	}																																			\
	LONG ReleaseInterface()																														\
	{	LONG nRef=InterlockedDecrement(&m_nRefCount);																							\
		if(nRef==0)																																\
		{	delete this;																														\
		}																																		\
		return nRef;																															\
	}																																			\


//////////////////////////////////////////////////////////////////////////
// 安全内存管理
#ifndef TMALLOC
	inline void* mem_init(void* pMemory,size_t cbSize)
	{	if(pMemory!=NULL&&cbSize>0)
			memset(pMemory,0,cbSize);
		return pMemory;
	}
	#define TMALLOC(n,T)		((T*)mem_init(((n)<=0)?NULL:malloc(size_of(T)*(n)),size_of(T)*(n)))
#endif
#ifndef TFREE
	#define TFREE(a)			if(a!=NULL) { free(a); a=NULL; }
#endif
#ifndef TDEL
	#define TDEL(a)				if(a!=NULL) { delete a; a=NULL; }
#endif
#ifndef TDELARRAY
	#define TDELARRAY(a)		if(a!=NULL) { delete[] a; a=NULL; }
#endif
#ifndef TDELOBJECT
	#define TDELOBJECT(a)		if(a!=NULL) { ::DeleteObject(a); a=NULL; }
#endif
#ifndef TMEMCPY
	inline LONG __memcpy(void* pDest,LONG nDestCount,void* pSrc,LONG nSrcCount,size_t cbUnit)
	{	LONG nCopyCount=min_of(nDestCount,nSrcCount);
		if(nCopyCount>0) memcpy(pDest,pSrc,nCopyCount*cbUnit);
		return nCopyCount;
	}
	#define TMEMCPY(pDest,nDestCount,pSrc,nSrcCount) __memcpy(pDest,nDestCount,pSrc,nSrcCount,size_of(*pDest))
#endif
#ifndef TZEROMEMORY
	#define TZEROMEMORY(pBuffer,cbBuffer) if((pBuffer)!=NULL&&(cbBuffer)!=0) memset(pBuffer,0,cbBuffer)
#endif


// 析构器(参阅: ASEStdDef.h)
#ifndef EMBED_OBJECT
#define EMBED_OBJECT
	typedef VOID (* DESTRUCTOR)(VOID*);
	#define DECLARE_DESTRUCT(CLASS)												\
		public:																	\
		static VOID __Destructor(LPVOID pvSelf)									\
		{	CLASS* pSelf=(CLASS*)pvSelf;										\
			if(pSelf!=NULL) pSelf->~CLASS();									\
		}
	typedef VOID* (* CONNECTOR)(VOID*);
	#define DECLARE_CONNECTOR(CLASS,TYPE)										\
		public:																	\
		static VOID* __Connector(LPVOID pvSelf)									\
		{	CLASS* pSelf=(CLASS*)pvSelf;										\
			if(pSelf!=NULL) return (VOID*)((TYPE*)pSelf);						\
			return NULL;														\
		}
	#define DECLARE_CONNECTOR_EX(CLASS,BYCLASS,TYPE)							\
		public:																	\
		static VOID* __Connector(LPVOID pvSelf)									\
		{	CLASS* pSelf=(CLASS*)pvSelf;										\
			if(pSelf!=NULL) return (VOID*)(TYPE*)static_cast<BYCLASS*>(pvSelf);	\
			return NULL;														\
		}
#endif


//////////////////////////////////////////////////////////////////////////
// 文件句柄和文件路径处理
#ifndef FFILELENGTH
	#define FFILELENGTH(a)		((fileno(a)>=0)?filelength(fileno(a)):0)
#endif
#ifndef TFCLOSE
	#define TFCLOSE(a)			if(a!=NULL) { fclose(a); a=NULL; }
#endif
// 获取文件长度
SAFEVCRT_IMP long filelength2(int fileno);
// 得到模块所在的路径
SAFEVCRT_IMP LPCSTR GetModulePath(HMODULE hModule=NULL,LPSTR lpszBuffer=NULL,LONG nSize=MAX_PATH);
// 转换为绝对路径
SAFEVCRT_IMP LPCSTR ConvertAbsolutePath(LPCSTR lpszPath,LPSTR lpszAbsPath,LONG nSize);
// 转换为绝对路径
SAFEVCRT_IMP LPCSTR ConvertAbsolutePathAt(LPCSTR lpszAt,LPCSTR lpszPath,LPSTR lpszAbsPath,LONG nSize);
// 转换为路径名
SAFEVCRT_IMP LPCSTR ConvertToDir(LPSTR lpszPath);
// 得到文件路径
SAFEVCRT_IMP LPCSTR GetFileDir(LPCSTR lpszPath,LPSTR lpszDir,LONG nSize);
// 判断文件是否存在
SAFEVCRT_IMP BOOL IsPathExist(LPCSTR lpszPath);
// 判断文件是否存在
SAFEVCRT_IMP BOOL IsPathExistEx(LPCSTR lpszPath,BOOL bNeedWrite);
// 读取文件时间
SAFEVCRT_IMP time_t GetFileModifyTime(LPCSTR lpszFilePath);
// 获取文件名和扩展名
SAFEVCRT_IMP LPCSTR GetFileName(LPCSTR pszPath,LPSTR pszTemp,LONG nSize);
SAFEVCRT_IMP LPCSTR GetFileExt(LPCSTR pszPath,LPSTR pszTemp,LONG nSize);
// 删除匹配的文件
SAFEVCRT_IMP void DeleteMatchFile(LPCTSTR pszPath,LPCTSTR pszPattern);
SAFEVCRT_IMP void DeleteDirInfo(LPCTSTR pszPath,BOOL bCheckSafe=TRUE);
SAFEVCRT_IMP void DeleteDirs(LPCTSTR pszPath,BOOL bCheckSafe=TRUE);
SAFEVCRT_IMP void CreateDirs(LPCTSTR pszPath);
SAFEVCRT_IMP void CopyMatchFile(LPCTSTR pszFrom,LPCTSTR pszTo,BOOL bIgnoreExists,BOOL bResetAttrib);
// 动态库装载
SAFEVCRT_IMP HMODULE LoadLibraryWithPath(LPCTSTR pszDll);
#if defined(PLATFORM_WINDOWS)
	#define fncmp stricmp
#else
	#define fncmp strcmp
#endif


//////////////////////////////////////////////////////////////////////////
// 系统字符串处理函数扩充
#define sprintf0(a,b)			sprintf(a,b)
#define sprintf1(a,b,c)			sprintf(a,b,c)
#define sprintf2(a,b,c,d)		sprintf(a,b,c,d)
#define sprintf3(a,b,c,d,e)		sprintf(a,b,c,d,e)
#define sprintf4(a,b,c,d,e,f)	sprintf(a,b,c,d,e,f)
#define sprintf5(a,b,c,d,e,f,g)	sprintf(a,b,c,d,e,f,g)

#ifndef nsprintf
	inline int __nsprintf(char* lpszString,int nSize,const char* lpszFmt,...)
	{	if(nSize<=0) return 0;
		va_list args;
		va_start(args,lpszFmt);
		int nRet=_vsnprintf(lpszString,nSize,lpszFmt,args);
		if(nRet<0||nRet==nSize)
		{	lpszString[nSize-1]=0;
			nRet=nSize-1;
		}
		va_end(args);
		return nRet;
	}
	#define nsprintf __nsprintf
#endif
#ifndef nvsprintf
	inline int __nvsprintf(char* lpszString,int nSize,const char* lpszFmt,va_list ap)
	{	if(nSize<=0) return 0;
		int nRet=_vsnprintf(lpszString,nSize,lpszFmt,ap);
		if(nRet<0||nRet==nSize)
		{	lpszString[nSize-1]=0;
			nRet=nSize-1;
		}
		return nRet;
	}
	#define nvsprintf __nvsprintf
#endif
#ifndef nscatprintf
	inline int __nscatprintf(char* lpszString,int nSize,const char* lpszFmt,...)
	{	if(nSize<=0) return 0;
		va_list args;
		va_start(args,lpszFmt);
		int nOldLen=strlen32(lpszString);
		if(nOldLen+1>=nSize) return 0;
		int nRet=_vsnprintf(lpszString+nOldLen,nSize-nOldLen,lpszFmt,args);
		if(nRet<0||nRet==nSize-nOldLen)
		{	lpszString[nSize-1]=0;
			nRet=nSize-1-nOldLen;
		}
		va_end(args);
		return nRet;
	}
	#define nscatprintf __nscatprintf
#endif
#ifndef nvscatprintf
	inline int __nvscatprintf(char* lpszString,int nSize,const char* lpszFmt,va_list ap)
	{	if(nSize<=0) return 0;
		int nOldLen=strlen32(lpszString);
		if(nOldLen+1>=nSize) return 0;
		int nRet=_vsnprintf(lpszString+nOldLen,nSize-nOldLen,lpszFmt,ap);
		if(nRet<0||nRet==nSize-nOldLen)
		{	lpszString[nSize-1]=0;
			nRet=nSize-1-nOldLen;
		}
		return nRet;
	}
	#define nvscatprintf __nvscatprintf
#endif
#ifndef nstrcpy
	inline void __nstrcpy(char* lpszString,const char* lpszSrc,int nSize)
	{	if(nSize<=0) return;
		if(lpszSrc!=NULL)
		{	if(lpszSrc[0]==0) { lpszString[0]='\0'; return; }
			strncpy(lpszString,lpszSrc,nSize);
			lpszString[nSize-1]=0;
		}
		else
		{	lpszString[0]=0;
		}
	}
	#define nstrcpy __nstrcpy
#endif
#ifndef nstrcat
	inline void __nstrcat(char* lpszFront,const char* lpszBack,int nSize)
	{	strncat(lpszFront,lpszBack,nSize);
		lpszFront[nSize-1]=0;
	}
	#define nstrcat __nstrcat
#endif

// 字符串处理
#ifndef SAFESTRING
	#define SAFESTRING(a)	(a[size_of(a)-1]=0)
#endif

// 字符串复制
#ifndef COPYSTRARRAY
	#define COPYSTRARRAY(a,b)	nstrcpy((char*)(a),(b),ARRAYSIZE(a))
#endif
#ifndef CATSTRARRAY
	#define CATSTRARRAY(a,b)	nstrcat((char*)(a),(b),ARRAYSIZE(a))
#endif

// 缓冲区处理
#ifndef COPYBINARRAY
	#define COPYBINARRAY(a,b)		memcpy((a),(b),size_of(a))
#endif

#ifndef COPYBINARRAY2
	#define COPYBINARRAY2(a,b,c)	if(min_of(size_of(a),(c))>0) { memcpy((a),(b),min_of(size_of(a),(c))); }
#endif

// 函数名映射
#define FUNC_WRAPPER1(T,R,F,T1,P1)							virtual R F(T1 P1)							{ return T->F(P1); }
#define FUNC_WRAPPER2(T,R,F,T1,P1,T2,P2)					virtual R F(T1 P1,T2 P2)					{ return T->F(P1,P2); }
#define FUNC_WRAPPER3(T,R,F,T1,P1,T2,P2,T3,P3)				virtual R F(T1 P1,T2 P2,T3 P3)				{ return T->F(P1,P2,P3); }
#define FUNC_WRAPPER4(T,R,F,T1,P1,T2,P2,T3,P3,T4,P4)		virtual R F(T1 P1,T2 P2,T3 P3,T4 P4)		{ return T->F(P1,P2,P3,P4); }
#define FUNC_WRAPPER5(T,R,F,T1,P1,T2,P2,T3,P3,T4,P4,T5,P5)	virtual R F(T1 P1,T2 P2,T3 P3,T4 P4,T5 P5)	{ return T->F(P1,P2,P3,P4,P5); }


// 代码段
#define BEGIN_SCOPE()		for(;;)	{
#define END_SCOPE()			break; }


//////////////////////////////////////////////////////////////////////////
// 数值处理

// 得到范围内限制的数据
#define RANGE_LIMIT(VAL,MIN_VAL,MAX_VAL)  \
	min_of(max_of((VAL),(MIN_VAL)),(MAX_VAL))

#define EN_RANGE_LIMIT(TYPE,VAL,MIN_VAL,MAX_VAL)  \
	VAL=(TYPE)min_of(max_of((VAL),(MIN_VAL)),(MAX_VAL))


// 提高浮点数的精度
SAFEVCRT_IMP double RoundDouble(double dExpression,int nNumDecimalPlaces=2);


// 扩展数学函数(最大公约数求解,最小公倍数求解,用于均衡算法)
SAFEVCRT_IMP int math_gcd(int a,int b);			// 求两个数的最大公约数(欧几里德算法)
SAFEVCRT_IMP int math_lcm(int a,int b);			// 求两个数的最小公倍数
SAFEVCRT_IMP int math_ngcd(int* pa,int n);		// 求N个数的最大公约数
SAFEVCRT_IMP int math_nlcm(int* pa,int n);		// 求N个数的最小公倍数



//////////////////////////////////////////////////////////////////////////
// 堆栈跟踪函数
//		堆栈跟踪实例
//		STACKFRAME StackFrame=GetPrevStackFrame(NULL);
//		while(StackFrame.m_lpNextEIP!=NULL&&StackFrame.m_lpEBP!=NULL)
//			StackFrame=GetPrevStackFrame(StackFrame.m_lpEBP);
typedef struct tagSTACKFRAME
{	LPVOID m_lpEBP;			// 调用的栈基址
	LPVOID m_lpESP;			// 调用的栈顶
	LPVOID m_lpNextEIP;		// 函数返回地址
} STACKFRAME,*LPSTACKFRAME;
typedef struct tagSTACKSNAP
{	LPVOID m_lpEBP;			// 调用的栈基址
	LPVOID m_lpESP;			// 调用的栈顶
	LPVOID m_lpNextEIP;		// 函数返回地址
	BYTE   m_szStack[1];	// 堆栈区间
} STACKSNAP,*LPSTACKSNAP;
SAFEVCRT_IMP STACKFRAME GetPrevStackFrame(LPVOID lpEBP);
SAFEVCRT_IMP LPVOID GetNextEIP();





//////////////////////////////////////////////////////////////////////////
// 调试函数
SAFEVCRT_IMP VOID clibSetSwapDir(LPCSTR pszSwapDir);
SAFEVCRT_IMP LPCSTR clibGetSwapDir();

// 打印跟踪内容
SAFEVCRT_IMP VOID clibTrace(LPCSTR pszFormat,...);
#ifdef _DEBUG
	#ifndef CLIBTRACE
		#define CLIBTRACE   clibTrace
	#endif
#else
	#ifndef CLIBTRACE
		#define CLIBTRACE   1 ? (void)0 : clibTrace
	#endif
#endif

// 打印跟踪内容(内存)
SAFEVCRT_IMP VOID clibTraceMemory(LPVOID lpBuffer,LONG nSize,LPCSTR lpszFmt,...);


// 断言
SAFEVCRT_IMP VOID clibReportAssert(LPCSTR lpszFile,LONG nLine,LPCSTR lpszExpr);
#ifdef _DEBUG
	#ifndef CLIBASSERT
		#define CLIBASSERT(expr) {if(!(expr)) { clibReportAssert(__FILE__, __LINE__,#expr); }}
	#endif
#else
	#ifndef CLIBASSERT
		#define CLIBASSERT(f)    ((void)0)
	#endif
#endif

// 断言
SAFEVCRT_IMP VOID clibReportVerify(LPCSTR lpszFile,LONG nLine,LPCSTR lpszExpr);
#ifndef CLIBVERIFY
	#define CLIBVERIFY(expr) {if(!(expr)) { clibReportVerify(__FILE__, __LINE__,#expr); }}
#endif


// 地址断言
SAFEVCRT_IMP BOOL clibIsValidAddress(const void* lp,UINT nBytes,BOOL bReadWrite=TRUE);


// 断言
SAFEVCRT_IMP VOID clibReportFixMe(LPCSTR lpszFile,LONG nLine,LPSTR lpszInfo);
#ifndef CLIB_FIXME
	#define CLIB_FIXME(lpszInfo) { clibReportFixMe(__FILE__, __LINE__,lpszInfo); }
#endif
#define CLIB_BUGBUG	CLIB_FIXME

SAFEVCRT_IMP VOID clibDump(LPCSTR pszFormat,...);
#ifdef _DEBUG
	#ifndef CLIBDUMP
		#define CLIBDUMP   clibDump
	#endif
#else
	#ifndef CLIBDUMP
		#define CLIBDUMP   1 ? (void)0 : clibDump
	#endif
#endif

SAFEVCRT_IMP VOID clibDumpMemory(LPVOID lpBuffer,LONG nSize,LPCSTR lpszFmt,...);
SAFEVCRT_IMP VOID SetDebugBreak(BOOL bBreakOn);
SAFEVCRT_IMP VOID CatchDebugBreak();
SAFEVCRT_IMP LPCSTR GetMessageName(UINT nMsg);
#ifdef _DEBUG
	#define IS_DEBUG_VERSION	(1)
#else
	#define IS_DEBUG_VERSION	(0)
#endif

//////////////////////////////////////////////////////////////////////////
// 环境变量处理
SAFEVCRT_IMP VOID AddEnvironmentSearchPath(LPCSTR pszPath);



// 版本串、版本号互相转换
SAFEVCRT_IMP WORD VersionString2Word(LPCSTR lpszVersionString,LPWORD lpwRelease);
SAFEVCRT_IMP VOID VersionWordString(WORD wVersion,WORD wRelease,LPSTR lpszVersionString,LONG nSize);
SAFEVCRT_IMP WORD FloatVersionToWord(FLOAT fVersion);
SAFEVCRT_IMP DWORD VersionString2Dword(LPCSTR lpszVersionString);
SAFEVCRT_IMP VOID VersionDwordString(DWORD dwVersion,LPSTR lpszVersionString,LONG nSize);
SAFEVCRT_IMP DWORD FloatVersionToDword(FLOAT fVersion);
SAFEVCRT_IMP LONG CompareVersion(DWORD dwVersion1,DWORD dwVersion2);


// 配置文件获取
SAFEVCRT_IMP BOOL WritePrivateProfileInt(LPCSTR lpAppName,LPCSTR lpKeyName,INT nValue,LPCSTR lpFileName);
#ifndef GetPrivateProfileStrArray
	#define GetPrivateProfileStrArray(lpAppName,lpKeyName,lpDefault,lpReturnedString,lpFileName)					\
	GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpReturnedString,size_of(lpReturnedString),lpFileName);
#endif



//////////////////////////////////////////////////////////////////////////
// 字符串处理
#define STRING_EMPTY	_T("")																	// 空串
#define STRING_PERCENTS _T("%s")																// "%s"
SAFEVCRT_IMP char* strtoupr(char*string);														// 字符串大小写转换
SAFEVCRT_IMP BOOL PatternCompare(LPCSTR lpszSource,LPCSTR lpszPattern);							// 模糊匹配
SAFEVCRT_IMP BOOL PatternCompareAsFileName(LPCSTR lpszSource,LPCSTR lpszPattern);				// 模糊匹配
SAFEVCRT_IMP const char* strnstr(const char* str1,size_t size,const char * str2);				// 在字符串内查找
SAFEVCRT_IMP const char* strnistr(const char* str1,size_t size,const char * str2);				// 在字符串内查找(忽略大小写)
SAFEVCRT_IMP const char* strnchr(const char* str1,size_t size,const char ch);					// 在字符串内查找
SAFEVCRT_IMP LPSTR AllTrimStr(LPSTR lpszString);												// 去掉头部尾部全部空格
SAFEVCRT_IMP LPSTR AllTrimSpace(LPSTR lpszString);												// 去掉头部尾部全部空格(同AllTrimStr)
SAFEVCRT_IMP LPSTR AllTrimBlank(LPSTR lpszString);												// 去掉头部尾部全部空格和TAB符
SAFEVCRT_IMP LPSTR AllTrimBlankAndZero(LPSTR lpszString);										// 去掉头部尾部全部空格和TAB符以及头部的'0'字符
SAFEVCRT_IMP LPSTR TrimSpace(LPSTR lpszString);													// 去掉全部空格(包括字符串中间的)
SAFEVCRT_IMP LPSTR TrimBlank(LPSTR lpszString);													// 去掉全部空格和TAB符(包括字符串中间的)
SAFEVCRT_IMP LPSTR TrimNonAlpla(LPSTR lpszString);												// 去掉全部非Alpha字符(包括字符串中间的)
SAFEVCRT_IMP LPSTR TrimNonEnglish(LPSTR lpszString);											// 去掉全部非英文字符(包括字符串中间的)
SAFEVCRT_IMP LPSTR TrimExclude(LPSTR lpszString,LPCTSTR pszExclude);							// 去掉指定范围的字符(包括字符串中间的)
SAFEVCRT_IMP LPSTR TrimPathStr(LPSTR lpszString,LONG nSize);									// 格式化路径串
SAFEVCRT_IMP LPSTR TrimComment(LPSTR lpszString,LPCSTR pszLead);								// 去掉注释
SAFEVCRT_IMP LPSTR StrLowerCase(LPSTR lpszString);												// 转换字符串到小写
SAFEVCRT_IMP LPSTR StrUpperCase(LPSTR lpszString);												// 转换字符串到大写
SAFEVCRT_IMP LPSTR Unix2Dos(LPSTR lpBuffer,LONG nSize);											// 转换UNIX格式文本到DOS格式文本
SAFEVCRT_IMP LPSTR FormatStrToWidth(LPSTR lpOut,LPCSTR lpIn,LONG nSize);						// 字符串补零
SAFEVCRT_IMP BOOL IsTailBy(LPCSTR lpszString,CHAR cChar);										// 判断字符串数否以字符结尾
SAFEVCRT_IMP LONG FindString(LPCSTR lpszString,LPCSTR lpszFindWhat,LONG nFrom=0);				// 字符串查找
SAFEVCRT_IMP LONG ReverseFindString(LPCSTR lpszString,LPCSTR lpszFindWhat);						// 字符串查找
SAFEVCRT_IMP LONG FindChar(LPCSTR lpszString,CHAR cFindWhat,LONG nFrom=0);						// 字符查找
SAFEVCRT_IMP LONG ReverseFindChar(LPCSTR lpszString,CHAR cFindWhat);							// 字符查找
SAFEVCRT_IMP LONG FindCharOneOf(LPCSTR lpszString,LPCSTR lpszCharSet,LONG nFrom=0);				// 字符查找
SAFEVCRT_IMP BOOL ReplaceString(LPSTR lpStr,DWORD dwSize,LPCSTR lpszFrom,LPCSTR lpszTo);		// 字符串替换
SAFEVCRT_IMP LPSTR ReplaceStringEx(LPSTR lpStr,LPCSTR lpszFrom,LPCSTR lpszTo);					// 字符串替换
SAFEVCRT_IMP VOID ReplaceString(LPSTR lpStr,DWORD dwSize,TCHAR chFrom,TCHAR chTo);				// 字符串替换
SAFEVCRT_IMP VOID ReplaceStringEx(LPSTR lpStr,TCHAR chFrom,TCHAR chTo);							// 字符串替换
SAFEVCRT_IMP LPSTR GetMidString(LPSTR lpszString,LONG nOffset,LONG nLength,LPSTR lpszTarget);	// 直接截取子串
SAFEVCRT_IMP LPSTR GetLeftString(LPSTR lpszString,LONG nLength,LPSTR lpszTarget);				// 直接截取子串(左)
SAFEVCRT_IMP LPSTR GetRightString(LPSTR lpszString,LONG nLength,LPSTR lpszTarget);				// 直接截取子串(右)
SAFEVCRT_IMP LPSTR MidString(LPSTR lpszString,LONG nOffset,LONG nLength=-1);					// 直接截取子串
SAFEVCRT_IMP LPSTR LeftString(LPSTR lpszString,LONG nCount);									// 取左侧的字符串
SAFEVCRT_IMP LPSTR RightString(LPSTR lpszString,LONG nCount);									// 取右侧的字符串
SAFEVCRT_IMP BOOL IsNumber(LPCSTR lpszString);													// 判断是否是数字串
SAFEVCRT_IMP VOID RandString(LPSTR pszString,LONG nStringLen);									// 产生随机字符串
SAFEVCRT_IMP BOOL ShiftBuffer(LPCSTR& pszBuffer,DWORD& cbBuffer,CHAR chSplitter,LPCSTR& pszPart,DWORD& cbPart);	// 从缓冲区中移出一片




//////////////////////////////////////////////////////////////////////////
// 类型转换

// HEX字符转换为数字
#define HEXCHARTOBYTE(ch)													\
	(BYTE)(((BYTE)(ch)>='A'&&(BYTE)(ch)<='Z')?((ch)-'A'+10):				\
	(((BYTE)(ch)>='a'&&(BYTE)(ch)<='z')?(ch-'a'+10):(ch-'0')))				\

// HEX数字转换为字符
#define HEXBYTETOCHAR(ch)													\
	(CHAR)(((BYTE)(ch)<=9)?((ch)+'0'):((ch)-10+'A'))						\

// MD5二进制描述(参阅: ASEStdDef.h)
#ifndef MD5BIN_DEFINED
	#define MD5BIN_DEFINED
	#pragma pack(1)
	typedef struct tagMD5BIN	// MD5码结构
	{	BYTE m_szMD5[16];		// MD5码缓冲区
	} MD5BIN,*LPMD5BIN;
	typedef const MD5BIN* LPCMD5BIN;
	const MD5BIN md5Null={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	#pragma pack()
	inline bool operator==(const MD5BIN s1,const MD5BIN s2)
	{	if(memcmp(&s1,&s2,size_of(MD5BIN))==0) return true;
		return false;
	}
	#if 0
		inline bool operator!=(const MD5BIN s1,const MD5BIN s2)
		{	if(memcmp(&s1,&s2,size_of(MD5BIN))==0) return false;
			return true;
		}
	#endif
#endif

// 项目HASH描述(参阅: ASEStdDef.h)
#ifndef IHASH_DEFINED
#define IHASH_DEFINED
	#pragma pack(1)
	typedef struct tagIHASH		// 项目HASH结构
	{	DWORD m_dwSize;			// 项目大小
		BYTE m_acHash[16];		// 项目HASH
	} IHASH,*LPIHASH;
	const IHASH ihashNull={0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	#pragma pack()
	inline bool operator==(const IHASH s1,const IHASH s2)
	{	if(memcmp(&s1,&s2,size_of(IHASH))==0) return true;
		return false;
	}
	#if 0
		inline bool operator!=(const IHASH s1,const IHASH s2)
		{	if(memcmp(&s1,&s2,size_of(IHASH))==0) return false;
			return true;
		}
	#endif
#endif

SAFEVCRT_IMP LPCSTR BoolToStr(BOOL i,LPSTR lpszTemp,LONG nSize);											// BOOL型转字符串
SAFEVCRT_IMP LPCSTR ByteToStr(BYTE i,LPSTR lpszTemp,LONG nSize);											// BYTE型转字符串
SAFEVCRT_IMP LPCSTR DwordToStr(DWORD i,LPSTR lpszTemp,LONG nSize);											// DWORD型转字符串
SAFEVCRT_IMP LPCSTR UintToStr(UINT i,LPSTR lpszTemp,LONG nSize);											// UINT型转字符串
SAFEVCRT_IMP LPCSTR UlongToStr(ULONG i,LPSTR lpszTemp,LONG nSize);											// ULONG型转字符串
SAFEVCRT_IMP LPCSTR UshortToStr(USHORT i,LPSTR lpszTemp,LONG nSize);										// USHORT型转字符串
SAFEVCRT_IMP LPCSTR CharToStr(CHAR i,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);							// CHAR型转字符串
SAFEVCRT_IMP LPCSTR IntToStr(INT i,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);								// INT型转字符串
SAFEVCRT_IMP LPCSTR Int64ToStr(INT64 i,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);							// INT64型转字符串
SAFEVCRT_IMP LPCSTR Uint64ToStr(UINT64 i,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);						// UINT64型转字符串
SAFEVCRT_IMP LPCSTR LongToStr(LONG i,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);							// LONG型转字符串
SAFEVCRT_IMP LPCSTR ShortToStr(SHORT i,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);							// SHORT型转字符串
SAFEVCRT_IMP LPCSTR FloatToStr(FLOAT fValue,LONG nDecimal,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);		// CY类型(FLOAT)转字符串
SAFEVCRT_IMP LPCSTR DoubleToStr(DOUBLE dValue,LONG nDecimal,LPSTR lpszTemp,LONG nSize,BOOL bSign=FALSE);	// CY类型(DOUBLE)转字符串
SAFEVCRT_IMP LONG FloatToLong(FLOAT fValue,LONG nDecimal);													// CY类型(FLOAT)转LONG
SAFEVCRT_IMP LONG DoubleToLong(DOUBLE dValue,LONG nDecimal);												// CY类型(DOUBLE)转LONG
SAFEVCRT_IMP FLOAT LongToFloat(LONG nValue,LONG nDecimal);													// CY类型(LONG)转FLOAT
SAFEVCRT_IMP DOUBLE LongToDouble(LONG nValue,LONG nDecimal);												// CY类型(LONG)转DOUBLE
SAFEVCRT_IMP DWORD StringToDword(LPCSTR lpszString);														// 数据字符串转换(支持16进制表达)
SAFEVCRT_IMP UINT64 StringToUint64(LPCSTR lpszString);														// 数据字符串转换(支持16进制表达)
SAFEVCRT_IMP LPVOID StringToPointer(LPCSTR lpszString);														// 指针字符串转换
SAFEVCRT_IMP LPCSTR DwordToHexStr(DWORD dwVar,LPSTR lpszTemp,LONG nSize);									// 数据转换为16进制字符串
SAFEVCRT_IMP DWORD HexStrToDword(LPCSTR pszString,DWORD cbString);											// 16进制字符串转换为数据

// 16进制转换
SAFEVCRT_IMP DWORD HexStrToBinary(LPCSTR lpszString,LPBYTE pBuffer,DWORD cbBuffer);							// 数据字符串转换(支持16进制表达)
SAFEVCRT_IMP DWORD HexStrToBinaryEx(LPCSTR lpszString,DWORD dwStrLen,LPBYTE pBuffer,DWORD cbBuffer);		// 数据字符串转换(支持16进制表达)
SAFEVCRT_IMP LPCSTR BinaryToHexStr(LPCBYTE pBuffer,DWORD cbBuffer,LPSTR lpszTemp,LONG nSize);				// 数据转换为16进制字符串

// 检查字符串类型
#define STRING_COMMON		(0)			// 常规字符串
#define STRING_NUMBER		(1)			// 数字
#define STRING_CHARACTER	(2)			// 字符
#define STRING_ALPHANUM		(3)			// 数字和字符
#define STRING_FLOAT		(4)			// 浮点
#define STRING_EMAIL		(5)			// 邮件地址
#define STRING_IPV4ADDR		(6)			// IPv4地址
SAFEVCRT_IMP BOOL IsStringType(LPCSTR pszString,LONG nType);


#define BOOLTOSTR(var,szTemp)		BoolToStr(var,szTemp,size_of(szTemp))
#define BYTETOSTR(var,szTemp)		ByteToStr(var,szTemp,size_of(szTemp))
#define UINTTOSTR(var,szTemp)		UintToStr(var,szTemp,size_of(szTemp))
#define ULONGTOSTR(var,szTemp)		UlongToStr(var,szTemp,size_of(szTemp))
#define USHORTTOSTR(var,szTemp)		UshortToStr(var,szTemp,size_of(szTemp))
#define CHARTOSTR(var,szTemp)		ByteToStr(var,szTemp,size_of(szTemp))
#define INTTOSTR(var,szTemp)		IntToStr(var,szTemp,size_of(szTemp))
#define LONGTOSTR(var,szTemp)		LongToStr(var,szTemp,size_of(szTemp))
#define SHORTTOSTR(var,szTemp)		ShortToStr(var,szTemp,size_of(szTemp))
#define PRICE_MIN					(0.0001f)
#define FLOAT_MIN					(0.000001f)

SAFEVCRT_IMP BOOL StringToBool(LPCSTR lpszString);															// BOOL型转字符串
SAFEVCRT_IMP LPCSTR BoolToString(BOOL bVar,LPSTR lpszTemp,LONG nSize);										// 字符串转BOOL型
SAFEVCRT_IMP COLORREF StringToColor(LPCSTR lpszString);														// COLORREF型转字符串
SAFEVCRT_IMP LPCSTR ColorToString(COLORREF crVar,LPSTR lpszTemp,LONG nSize);								// 字符串转COLORREF型
#ifdef PLATFORM_WINDOWS
	SAFEVCRT_IMP DWORD StringToHotKey(LPCSTR lpszString);													// HOTKEY型转字符串
	SAFEVCRT_IMP LPCSTR HotKeyToString(DWORD dwVar,LPSTR lpszTemp,LONG nSize);								// 字符串转HOTKEY型
#endif
SAFEVCRT_IMP MD5BIN StringToMd5(LPCSTR lpszString);															// MD5型转字符串
SAFEVCRT_IMP LPCSTR Md5ToString(MD5BIN md5Var,LPSTR lpszTemp,LONG nSize);									// 字符串转MD5型
SAFEVCRT_IMP RECT StringToRect(LPCSTR lpszString);															// RECT型转字符串
SAFEVCRT_IMP LPCSTR RectToString(RECT rcVar,LPSTR lpszTemp,LONG nSize);										// 字符串转RECT型
SAFEVCRT_IMP POINT StringToPoint(LPCSTR lpszString);														// POINT型转字符串
SAFEVCRT_IMP LPCSTR PointToString(POINT ptVar,LPSTR lpszTemp,LONG nSize);									// 字符串转POINT型
SAFEVCRT_IMP SIZE StringToSize(LPCSTR lpszString);															// SIZE型转字符串
SAFEVCRT_IMP LPCSTR SizeToString(SIZE szVar,LPSTR lpszTemp,LONG nSize);										// 字符串转SIZE型

const RECT rectNull={0,0,0,0};
const SIZE sizeNull={0,0};
const POINT pointNull={0,0};



//////////////////////////////////////////////////////////////////////////
// 字符串分析处理,将单字分割的字符串转换为数组
// (注意:数据转换失败将破坏原有缓冲区)
SAFEVCRT_IMP LONG ParseStrFields(LPSTR lpszString,CHAR cSplitter,BOOL bTransBracket,LPSTR* lppFields,LONG nMaxField);
// 查找字段
SAFEVCRT_IMP LONG FindStrFields(LPCSTR lpszField,LPSTR* lppFields,LONG nFieldNum);
// 文本串分析处理(分解文本串,例如多行文本的分解出头最少几行,如果超过指定行,其他行何必作为最后一段文本)
SAFEVCRT_IMP LONG ParseTextFields(LPSTR lpszText,LPCSTR lpszSplitter,LONG nWantField,LPSTR* lppFields);
// 字符串分析处理,将字符串分割的字符串转换为数组
SAFEVCRT_IMP LONG ParseStrFieldsEx(LPSTR lpszString,LPCSTR lpszSplitter,BOOL bTransBracket,LPSTR* lppFields,LONG nMaxCount);
SAFEVCRT_IMP LONG ParseNStrFieldsEx(LPSTR lpszString,LPCSTR lpszSplitter,BOOL bTransBracket,BOOL bParseAll,LPSTR* lppFields,LONG nMaxCount);
// 分解A:\r\nB\r\nA:\r\nB\r\n格式的字符串为数组
SAFEVCRT_IMP LONG ParseLabelFields(LPSTR lpszString,LONG nMaxCount,LPSTR* lppLabels,LPSTR* lpContent);
// 分解A:B\r\nA:B\r\n格式的字符串为数组
SAFEVCRT_IMP LONG ParseLabelFieldsEx(LPSTR lpszString,LONG nMaxCount,LPSTR* lppLabels,LPSTR* lpContent,LPCSTR pszLineSpliter="\r\n",LPCSTR pszFieldSplitter=":");
SAFEVCRT_IMP LONG ParseNLabelFieldsEx(LPSTR lpszString,LONG nMaxCount,BOOL bParseAll,LPSTR* lppLabels,LPSTR* lpContent,LPCSTR pszLineSpliter="\r\n",LPCSTR pszFieldSplitter=":");
// 从字符串中提取字符串(序号从1开始)
SAFEVCRT_IMP LONG GetSpliterCount(LPCSTR lpszSrc,CHAR cSpliter);
SAFEVCRT_IMP LONG GetSpliterCount(LPCSTR lpszSrc,LPCSTR lpszSpliter);
SAFEVCRT_IMP LPCSTR GetStr(LPCSTR lpszSrc,LPSTR lpszDest,LONG nSize,LONG nIndex,CHAR cSpliter);
SAFEVCRT_IMP LPCSTR GetStrEx(LPCSTR lpszSrc,LPSTR lpszDest,LONG nSize,LONG nIndex,LPCSTR lpszSpliter);
SAFEVCRT_IMP LPCSTR GetKeyStr(LPCSTR lpszSrc,LPSTR lpszDest,LONG nSize,LPCTSTR pszKey,CHAR cSpliter,CHAR cSpliter2='=');
SAFEVCRT_IMP LPCSTR GetKeyStrEx(LPCSTR lpszSrc,LPSTR lpszDest,LONG nSize,LPCTSTR pszKey,LPCSTR lpszSpliter,CHAR cSpliter2='=');






//////////////////////////////////////////////////////////////////////////
// 编码和转换
// 进行BASE64编码和解码
// 用于BASE64编码,解码的常量,编码缓冲区长度和原始长度得关系为
// 编码后长度=(编码前长度*8+5)/6
SAFEVCRT_IMP LONG Base64DecodeString(LPCSTR lpszDecoding,LPBYTE lpszDecoded,LONG nDecodedSize);
SAFEVCRT_IMP LONG Base64DecodeBinary(LPCSTR lpszDecoding,LPBYTE lpszDecoded,LONG nDecodedSize);
SAFEVCRT_IMP LONG Base64Encode(LPBYTE lpszEncoding,LONG nInputSize,LPSTR lpszEncoded,LONG nEncodedSize);
#define Base64Decode Base64DecodeString

// 进行URL编码和解码
// 用于URL编码,解码的常量,编码缓冲区长度和原始长度得关系为
// 进行URL编码,输入的缓冲区长度包括'\0',返回字符串长度(不包括'\0')
SAFEVCRT_IMP LONG UrlEncode(LPCSTR bufin,LPSTR bufcoded,LONG outbufmax);
SAFEVCRT_IMP LPCSTR UrlDecode(LPSTR bufinout);
SAFEVCRT_IMP LONG UrlDecode(LPCSTR bufcoded,LPSTR bufplain,LONG outbufsize);
SAFEVCRT_IMP LPCSTR JsonDecode(LPSTR pszCoded);

// 常用代码页
#define CP_GB18030	54936
#define CP_GB2312	936
#define CP_BIG5		950
#define CP_ENU		1252
#define CP_UTF16LE	1200
#define CP_UTF16BE	1201
// 繁简转换
SAFEVCRT_IMP LONG GB2312ToBIG5(LPCSTR pszGb2312,LONG nGb2312Len,LPSTR pszBig5,LONG cbBig5);
SAFEVCRT_IMP LONG BIG5ToGB2312(LPCSTR pszBig5,LONG nBig5Len,LPSTR pszGb2312,LONG cbGb2312);
SAFEVCRT_IMP VOID ConvertCodePage(LPTSTR pszBuf,LONG cbBuf,UINT nDataCodePage,UINT nUserCodePage,BOOL bToUser);
// MBCS和UTF8转换
SAFEVCRT_IMP LONG UTF8ToMBCS(LPCSTR pszUtf8,UINT nCodePage,LPSTR pszMbcs,LONG cbMbcs);
SAFEVCRT_IMP LONG MBCSToUTF8(LPCSTR pszMbcs,UINT nCodePage,LPSTR pszUtf8,LONG cbUtf8);
SAFEVCRT_IMP LONG MBCSTransCodePage(LPCSTR pszSrc,UINT nSrcCodePage,LPSTR pszDest,LONG cbDest,UINT nDestCodePage);










//////////////////////////////////////////////////////////////////////////
// 时间日期以及转换函数
#pragma pack(1)
	struct bc_date
	{	unsigned short da_year; // 年(YYYY)
		char  da_mon;			// 月(MM)
		char  da_day;			// 日(DD)
	};
	struct bc_time
	{	char  ti_hour;			// 时(HH)
		char  ti_min;			// 分(MM)
		char  ti_sec;			// 秒(SS)
	};
#pragma pack()
SAFEVCRT_IMP UINT64 GetMicroseconds();																	// 获取程序启动以来的微秒数
SAFEVCRT_IMP DWORD GetMillSeconds();																	// 获取系统启动以来的毫秒数
SAFEVCRT_IMP struct tm get_localtime(const time_t* ptTime);												// 取GMT对应的本地时间
SAFEVCRT_IMP LONG get_ydays(LONG nYear);																// 取得某年的天数
SAFEVCRT_IMP LONG get_mdays(LONG nYear,LONG nMonth);													// 取得某年某月的天数
SAFEVCRT_IMP LONG get_nextdays(LONG nDate,LONG nSpanDays);												// 取得某日期后N天得日期
SAFEVCRT_IMP LONG get_prevdays(LONG nDate,LONG nSpanDays);												// 取得某日期前N天得日期
SAFEVCRT_IMP LONG get_spandays(LONG nDate1,LONG nDate2);												// 获取两个日期之间的间隔
SAFEVCRT_IMP VOID gettime(struct bc_time* timep);														// 得到当前时间
SAFEVCRT_IMP LONG getltime();																			// 得到当前时间
SAFEVCRT_IMP VOID getdate(struct bc_date* datep);														// 得到当前日期
SAFEVCRT_IMP LONG getldate();																			// 得到当前日期
SAFEVCRT_IMP LONG time2ldate(time_t tTime);																// 时间转换为日期
SAFEVCRT_IMP BOOL IsLongTimeInRange(DWORD dwTime,DWORD dwTime1,DWORD dwTime2);							// 判断时间是否在范围内(支持隔夜,不包含前一个时间点)
SAFEVCRT_IMP time_t MakeTime(LONG nYear,LONG nMon,LONG nDay,LONG nHour,LONG nMin,LONG nSec);			// 转换为时间
SAFEVCRT_IMP time_t MakeTime(LONG lDate,LONG lTime);													// 转换为时间
SAFEVCRT_IMP time_t TimeFromInetString(LPCSTR lpszTime);												// 字符串转换到时间(Fri, 02 Jun 2006 08:01:17格式)
SAFEVCRT_IMP time_t TimeFromString(LPCSTR lpszTime);													// 字符串转换到时间(YYYY-MM-DD HH:MM:SS格式)
SAFEVCRT_IMP LPCSTR TimeToString(time_t tTime,LPSTR lpszTime,LONG nSize);								// 时间转换到字符串(YYYY-MM-DD HH:MM:SS格式)
SAFEVCRT_IMP LPCSTR TimeToShortString(time_t tTime,LPSTR lpszTime,LONG nSize);							// 时间转换到字符串(HH:MM:SS格式)
SAFEVCRT_IMP DWORD LongTimeFromString(LPCSTR lpszTime);													// 字符串转换到时间(HH:MM:SS格式)
SAFEVCRT_IMP LPCSTR LongTimeToString(DWORD dwTime,LPSTR lpszTime,LONG nSize);							// 时间转换到字符串(HH:MM:SS格式)
SAFEVCRT_IMP DWORD LongDateFromString(LPCSTR lpszDate);													// 字符串转换到日期(YYYY-MM-DD格式)
SAFEVCRT_IMP LPCSTR LongDateToString(DWORD dwDate,LPSTR lpszDate,LONG nSize);							// 日期转换到字符串(YYYY-MM-DD格式)
SAFEVCRT_IMP systm_t systm();																			// 取得系统时间(毫秒单位)
SAFEVCRT_IMP LPCSTR SysTmToShortString(systm_t tTime,LPSTR lpszTime,LONG nSize);						// 时间转换到字符串(HH:MM:SS.mmm格式)
SAFEVCRT_IMP LPCSTR SysTmToLongString(systm_t tTime,LPSTR lpszTime,LONG nSize);							// 时间转换到字符串(YYMMDD-HH:MM:SS.mmm格式)

#define time2systm(t) ((systm_t)t*1000)


//////////////////////////////////////////////////////////////////////////
// 版本处理
SAFEVCRT_IMP VOID BuildDate2Long(LPCSTR pszBuildDate,LONG* pnBuildDate);
SAFEVCRT_IMP VOID BuildTime2Long(LPCSTR pszBuildTime,LONG* pnBuildTime);
SAFEVCRT_IMP LONG GetBuildNo(LPCSTR pszBuildDate,LPCSTR pszBuildTime);
SAFEVCRT_IMP LPCSTR GetBuildNoStr(LPCSTR pszBuildDate,LPCSTR pszBuildTime,LPSTR pszTemp,DWORD cbTemp);

#ifdef USE_VERSION_EX
	// 替换可执行文件的图标(可能导致目标程序出现GroupError)
	SAFEVCRT_IMP BOOL ReplaceIcon(LPCTSTR lpszPEFile,LPCTSTR lpszIconFile);
	// 读取PE版本信息和语言信息
	SAFEVCRT_IMP BOOL PEGetVersionInfoAndTranslation(LPCSTR pszPEFile,VS_FIXEDFILEINFO* pVersionInfo,WORD* pwLanguage,WORD* pwCodePage);
	// 更新PE版本信息
	SAFEVCRT_IMP BOOL PESetVersionInfo(LPCSTR pszPEFile,VS_FIXEDFILEINFO* pVersionInfo);
	SAFEVCRT_IMP BOOL PESetVersion(LPCSTR pszPEFile,LPCSTR pszVersion);
	SAFEVCRT_IMP BOOL PESetVersionIncrease(LPCSTR pszPEFile);
#endif






//========================================================================================================================
// 排序处理
SAFEVCRT_IMP VOID SortValue(DWORD* pdwValues,const DWORD* pdwKeys,DWORD dwCount,BOOL bIncrease);
SAFEVCRT_IMP VOID SortValue(DWORD* pdwValues,const INT64* pi64Keys,DWORD dwCount,BOOL bIncrease);







//========================================================================================================================
// 地址解析函数
// 解析URL或者站点名,例如ftp://abc@abc:kaka.com.cn:23/index.html#link1
// 解析内容包括:协议名(小写),服务器名,端口号,目标URL,目标节,是否使用用户校验,用户名,密码
// 不能解析本地地址和共享地址
// lpszProtocol		MAX_INET_SCHEME_LEN
// lpszHost			MAX_INET_HOSTNAME_LEN
// lpszFolder		MAX_INET_PATH_LEN
// lpszObject		MAX_INET_PATH_LEN
// lpszObjectName	MAX_INET_PATH_LEN
// lpszParagraph	MAX_INET_PATH_LEN
// lpszUsername		MAX_INET_USERNAME_LEN
// lpszPassword		MAX_INET_PASSWORD_LEN
SAFEVCRT_IMP LONG ParseURLEx(LPCSTR lpszSiteOrURL,LPSTR lpszProtocol,LPSTR lpszHost,USHORT& usPort,LPSTR lpszFolder,LPSTR lpszObject,
			LPSTR lpszObjectName,LPSTR lpszParagraph,BOOL& bAuthorization,LPSTR lpszUsername,LPSTR lpszPassword);
SAFEVCRT_IMP LONG ParseProtocol(LPCSTR lpszSiteOrURL,LPSTR lpszProtocol);
SAFEVCRT_IMP BOOL IsUrl(LPCSTR lpszSiteOrURL);






//========================================================================================================================
// 地址转换和操作重载

// 地址转换函数
SAFEVCRT_IMP IPV4 SockAddrToIPv4(SOCKADDR_IN* psa);													// SOCKADDR转换IPv4地址
SAFEVCRT_IMP WORD SockAddrToPortIPv4(SOCKADDR_IN* psa);												// SOCKADDR转换IPv4端口
SAFEVCRT_IMP IPV4 BytesToIPv4(LPBYTE pBytes);														// BYTES转换IPv4地址
SAFEVCRT_IMP IPV4 StringToIPv4(LPCSTR lpszIpv4);													// 字符串转IPv4地址
SAFEVCRT_IMP VOID StringToSubNetIPv4(LPCSTR lpszIpv4,IPV4& Ip,IPV4& Mask);							// 字符串转IPv4(子网)地址
SAFEVCRT_IMP VOID IPv4ToSockAddr(const IPV4& Ip,USHORT nPort,SOCKADDR_IN* psa,DWORD& cbsa);			// IPv4地址转换SOCKADDR
SAFEVCRT_IMP VOID IPv4ToBytes(const IPV4& Ip,LPBYTE pBytes);										// IPv4地址转换BYTES
SAFEVCRT_IMP LPCSTR IPv4ToString(const IPV4& Ip,LPSTR pszTemp,LONG nSize);							// IPv4地址转字符串
SAFEVCRT_IMP LPCSTR IPv4ToCommaString(const IPV4& Ip,LPSTR pszTemp,LONG nSize);						// IPv4地址转字符串(逗号分割)
SAFEVCRT_IMP LPCSTR SubNetIPv4ToString(const IPV4& Ip,const IPV4& Mask,LPSTR pszTemp,LONG nSize);	// IPv4(子网)地址转字符串
SAFEVCRT_IMP IPV4 MaskIPv4(const IPV4& Ip,const IPV4& IpMask);										// IPv4求掩码
SAFEVCRT_IMP IPV4 AddIPv4(const IPV4& Ip1,const IPV4& Ip2,LONG& nSignFlag);							// IPv4求和
SAFEVCRT_IMP IPV4 SubIPv4(const IPV4& Ip1,const IPV4& Ip2,LONG& nSignFlag);							// IPv4求差
SAFEVCRT_IMP IPV4 SpanToIPv4(const IPV4& Ip,const LONG nSpan);										// IPv4增减
SAFEVCRT_IMP LONG SpanBetweenIPv4(const IPV4& Ip1,const IPV4& Ip2);									// IPv4求和差
SAFEVCRT_IMP int CompareIPv4(const IPV4& Ip1,const IPV4& Ip2);										// IPv4比较
SAFEVCRT_IMP BOOL IsLoopbackIPv4(const IPV4& Ip);													// IPv4回环地址判定
SAFEVCRT_IMP BOOL IsBoardcastIPv4(const IPV4& Ip);													// IPv4广播地址判定
SAFEVCRT_IMP BOOL IsEthernetIPv4(const IPV4& Ip);													// IPv4局域网地址判定
SAFEVCRT_IMP BOOL IsInternetIPv4(const IPV4& Ip);													// IPv4公网地址判定
SAFEVCRT_IMP CLIB_INLINE IPV4 operator+(const IPV4& Ip,const LONG nSpan) { return SpanToIPv4(Ip,nSpan); }
SAFEVCRT_IMP CLIB_INLINE LONG operator-(const IPV4& Ip1,const IPV4& Ip2) { return SpanBetweenIPv4(Ip1,Ip2); }
SAFEVCRT_IMP CLIB_INLINE const bool operator<(const IPV4& Ip1,const IPV4& Ip2) { return CompareIPv4(Ip1,Ip2)<0; }
SAFEVCRT_IMP CLIB_INLINE const bool operator>(const IPV4& Ip1,const IPV4& Ip2) { return CompareIPv4(Ip1,Ip2)>0; }
SAFEVCRT_IMP CLIB_INLINE bool operator==(const IPV4& Ip1,const IPV4& Ip2) { return CompareIPv4(Ip1,Ip2)==0; }
SAFEVCRT_IMP CLIB_INLINE bool operator!=(const IPV4& Ip1,const IPV4& Ip2) { return CompareIPv4(Ip1,Ip2)!=0; }


SAFEVCRT_IMP IPV6 SockAddrToIPv6(SOCKADDR_IN6* psa);												// SOCKADDR转换IPv6地址
SAFEVCRT_IMP WORD SockAddrToPortIPv6(SOCKADDR_IN6* psa);											// SOCKADDR转换IPv6端口
SAFEVCRT_IMP IPV6 BytesToIPv6(LPBYTE pBytes);														// BYTES转换IPv6地址
SAFEVCRT_IMP IPV6 StringToIPv6(LPCSTR lpszIpv6);													// 字符串转IPv6地址
SAFEVCRT_IMP VOID StringToSubNetIPv6(LPCSTR lpszIpv6,IPV6& Ip,IPV6& Mask);							// 字符串转IPv6(子网)地址
SAFEVCRT_IMP VOID IPv6ToSockAddr(const IPV6& Ip,USHORT nPort,SOCKADDR_IN6* psa,DWORD& cbsa);		// IPv6地址转换SOCKADDR
SAFEVCRT_IMP VOID IPv6ToBytes(const IPV6& Ip,LPBYTE pBytes);										// IPv6地址转换BYTES
SAFEVCRT_IMP LPCSTR IPv6ToString(const IPV6& Ip,LPSTR pszTemp,LONG nSize);							// IPv6地址转字符串
SAFEVCRT_IMP LPCSTR IPv6ToCommaString(const IPV6& Ip,LPSTR pszTemp,LONG nSize);						// IPv6地址转字符串(逗号分割)
SAFEVCRT_IMP LPCSTR SubNetIPv6ToString(const IPV6& Ip,const IPV6& Mask,LPSTR pszTemp,LONG nSize);	// IPv6(子网)地址转字符串
SAFEVCRT_IMP IPV6 MaskIPv6(const IPV6& Ip,const IPV6& IpMask);										// IPv6求掩码
SAFEVCRT_IMP IPV6 AddIPv6(const IPV6& Ip1,const IPV6& Ip2,LONG& nSignFlag);							// IPv6求和
SAFEVCRT_IMP IPV6 SubIPv6(const IPV6& Ip1,const IPV6& Ip2,LONG& nSignFlag);							// IPv6求差
SAFEVCRT_IMP IPV6 SpanToIPv6(const IPV6& Ip,const LONG nSpan);										// IPv6增减
SAFEVCRT_IMP LONG SpanBetweenIPv6(const IPV6& Ip1,const IPV6& Ip2);									// IPv6求和差
SAFEVCRT_IMP int CompareIPv6(const IPV6& Ip1,const IPV6& Ip2);										// IPv6比较
SAFEVCRT_IMP BOOL IsLoopbackIPv6(const IPV6& Ip);													// IPv6回环地址判定
SAFEVCRT_IMP BOOL IsBoardcastIPv6(const IPV6& Ip);													// IPv6广播地址判定
SAFEVCRT_IMP BOOL IsEthernetIPv6(const IPV6& Ip);													// IPv6局域网地址判定
SAFEVCRT_IMP BOOL IsInternetIPv6(const IPV6& Ip);													// IPv6公网地址判定
SAFEVCRT_IMP CLIB_INLINE IPV6 operator+(const IPV6& Ip,const LONG nSpan) { return SpanToIPv6(Ip,nSpan); }
SAFEVCRT_IMP CLIB_INLINE LONG operator-(const IPV6& Ip1,const IPV6& Ip2) { return SpanBetweenIPv6(Ip1,Ip2); }
SAFEVCRT_IMP CLIB_INLINE const bool operator<(const IPV6& Ip1,const IPV6& Ip2) { return CompareIPv6(Ip1,Ip2)<0; }
SAFEVCRT_IMP CLIB_INLINE const bool operator>(const IPV6& Ip1,const IPV6& Ip2) { return CompareIPv6(Ip1,Ip2)>0; }
SAFEVCRT_IMP CLIB_INLINE bool operator==(const IPV6& Ip1,const IPV6& Ip2) { return CompareIPv6(Ip1,Ip2)==0; }
SAFEVCRT_IMP CLIB_INLINE bool operator!=(const IPV6& Ip1,const IPV6& Ip2) { return CompareIPv6(Ip1,Ip2)!=0; }

SAFEVCRT_IMP MAC StringToMac(LPCSTR lpszMac);														// 字符串转MAC地址
SAFEVCRT_IMP LPCSTR MacToString(MAC Mac,LPSTR pszTemp,LONG nSize);									// MAC地址转字符串
SAFEVCRT_IMP inline bool operator==(const MAC& Mac1,const MAC& Mac2) { return memcmp(&Mac1,&Mac2,size_of(Mac1))==0; }
SAFEVCRT_IMP inline bool operator!=(const MAC& Mac1,const MAC& Mac2) { return memcmp(&Mac1,&Mac2,size_of(Mac1))!=0; }


// 地址判定和地址之间相互转换
SAFEVCRT_IMP BOOL IsIPv4(const IPV6& Ipv6);															// IPv4地址判定
SAFEVCRT_IMP IPV6 IPConvert4To6(const IPV4& Ipv4);													// IP地址转换(IPV4->IPV6)
SAFEVCRT_IMP IPV4 IPConvert6To4(const IPV6& Ipv6);													// IP地址转换(IPV6->IPV4)
SAFEVCRT_IMP MAC IPConvertFakeMac(const IPV4& Ipv4,BYTE cKind);										// IP地址转换(IPV4->MAC)
SAFEVCRT_IMP MAC IPConvertRandMac();																// IP地址转换(IPV4->MAC)

// 地址类型的转换HASH函数
SAFEVCRT_IMP CLIB_INLINE BOOL CLIBAPI CollCompareElements(const UNALIGNED IPV4* pElement1,const UNALIGNED IPV4* pElement2) { return *pElement1==*pElement2; }
SAFEVCRT_IMP CLIB_INLINE UINT CLIBAPI GenHashKey(IPV4 Ip,UINT nKeyOffset) { UNREFERENCED_PARAMETER(nKeyOffset); return Ip.m_acIpData[size_of(Ip.m_acIpData)-2]*256+Ip.m_acIpData[size_of(Ip.m_acIpData)-1]; }
SAFEVCRT_IMP CLIB_INLINE BOOL CLIBAPI CollCompareElements(const UNALIGNED IPV6* pElement1,const UNALIGNED IPV6* pElement2) { return *pElement1==*pElement2; }
SAFEVCRT_IMP CLIB_INLINE UINT CLIBAPI GenHashKey(IPV6 Ip,UINT nKeyOffset) { UNREFERENCED_PARAMETER(nKeyOffset); return Ip.m_acIpData[size_of(Ip.m_acIpData)-2]*256+Ip.m_acIpData[size_of(Ip.m_acIpData)-1]; }
SAFEVCRT_IMP CLIB_INLINE BOOL CLIBAPI CollCompareElements(const UNALIGNED MAC* pElement1,const UNALIGNED MAC* pElement2) { return *pElement1==*pElement2; }
SAFEVCRT_IMP CLIB_INLINE UINT CLIBAPI GenHashKey(MAC Mac,UINT nKeyOffset) { UNREFERENCED_PARAMETER(nKeyOffset); return (UINT)(Mac.m_acMacData[1]*256+Mac.m_acMacData[0]); }



#ifndef NO_EXTEND_CRT

	// 完整模式
	#if defined(SAFEVCRT_FULLMODE)

		// MD5处理
		#define LITEMD5_IMP SAFEVCRT_IMP						// MD5处理
		#ifndef SAFEVCRT_LIB
			#include "litemd5/md5.h"							// MD5处理
		#endif

		// 基础数据结构处理
		#define COLLECTION_IMP SAFEVCRT_IMP						// 基础数据结构处理
		#ifndef SAFEVCRT_LIB
			#include "collection/clibstr.h"						// 字符串类
			#include "collection/coll.h"						// 基本数据结构
			#include "collection/clibplex.h"					// 数据块链
			#include "collection/clibfixedalloc.h"				// 简单的内存池管理器
			#include "collection/sampling.h"					// 采样器
			#include "collection/variantdata.h"					// 复合结构
		#endif

		// XML文件处理
		#define LITEXML_IMP SAFEVCRT_IMP						// XML文件处理
		#ifndef SAFEVCRT_LIB
			#include "litexml/xml.h"							// XML类
			#include "litexml/xmlprofile.h"						// XML属性表类
		#endif

		// 基础数据结构处理(模版类)
		#define COLLECTION_IMP SAFEVCRT_IMP						// 基础数据结构处理
		#ifndef SAFEVCRT_LIB
			#include "collection/clibtempl.h"					// 基本数据结构(模板类)
		#endif

		// 加密处理
		#define SECURITY_IMP SAFEVCRT_IMP						// 加密处理
		#ifndef SAFEVCRT_LIB
			#include "security/blowfish.h"						// Blowfish加密算法
			#include "security/des3.h"							// DES3加密算法
			#include "security/des3ex.h"						// DES3加密算法(PKCS标准)
			#include "security/talgorefer.h"					// 算法引用
			#include "security/talgobase.h"						// 算法基类
			#include "security/thashbase.h"						// HASH算法基类
			#include "security/thashrender.h"					// HASH算法引用类
			#include "security/tsessionbase.h"					// SESSIN算法基类
			#include "security/tsessionrender.h"				// SESSIN算法引用类
			#include "security/tpki.h"							// PKI算法类
			#include "security/rsakeyqueue.h"					// RSA队列
		#endif

		// 文件IO处理
		#define LITEFILEIO_IMP SAFEVCRT_IMP						// 文件IO处理
		#ifndef SAFEVCRT_LIB
			#include "litefileio/cachedfile.h"					// 具有缓存特性文件处理
			#include "litefileio/anyfiles.h"					// 跨平台文件类
			#include "litefileio/imagefile.h"					// 镜像文件处理类(支持IMG文件)
			#include "litefileio/miniarchive.h"					// 序列化支持
			#include "litefileio/vfs.h"							// 虚拟文件系统支持
		#endif

		// INI处理
		#define LITEINI_IMP SAFEVCRT_IMP						// INI文件处理
		#ifndef SAFEVCRT_LIB
			#include "liteini/iniconf.h"						// INI文件处理
		#endif

		// DBF处理
		#define LITEDBF_IMP SAFEVCRT_IMP						// DBF文件处理
		#ifndef SAFEVCRT_LIB
			#include "litedbf/dbf.h"							// DBF文件处理
		#endif

		// GDI处理
		#define LITEGDI_IMP SAFEVCRT_IMP						// GDI处理
		#ifndef SAFEVCRT_LIB
			#include "litegdi/gdiplus.h"						// GDI处理
		#endif

		// 压缩处理
		#define COMPRESS_IMP SAFEVCRT_IMP						// 压缩处理
		#ifndef SAFEVCRT_LIB
			#if defined(PLATFORM_WINDOWS)
				#include "compress/zlib.h"						// 压缩处理
			#endif
			#include "compress/zmore.h"							// 压缩处理
			#include "compress/zfile.h"							// 压缩处理
			#include "compress/unzipfile.h"						// 压缩处理
		#endif

		// 多语言处理
		#define MLANG_IMP SAFEVCRT_IMP							// 多语言处理
		#ifndef SAFEVCRT_LIB
			#include "mlang/mlang.h"							// 多语言处理
		#endif

		// 杂项库
		#define CLIBMISC_IMP SAFEVCRT_IMP						// 杂项处理
		#ifndef SAFEVCRT_LIB
			#include "clibmisc/mt.h"							// 互斥对象
			#include "clibmisc/misc.h"							// 杂项函数
			#include "clibmisc/memory.h"						// 内存处理
			#include "clibmisc/staticinit.h"					// 静态初始化
			#include "clibmisc/escapencode.h"					// ESCA编码
			#include "clibmisc/uuencode.h"						// BASE64编码
			#include "clibmisc/urlencode.h"						// URL编码
			#include "clibmisc/perfcheck.h"						// 性能检测模块
			#include "clibmisc/wrscore.h"						// 均衡模块
			#include "clibmisc/rsmanager.h"						// 资源管理
			#include "clibmisc/memtable.h"						// 内存表
			#include "clibmisc/safetyvalve.h"					// 安全阀
			#include "clibmisc/niostream.h"						// 流处理
		#endif

	// 最小模式
	#else
		// MD5处理
		#define LITEMD5_IMP SAFEVCRT_IMP						// MD5处理
		#ifndef SAFEVCRT_LIB
			#include "litemd5/md5.h"							// MD5处理
		#endif

		// 基础数据结构处理
		#define COLLECTION_IMP SAFEVCRT_IMP						// 基础数据结构处理
		#ifndef SAFEVCRT_LIB
			#include "collection/clibstr.h"						// 字符串类
			#include "collection/coll.h"						// 基本数据结构
			#include "collection/clibplex.h"					// 数据块链
			#include "collection/clibfixedalloc.h"				// 简单的内存池管理器
		#endif

		// XML文件处理
		#define LITEXML_IMP SAFEVCRT_IMP						// XML文件处理
		#ifndef SAFEVCRT_LIB
			#include "litexml/xml.h"							// XML类
			#include "litexml/xmlprofile.h"						// XML属性表类
		#endif

		// 基础数据结构处理(模版类)
		#define COLLECTION_IMP SAFEVCRT_IMP						// 基础数据结构处理
		#ifndef SAFEVCRT_LIB
			#include "collection/clibtempl.h"					// 基本数据结构(模板类)
		#endif

		#define SECURITY_IMP SAFEVCRT_IMP						// 加密处理
		#ifndef SAFEVCRT_LIB
			#include "security/blowfish.h"						// Blowfish加密算法
			#include "security/des3.h"							// Blowfish加密算法
			#include "security/tpki.h"							// PKI算法类
		#endif
		
		// GDI处理
		#define LITEGDI_IMP SAFEVCRT_IMP						// GDI处理
		#ifndef SAFEVCRT_LIB
			#include "litegdi/gdiplus.h"						// GDI处理
		#endif

		// 多语言处理
		#define MLANG_IMP SAFEVCRT_IMP							// 多语言处理
		#ifndef SAFEVCRT_LIB
			#include "mlang/mlang.h"							// 多语言处理
		#endif

		// 杂项库
		#define CLIBMISC_IMP SAFEVCRT_IMP						// 杂项处理
		#ifndef SAFEVCRT_LIB
			#include "clibmisc/misc.h"							// 杂项函数
		#endif
	#endif // defined(SAFEVCRT_FULLMODE)
#endif


#ifndef SAFEVCRT_NOAUTOLINK
	#if defined(PLATFORM_WINDOWS) && !defined(SAFEVCRT_LIB)
		#pragma once
		#pragma comment(lib,"ws2_32.lib")

		#if defined(_ARCH_AMD64_)
			// 动态模式(仅支持完整模式)
			#if defined(SAFEVCRT_DYNAMIC)
				#ifdef _DEBUG
					#pragma message("Automatically linking with SafeVCRTx64d.dll....")
					#pragma comment(lib,"SafeVcrtx64d.lib")
				#else
					#pragma message("Automatically linking with SafeVCRTx64.dll....")
					#pragma comment(lib,"SafeVcrtx64.lib")
				#endif
			// 完整模式(静态)
			#elif defined(SAFEVCRT_FULLMODE)
				#ifdef _DEBUG
					#pragma message("Automatically linking with SafeVCRTSx64d.lib....")
					#pragma comment(lib,"SafeVcrtSx64d.lib")
				#else
					#pragma message("Automatically linking with SafeVCRTSx64.lib....")
					#pragma comment(lib,"SafeVcrtSx64.lib")
				#endif
			// 精简模式(静态)
			#elif defined(SAFEVCRT_MINIMODE)
				#ifdef _DEBUG
					#pragma message("Automatically linking with SafeVCRTMx64d.lib....")
					#pragma comment(lib,"SafeVcrtMx64d.lib")
				#else
					#pragma message("Automatically linking with SafeVCRTMx64.lib....")
					#pragma comment(lib,"SafeVcrtMx64.lib")
				#endif
			#else
				#error SafeVCRT.h mode unknown
			#endif
		#else
			// 动态模式(仅支持完整模式)
			#if defined(SAFEVCRT_DYNAMIC)
				#ifdef _DEBUG
					#pragma message("Automatically linking with SafeVCRTd.dll....")
					#pragma comment(lib,"SafeVcrtd.lib")
				#else
					#pragma message("Automatically linking with SafeVCRT.dll....")
					#pragma comment(lib,"SafeVcrt.lib")
				#endif
			// 完整模式(静态)
			#elif defined(SAFEVCRT_FULLMODE)
				#ifdef _DEBUG
					#pragma message("Automatically linking with SafeVCRTSd.lib....")
					#pragma comment(lib,"SafeVcrtSd.lib")
				#else
					#pragma message("Automatically linking with SafeVCRTS.lib....")
					#pragma comment(lib,"SafeVcrtS.lib")
				#endif
			// 精简模式(静态)
			#elif defined(SAFEVCRT_MINIMODE)
				#ifdef _DEBUG
					#pragma message("Automatically linking with SafeVCRTMd.lib....")
					#pragma comment(lib,"SafeVcrtMd.lib")
				#else
					#pragma message("Automatically linking with SafeVCRTM.lib....")
					#pragma comment(lib,"SafeVcrtM.lib")
				#endif
			#else
				#error SafeVCRT.h mode unknown
			#endif
		#endif
	#endif
#endif

#endif
#endif
