//
// Created by Zhengxiao Du on 2018-12-20.
//

#ifndef MYDB_TABLE_H
#define MYDB_TABLE_H

#include "../parser/Tree.h"
#include "../rm/RM_FileHandle.h"
#include "../rm/RecordManager.h"
#include "../rm/RID.h"
#include "../ix/ix.h"
#include "../parser/Expr.h"

#include <vector>
#include <map>
#include <memory>

class Table {
public:
    explicit Table(const std::string &tableName);

    ~Table();

    int getOffset(const std::string &attribute) const;

    int getColumnIndex(const std::string &attribute) const;

    const BindAttribute &getAttrInfo(int index) const;

    int getAttrCount() const;

    bool getIndexAvailable(int index);

    IX_IndexHandle &getIndexHandler(int index);

    std::string checkData(char *data);

    int deleteData(const RID &rid);

    int insertData(const IdentList *columnList, const ConstValueList *constValues);

    int updateData(const RM_Record &record, const std::vector<int> &attrIndexes, SetClauseList *setClauses);

    int insertIndex(char *data, const RID &rid);

    int deleteIndex(char *data, const RID &rid);

    RM_FileHandle &getFileHandler();

    std::string tableName;
private:
    int tryOpenFile();

    int tryOpenIndex(int indexNo);

    int tryOpenForeignIndex(int constNo);

    std::vector<BindAttribute> attrInfos;
    int recordSize;

    ColumnDecsList columns;
    TableConstraintList tableConstraints;
    RM_FileHandle fileHandle;
    std::vector<IX_IndexHandle *> indexHandles;
    std::vector<std::unique_ptr<Table>> foreignTables;
    std::vector<int> foreignAttrInt;
    std::vector<int> constrAttrI;
};


#endif //MYDB_TABLE_H
