#pragma once

typedef enum sfmStatus {
    SFM_SUCCESS = 0,
    SFM_INVALID_PARAM = -1,
    SFM_INVALID_DESC = -2,
    SFM_ALLOC_MEMORY_FAILED = -3,
    SFM_INTERNAL_ERROR = -4,
    SFM_NULL_PTR = -5,
    SFM_UNKNOWN_ERROR = -6
} sfmStatus_t;