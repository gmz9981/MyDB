//
// Created by Zhengxiao Du on 2018/11/24.
//

#include "Tree.h"
#include <utility>
#include <cstdlib>
#include <cstring>

using std::string;
using std::vector;

#include <stdarg.h>

static void MyDebugPrintf(const char *format, ...) {
    va_list argPtr;
    int count;

    va_start(argPtr, format);                  /*  获取可变参数列表  */
    fflush(stdout);                            /*  强制刷新输出缓冲区  */
    count = vfprintf(stderr, format, argPtr);  /*  将信息输出到标准出错流设备  */
    va_end(argPtr);                            /*  可变参数列表结束  */
}

#ifdef DEBUG   /*  如果定义了插桩信息宏，就将调试信息指向调试函数  */
#define DebugPrintf  MyDebugPrintf

#else           /*  如果未定义插桩信息宏，那么就将调试信息指向空NOP  */
#define DebugPrintf

#endif

Tree *Tree::tree = nullptr;

/* SelectTree */
Select::Select(IdentList *relations, WhereClauseTree *whereClause) {
    this->attributes = nullptr;
    this->relations = relations;
    this->whereClause = whereClause;
    this->groupAttrName = string();
}

Select::Select(AttributeList *attributes,
               IdentList *relations,
               WhereClauseTree *whereClause,
               const char *groupAttrName) {
    this->attributes = attributes;
    this->relations = relations;
    this->whereClause = whereClause;
    this->groupAttrName = string(groupAttrName);
}

Select::~Select() {
    delete attributes;
    delete relations;
    delete whereClause;
}

void Select::visit() {
    DebugPrintf("select\n");
//    vector<AttributeTree::AttributeDescriptor> attrs;
//    if (attributes) {
//        attrs = attributes->getDescriptors();
//    } else {
//        attrs = vector<AttributeTree::AttributeDescriptor>();
//    }
//    vector<string> rels = relations->getRelations();
//    vector<Comparison::ComparisonDescriptor> coms;
//    if (whereClause != nullptr) {
//        coms = whereClause->getComparision();
//        SystemManager::instance()->Select(attrs, rels, coms, groupAttrName);
//    } else {
//        SystemManager::instance()->Select(attrs, rels, coms, groupAttrName);
//    }
}


/* InsertTree */
Insert::Insert(const char *relationName, ConstValueLists *insertValueTree) {
    this->relationName = string(relationName);
    this->insertValueTree = insertValueTree;
}

Insert::~Insert() {
    delete insertValueTree;
}

void Insert::visit() {
    DebugPrintf("insert\n");
//    for(const auto& constValues : insertValueTree->values) {
//        SystemManager::instance()->Insert(relationName, constValues->getConstValues());
//    }
}

/* UpdateTree */
Update::Update(string relationName,
               SetClauseList *setClauseList,
               WhereClauseTree *whereClause) {
    this->relationName = std::move(relationName);
    this->setClauses = setClauseList;
    this->whereClause = whereClause;
}

Update::~Update() {
    delete setClauses;
    delete whereClause;
}

void Update::visit() {
    DebugPrintf("update\n");
//    AttributeTree::AttributeDescriptor attr = attribute->getDescriptor();
//    AttrValue val = constValue->getDescriptor();
//    vector<Comparison::ComparisonDescriptor> coms = whereClause->getComparision();
//    SystemManager::instance()->Update(relationName, attr, val, coms);
}

/* DeleteTree */
Delete::Delete(const char *relationName, WhereClauseTree *whereClause) {
    this->relationName = string(relationName);
    this->whereClause = whereClause;
}

Delete::~Delete() {
    delete whereClause;
}

void Delete::visit() {
    DebugPrintf("delete\n");
//    vector<Comparison::ComparisonDescriptor> coms;
//    if (whereClause != nullptr)
//        coms = whereClause->getComparision();
//    SystemManager::instance()->Delete(relationName, coms);
}

/* CreateDatabaseTree */
CreateDatabase::CreateDatabase(const char *dbName) {
    this->dbName = string(dbName);
}

CreateDatabase::~CreateDatabase() = default;

void CreateDatabase::visit() {
    DebugPrintf("create database %s\n", dbName.c_str());
//    SystemManager::instance()->createDB(dbName.c_str());
}

/* CreateTableTree */
CreateTable::CreateTable(const char *tableName, ColumnDecsList *columns) {
    this->tableName = string(tableName);
    this->columns = columns;
}

CreateTable::~CreateTable() {
    delete columns;
}

void CreateTable::visit() {
    DebugPrintf("create table %s\n", tableName.c_str());
//    int attrCount = columns->getColumnCount();
//    AttrInfo *attrInfos = columns->getAttrInfos();
//    SystemManager::instance()->createTable(tableName.c_str(), attrCount, attrInfos);
//    columns->deleteAttrInfos();
}


/* CreateIndexTree */
CreateIndex::CreateIndex(const char *relName, AttributeNode *attr) {
    this->relName = string(relName);
    this->attribute = attr;
}

CreateIndex::~CreateIndex() {
    delete attribute;
}

void CreateIndex::visit() {
    DebugPrintf("create index %s\n", relName.c_str());
//    auto attr = attribute->getDescriptor();
//    SystemManager::instance()->createIndex(tableName, attr);
}


DropIndex::DropIndex(const char *relName, AttributeNode *attr) {
    this->tableName = string(relName);
    this->attribute = attr;
}

DropIndex::~DropIndex() {
    delete attribute;
}

void DropIndex::visit() {
    DebugPrintf("drop index %s\n", tableName.c_str());
//    auto attr = attribute->getDescriptor();
//    SystemManager::instance()->dropIndex(tableName, attr);
}


/* DropDatabaseTree */
DropDatabase::DropDatabase(const char *dbName) {
    this->dbName = string(dbName);
}

DropDatabase::~DropDatabase() = default;

void DropDatabase::visit() {
//    SystemManager::instance()->dropDB(dbName.c_str());
}

/* DropTableTree */
DropTable::DropTable(const char *tableName) {
    this->tableName = string(tableName);
}

DropTable::~DropTable() = default;

void DropTable::visit() {
    DebugPrintf("drop table %s\n", tableName.c_str());
//    SystemManager::instance()->dropTable(tableName.c_str());
}

///* ColumnsTree */
//ColumnsTree::ColumnsTree() {
//}
//
//ColumnsTree::~ColumnsTree() {
//    for (const auto &column : columns)
//        delete column;
//}
//
//void ColumnsTree::addColumn(ColumnTree *column) {
//    for (const auto &col : columns)
//        if (col->columnName == column->columnName)
//            Error("duplicated column name");
//    columns.push_back(column);
//}
//
//bool ColumnsTree::setPrimaryKey(const char *attr) {
//    bool found = false;
//    for(auto& tree : columns) {
//        if(tree->columnName == string(attr)) {
//            tree->isPrimaryKey = 1;
//            tree->notNull = 1;
//            found = true;
//            break;
//        }
//    }
//    return found;
//}
//
//
//int ColumnsTree::getColumnCount() {
//    return (int) columns.size();
//}
//
//AttrInfo *ColumnsTree::getAttrInfos() {
//    attrInfos = new AttrInfo[columns.size()];
//    for (int i = 0; i < columns.size(); ++i)
//        attrInfos[i] = columns[i]->getAttrInfo();
//    return attrInfos;
//}
//
//void ColumnsTree::deleteAttrInfos() {
//    delete attrInfos;
//    attrInfos = nullptr;
//}
//
/* ColumnNode */
ColumnNode::ColumnNode(const char *columnName, AttrType type, int size,
                       int columnFlag) {
    this->columnName = string(columnName);
    this->type = type;
    this->size = size;
    this->columnFlag = columnFlag;
    if (type == AttrType::STRING)
        this->size++;
}

ColumnNode::~ColumnNode() = default;

AttrInfo ColumnNode::getAttrInfo() {
    AttrInfo attrInfo;
    strcpy(attrInfo.attrName, columnName.c_str());
    attrInfo.attrType = (AttrType) type;
    attrInfo.attrLength = size;
    attrInfo.isPrimaryKey = isPrimaryKey;
    return attrInfo;
}


/* AttributeList */
AttributeList::AttributeList() = default;

AttributeList::~AttributeList() {
    for (const auto &attribute : attributes)
        delete attribute;
}

void AttributeList::addAttribute(AttributeNode *attribute) {
    attributes.push_back(attribute);
}

vector<AttributeNode::AttributeDescriptor> AttributeList::getDescriptors() const {
    vector<AttributeNode::AttributeDescriptor> attrs;
    for (auto attribute : attributes)
        attrs.push_back(attribute->getDescriptor());
    return attrs;
}

/* AttributeTree */
AttributeNode::AttributeNode(const char *relationName, const char *attributeName, AggregationType aggregationType) {
    this->table = string(relationName);
    this->attribute = string(attributeName);
    this->aggregationType = aggregationType;
}

AttributeNode::AttributeNode(const char *attributeName, AggregationType aggregationType) {
    this->attribute = string(attributeName);
    this->aggregationType = aggregationType;
}

AttributeNode::AttributeDescriptor AttributeNode::getDescriptor() const {
    return AttributeDescriptor(table, attribute, aggregationType);
}

bool AttributeNode::operator==(const AttributeNode &attribute) const {
    return this->table == attribute.table &&
           this->attribute == attribute.attribute &&
           this->aggregationType == attribute.aggregationType;
}

AttributeNode::~AttributeNode() = default;

/* WhereClauseTree */
WhereClauseTree::WhereClauseTree(ConditionsTree *conditions) {
    this->conditions = conditions;
}

WhereClauseTree::WhereClauseTree() {
    conditions = nullptr;
}

WhereClauseTree::~WhereClauseTree() {
    delete conditions;
}

vector<Comparison::ComparisonDescriptor> WhereClauseTree::getComparision() {
    return conditions->getComparisions();
}

/* ConditionsTree */
ConditionsTree::ConditionsTree() = default;

ConditionsTree::~ConditionsTree() {
    for (const auto &comparision : comparisons)
        delete comparision;
}

void ConditionsTree::addComparison(Comparison *comparison) {
    comparisons.push_back(comparison);
}

vector<Comparison::ComparisonDescriptor> ConditionsTree::getComparisions() {
    vector<Comparison::ComparisonDescriptor> coms;
    for (auto &comparison : comparisons)
        coms.push_back(comparison->getDescriptor());
    return coms;
}

/* Comparison */
Comparison::Comparison(AttributeNode *attribute) {
    this->op = CompOp::NO_OP;
    this->attribute = attribute;
    this->constValue = nullptr;
    this->isAttrCmp = false;
}

Comparison::Comparison(AttributeNode *attribute, CompOp op, ConstValueNode *constValue) {
    this->op = op;
    this->attribute = attribute;
    this->constValue = constValue;
    this->isAttrCmp = false;
}

Comparison::Comparison(AttributeNode *attribute, CompOp op, AttributeNode *attribute2) {
    this->op = op;
    this->attribute = attribute;
    this->attribute2 = attribute2;
    this->isAttrCmp = true;
}

Comparison::~Comparison() {
    delete attribute;
    if (isAttrCmp)
        delete attribute2;
    else
        delete constValue;
}

Comparison::ComparisonDescriptor Comparison::getDescriptor() {
    ComparisonDescriptor com;
    com.attr = this->attribute->getDescriptor();
    if (!isAttrCmp && this->constValue != nullptr)
        com.val = this->constValue->getDescriptor();
    else if (isAttrCmp)
        com.attr2 = this->attribute2->getDescriptor();
    com.isAttrCmp = isAttrCmp;
    com.op = this->op;
    return com;
}


/* ConstValuesTree */
ConstValueList::ConstValueList() = default;

ConstValueList::~ConstValueList() {
    for (const auto &constValue: constValues)
        delete constValue;
}

void ConstValueList::addConstValue(ConstValueNode *constValue) {
    constValues.push_back(constValue);
}

vector<AttrValue> ConstValueList::getConstValues() {
    vector<AttrValue> vals;
    for (auto &constValue : constValues)
        vals.push_back(constValue->getDescriptor());
    return vals;
}


/* ConstValueTree */
ConstValueNode::ConstValueNode(int i) {
    this->i = i;
    this->type = AttrType::INT;
    this->isNull = false;
}

ConstValueNode::ConstValueNode(float f) {
    this->f = f;
    this->type = AttrType::FLOAT;
    this->isNull = false;
}

ConstValueNode::ConstValueNode(const char *s) {
    //TODO check the length here
    this->s = string(s, 1, strlen(s) - 2);
    this->type = AttrType::STRING;
    this->isNull = false;
}

ConstValueNode::~ConstValueNode() = default;

AttrValue ConstValueNode::getDescriptor() {
    return AttrValue{type, i, f, s, isNull};
}

bool AttrValue::operator==(const AttrValue &val) const {
    if (this->isNull || val.isNull)
        return false;
    if ((type == AttrType::INT || type == AttrType::FLOAT) &&
        (val.type == AttrType::INT || val.type == AttrType::FLOAT)) {
        float f1 = this->type == AttrType::INT ? this->i : this->f;
        float f2 = val.type == AttrType::INT ? val.i : val.f;
        return f1 == f2;
    } else if (type == AttrType::STRING && val.type == AttrType::STRING) {
        return this->s == val.s;
    } else {
        // TODO date comparision
    }
    return false;
}

bool AttrValue::operator!=(const AttrValue &val) const {
    if (this->isNull || val.isNull)
        return false;
    if ((type == AttrType::INT || type == AttrType::FLOAT) &&
        (val.type == AttrType::INT || val.type == AttrType::FLOAT)) {
        float f1 = this->type == AttrType::INT ? this->i : this->f;
        float f2 = val.type == AttrType::INT ? val.i : val.f;
        return f1 != f2;
    } else if (type == AttrType::STRING && val.type == AttrType::STRING) {
        return this->s != val.s;
    } else {
        // TODO cannot compare
    }
    return false;
}

//bool AttrValue::operator >= (const AttrValue &val) const {
//    if(this->isNull || val.isNull)
//        return false;
//    if ((type == AttrType::INT || type == AttrType::FLOAT) &&
//        (val.type == AttrType::T_INT || val.type == AttrType::T_FLOAT)) {
//        float f1 = this->type == AttrType::T_INT ? this->i : this->f;
//        float f2 = val.type == AttrType::T_INT ? val.i : val.f;
//        return f1 >= f2;
//    } else if (type == AttrType::T_STRING && val.type == AttrType::T_STRING) {
//        return this->s >= val.s;
//    } else {
//        // TODO cannot compare
//    }
//    return false;
//}
//
//bool AttrValue::operator <= (const AttrValue &val) const {
//    if(this->isNull || val.isNull)
//        return false;
//    if ((type == AttrType::T_INT || type == AttrType::T_FLOAT) &&
//        (val.type == AttrType::T_INT || val.type == AttrType::T_FLOAT)) {
//        float f1 = this->type == AttrType::T_INT ? this->i : this->f;
//        float f2 = val.type == AttrType::T_INT ? val.i : val.f;
//        return f1 <= f2;
//    } else if (type == AttrType::T_STRING && val.type == AttrType::T_STRING) {
//        return this->s <= val.s;
//    } else {
//        // TODO cannot compare
//    }
//    return false;
//}
//
//bool AttrValue::operator > (const AttrValue &val) const {
//    if(this->isNull || val.isNull)
//        return false;
//    if ((type == AttrType::T_INT || type == AttrType::T_FLOAT) &&
//        (val.type == AttrType::T_INT || val.type == AttrType::T_FLOAT)) {
//        float f1 = this->type == AttrType::T_INT ? this->i : this->f;
//        float f2 = val.type == AttrType::T_INT ? val.i : val.f;
//        return f1 > f2;
//    } else if (type == AttrType::T_STRING && val.type == AttrType::T_STRING) {
//        return this->s > val.s;
//    } else {
//        // TODO cannot compare
//    }
//    return false;
//}
//
//bool AttrValue::operator < (const AttrValue &val) const {
//    if(this->isNull || val.isNull)
//        return false;
//    if ((type == AttrType::T_INT || type == AttrType::T_FLOAT) &&
//        (val.type == AttrType::T_INT || val.type == AttrType::T_FLOAT)) {
//        float f1 = this->type == AttrType::T_INT ? this->i : this->f;
//        float f2 = val.type == AttrType::T_INT ? val.i : val.f;
//        return f1 < f2;
//    } else if (type == AttrType::T_STRING && val.type == AttrType::T_STRING) {
//        return this->s < val.s;
//    } else {
//        // TODO cannot compare
//    }
//    return false;
//}

UseDatabase::UseDatabase(const char *dbName) {
    this->dbName = dbName;
}

void UseDatabase::visit() {
    DebugPrintf("Use %s\n", dbName.c_str());
//    SystemManager::instance()->openDB(dbName.c_str());
}

DescTable::DescTable(const char *relName) {
    this->tableName = string(relName);
}

DescTable::~DescTable() = default;

void DescTable::visit() {
    DebugPrintf("desc table %s\n", tableName.c_str());
//    if(tableName.empty())
//        SystemManager::instance()->help();
//    else
//        SystemManager::instance()->help(tableName.c_str());
}

ConstValueLists::ConstValueLists() = default;

ConstValueLists::~ConstValueLists() {
    for (auto v : values) {
        delete v;
    }
}

void ConstValueLists::addConstValues(ConstValueList *constValuesTree) {
    values.push_back(constValuesTree);
}

SetClauseList::SetClauseList() = default;

void SetClauseList::addSetClause(AttributeNode *attr, ConstValueNode *value) {
    this->clauses.emplace_back(attr, value);
}

IdentList::IdentList() = default;

void IdentList::addIdent(const char *ident) {
    this->idents.emplace_back(ident);
}

IdentList::~IdentList() = default;

ShowDatabases::ShowDatabases() = default;

void ShowDatabases::visit() {
    DebugPrintf("show databases\n");
//    Tree::visit();
}

ColumnDecsList::ColumnDecsList() = default;

ColumnDecsList::~ColumnDecsList() {
    for (auto v: columns) {
        delete v;
    }
}

void ColumnDecsList::addColumn(ColumnNode *column) {
    columns.push_back(column);
}

TbOptDec::TbOptDec(IdentList *column_list) : column_list(column_list) {

}

TbOptDec::TbOptDec(const char *foreign_key, const char *table, const char *column) : column_list(nullptr),
                                                                                     foreign_key(foreign_key),
                                                                                     table(table), column(column) {

}

TbOptDec::~TbOptDec() {
    delete column_list;
}

void TbOptDecList::addTbDec(TbOptDec *dec) {
    tbDecs.push_back(dec);
}

TbOptDecList::~TbOptDecList() {
    for(auto v: tbDecs) {
        delete v;
    }
}
