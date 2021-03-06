//
// Created by Zhengxiao Du on 2018-12-09.
//

#ifndef MYDB_QL_MANAGER_H
#define MYDB_QL_MANAGER_H

#include "../parser/Tree.h"
#include "../rm/RM_Record.h"
#include "../rm/RM_FileHandle.h"
#include "../sm/sm.h"
#include "Table.h"
#include "../Constants.h"
#include <functional>
#include <memory>
#include <list>

class QL_Manager {
private:
    SM_Manager sm;

    std::vector<RM_Record> recordCaches;

    using CallbackFunc = std::function<void(const RM_Record &)>;

    using MultiTableFunc = std::function<void(const std::vector<RM_Record> &)>;


    void printException(const AttrBindException &exception);

    int iterateTables(Table &table, Expr *condition, CallbackFunc callback);

    int
    iterateTables(std::vector<std::unique_ptr<Table>> &tables, int current, Expr *condition, MultiTableFunc callback,
                  std::list<Expr *> &indexExprs);

    void bindAttribute(Expr *expr, const std::vector<std::unique_ptr<Table>> &tables);

    int openTables(const std::vector<std::string> &tableNames, std::vector<std::unique_ptr<Table>> &tables);

    int whereBindCheck(Expr *expr, std::vector<std::unique_ptr<Table>> &tables);

public:
    // select attributes on relations with where condition and group attributes
    int exeSelect(AttributeList *attributes, IdentList *relations, Expr *whereClause, const std::string &groupAttrName);

    // insert into relation table(可以用columnList指定要插入的列)
    int exeInsert(std::string relation, IdentList *columnList, ConstValueLists *insertValueTree);

    // update relation table as setClauses with where condition
    int exeUpdate(std::string relation, SetClauseList *setClauses, Expr *whereClause);

    // delete the data in relation table with where condition
    int exeDelete(std::string relation, Expr *whereClause);

    static QL_Manager &getInstance();
};

#define QL_TABLE_FAIL (START_QL_WARN + 1)
#define QL_TYPE_CHECK (START_QL_WARN + 2)

#endif //MYDB_QL_MANAGER_H
