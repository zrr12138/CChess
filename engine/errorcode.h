//
// Created by zhengran on 2024/4/6.
//

#ifndef CCHESS_ERROR_CODE_H
#define CCHESS_ERROR_CODE_H
namespace CChess {
    enum Errorcode {
        OK = 0,
#define DECLARE_CCHESS_ERRORCODE(name, errorcode) name = errorcode,

#include "store_errorcode.inc"

#undef DECLARE_KWAISTORE_ERRORCODE
    };
}
#endif //CCHESS_ERROR_CODE_H
