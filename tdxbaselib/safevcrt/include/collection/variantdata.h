//////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2006 - All Rights Reserved
//  ģ������: VariantData
//  ��������: 2006/08/05
//  �����д: zhanglijun
//  ����˵��: ���ݾۺ���
//            ���ݾۺ�����Ҫ������֯����䳤�Ľṹ,����ͳһ�Ļ�����,������ǿЭ�������/�������Լ�
//            �ں����������紫�����С�������,��ǿ�ӿڵ����̶�,���ڶ���䳤���ݵĴ���
//  ����ʾ��: 
//  �޸ļ�¼:
//////////////////////////////////////////////////////////////////////////
#ifndef __VARIANT_DATA__H__
#define __VARIANT_DATA__H__
#if _MSC_VER > 1000
#pragma once
#endif

// ʹ��1�ֽڶ���
#pragma pack(1)

// �����ֶη��صĽṹ(����ֱ�ӷ���)
typedef struct tagVARIANTDINFO
{	LPDWORD	m_lpcbUsed;		// ʹ���еĻ�����ָ��
	LPBYTE	m_lpItem;		// ��ǰ������ָ��
	DWORD	m_cbItem;		// ��ǰ�������С
	DWORD	m_dwMaxSize;	// ����������С
} VARIANTDINFO,*LPVARIANTDINFO;

class COLLECTION_IMP CVariantData
{
public:
	CVariantData();
	virtual~CVariantData();

	// �����ۺ�������(��������С9�ֽ�)
	// ����:	lpBuffer	�ⲿ������
	//			cbBuffer	�ⲿ��������С
	//			cbUsed		��ʹ�õ��ֽ���,Ϊ0�������´���
	// ����:	(BOOL)		�Ƿ񴴽��ɹ�
	BOOL Create(LPBYTE lpBuffer,DWORD cbBuffer,DWORD cbUsed);

	// �õ�ʹ���е���������С
	DWORD GetUsedLength();

	// ����������
	// ����:	cKey		������
	//			lpData		���ݻ�����
	//			cbData		���ݴ�С
	// ����:	(BOOL)		�Ƿ����ӳɹ�
	BOOL SetData(BYTE cKey,const LPBYTE lpData,DWORD cbData,LPVARIANTDINFO lpVariantDInfo=NULL);

	// �����ַ���������
	// ����:	cKey		������
	//			nMaxLength	�����������
	//			lpString	���ݻ�����
	//			nLength		�ļ�����,���<0���Զ��ж�
	// ����:	(BOOL)		�Ƿ����ӳɹ�
	BOOL SetString(BYTE cKey,LONG nMaxLength,LPCSTR lpString,LONG nLength);

	// ɾ��������
	// ����:	cKey		������
	// ����:	(BOOL)		�Ƿ�ɾ���ɹ�
	BOOL DelData(BYTE cKey);

	// �ж��������Ƿ����
	// ����:	cKey		������
	// ����:	(BOOL)		�Ƿ����ݴ���
	BOOL IsDataExist(BYTE cKey);
	
	// ���������ʼ��ַ
	// ����:	cKey		������
	// ����:	(LPBYTE)	�����ʼλ��,���������,�򷵻�NULL
	LPBYTE GetDataAddr(BYTE cKey);

	// �õ��������
	// ����:	cKey		������
	// ����:	(DWORD)		���ݳ���,(DWORD)-1�������ݲ�����
	DWORD GetDataLength(BYTE cKey);

	// �õ��������(���ݻ������ᱻ����)
	// ����:	cKey		������
	//			lpBuffer	���ݻ�����
	//			cbBuffer	���ݻ�������С
	// ����:	(DWORD)		���ݳ���,(DWORD)-1�������ݲ����ڻ������ݳ�������
	DWORD GetData(BYTE cKey,LPBYTE lpBuffer,DWORD cbBuffer);

protected:
	// ��������ʽ
	typedef struct tagDATIDX
	{	BYTE  m_cKey;
		DWORD m_cbData;
	} DATIDX,*LPDATIDX;
	// �õ�ÿ�������������ɵ����������
	inline BYTE CalcIdxNumPerBlk()
	{	return (BYTE)4;
	}
	// �õ�ÿ����������Ĵ�С
	inline DWORD CalcIdxBufSizePerBlk()
	{	return CalcIdxNumPerBlk()*size_of(DATIDX);
	}
	// ���������ΪcKeyNumʱ������������������������������������
	// ע:������������Ϊ��
	inline BYTE CalcMaxIdxNum(BYTE cKeyNum)
	{	return (BYTE)max_of((cKeyNum+CalcIdxNumPerBlk()-1)/CalcIdxNumPerBlk()*CalcIdxNumPerBlk(),CalcIdxNumPerBlk());
	}
	// ���������ΪcKeyNumʱ��������������������������Ĵ�С
	inline DWORD CalcIdxBufSize(BYTE cKeyNum)
	{	return CalcMaxIdxNum(cKeyNum)*size_of(DATIDX);
	}
protected:
	DWORD FindKeyOffset(BYTE cKey,LPBYTE lpIdx);
protected:
	BOOL		m_bCreated;		// �Ƿ񱻴���
	LPBYTE		m_lpBuffer;		// ������
	DWORD		m_cbBuffer;		// ��������С
	DWORD		m_cbUsed;		// ʹ���еĻ�����
	LPBYTE		m_lpcDatsNum;	// ������
	LPDATIDX	m_lpDatIdxs;	// ������ָ��
	LPBYTE		m_lpDatas;		// ���ݻ�����ָ��
};


class COLLECTION_IMP CSequence
{
public:
	CSequence(LPBYTE lpBuffer,DWORD cbBuffer,DWORD cbUsed=0);
	CSequence();
	virtual~CSequence();

	VOID SetBuffer(LPBYTE lpBuffer,DWORD cbBuffer,DWORD cbUsed=0);
	LPBYTE GetBuffer() { return m_lpBuffer; }
	DWORD GetBufferSize() const { return m_cbBuffer; }
	DWORD GetFreeSize() const { return m_cbBuffer-m_cbUsed; }
	DWORD GetUsed() const { return m_cbUsed; }
	VOID SeekToBegin() { m_dwOffset=0; }
	VOID SeekToEnd() { m_dwOffset=m_cbUsed; }
	VOID SeekTo(DWORD dwOffset) { m_dwOffset=dwOffset; }
	VOID Truncate() { m_cbUsed=m_dwOffset=0; }
	DWORD Tell() const { return m_dwOffset; }
	LPBYTE GetRest() const { return m_lpBuffer+m_dwOffset; }
	DWORD GetRestSize() const { return max_of(m_dwOffset,GetUsed())-m_dwOffset; }
	BOOL IsEmpty() const { return m_cbUsed==0; }

	// ������������д��Ͷ�ȡ
	#define DECLARE_WRITE_STREAM_X(FUNC,TYPE)					\
		BOOL FUNC(TYPE xValue)									\
			{	if(m_dwOffset+size_of(TYPE)>=m_cbBuffer)			\
				{	m_bOverflow=TRUE;							\
					return FALSE;								\
				}												\
				*((TYPE*)(m_lpBuffer+m_dwOffset))=xValue;		\
				m_dwOffset+=size_of(TYPE);						\
				if(m_dwOffset>m_cbUsed) m_cbUsed=m_dwOffset;	\
				return TRUE;									\
			}
	DECLARE_WRITE_STREAM_X(WriteChar,CHAR);
	DECLARE_WRITE_STREAM_X(WriteByte,BYTE);
	DECLARE_WRITE_STREAM_X(WriteShort,SHORT);
	DECLARE_WRITE_STREAM_X(WriteUShort,USHORT);
	DECLARE_WRITE_STREAM_X(WriteLong,LONG);
	DECLARE_WRITE_STREAM_X(WriteULong,ULONG);
	DECLARE_WRITE_STREAM_X(WriteLongLong,INT64);
	DECLARE_WRITE_STREAM_X(WriteULongLong,UINT64);
	DECLARE_WRITE_STREAM_X(WriteInt,INT);
	DECLARE_WRITE_STREAM_X(WriteUint,UINT);
	DECLARE_WRITE_STREAM_X(WriteWord,WORD);
	DECLARE_WRITE_STREAM_X(WriteDWord,DWORD);
	DECLARE_WRITE_STREAM_X(WriteBool,BOOL);
	DECLARE_WRITE_STREAM_X(WriteFloat,FLOAT);
	DECLARE_WRITE_STREAM_X(WriteDouble,double);
	#undef DECLARE_WRITE_STREAM_X

	#define DECLARE_READ_STREAM_X(FUNC,TYPE)						\
		BOOL FUNC(TYPE& xValue)										\
			{	if(m_dwOffset+size_of(TYPE)>m_cbUsed) return FALSE;	\
				xValue=*((TYPE*)(m_lpBuffer+m_dwOffset));			\
				m_dwOffset+=size_of(TYPE);							\
				return TRUE;										\
			}
	DECLARE_READ_STREAM_X(ReadChar,CHAR);
	DECLARE_READ_STREAM_X(ReadByte,BYTE);
	DECLARE_READ_STREAM_X(ReadShort,SHORT);
	DECLARE_READ_STREAM_X(ReadUShort,USHORT);
	DECLARE_READ_STREAM_X(ReadLong,LONG);
	DECLARE_READ_STREAM_X(ReadULong,ULONG);
	DECLARE_READ_STREAM_X(ReadLongLong,INT64);
	DECLARE_READ_STREAM_X(ReadULongLong,UINT64);
	DECLARE_READ_STREAM_X(ReadInt,INT);
	DECLARE_READ_STREAM_X(ReadUint,UINT);
	DECLARE_READ_STREAM_X(ReadWord,WORD);
	DECLARE_READ_STREAM_X(ReadDWord,DWORD);
	DECLARE_READ_STREAM_X(ReadBool,BOOL);
	DECLARE_READ_STREAM_X(ReadFloat,FLOAT);
	DECLARE_READ_STREAM_X(ReadDouble,double);
	#undef DECLARE_READ_STREAM_X

	// �տ�д��
	BOOL WriteBlank(DWORD cbBlank);

	// ���ݿ�д��Ͷ�ȡ
	BOOL WriteBinary(LPCVOID lpBuffer,DWORD cbBuffer);
	BOOL ReadBinary(LPVOID lpBuffer,DWORD cbBuffer);

	// �ַ���д��Ͷ�ȡ
	BOOL WriteString(LPCSTR lpString);
	BOOL ReadString(LPSTR lpBuffer,DWORD cbBuffer);
	BOOL ReadString(LPCSTR& lpBuffer);

	// �������͵Ķ�ȡ��д��
	BOOL WriteIpv4(IPV4 Ip) { return WriteBinary(&Ip,size_of(Ip)); }
	BOOL WriteIpv6(IPV6 Ip) { return WriteBinary(&Ip,size_of(Ip)); }
	BOOL WriteMac(MAC Mac) { return WriteBinary(&Mac,size_of(Mac)); }
	BOOL ReadIpv4(IPV4& Ip) { return ReadBinary(&Ip,size_of(Ip)); }
	BOOL ReadIpv6(IPV6& Ip) { return ReadBinary(&Ip,size_of(Ip)); }
	BOOL ReadMac(MAC& Mac) { return ReadBinary(&Mac,size_of(Mac)); }

	// �����
	BOOL IsOverflow() { return m_bOverflow; }

	// �ڴ���
	DWORD BeginSection();
	BOOL EndSection(DWORD dwSectionOffset);
	BOOL ReadSection(CSequence& Sequence);

	// ����������
	BOOL WriteBuffer(LPCBYTE pBuffer,DWORD cbData);
	BOOL WriteLiteBuffer(LPCBYTE pBuffer,DWORD cbData);
	BOOL ReadBuffer(LPBYTE pBuffer,DWORD cbBuffer,DWORD& cbData);
	BOOL ReadBuffer(LPBYTE& pBuffer,DWORD& cbData);
	BOOL ReadLiteBuffer(LPBYTE pBuffer,DWORD cbBuffer,DWORD& cbData);
	BOOL ReadLiteBuffer(LPBYTE& pBuffer,DWORD& cbData);

protected:
	LPBYTE m_lpBuffer;
	DWORD m_cbBuffer;
	DWORD m_cbUsed;
	DWORD m_dwOffset;
	BOOL m_bOverflow;
};

#pragma pack()


#endif