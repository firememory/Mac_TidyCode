#ifndef __IMAGEFILE__H__
#define __IMAGEFILE__H__
#include "../safevcrt.h"
#include "miniarchive.h"

#define MAX_DOSPATH	256			// ���DOS·��

// �����ļ���ʽ
enum enumIMGFMT
{	IMGFMT_IMG,					// .IMG��ʽ
	IMGFMT_VFD,					// .VFD��ʽ(�ݲ�֧��)
	IMGFMT_DDI,					// .DDI��ʽ(�ݲ�֧��)
};


// ����λ�õĶ���
typedef struct tagCHS
{	BYTE	m_cHead;			// ��ͷ
	BYTE	m_cSector:6;		// ����
	WORD	m_wCylinder:10;		// ����
} _PACKED CHS;


//��������
enum enumDPTTYPE
{	DP_UNUSED      = 0X00,		// δʹ�÷���
    DP_FAT12       = 0X01,		// FAT12����
    DP_XENIX       = 0X02,		// XENIX����
    DP_FAT16       = 0X04,		// FAT16����
    DP_EXTEND      = 0X05,		// ��չ����
    DP_BIGDOS      = 0X06,
    DP_HPFS        = 0X07,
    DP_SPLIT       = 0X08,
    DP_FAT32       = 0X0B,
    DP_DM          = 0X50,
    DP_GB          = 0X56,
    DP_SPEED       = 0X61,
    DP_IX386       = 0X63,
    DP_NOVELL286   = 0X64,
    DP_NOVELL386   = 0X65,
    DP_PCIX        = 0X75,
    DP_CPM         = 0XDB,
    DP_BBT         = 0XFF,
};


// �����������Ľṹ,ֻ��Ӳ�̾������,���̾��񲻴��ڷ���,���������MBR
// Ҳ�����ڷ�����¼
// (Disk PartitionTable)
typedef struct tagDPT
{   BYTE	m_cBootFlag;			// ������־
    CHS     m_StartCHS;				// ������ʼ�����桢��ͷ������
    BYTE    m_cSystemID;			// ��������
    CHS     m_EndCHS;				// �������������桢��ͷ������
    DWORD   m_dwRelativeSectors;	// ���������������ָ��������ڼ�¼�÷����ķ�����������λ��֮��
    DWORD   m_dwTotalSectors;		// ������������
} _PACKED DPT;


// ���������������ṹ(MBR)
// ���̲�����
typedef struct tagMBR
{	BYTE	m_aBootCode[0x1be];		// ��������
    DPT     m_DPT[4];               // ������
    WORD	m_wEndingFlag;			// ������ʶ(55AA)
} _PACKED MBR;


// ����DBR(DOS Boot Record)�ṹ,�ýṹ����ÿ�������ĵ�һ������
// Ҳ�������̿�ʼ�ķ���,����ṹ�ͷ����������
// 16λ������ʽ��12λ������ʽ��ͬ
// DBR�а�����Ҫ��BPB(BIOS Paramenter Block-���̲�����)����Ҫ����
//���̽�������
enum enumMEDIATYPE
{	DM_HARDDISK = 0xF8,				// Ӳ��
	DM_FLOPPY   = 0xF0,				// ����
    DM_RAMDisk  = 0xFA				// RAM��
};

typedef struct tagBPBFAT
{	WORD   m_wSectorBytes;			// ÿ�����ֽ���
    BYTE   m_cSectorsPerCluster;	// ÿ��������
    WORD   m_wReservedSectors;		// ����������
    BYTE   m_cNbrFat;				// FAT�ĸ���
    WORD   m_wRootEntry;			// ��Ŀ¼����
    WORD   m_wTotalSectors;			// ������������(����С��32Mʱ)
    BYTE   m_cMedia;				// �������ʱ�ʶ
    WORD   m_wSectorsPerFAT;		// ÿ��FATռ��������
    WORD   m_wSectorsPerTrack;		// ÿ��������
    WORD   m_wHeads;				// ��ͷ��
    DWORD  m_dwHiddenSectors;       // ����������
    DWORD  m_dwBigTotalSectors;     // ������������(��������32Mʱ)
    BYTE   m_cDrive;				// ��������
    BYTE   m_cHead;					// ��ͷ��
    BYTE   m_cboot_rec_sig;			// ����δ֪
    DWORD  m_dwVolSerialNo;			// �������к�
    CHAR   m_szLabel[11];           // ���̾����
    CHAR   m_szFileSys_ID[8];       // �ļ�ϵͳ��ʶ��
} _PACKED BPBFAT;
typedef BPBFAT BPBFAT12,BPBFAT16;

#define FAT_ENDFLAG	0XAA55			// ������FAT������

typedef struct tagDBRFAT
{	BYTE	m_aJMPCode[3];			// ������ת����
    CHAR	m_szSystemID[8];		// ���̱�־�Ͱ汾��
	BPBFAT	m_BPB;					// BPB��¼
	BYTE	m_aBPBEx[26];			// ��չBPB
	BYTE	m_aBootCode[422];		// ��������
	WORD	m_wEndingFlag;			// ������ʶ(55AA)
} _PACKED DBRFAT;
typedef DBRFAT DBRFAT12,DBRFAT16;


// DBR����ӵľ���ϵͳ���ļ������,FAT1��FAT2,
// FAT��ϵͳ��¼���ȡֵ����(16����) 
// FAT12��¼���ȡֵ	FAT16��¼���ȡֵ	FAT32��¼���ȡֵ	��Ӧ�صı������ 
// 000					0000				00000000			δ����Ĵ� 
// 002~FFF				0002~FFEF			00000002~FFFFFFEF	�ѷ���Ĵ� 
// FF0~FF6				FFF0~FFF6			FFFFFFF0~FFFFFFF6	ϵͳ���� 
// FF7					FFF7				FFFFFFF7			���� 
// FF8~FFF				FFF8~FFFF			FFFFFFF8~FFFFFFFF	�ļ�������
// ϵͳ��Ҫ��ȡȫ����FAT���жϴ��̿ռ�ķ������
// FAT12,FAT16,FAT32��Ҫ�������������,ÿ����¼�ֱ���12λ,16λ,32λ
// ǰ�����ص�FAT����ϵͳ����,�ֱ��Ǳ���3�ֽ�,4�ֽ�,8�ֽ�
// �������ֽ��е����ֽ�Ϊý������
// FAT16 FAT����ĺ���
enum enumFAT16TYPE
{	FAT_UNUSED			= 0,			// ��δ����
	FAT12_RESERVED0		= 0x0FF0,		// ����
    FAT12_RESERVED6		= 0x0FF6,		// ����
    FAT12_BADCLUSTER    = 0x0FF7,		// ��ʾ����,���ô��д������𻵣�
    FAT12_FILEEND1		= 0x0FF8,		// �ļ�������
    FAT12_FILEEND8		= 0x0FFF,		// �ļ�������
	FAT12_FILEEND		= 0x0FFF,		// �ļ�������
	FAT16_RESERVED0		= 0xFFF0,		// ����
    FAT16_RESERVED6		= 0xFFF6,		// ����
    FAT16_BADCLUSTER    = 0xFFF7,		// ��ʾ����,���ô��д������𻵣�
    FAT16_FILEEND1		= 0xFFF8,		// �ļ�������
    FAT16_FILEEND8		= 0xFFFF,		// �ļ�������
	FAT16_FILEEND		= 0xFFFF,		// �ļ�������
	FAT32_RESERVED0		= 0xFFFFFFF0,	// ����
    FAT32_RESERVED6		= 0xFFFFFFF6,	// ����
    FAT32_BADCLUSTER    = 0xFFFFFFF7,	// ��ʾ����,���ô��д������𻵣�
    FAT32_FILEEND1		= 0xFFFFFFF8,	// �ļ�������
    FAT32_FILEEND8		= 0xFFFFFFFF,	// �ļ�������
	FAT32_FILEEND		= 0xFFFFFFFF,	// �ļ�������
};


// FAT������ŵ��Ǹ�Ŀ¼��,����С��BPBFAT����(ע��FAT12��FAT16������ͬ��BPB�ṹ)
// �����Ŀ¼���ṹ��Ԫ


// Ŀ¼���е�ʱ�䶨��
typedef struct tagTIMEFAT
{	WORD	m_wSec:5;		// ��
    WORD	m_wMin:6;		// ��
    WORD	m_wHour:5;		// ʱ
} _PACKED TIMEFAT;


// Ŀ¼���е����ڶ���
typedef struct tagDATEFAT
{	WORD    m_wDay:5;		// ��
    WORD    m_wMon:4;		// ��
    WORD    m_wYear:7;		// ��
} _PACKED DATEFAT;


// Ŀ¼�����ֽں���
enum enumFDTLEADCHAR
{	FDT_UNUSED           = 0x00,	// ������û��ʹ��
    FDT_ISSUBDIR		 = 0x2e,	// ��һ����Ŀ¼ .,..Ϊ��Ŀ¼
    FDT_FILEISDELETED    = 0xe5,	// �ļ���ɾ��
};


// Ŀ¼����ĺ���
typedef struct tagFDTFAT
{   BYTE		m_szFileName[8];		// ���ļ���
    BYTE        m_szExtName[3];			// ��չ�ļ���
    BYTE        m_bReadOnly:1;			// �ļ�����:ֻ��λ
    BYTE        m_bHidden:1;			// �ļ�����:����
    BYTE        m_bSystem:1;			// �ļ�����:ϵͳ
    BYTE        m_bVolume:1;			// �ļ�����:����
	BYTE        m_bDirectory:1;			// �ļ�����:Ŀ¼
    BYTE        m_bArchives:1;			// �ļ�����:����
	BYTE        m_cReserived1:2;		// ����
	BYTE        m_cReserived2;			// ����
	BYTE		m_cCreateMilliSec;		// �ļ�����ʱ��ĺ���λ(FAT32)
	TIMEFAT		m_tCreateTime;			// �ļ�����ʱ��(FAT32)
    DATEFAT		m_tCreateDate;			// �ļ���������(FAT32)
	DATEFAT		m_tAccessDate;			// �ļ�����������(FAT32)
	WORD        m_wStartClusterHigh;	// �ļ���ʼ�ظ�λ(FAT32)
    TIMEFAT		m_tModifyTime;			// �ļ�����޸�ʱ��
    DATEFAT		m_tModifyDate;			// �ļ�����޸�����
    WORD        m_wStartClusterLow;		// �ļ���ʼ�ص�λ
    DWORD       m_dwFileLength;			// �ļ�����
} _PACKED FDTFAT;
typedef FDTFAT FDTFAT12,FDTFAT16;

// �򿪵�Ŀ¼����
typedef struct tagFDTOBJ
{	DWORD		m_dwFDTCluster;			// Ŀ¼��(���Ϊ0����λ�ڸ�Ŀ¼����)
	DWORD		m_dwFDTIndex;			// �ļ�|Ŀ¼������,��Ǵ���Ŀ¼�ص�λ��
	FDTFAT		m_FDT;					// �ļ�|Ŀ¼FDT��,����ָ������
} _PACKED FDTOBJ;

// �ļ����(��֧�ֹ���)
typedef struct tagIMGFDOBJ
{	FDTOBJ		m_FDTObj;
	DWORD		m_dwPtr;
	BOOL		m_bWritable;
} _PACKED IMGFDOBJ;
typedef IMGFDOBJ* HIMGFDOBJ;

// �����ļ����ҽ��
typedef struct tagIMGFINDDATA
{	CHAR	m_szName[MAX_DOSPATH];			// ����·��
	LONG	m_nLength;						// ���󳤶�
	BOOL	m_bDirectory;					// �����Ƿ�ΪĿ¼
	time_t	m_tLastModify;					// ��������޸�ʱ��(��ȷ����)
} _PACKED IMGFINDDATA;


// �����ļ�����
class LITEFILEIO_IMP CImageFS
{
public:
	CImageFS();
	virtual ~CImageFS();
	BOOL		__attachimage(IN CAnyFile* pAnyFile);
	int			__getfiletime(IN const char* path,OUT time_t* modifytime);
	int			__setfiletime(IN const char* path,IN time_t modifytime);
	int			__mkdir(IN const char* dirname);
	int			__rmdir(IN const char* dirname);
	int			__access(IN const char* path,IN int mode);
	int			__chmod(IN const char* filename,IN int pmode);
	int			__remove(IN const char* path);
	int			__stat(IN const char* path,OUT struct _stat* buffer);
	int			__unlink(IN const char* filename);
	int			__chsize(IN int handle,IN long size);
	long		__filelength(IN int handle);
	int			__fstat(IN int handle,OUT struct _stat* buffer);
	HIMGFDOBJ	__fopen(IN const char* filename,const char* mode);
	int			__chsize(IN HIMGFDOBJ hImgFDObj,IN long size);
	long		__filelength(IN HIMGFDOBJ hImgFDObj);
	int			__fstat(IN HIMGFDOBJ hImgFDObj,OUT struct _stat* buffer);
	INT_PTR		__fileno(IN HIMGFDOBJ hImgFDObj);
	void		__rewind(IN HIMGFDOBJ hImgFDObj);
	long		__ftell(IN HIMGFDOBJ hImgFDObj);
	int			__feof(IN HIMGFDOBJ hImgFDObj);
	int			__fseek(IN HIMGFDOBJ hImgFDObj,IN long offset,IN int origin);
	size_t		__fread(IN void* buffer,IN size_t size,IN size_t count,IN HIMGFDOBJ hImgFDObj);
	size_t		__fwrite(IN const void* buffer,IN size_t size,IN size_t count,IN HIMGFDOBJ hImgFDObj);
	int			__fprintf(IN HIMGFDOBJ hImgFDObj,IN char* lpszFmt,IN ...);
	int			__fclose(IN HIMGFDOBJ hImgFDObj);
	int			__fflush(IN HIMGFDOBJ hImgFDObj);
	int			__scandir(IN const char* dirname,IMGFINDDATA** lppImgFindData);
	int			__format();
protected:
	BOOL SearchFDT(IN LPCTSTR lpszPath,OUT FDTOBJ* pFDTObj);
	BOOL CreateFDT(IN LPCTSTR lpszPath,IN BOOL bDirectory,OUT FDTOBJ* pFDTObj);
	BOOL RemoveFDT(IN FDTOBJ* pFDTObj);
	BOOL UpdateFDT(IN FDTOBJ* pFDTObj);
	VOID DOSNameFromFDT(IN FDTFAT* pFDT,OUT char* lpszFileName);
	VOID DOSNameToFDT(IN FDTFAT* pFDT,OUT char* lpszFileName);
	VOID ToFATDateTime(IN time_t tTime,INOUT DATEFAT* pDateFAT,INOUT TIMEFAT* pTimeFAT);
	VOID FromFATDateTime(IN DATEFAT* pDateFAT,IN TIMEFAT* pTimeFAT,OUT time_t* ptTime);
	VOID GetNowFATDate(INOUT DATEFAT* pDateFAT);
	VOID GetNowFATTime(INOUT TIMEFAT* pTimeFAT);
	DWORD StartClusterFromFDT(IN FDTFAT* pFDT);
	DWORD StartClusterToFDT(IN FDTFAT* pFDT,IN DWORD dwStartCluster);
	DWORD ReadClusterLink(IN DWORD dwCluster,IN DWORD dwOffset,IN DWORD dwSize,IN LPBYTE lpBuffer);
	DWORD WriteClusterLink(IN DWORD dwCluster,IN DWORD dwOffset,IN DWORD dwSize,IN LPBYTE lpBuffer);
	DWORD TailOfClusterLink(IN DWORD dwCluster);
	DWORD ClusterOfClusterLink(IN DWORD dwCluster,IN DWORD dwNo);
	DWORD CountOfClusterLink(IN DWORD dwCluster);
	DWORD AllocClusterLink(DWORD dwCount);
	VOID FreeClusterLink(IN DWORD dwCluster);
	BOOL AdjuestClusterLink(IN DWORD dwOldCluster,IN DWORD dwNewCount,IN BOOL bAutoTruncate,OUT DWORD* lpdwNewCluster);
	BOOL IsValidCluster(IN DWORD dwCluster);
	BOOL IsBadCluster(IN DWORD dwCluster);
	BOOL IsTailCluster(IN DWORD dwCluster);
	BOOL IsEmptyCluster(IN DWORD dwCluster);
	BOOL GetNextCluster(IN DWORD dwCluster,OUT LPDWORD pdwNextCluster);
	BOOL SetNextCluster(IN DWORD dwCluster,IN DWORD dwNextCluster);
	BOOL SetNextClusterToEnd(IN DWORD dwCluster);
	VOID FlushFAT(IN DWORD dwCluster);
protected:
	CAnyFile*	m_pAnyFile;			// �����ļ�
	DBRFAT		m_DBR;				// DBR��¼
	DWORD		m_dwTotalSectors;	// ��������
	DWORD		m_dwBytesPerCluster;// ÿ���ص��ֽ���
	DWORD		m_dwMinClusters;	// ��С�����
	DWORD		m_dwMaxClusters;	// �������
	DWORD		m_dwTotalClusters;	// �ܴ���
	CHAR		m_szLabel[12];      // ���̾����
    CHAR		m_szFileSysID[9];	// �ļ�ϵͳ��ʶ��
	BYTE		m_cDPTType;			// ������ʽ
	DWORD		m_cbFATSize;		// FAT����С
	LPBYTE		m_lpFAT;			// FAT��
	FDTFAT*		m_lpFDT;			// FDT(DIR)������(������FAT��2)
	DWORD		m_dwFATOffset;		// FAT��ƫ��
	DWORD		m_dwFDTOffset;		// FDT��ƫ��
	DWORD		m_dwDATOffset;		// ������ƫ��
};



#endif