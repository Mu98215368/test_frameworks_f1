#pragma once
namespace bsp
{
enum class ErrorCode : int
{
    OK = 0,
    TIMEOUT     = -1,
    CHECK_FAIL  = -2,
    INIT_FAIL   = -3,
    INVALID_ARG = -4,
    BUS_BUSY    = -5,
    NO_DATA     = -6,
    ERROR       = -7
};
}
