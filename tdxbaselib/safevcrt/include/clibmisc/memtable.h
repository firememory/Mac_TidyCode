//////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2013 - All Rights Reserved
//  ģ������: clibtempl
//  ��������: 2003/01/16
//  �����д: zhanglijun
//  ����˵��: �ڴ��
//  ����ʾ��: 
//  �޸ļ�¼:
//////////////////////////////////////////////////////////////////////////
#ifndef __MEM_TABLE__H__
#define __MEM_TABLE__H__
#if _MSC_VER > 1000
#pragma once
#endif

// �����ֶ�
typedef struct tagRECKRNL
{	BOOL m_bDeleted;			// ɾ����־
	LONG m_nRecordVer;			// ��¼�汾��
	LONG m_nRecordRef;			// ��¼���ü���
	LPVOID m_pCopyFrom;			// ԭ��¼ָ��
} RECKRNL;


// ��������
interface IIndexRender
{	virtual BOOL OnInsertRecord(LPVOID pRecordVoid) PURE;
	virtual VOID OnDeleteRecord(LPVOID pRecordVoid) PURE;
	virtual VOID OnSelectUsingIndex(LONG nTop,LPVOID pKeyVoid,LPVOID plstResultsVoid) PURE;
	virtual DWORD OnSelectUsingIndexEx(DWORD dwMax,LONG nTop,LPVOID pKeyVoid,LPVOID* ppResults) PURE;
	virtual VOID DeleteRender() PURE;
};


// ����ʵ����
template<class KEY,class RECORD,int SUBINDEXNUM>
class TIndex : public IIndexRender
{
public:
	static IIndexRender* Create(LONG nBlockSize,LONG nHashSize,BOOL bUnique) { return new TIndex<KEY,RECORD,SUBINDEXNUM>(nBlockSize,nHashSize,bUnique); }
public:
	TIndex(LONG nBlockSize,LONG nHashSize,BOOL bUnique)
		: m_ChainMap()
		, m_ChainList(nBlockSize)
		, m_bUnique(bUnique)
		{	m_ChainMap.InitHashTable(nHashSize);
		}
	virtual~TIndex()
		{
		}
	// ��¼����ص�(��������)
	virtual BOOL OnInsertRecord(LPVOID pRecordVoid);
	// ��¼ɾ���ص�(��������)
	virtual VOID OnDeleteRecord(LPVOID pRecordVoid);
	// ��¼���һص�
	virtual VOID OnSelectUsingIndex(LONG nTop,LPVOID pKeyVoid,LPVOID plstResultsVoid);
	virtual DWORD OnSelectUsingIndexEx(DWORD dwMax,LONG nTop,LPVOID pKeyVoid,LPVOID* ppResults);
	// ����ɾ��
	virtual VOID DeleteRender();
protected:
	typedef struct tagCHAIN
	{	RECORD* m_pRecord;
		POSITION m_posNext;
	} CHAIN;
	typedef TMap<KEY,const KEY&,LPVOID,LPVOID> CHAINMAP;
	typedef TList<CHAIN,CHAIN&> CHAINLIST;
protected:
	// ��������
	BOOL InsertIndex(const KEY& Key,RECORD* pRecord);
	// ����ɾ��
	VOID DeleteIndex(const KEY& Key,RECORD* pRecord);
protected:
	CHAINMAP m_ChainMap;
	CHAINLIST m_ChainList;
	BOOL m_bUnique;
};


template<class PKEY,class RECORD,INT INDEXNUM>
class TMemTable
{
public:
	TMemTable(LONG nBlockSize,LONG nHashSize)
		: m_RecordMap()
		, m_RecordList(nBlockSize)
		{	m_RecordMap.InitHashTable(nHashSize);
			if(INDEXNUM>0) memset(m_apIndexRender,0,size_of(LPVOID)*INDEXNUM);
		}
	virtual~TMemTable()
		{	for(LONG nIndex=0; nIndex<INDEXNUM; nIndex++)
			{	IIndexRender* pIIndexRender=m_apIndexRender[nIndex];
				if(pIIndexRender==NULL) continue;
				pIIndexRender->DeleteRender();
			}
			if(INDEXNUM>0) memset(m_apIndexRender,0,size_of(LPVOID)*INDEXNUM);
		}
protected:
	typedef TMap<PKEY,const PKEY&,LPVOID,LPVOID> RECORDMAP;
	typedef TList<RECORD,RECORD&> RECORDLIST;
	typedef TList<RECORD*,RECORD*&> RECORDPTRLIST;
	typedef VOID (*ARGS_SHIFTER)(va_list& args);
	typedef VOID (*VIEWER)(RECORD* pRecord);
	typedef BOOL (*WHERE)(RECORD* pRecord,va_list args);
	typedef VOID (*UPDATE_ROUTINE)(RECORD* pRecord,va_list args);
	typedef VOID (*INSERTUPDATE_ROUTINE)(RECORD* pRecord,const PKEY* pKey,BOOL bInsert,va_list args);
public:
	// ��������
	VOID BindIndex(LONG nIndex,IIndexRender* pIIndexRender);

	// ��ȡ��¼��Ŀ
	LONG GetRecordNum();

	// �г�����
	VOID List(LONG nTop,VIEWER pfnView);

	// �����¼
	BOOL Insert(RECORD& Record);
	
	// ��ȡ��¼��Ŀ
	LONG GetCount() { return m_RecordMap.GetCount(); }

	// ѡȡ��¼: ��������,��������,��������
	BOOL Select(RECORD& Record,const PKEY& PKey);
	LONG SelectWhere(RECORDLIST* plstResult,LONG nTop,WHERE pfnWhere,...);
	LONG SelectByIndex(RECORDLIST* plstResult,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,...);

	// ѡȡ�������޸ĸ���: ��������,��������,��������
	RECORD* SelectForUpdate(const PKEY& PKey);
	LONG SelectForUpdateWhere(RECORDPTRLIST* plstForUpdate,LONG nTop,WHERE pfnWhere,...);
	LONG SelectForUpdateByIndex(RECORDPTRLIST* plstForUpdate,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,...);

	// �ύ�����޸�,�����ύ�����޸�(���ύ��ȫ���ύ,�����ύ�ĳ���)
	BOOL CommitUpdate(RECORD* pRecord,BOOL bCancel=FALSE);
	BOOL BatchCommitUpdate(RECORDPTRLIST* plstForUpdate,BOOL bCancel=FALSE);

	// ���¼�¼
	BOOL Update(RECORD* pAffect,const PKEY& PKey,UPDATE_ROUTINE pfnUpdateRoutine,...);
	LONG UpdateWhere(RECORDLIST* plstAffect,LONG nTop,WHERE pfnWhere,ARGS_SHIFTER pfnWhereShifer,UPDATE_ROUTINE pfnUpdateRoutine,...);
	LONG UpdateByIndex(RECORDLIST* plstAffect,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,ARGS_SHIFTER pfnWhereShifer,UPDATE_ROUTINE pfnUpdateRoutine,...);

	// ��������
	BOOL InsertOrUpdate(RECORD* pAffect,const PKEY& PKey,INSERTUPDATE_ROUTINE pfnInsertUpdateRoutine,...);
	BOOL InsertOrUpdateWhere(RECORD* pAffect,WHERE pfnWhere,ARGS_SHIFTER pfnWhereShifer,INSERTUPDATE_ROUTINE pfnInsertUpdateRoutine,...);
	BOOL InsertOrUpdateByIndex(RECORD* pAffect,LONG nIndex,LPVOID pKey,WHERE pfnWhere,ARGS_SHIFTER pfnWhereShifer,INSERTUPDATE_ROUTINE pfnInsertUpdateRoutine,...);

	// ɾ����¼: ��������,��������,��������
	BOOL Delete(RECORD* pAffect,const PKEY& PKey);
	LONG DeleteWhere(RECORDLIST* plstAffect,LONG nTop,WHERE pfnWhere,...);
	LONG DeleteByIndex(RECORDLIST* plstAffect,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,...);

	// ���ٷ���
	LONG SelectWhereEx(RECORD* pResult,DWORD dwMax,LONG nTop,WHERE pfnWhere,...);
	LONG SelectByIndexEx(RECORD* pResult,DWORD dwMax,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,...);
	LONG SelectForUpdateWhereEx(RECORD** ppForUpdate,DWORD dwMax,LONG nTop,WHERE pfnWhere,...);
	LONG SelectForUpdateByIndexEx(RECORD** ppForUpdate,DWORD dwMax,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,...);
	BOOL BatchCommitUpdateEx(RECORD** ppForUpdate,DWORD dwCount,BOOL bCancel=FALSE);
	LONG UpdateWhereEx(RECORD* pAffect,DWORD dwMax,LONG nTop,WHERE pfnWhere,ARGS_SHIFTER pfnWhereShifer,UPDATE_ROUTINE pfnUpdateRoutine,...);
	LONG UpdateByIndexEx(RECORD* pAffect,DWORD dwMax,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,ARGS_SHIFTER pfnWhereShifer,UPDATE_ROUTINE pfnUpdateRoutine,...);
	LONG DeleteWhereEx(RECORD* pAffect,DWORD dwMax,LONG nTop,WHERE pfnWhere,...);
	LONG DeleteByIndexEx(RECORD* pAffect,DWORD dwMax,LONG nTop,LONG nIndex,LPVOID pKey,WHERE pfnWhere,...);

protected:
	// ���Ҽ�¼
	VOID SelectRecord(LONG nTop,WHERE pfnWhere,va_list argWhere,RECORDPTRLIST& lstPreliminary);
	VOID SelectRecord(LONG nTop,IIndexRender* pIIndexRender,LPVOID pKey,WHERE pfnWhere,va_list argWhere,RECORDPTRLIST& lstPreliminary);
	// ���Ҽ�¼(ʹ������)
	DWORD SelectRecordEx(DWORD dwMax,LONG nTop,WHERE pfnWhere,va_list argWhere,RECORD** ppPreliminarys);
	DWORD SelectRecordEx(DWORD dwMax,LONG nTop,IIndexRender* pIIndexRender,LPVOID pKey,WHERE pfnWhere,va_list argWhere,RECORD** ppPreliminarys);
	// �����¼
	BOOL InsertRecord(RECORD* pAffect,const PKEY& PKey,RECORD& Record);
	// ɾ����¼
	VOID DeleteRecord(RECORD* pAffect,RECORD* pRecord);
	// ������¼����(�޸���;)
	RECORD* CreateCopyRecord(RECORD* pRecord);
protected:
	CLock m_CsTable;
	RECORDMAP m_RecordMap;
	RECORDLIST m_RecordList;
	IIndexRender* m_apIndexRender[INDEXNUM+1];
};



// ��������ʵ��
#include "memtable.inl"

#endif