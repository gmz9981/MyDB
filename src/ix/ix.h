//
// ix.h
//
//   Index Manager Component Interface
//

#ifndef IX_H
#define IX_H

// Please do not include any other files than the ones below in this file.

#include "../Constants.h"
#include "../rm/RID.h"
#include "../pf/pf.h"
#include "ix_internal.h"
#include "../rm/RM_FileHandle.h"

class IX_IndexHandle;

//
// IX_Manager: provides IX index file management
//
class IX_Manager {
    PF_Manager *pfm;
	
    // RM_Manager rmm;

public:
    ~IX_Manager  ();                             // Destructor
    RC CreateIndex  (const char *fileName,          // Create new index
                     int        indexNo,
                     AttrType   attrType,
                     int        attrLength);
    RC DestroyIndex (const char *fileName,          // Destroy index
                     int        indexNo);
    RC OpenIndex    (const char *fileName,          // Open index
                     int        indexNo,
                     IX_IndexHandle &indexHandle,
					 RM_FileHandle &rmFileHandle,
					 int _attrOffset);
    RC CloseIndex   (IX_IndexHandle &indexHandle);  // Close index
    static RC indexAvailable();
    static IX_Manager & getInstance();
private:
	explicit IX_Manager   (PF_Manager *pfm);              // Constructor
	char* GenFileName(const char* filename, int indexNo);
};

//
// IX_IndexHandle: IX Index File interface
//
class IX_IndexHandle {
public:
    IX_IndexHandle  ();                             // Constructor
    ~IX_IndexHandle ();                             // Destructor
    RC InsertEntry     (const RID &rid);  // Insert new index entry
    RC DeleteEntry     (const RID &rid);  // Delete index entry
    RC ForcePages      ();                             // Copy index to disk
	
	RC init(const char* indexFileName, PF_Manager *_pfm, RM_FileHandle *_rmFileHandle, int _attrOffset);
	RC CloseIndex();
	LeafNode FindLeafNode(void *pData);
	
	void PrintFullLinkList(); // print full link list just for attrtype = int
	
	void GetNextRIDPositionInfo(RIDPositionInfo &ridPositionInfo, int dir, bool EQ_OP);
	void GetGeqRIDPos(const void *pData, RIDPositionInfo &ridPositionInfo, bool returnFirstRID, bool LE);
	
	int cmp(const RID, const RID);
	int cmp(const void*, const void*);
	char* getValueFromRecRID(const RID rid);
	void checkLinkListIsValid();
private:
	
	IndexInfo *indexInfo;
	PF_FileHandle fileHandle;
	PF_Manager *pfm;
	RM_FileHandle *rmFileHandle;
	int attrOffset;
	PageNum pinnedPageList[MAX_DEPTH];
	int pinnedPageNum;
	PageNum disposedPageList[MAX_DEPTH];
	int disposedPageNum;
	
	RC insertIntoRIDPage(const RID rid, const PageNum pageNum);
	RC deleteFromRIDPage(const RID rid, const PageNum pageNum, int &lastRIDCount, RID &replacedRID);
	PageNum InsertEntryFromPage(RID rid, PageNum &pageNum, PageNum fatherPage, int nodePos);
	RC DeleteEntryFromPage(RID rid, PageNum& pageNum, PageNum fatherPageNum, int thisPos);
	PageNum FindLeafPageFromPage(void *pData, PageNum pageNum);
	
	int getRIDPageSize(const PageNum pageNum);
	int getLeafNodeSize(const PageNum pageNum);
	void getPageData(const PageNum pageNum, char*& pageData);
	void getLeafNode(const PageNum pageNum, LeafNode*& leafNode);
	void getInternalNode(const PageNum pageNum, InternalNode*& internalNode);
	PageNum allocateNewPage(PF_PageHandle &pageHandle);
	void getExistedPage(PageNum pageNum, PF_PageHandle &pageHandle);
	
	void addPinnedPage(const PageNum pageNum);
	void addDisposedPage(const PageNum pageNum);
	void unpinAllPages();
	void disposeAllPages();
	
	void assertIncrease(RID* ridList, int length);
	void assertIncrease(LeafNode* leafNode);
	void assertIncrease(InternalNode* internalNode);
};

//
// IX_IndexScan: condition-based scan of index entries
//
class IX_IndexScan {
public:

    IX_IndexScan  ();                                 // Constructor
    ~IX_IndexScan ();                                 // Destructor
    RC OpenScan      (IX_IndexHandle &indexHandle, // Initialize index scan
                      CompOp      compOp,
                      const void        *value,
                      ClientHint  pinHint = ClientHint::NO_HINT);
    RC GetNextEntry  (RID &rid);                      // Get next matching entry
    RC CloseScan     ();     	// Terminate index scan
private:
	IX_IndexHandle *indexHandle;
	RIDPositionInfo ridPositionInfo;
	int dir;
	const void *skipValue;
	bool EQ_OP;

	RIDList *ridHead, *tail;
};

//
// Print-error function
//
void IX_PrintError(RC rc);

void IX_Test();

#define IX_EOF                  (START_IX_WARN + 0)
#define IX_ENTRY_EXISTS         (START_IX_WARN + 1)
#define IX_ENTRY_DOES_NOT_EXIST (START_IX_WARN + 2)
#define IX_SCAN_NOT_OPENED      (START_IX_WARN + 3)
#define IX_SCAN_NOT_CLOSED      (START_IX_WARN + 4)
#define IX_BUCKET_FULL          (START_IX_WARN + 5)
#define IX_LASTWARN IX_SCAN_NOT_CLOSED


#define IX_ATTR_TOO_LARGE       (START_IX_ERR - 0)
#define IX_LASTERROR IX_ATTR_TOO_LARGE

#endif // IX_H
