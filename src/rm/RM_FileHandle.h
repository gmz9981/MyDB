//
// Created by Zhengxiao Du on 2018/10/28.
//

#ifndef MYDB_RM_FILEHANDLE_H
#define MYDB_RM_FILEHANDLE_H

#include "HeaderPage.h"
#include "RM_Record.h"

#include "RID.h"
#include "../Constants.h"
#include "../pf/pf.h"

class RM_FileHandle {
public:
    friend class RecordManager;
    friend class RM_FileScan;

    RM_FileHandle() = default;                                  // Constructor
    RM_FileHandle(const RM_FileHandle &) = delete;
    ~RM_FileHandle();                                  // Destructor

    int getRec(const RID &rid, RM_Record & record) const;

    // Get a record
    RID insertRec(const char *pData);       // Insert a new record,
    //   return record id
    int deleteRec(const RID &rid);                    // Delete a record
    int updateRec(const RM_Record &rec);              // Update a record
    bool is_initialized() const;
private:
    int insertPage();

    unsigned getOffset(unsigned slot_num) const;

    HeaderPage _header_page;
    bool _header_modified;
    PF_FileHandle _pf_file_handle;
    bool _initialized = false;
};

#endif //MYDB_RM_FILEHANDLE_H
