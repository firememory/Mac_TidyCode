#ifndef __XML_PROFILE__H__
#define __XML_PROFILE__H__
#if _MSC_VER > 1000
#pragma once
#endif
#include "xml.h"

class CXMLProfile;
class CXMLProfileEnumerator;
class CXMLProfileSection;

//////////////////////////////////////////////////////////////////////////
// XML属性表类
class LITEXML_IMP CXMLProfile
{
public:
	enum enumOPTION
	{	O_NONE=0,
		O_REPL_DEFVAL=1,
		O_ERASE_DEFVAL=2,
	};
	CXMLProfile(CXML* pXML,BOOL bLoading,LONG nOptions=O_NONE);
	CXMLProfile(LPSTR lpszSection,UINT nLANGCodePage,BOOL bLoading,LONG nOptions=O_NONE);
	virtual~CXMLProfile();
public:
	BOOL SetDefaultXML(CXML* pXMLDefault,LPCSTR pszPath);
	BOOL LoadFromMem(const unsigned char* lpXmlBuf,LONG nXmlLen,UINT nFileLANGCodePage=CP_GB2312);
	BOOL LoadFromFile(LPCSTR pszFileName,UINT nFileLANGCodePage=CP_GB2312);
	BOOL SaveToFile(LPCSTR pszFileName,UINT nFileLANGCodePage=CP_GB2312,UINT nFileCONVCodePage=CP_UTF8);
	BOOL SetLoading(BOOL bLoading);
	BOOL IsAutoDefValSupported();
	LONG GetOptions();
	LONG SetOptions(LONG nOptions);
	LONG ModifyOptions(LONG nRemove,LONG nAdd);
	BOOL IsLoading();
	BOOL IsStoring();
	CXML* GetXML();
	CXML* GetXMLDefault();
	// 得到父节点为根节点的节
	CXMLProfileSection GetSection();
	// 得到父节点为根节点某子节点的节或者对应的枚举
	CXMLProfileSection GetSection(LPCSTR pszSection,DWORD dwCreateFlag=XMLCREATE_APPEND);
	CXMLProfileSection GetSectionEx(LPCSTR pszSection,DWORD dwCreateFlag=XMLCREATE_APPEND);
	CXMLProfileEnumerator GetEnumerator(LPCSTR pszSectionPattern,LONG nNodeCount);
protected:
	BOOL m_bLoading;
	LONG m_nOptions;
	CXML* m_pXML;
	BOOL m_bExternXML;
	HXMLNODE m_hRoot;
	CXML* m_pXMLDefault;
	HXMLNODE m_hRootInDefault;
};



//////////////////////////////////////////////////////////////////////////
// XML节点枚举类
class LITEXML_IMP CXMLProfileEnumerator
{
public:
	CXMLProfileEnumerator(CXMLProfile* pPX,HXMLNODE hParentNode,LPCSTR pszSectionPattern,LONG nNodeCount);
	virtual~CXMLProfileEnumerator();
public:
	LONG GetNodeCount();
	POSITION GetPosition();
	CXMLProfileSection GetNext(POSITION& posNode);
protected:
	CXMLProfile* m_pPX;
	HXMLNODE m_hParentNode;
	CHAR m_szPattern[MAX_PATH];
	BOOL m_bLikeCompare;
	LONG m_nNodeCount;
};


//////////////////////////////////////////////////////////////////////////
// XML属性节点类
class LITEXML_IMP CXMLProfileSection
{
public:
	CXMLProfileSection(CXMLProfile* pPX,HXMLNODE hNode,HXMLNODE hNodeDefault=NULL);
	virtual~CXMLProfileSection();
	BOOL IsAutoDefValSupported();
	LONG GetOptions();
	LONG SetOptions(LONG nOptions);
	LONG ModifyOptions(LONG nRemove,LONG nAdd);
	BOOL IsLoading();
	BOOL IsStoring();
	BOOL IsValid();
	CXML* GetXML();
	CXML* GetXMLDefault();
	CXMLProfileSection GetSection(LPCSTR pszSection,DWORD dwCreateFlag=XMLCREATE_APPEND);
	CXMLProfileEnumerator GetEnumerator(LPCSTR pszSectionPattern,LONG nNodeCount);
	HXMLNODE GetNode();
	HXMLNODE GetNodeDefault();
	LPCTSTR GetNodeKey();
public:
	// 重置或者删除子节点
	BOOL ResetSection();
	BOOL DeleteChildNodes();
	// 标准格式(包括默认字段):
	//		<Section PropName="???"></Section>
	// 字段格式:
	//		<Section F???="???"></Section>
	// INI格式:
	//		<Section>
	//			<AppName>
	//				<KeyName Val="???"/>
	//			</AppName>
	//		</Section>
	// INI扩展格式
	//		<Section>
	//			<AppName KeyName="???"/>
	//		</Section>
	
	// 默认属性读取
	#define DECLARE_READ_DEFAULT_X(FUNC,TYPE,NULVAL) \
		TYPE FUNC(LPCSTR pszPropName,TYPE xDefault=NULVAL);
	DECLARE_READ_DEFAULT_X(ReadDefaultChar,CHAR,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultByte,BYTE,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultShort,SHORT,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultUShort,USHORT,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultLong,LONG,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultULong,ULONG,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultInt,INT,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultUint,UINT,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultInt64,INT64,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultUint64,UINT64,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultDWordPtr,DWORD_PTR,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultLongInt,LONGINT,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultULongInt,ULONGINT,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultWord,WORD,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultDWord,DWORD,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultPointer,LPVOID,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultBool,BOOL,FALSE);
	DECLARE_READ_DEFAULT_X(ReadDefaultFloat,FLOAT,0.0f);
	DECLARE_READ_DEFAULT_X(ReadDefaultDouble,DOUBLE,0.0);
	DECLARE_READ_DEFAULT_X(ReadDefaultTime,time_t,0);
	DECLARE_READ_DEFAULT_X(ReadDefaultColor,COLORREF,0);
	#ifdef PLATFORM_WINDOWS
		DECLARE_READ_DEFAULT_X(ReadDefaultHotKey,DWORD,0);
	#endif
	DECLARE_READ_DEFAULT_X(ReadDefaultMd5,MD5BIN,md5Null);
	DECLARE_READ_DEFAULT_X(ReadDefaultRect,RECT,rectNull);
	DECLARE_READ_DEFAULT_X(ReadDefaultPoint,POINT,pointNull);
	DECLARE_READ_DEFAULT_X(ReadDefaultSize,SIZE,sizeNull);
	LPCSTR ReadDefaultNChar(LPCSTR pszPropName,LPCSTR lpszDefault);

	// 属性读取(包含返回值)
	#define DECLARE_READ_X(FUNC,TYPE,NULVAL) \
		BOOL FUNC(LPCSTR pszPropName,TYPE& xValue,TYPE xDefault=NULVAL);
	DECLARE_READ_X(ReadChar,CHAR,0);
	DECLARE_READ_X(ReadByte,BYTE,0);
	DECLARE_READ_X(ReadShort,SHORT,0);
	DECLARE_READ_X(ReadUShort,USHORT,0);
	DECLARE_READ_X(ReadLong,LONG,0);
	DECLARE_READ_X(ReadULong,ULONG,0);
	DECLARE_READ_X(ReadInt,INT,0);
	DECLARE_READ_X(ReadUint,UINT,0);
	DECLARE_READ_X(ReadInt64,INT64,0);
	DECLARE_READ_X(ReadUint64,UINT64,0);
	DECLARE_READ_X(ReadDWordPtr,DWORD_PTR,0);
	DECLARE_READ_X(ReadLongInt,LONGINT,0);
	DECLARE_READ_X(ReadULongInt,ULONGINT,0);
	DECLARE_READ_X(ReadWord,WORD,0);
	DECLARE_READ_X(ReadDWord,DWORD,0);
	DECLARE_READ_X(ReadPointer,LPVOID,0);
	DECLARE_READ_X(ReadBool,BOOL,0);
	DECLARE_READ_X(ReadFloat,FLOAT,0);
	DECLARE_READ_X(ReadDouble,DOUBLE,0);
	DECLARE_READ_X(ReadTime,time_t,0);
	DECLARE_READ_X(ReadColor,COLORREF,0);
	#ifdef PLATFORM_WINDOWS
		DECLARE_READ_X(ReadHotKey,DWORD,0);
	#endif
	DECLARE_READ_X(ReadMd5,MD5BIN,md5Null);
	DECLARE_READ_X(ReadRect,RECT,rectNull);
	DECLARE_READ_X(ReadPoint,POINT,pointNull);
	DECLARE_READ_X(ReadSize,SIZE,sizeNull);
	BOOL ReadNChar(LPCSTR pszPropName, LPSTR lpszValue, DWORD dwLength, LPCSTR lpszDefault);

	// 属性读取(无返回值)
	#define DECLARE_GET_X(FUNC,TYPE,NULVAL) \
		TYPE FUNC(LPCSTR pszPropName,TYPE xDefault=NULVAL);
	DECLARE_GET_X(GetChar,CHAR,0);
	DECLARE_GET_X(GetByte,BYTE,0);
	DECLARE_GET_X(GetShort,SHORT,0);
	DECLARE_GET_X(GetUShort,USHORT,0);
	DECLARE_GET_X(GetLong,LONG,0);
	DECLARE_GET_X(GetULong,ULONG,0);
	DECLARE_GET_X(GetInt,INT,0);
	DECLARE_GET_X(GetUint,UINT,0);
	DECLARE_GET_X(GetWord,WORD,0);
	DECLARE_GET_X(GetDWord,DWORD,0);
	DECLARE_GET_X(GetPointer,LPVOID,0);
	DECLARE_GET_X(GetBool,BOOL,0);
	DECLARE_GET_X(GetFloat,FLOAT,0);
	DECLARE_GET_X(GetDouble,DOUBLE,0);
	DECLARE_GET_X(GetTime,time_t,0);
	DECLARE_GET_X(GetColor,COLORREF,0);
	#ifdef PLATFORM_WINDOWS
		DECLARE_GET_X(GetHotKey,DWORD,0);
	#endif
	DECLARE_GET_X(GetMd5,MD5BIN,md5Null);
	DECLARE_GET_X(GetRect,RECT,rectNull);
	DECLARE_GET_X(GetPoint,POINT,pointNull);
	DECLARE_GET_X(GetSize,SIZE,sizeNull);
	LPCSTR GetNChar(LPCSTR pszPropName, LPSTR lpszValue,DWORD dwLength,LPCSTR lpszDefault);



	// 字段格式读取
	#define DECALRE_READ_FIELD_X(FUNC,TYPE,READFUNC) \
		BOOL FUNC(WORD wPropID,TYPE& xValue,TYPE xDefault)
	DECALRE_READ_FIELD_X(ReadFieldByte,BYTE,0);
	DECALRE_READ_FIELD_X(ReadFieldLong,LONG,0);
	DECALRE_READ_FIELD_X(ReadFieldULong,ULONG,0);
	BOOL ReadFieldNChar(WORD wPropID,LPSTR lpszValue,DWORD dwLength,LPCSTR lpszDefault);


	// INI格式读取
	#define DECALRE_GET_INI_X(FUNC,TYPE) \
		BOOL FUNC(LPCSTR pszAppName,LPCSTR pszKeyName,TYPE& xValue,TYPE xDefault);
	DECALRE_GET_INI_X(GetIniChar,CHAR);
	DECALRE_GET_INI_X(GetIniByte,BYTE);
	DECALRE_GET_INI_X(GetIniShort,SHORT);
	DECALRE_GET_INI_X(GetIniUShort,USHORT);
	DECALRE_GET_INI_X(GetIniLong,LONG);
	DECALRE_GET_INI_X(GetIniULong,ULONG);
	DECALRE_GET_INI_X(GetIniInt,INT);
	DECALRE_GET_INI_X(GetIniUint,UINT);
	DECALRE_GET_INI_X(GetIniWord,WORD);
	DECALRE_GET_INI_X(GetIniDWord,DWORD);
	DECALRE_GET_INI_X(GetIniPointer,LPVOID);
	DECALRE_GET_INI_X(GetIniBool,BOOL);
	DECALRE_GET_INI_X(GetIniFloat,FLOAT);
	DECALRE_GET_INI_X(GetIniDouble,DOUBLE);
	BOOL GetIniString(LPCSTR pszAppName,LPCSTR pszKeyName, LPSTR lpszValue, DWORD dwLength, LPCSTR lpszDefault);


	// INI扩展格式读取
	#define DECALRE_GET_INIEX_X(FUNC,TYPE) \
		BOOL FUNC(LPCSTR pszAppName,LPCSTR pszKeyName,TYPE& xValue,TYPE xDefault);
	DECALRE_GET_INI_X(GetIniExChar,CHAR);
	DECALRE_GET_INI_X(GetIniExByte,BYTE);
	DECALRE_GET_INI_X(GetIniExShort,SHORT);
	DECALRE_GET_INI_X(GetIniExUShort,USHORT);
	DECALRE_GET_INI_X(GetIniExLong,LONG);
	DECALRE_GET_INI_X(GetIniExULong,ULONG);
	DECALRE_GET_INI_X(GetIniExInt,INT);
	DECALRE_GET_INI_X(GetIniExUint,UINT);
	DECALRE_GET_INI_X(GetIniExWord,WORD);
	DECALRE_GET_INI_X(GetIniExDWord,DWORD);
	DECALRE_GET_INI_X(GetIniExPointer,LPVOID);
	DECALRE_GET_INI_X(GetIniExBool,BOOL);
	DECALRE_GET_INI_X(GetIniExFloat,FLOAT);
	DECALRE_GET_INI_X(GetIniExDouble,DOUBLE);
	BOOL GetIniExString(LPCSTR pszAppName,LPCSTR pszKeyName, LPSTR lpszValue, DWORD dwLength, LPCSTR lpszDefault);

	// 默认属性写入
	BOOL WriteAutoDefVal(LPCSTR pszPropName);

	// 标准属性写入
	#define DECLARE_WRITE_X(FUNC,TYPE) \
		BOOL FUNC(LPCSTR pszPropName,TYPE xValue)
	DECLARE_WRITE_X(WriteChar,CHAR);
	DECLARE_WRITE_X(WriteByte,BYTE);
	DECLARE_WRITE_X(WriteShort,SHORT);
	DECLARE_WRITE_X(WriteUShort,USHORT);
	DECLARE_WRITE_X(WriteLong,LONG);
	DECLARE_WRITE_X(WriteULong,ULONG);
	DECLARE_WRITE_X(WriteInt,INT);
	DECLARE_WRITE_X(WriteUint,UINT);
	DECLARE_WRITE_X(WriteInt64,INT64);
	DECLARE_WRITE_X(WriteUint64,UINT64);
	DECLARE_WRITE_X(WriteDWordPtr,DWORD_PTR);
	DECLARE_WRITE_X(WriteLongInt,LONGINT);
	DECLARE_WRITE_X(WriteULongInt,ULONGINT);
	DECLARE_WRITE_X(WriteWord,WORD);
	DECLARE_WRITE_X(WriteDWord,DWORD);
	DECLARE_WRITE_X(WritePointer,LPVOID);
	DECLARE_WRITE_X(WriteBool,BOOL);
	DECLARE_WRITE_X(WriteFloat,FLOAT);
	DECLARE_WRITE_X(WriteDouble,DOUBLE);
	DECLARE_WRITE_X(WriteNChar,LPCSTR);
	DECLARE_WRITE_X(WriteTime,time_t);
	DECLARE_WRITE_X(WriteColor,COLORREF);
	DECLARE_WRITE_X(WriteHotKey,DWORD);
	DECLARE_WRITE_X(WriteMd5,MD5BIN);
	DECLARE_WRITE_X(WriteRect,RECT);
	DECLARE_WRITE_X(WritePoint,POINT);
	DECLARE_WRITE_X(WriteSize,SIZE);

	// 字段格式写入
	#define DECLARE_WRITE_FIELD_X(FUNC,TYPE) \
		BOOL FUNC(WORD wPropID, TYPE xValue);
	DECLARE_WRITE_FIELD_X(WriteFieldByte,BYTE);
	DECLARE_WRITE_FIELD_X(WriteFieldLong,LONG);
	DECLARE_WRITE_FIELD_X(WriteFieldULong,ULONG);
	DECLARE_WRITE_FIELD_X(WriteFieldNChar,LPCSTR);

	// INI格式写入
	#define DECALRE_SET_INI_X(FUNC,TYPE) \
		BOOL FUNC(LPCSTR pszAppName,LPCSTR pszKeyName,TYPE xValue);
	DECALRE_SET_INI_X(SetIniChar,CHAR);
	DECALRE_SET_INI_X(SetIniByte,BYTE);
	DECALRE_SET_INI_X(SetIniShort,SHORT);
	DECALRE_SET_INI_X(SetIniUShort,USHORT);
	DECALRE_SET_INI_X(SetIniLong,LONG);
	DECALRE_SET_INI_X(SetIniULong,ULONG);
	DECALRE_SET_INI_X(SetIniInt,INT);
	DECALRE_SET_INI_X(SetIniUint,UINT);
	DECALRE_SET_INI_X(SetIniWord,WORD);
	DECALRE_SET_INI_X(SetIniDWord,DWORD);
	DECALRE_SET_INI_X(SetIniPointer,LPVOID);
	DECALRE_SET_INI_X(SetIniBool,BOOL);
	DECALRE_SET_INI_X(SetIniFloat,FLOAT);
	DECALRE_SET_INI_X(SetIniDouble,DOUBLE);
	DECALRE_SET_INI_X(SetIniString,LPCSTR);

	// INI扩展格式写入
	#define DECALRE_SET_INIEX_X(FUNC,TYPE) \
		BOOL FUNC(LPCSTR pszAppName,LPCSTR pszKeyName,TYPE xValue);
	DECALRE_SET_INIEX_X(SetIniExChar,CHAR);
	DECALRE_SET_INIEX_X(SetIniExByte,BYTE);
	DECALRE_SET_INIEX_X(SetIniExShort,SHORT);
	DECALRE_SET_INIEX_X(SetIniExUShort,USHORT);
	DECALRE_SET_INIEX_X(SetIniExLong,LONG);
	DECALRE_SET_INIEX_X(SetIniExULong,ULONG);
	DECALRE_SET_INIEX_X(SetIniExInt,INT);
	DECALRE_SET_INIEX_X(SetIniExUint,UINT);
	DECALRE_SET_INIEX_X(SetIniExWord,WORD);
	DECALRE_SET_INIEX_X(SetIniExDWord,DWORD);
	DECALRE_SET_INIEX_X(SetIniExPointer,LPVOID);
	DECALRE_SET_INIEX_X(SetIniExBool,BOOL);
	DECALRE_SET_INIEX_X(SetIniExFloat,FLOAT);
	DECALRE_SET_INIEX_X(SetIniExDouble,DOUBLE);
	DECALRE_SET_INIEX_X(SetIniExString,LPCSTR);

	// 从指定的数据源中装载
	// 如果根路径为NULL,目标XML的根下的全部节点将被复制,否则指定路径下的节点将被复制
	BOOL LoadFeatureFromXmlFile(const char* lpszXmlFile,LPCSTR pszSecKey,UINT nFileLANGCodePage,const char* pszRoot,BOOL bCopyRoot=FALSE);
	BOOL LoadFeatureFromXmlMem(const unsigned char* lpXmlBuf,LONG nXmlLen,UINT nFileLANGCodePage,const char* pszRoot,BOOL bCopyRoot=FALSE);
	BOOL LoadFeatureFromXml(CXML* pXml,const char* pszRoot,BOOL bCopyRoot=FALSE);
	BOOL LoadFeatureFromIniFile(const char* lpszIniFile,UINT nFileLANGCodePage);
	BOOL LoadFeatureFromIniMem(LPBYTE pszIniBuf,LONG nIniLen,UINT nFileLANGCodePage);
protected:
	// 读取属性,读取不到数据或者读取到默认数据返回FALSE,否则返回TRUE
	BOOL ReadAttrib(LPCSTR pszPropName,LPSTR lpszValue,DWORD dwLength,LPCSTR lpszDefault);
	LPCSTR ReadAttribDefault(LPCSTR pszPropName,LPCSTR lpszDefault);
	// 写入属性,写入失败返回FALSE,写入成功返回TRUE
	BOOL WriteAttrib(LPCSTR pszPropName, LPCSTR lpszValue);
protected:
	CXMLProfile* m_pPX;
	HXMLNODE m_hNode;
	HXMLNODE m_hNodeDefault;
};



//////////////////////////////////////////////////////////////////////////
// 动态交换函数
LITEXML_IMP BOOL EPX_Char(CXMLProfileSection* pPX, LPCSTR pszPropName, CHAR& cValue, CHAR cDefault);
LITEXML_IMP BOOL EPX_Byte(CXMLProfileSection* pPX, LPCSTR pszPropName, BYTE& nValue, BYTE nDefault);

LITEXML_IMP BOOL EPX_Short(CXMLProfileSection* pPX, LPCSTR pszPropName, SHORT& sValue, SHORT sDefault);
LITEXML_IMP BOOL EPX_UShort(CXMLProfileSection* pPX, LPCSTR pszPropName, USHORT& usValue, USHORT usDefault);

LITEXML_IMP BOOL EPX_Long(CXMLProfileSection* pPX, LPCSTR pszPropName, LONG& lValue, LONG lDefault);
LITEXML_IMP BOOL EPX_ULong(CXMLProfileSection* pPX, LPCSTR pszPropName, ULONG& lValue, ULONG lDefault);
LITEXML_IMP BOOL EPX_Int(CXMLProfileSection* pPX, LPCSTR pszPropName, INT& nValue, INT nDefault);
LITEXML_IMP BOOL EPX_Uint(CXMLProfileSection* pPX, LPCSTR pszPropName, UINT& nValue, UINT nDefault);
LITEXML_IMP BOOL EPX_Int64(CXMLProfileSection* pPX, LPCSTR pszPropName, INT64& nValue, INT64 nDefault);
LITEXML_IMP BOOL EPX_Uint64(CXMLProfileSection* pPX, LPCSTR pszPropName, UINT64& nValue, UINT64 nDefault);
LITEXML_IMP BOOL EPX_DWordPtr(CXMLProfileSection* pPX, LPCSTR pszPropName, DWORD_PTR& nValue, DWORD_PTR nDefault);
LITEXML_IMP BOOL EPX_LongInt(CXMLProfileSection* pPX, LPCSTR pszPropName, LONGINT& nValue, LONGINT nDefault);
LITEXML_IMP BOOL EPX_ULongInt(CXMLProfileSection* pPX, LPCSTR pszPropName, ULONGINT& nValue, ULONGINT nDefault);
LITEXML_IMP BOOL EPX_Word(CXMLProfileSection* pPX, LPCSTR pszPropName, WORD& wValue, WORD wDefault);
LITEXML_IMP BOOL EPX_DWord(CXMLProfileSection* pPX, LPCSTR pszPropName, DWORD& dwValue, DWORD dwDefault);
LITEXML_IMP BOOL EPX_Pointer(CXMLProfileSection* pPX, LPCSTR pszPropName, LPVOID& pValue, LPVOID pDefault);
LITEXML_IMP BOOL EPX_Bool(CXMLProfileSection* pPX, LPCSTR pszPropName, BOOL& bValue, BOOL bDefault);
LITEXML_IMP BOOL EPX_Float(CXMLProfileSection* pPX, LPCSTR pszPropName, FLOAT& floatValue, FLOAT floatDefault);
LITEXML_IMP BOOL EPX_Double(CXMLProfileSection* pPX, LPCSTR pszPropName, DOUBLE& doubleValue, DOUBLE doubleDefault);
LITEXML_IMP BOOL EPX_NChar(CXMLProfileSection* pPX, LPCSTR pszPropName, LPSTR lpszValue, DWORD dwLength, LPCSTR lpszDefault);
LITEXML_IMP BOOL EPX_Time(CXMLProfileSection* pPX, LPCSTR pszPropName, time_t& tValue, time_t tDefault);
LITEXML_IMP BOOL EPX_Color(CXMLProfileSection* pPX, LPCSTR pszPropName, COLORREF& crValue, COLORREF crDefault);
#ifdef PLATFORM_WINDOWS
	LITEXML_IMP BOOL EPX_HotKey(CXMLProfileSection* pPX, LPCSTR pszPropName, DWORD& dwValue, DWORD dwDefault);
#endif
LITEXML_IMP BOOL EPX_Md5(CXMLProfileSection* pPX, LPCSTR pszPropName, MD5BIN& md5Value,MD5BIN md5Default=md5Null);
LITEXML_IMP BOOL EPX_Rect(CXMLProfileSection* pPX, LPCSTR pszPropName, RECT& rcValue, RECT rcDefault);
LITEXML_IMP BOOL EPX_Point(CXMLProfileSection* pPX, LPCSTR pszPropName, POINT& ptValue, POINT ptDefault);
LITEXML_IMP BOOL EPX_Size(CXMLProfileSection* pPX, LPCSTR pszPropName, SIZE& sizeValue, SIZE sizeDefault);


#endif
