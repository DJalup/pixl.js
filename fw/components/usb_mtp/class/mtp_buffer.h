//
// Created by solos on 2021/12/1.
//

#ifndef FW_MTP_BUFFER_H
#define FW_MTP_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t *buff;
    uint32_t pos;
    uint32_t limit;
    size_t cap;
} mtp_buffer_t;


#define MTP_NEW_BUFFER(name, _buff, _cap)                 \
    mtp_buffer_t name = {                         \
       .buff = (_buff),                                  \
       .pos = 0,                                        \
       .limit  = 0,                                     \
       .cap = (_cap)                                     \
    }

#define MTP_NEW_BUFFER_LOCAL(name, _cap)            \
    uint8_t _##name##_data[(_cap)];                  \
    mtp_buffer_t name = {                               \
       .buff = _##name##_data,                           \
       .pos = 0,                                        \
       .limit  = 0,                                     \
       .cap = (_cap)                                \
    }

/**********************************************************************************/
/* MTP support functions */
/**********************************************************************************/
/*!
 *  @brief    Put uint64_t into global array - mtp_transmit_buff
 *
 *
 *  @param[in]   : value
 *
 *  @return      : None
 */
static inline void mtp_buff_put_u64(mtp_buffer_t *buffer, uint64_t value) {
    *(uint32_t *) (&buffer->buff[buffer->limit]) = value & 0xFFFFFFFF;
    buffer->limit += 4;
    *(uint32_t *) (&buffer->buff[buffer->limit]) = (value >> 32);
    buffer->limit += 4;
}

/*!
 *  @brief      Put uint32_t into global array - mtp_transmit_buff
 *
 *
 *  @param[in]   : value
 *
 *  @return      : None
 */
static inline void mtp_buff_put_u32(mtp_buffer_t *buffer, uint32_t value) {
    *(uint32_t *) (&buffer->buff[buffer->limit]) = value;
    buffer->limit += 4;
}

/*!
 *  @brief    Put uint16_t into global array - mtp_transmit_buff
 *
 *
 *  @param[in]   : value
 *
 *  @return      : None
 */
static inline void mtp_buff_put_u16(mtp_buffer_t *buffer, uint16_t value) {
    *(uint16_t *) (&buffer->buff[buffer->limit]) = value;
    buffer->limit += 2;
}

/*!
 *  @brief    Put uint8_t into global array - mtp_transmit_buff
 *
 *
 *  @param[in]   : value
 *
 *  @return      : None
 */
static inline void mtp_buff_put_u8(mtp_buffer_t *buffer, uint8_t value) {
    *(uint8_t *) (&buffer->buff[buffer->limit]) = value;
    buffer->limit += 1;
}

/*!
 *  @brief    Put byte array into global array - mtp_transmit_buff
 *
 *
 *  @param[in]   : value,length
 *
 *  @return      : None
 */
static inline void mtp_buff_put_byte_array(mtp_buffer_t *buffer, void *value, size_t len) {
    memcpy(&buffer->buff[buffer->limit], value, len);
    buffer->limit += len;
}

/*!
 *  @brief    Put string into global array - mtp_transmit_buff (uint8_t length and UTF-16LE string)
 *
 *
 *  @param[in]   : char *string
 *
 *  @return      : None
 */
static inline void mtp_buff_put_string(mtp_buffer_t *buffer, char *string) {
    if (*string) {
        mtp_buff_put_u8(buffer, strlen(string) + 1);
        while (*string) {
            mtp_buff_put_u16(buffer, *string);
            ++string;
        }
        mtp_buff_put_u16(buffer, 0);
    } else
        mtp_buff_put_u8(buffer, 0);
}

static inline uint64_t mtp_buff_get_u64(mtp_buffer_t *buffer) {
    uint64_t value = 0;
    value = *(uint32_t *) (&buffer->buff[buffer->pos]);
    buffer->pos += 4;
    value |= (uint64_t) (*(uint32_t *) (&buffer->buff[buffer->pos])) << 32;
    buffer->pos += 4;
    return value;
}

static inline uint32_t mtp_buff_get_u32(mtp_buffer_t *buffer) {
    uint32_t value = 0;
    value = *(uint32_t *) (&buffer->buff[buffer->pos]);
    buffer->pos += 4;
    return value;
}

static inline uint16_t mtp_buff_get_u16(mtp_buffer_t *buffer) {
    uint16_t value = 0;
    value = *(uint16_t *) (&buffer->buff[buffer->pos]);
    buffer->pos += 2;
    return value;
}

static inline uint8_t mtp_buff_get_u8(mtp_buffer_t *buffer) {
    uint8_t value = 0;
    value = buffer->buff[buffer->pos];
    buffer->pos += 1;
    return value;
}

/*!
 *  @brief    Read byte array from global variable - mtp_receive_buff
 *
 *
 *  @param[in]   : uint8_t *value, length
 *
 *  @return      : None
 */
static inline void mtp_buff_get_byte_array(mtp_buffer_t *buffer, uint8_t *value, size_t len) {
    if (value != NULL) {
        memcpy(value, &buffer->buff[buffer->pos], len);
    }
    buffer->pos += len;
}

/*!
 *  @brief    Read UTF-16 string from global variable - mtp_receive_buff
 *
 *
 *  @param[in]   : char *string
 *
 *  @return      : None
 */
static inline void mtp_buff_get_string(mtp_buffer_t *buffer, char *string) {
    int length = mtp_buff_get_u8(buffer);
    if (!string) {
        mtp_buff_get_byte_array(buffer, NULL, length * 2);
    } else {
        for (int i = 0; i < length; i++) {
            *(string++) = mtp_buff_get_u16(buffer);
        }
    }
}

static inline void mtp_buff_reset(mtp_buffer_t* buffer){
    buffer->pos = 0;
    buffer->limit = 0;
}

static inline void mtp_buff_reset_pos(mtp_buffer_t *buffer) {
    buffer->pos = 0;
}

static inline uint32_t mtp_buff_get_pos(mtp_buffer_t* buffer){
    return buffer->pos;
}

static inline size_t mtp_buff_get_size(mtp_buffer_t *buffer) {
    return buffer->limit;
}

static inline size_t mtp_buff_get_remain_size(mtp_buffer_t *buffer) {
    return buffer->limit - buffer->pos;
}

static inline void mtp_buff_reset_limit(mtp_buffer_t *buffer) {
    buffer->limit = 0;
}

static inline uint32_t mtp_buff_set_limit(mtp_buffer_t* buffer, uint32_t limit){
    buffer->limit = limit;
}

static inline uint8_t *mtp_buff_get_data(mtp_buffer_t *buffer) {
    return buffer->buff;
}

static inline uint8_t *mtp_buff_get_data_ptr_pos(mtp_buffer_t *buffer) {
    return buffer->buff + buffer->pos;
}

static inline uint8_t* mtp_buff_get_data_ptr_limit(mtp_buffer_t* buffer){
    return buffer->buff + buffer->limit;
}

static inline size_t mtp_buffer_get_available_cap(mtp_buffer_t* buffer){
    return buffer->cap - buffer->limit;
}
#endif //FW_MTP_BUFFER_H
