//
// Created by Zhengxiao Du on 2018-12-09.
//

#include "Expr.h"
#include "Tree.h"
#include <regex>

Expr::Expr(int i) {
    is_null = false;
    calculated = true;
    value.i = i;
    nodeType = NodeType::CONST_NODE;
    oper.constType = AttrType::INT;
}

Expr::Expr(float f) {
    is_null = false;
    calculated = true;
    value.f = f;
    nodeType = NodeType::CONST_NODE;
    oper.constType = AttrType::FLOAT;
}

Expr::Expr(const char *s) {
    is_null = false;
    calculated = true;
    value_s = std::string(s, 1, strlen(s) - 2);;
    nodeType = NodeType::CONST_NODE;
    oper.constType = AttrType::STRING;
}

Expr::~Expr() {
    delete left;
    delete right;
}

Expr::Expr() = default;

Expr::Expr(Expr *left, ArithOp op, Expr *right) {
    is_null = false;
    this->left = left;
    this->right = right;
    this->oper.arith = op;
    this->nodeType = NodeType::ARITH_NODE;
}

Expr::Expr(Expr *left, CompOp op, Expr *right) {
    is_null = false;
    this->left = left;
    this->right = right;
    this->oper.comp = op;
    this->nodeType = NodeType::COMP_NODE;
}

Expr::Expr(Expr *left, LogicOp op, Expr *right) {
    is_null = false;
    this->left = left;
    this->right = right;
    this->oper.logic = op;
    this->nodeType = NodeType::LOGIC_NODE;
}

Expr::Expr(AttributeNode *attributeNode) {
    is_null = false;
    this->attribute = attributeNode;
    this->nodeType = NodeType::ATTR_NODE;
    this->attrInfo.attrName = attributeNode->attribute;
    this->attrInfo.tableName = attributeNode->table;
}

template<typename T>
bool comp_function(const T &a, const T &b, CompOp compOp) {
    switch (compOp) {
        case CompOp::EQ_OP:
            return a == b;
        case CompOp::GE_OP:
            return not(a < b);
        case CompOp::GT_OP:
            return not(a < b) and not(a == b);
        case CompOp::LE_OP:
            return a < b or a == b;
        case CompOp::LT_OP:
            return a < b;
        case CompOp::NE_OP:
            return not(a == b);
        case CompOp::NO_OP:
            return true;
        case CompOp::IS_OP:
            // todo implement this
            break;
        default:
            return false;
    }
    return false;
}

bool comp_function(const std::string &a, const std::string &b, CompOp compOp) {
    if (compOp == CompOp::LIKE_OP) {
        std::string regexStr;
        char status = 'A';
        for (char i : b) {
            if (status == 'A') {
                // common status
                if (i == '\\') {
                    status = 'B';
                } else if (i == '[') {
                    regexStr += "[";
                    status = 'C';
                } else if (i == '%') {
                    regexStr += ".*";
                } else if (i == '_') {
                    regexStr += ".";
                } else {
                    regexStr += i;
                }
            } else if (status == 'B') {
                // after '\'
                if (i == '%' || i == '_' || i == '!') {
                    regexStr += i;
                } else {
                    regexStr += "\\";
                    regexStr += i;
                }
                status = 'A';
            } else {
                // after '[' inside []
                if (i == '!') {
                    regexStr += "^";
                } else {
                    regexStr += i;
                }
                status = 'A';
            }
        }
        std::regex reg(regexStr);
        return std::regex_match(a, reg);
    } else {
        switch (compOp) {
            case CompOp::EQ_OP:
                return a == b;
            case CompOp::GE_OP:
                return a >= b;
            case CompOp::GT_OP:
                return a > b;
            case CompOp::LE_OP:
                return a <= b;
            case CompOp::LT_OP:
                return a < b;
            case CompOp::NE_OP:
                return a != b;
            case CompOp::NO_OP:
                return true;
            case CompOp::IS_OP:
                // todo implement this
                break;
            default:
                return false;
        }
    }
    return false;
}

template<typename T>
T arith_function(T a, T b, ArithOp arithOp) {
    switch (arithOp) {
        case ArithOp::ADD_OP:
            return a + b;
        case ArithOp::SUB_OP:
            return a - b;
        case ArithOp::MUL_OP:
            return a * b;
        case ArithOp::DIV_OP:
            return a / b;
        case ArithOp::MINUS_OP:
            return -a;
        case ArithOp::NO_OP:
            break;
    }
    return T{};
}

bool logic_function(bool a, bool b, LogicOp logicOp) {
    switch (logicOp) {
        case LogicOp::AND_OP:
            return a and b;
        case LogicOp::OR_OP:
            return a or b;
        case LogicOp::NO_OP:
            break;
    }
    return false;
}


void Expr::calculate(char *data, const std::string &relationName) {
    if (left != nullptr) {
        left->calculate(data, relationName);
    }
    if (right != nullptr) {
        right->calculate(data, relationName);
    }
    if (left != nullptr and !left->calculated) {
        return;
    }
    if (right != nullptr and !right->calculated) {
        return;
    }
    switch (this->nodeType) {
        case NodeType::ARITH_NODE:
            if (not calculated) {
                calculated = true;
                switch (this->dataType) {
                    case AttrType::INT:
                        if (this->oper.arith == ArithOp::MINUS_OP) {
                            this->value.i = arith_function(left->value.i, left->value.i, this->oper.arith);
                        } else {
                            this->value.i = arith_function(left->value.i, right->value.i, this->oper.arith);
                        }
                        break;
                    case AttrType::FLOAT:
                        if (this->oper.arith == ArithOp::MINUS_OP) {
                            this->value.f = arith_function(left->value.f, left->value.f, this->oper.arith);
                        } else {
                            this->value.f = arith_function(left->value.f, right->value.f, this->oper.arith);
                        }
                        break;
                    case AttrType::STRING:
                    case AttrType::DATE:
                    case AttrType::VARCHAR:
                    case AttrType::NO_ATTR:
                        break;
                    case AttrType::BOOL:
                        break;
                }
            }
            break;
        case NodeType::COMP_NODE:
            if (not calculated) {
                calculated = true;
                switch (left->dataType) {
                    case AttrType::INT:
                    case AttrType::DATE:
                        this->value.b = comp_function(left->value.i, right->value.i, oper.comp);
                        break;
                    case AttrType::FLOAT:
                        this->value.b = comp_function(left->value.f, right->value.f, oper.comp);
                        break;
                    case AttrType::STRING:
                        this->value.b = comp_function(left->value_s, right->value_s, oper.comp);
                        break;
                    case AttrType::VARCHAR:
                    case AttrType::NO_ATTR:
                        break;
                    case AttrType::BOOL:
                        break;
                }
            }
            break;
        case NodeType::LOGIC_NODE:
            if (not calculated) {
                calculated = true;
                this->value.b = logic_function(left->value.b, right->value.b, oper.logic);
                break;
            }
        case NodeType::CONST_NODE:
            calculated = true;
            break;
        case NodeType::ATTR_NODE:
            if (not calculated) {
                if (relationName == attrInfo.attrName) {
                    calculated = true;
                    switch (this->attrInfo.attrType) {
                        case AttrType::INT:
                        case AttrType::DATE:
                            this->value.i = *reinterpret_cast<int *>(data + this->attrInfo.attrOffset);
                            if (dataType == AttrType::FLOAT) {
                                this->value.f = static_cast<float>(this->value.i);
                            }
                            break;
                        case AttrType::FLOAT:
                            this->value.f = *reinterpret_cast<float *>(data + this->attrInfo.attrOffset);
                            break;
                        case AttrType::STRING:
                            this->value_s = std::string(data + this->attrInfo.attrOffset);
                            break;
                        case AttrType::VARCHAR:
                        case AttrType::NO_ATTR:
                            break;
                        case AttrType::BOOL:
                            break;
                    }
                }
                break;
            }
    }
}

void Expr::type_check() {
    postorder([](Expr *expr) -> void {
        switch (expr->nodeType) {
            case NodeType::ARITH_NODE:
                switch (expr->oper.arith) {
                    case ArithOp::ADD_OP:
                    case ArithOp::SUB_OP:
                    case ArithOp::MUL_OP:
                    case ArithOp::DIV_OP:
                        if ((expr->left != nullptr and expr->left->dataType == AttrType::FLOAT) or
                            (expr->right != nullptr and expr->right->dataType == AttrType::FLOAT)) {
                            expr->dataType = AttrType::FLOAT;
                            if (expr->left != nullptr and expr->left->dataType != AttrType::FLOAT) {
                                expr->left->convert_to_float();
                            }
                            if (expr->right != nullptr and expr->right->dataType != AttrType::FLOAT) {
                                expr->right->convert_to_float();
                            }
                        } else {
                            expr->dataType = AttrType::INT;
                        }
                        break;
                    case ArithOp::MINUS_OP:
                        expr->dataType = expr->left->dataType;
                        break;
                    case ArithOp::NO_OP:
                        expr->dataType = AttrType::NO_ATTR;
                        break;
                }
                break;
            case NodeType::LOGIC_NODE:
                expr->dataType = AttrType::BOOL;
                break;
            case NodeType::ATTR_NODE:
                expr->dataType = expr->attrInfo.attrType;
                break;
            case NodeType::COMP_NODE:
                expr->dataType = AttrType::BOOL;
                break;
            case NodeType::CONST_NODE:
                expr->dataType = expr->oper.constType;
                break;
        }
    });
}

void Expr::convert_to_float() {
    postorder([](Expr *expr) -> void {
        switch (expr->nodeType) {
            case NodeType::ARITH_NODE:
                expr->dataType = AttrType::FLOAT;
                break;
            case NodeType::LOGIC_NODE:
                break;
            case NodeType::ATTR_NODE:
                expr->dataType = AttrType::FLOAT;
                break;
            case NodeType::COMP_NODE:
                break;
            case NodeType::CONST_NODE:
                switch (expr->oper.constType) {
                    case AttrType::INT:
                    case AttrType::DATE:
                        expr->dataType = AttrType::FLOAT;
                        expr->value.f = static_cast<float>(expr->value.i);
                        break;
                    case AttrType::FLOAT:
                        break;
                    case AttrType::STRING:
                        break;
                    case AttrType::VARCHAR:
                        break;
                    case AttrType::BOOL:
                        break;
                    case AttrType::NO_ATTR:
                        break;
                }
                break;
        }
    }, [](Expr *expr) -> bool {
        return expr->dataType == AttrType::FLOAT;
    });
}

void Expr::postorder(std::function<void(Expr *)> callback, std::function<bool(Expr *)> stop_condition) {
    if (stop_condition != nullptr) {
        if (stop_condition(this)) {
            return;
        }
    }
    if (left != nullptr) {
        callback(left);
    }
    if (right != nullptr) {
        callback(right);
    }
    callback(this);
}

void Expr::init_calculate() {
    postorder([](Expr *expr) -> void {
        if (expr->nodeType != NodeType::CONST_NODE) {
            expr->calculated = false;
        }
    });
}

