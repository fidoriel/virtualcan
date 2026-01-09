
#include "can_message.h"
#include "util.h"
#include <stdio.h>

namespace virtualcan {

void print_can_message(const CanMessage* can_msg)
{
    int i;
    printf("Can msg: id=%X ", can_msg->id);
    printf("ext=%u rtr=%u err=%u data=", can_msg->extended, can_msg->rtr, can_msg->err);
    for (i = 0; i < can_msg->data_size; i++) {
        printf("%02X ", can_msg->data[i]);
    }

    printf("\n");
}

// serialize a can message in some format
void pack_can_msg(uint8_t* buffer, CanMessage* msg)
{
    pack_u32(buffer, msg->id);
    uint8_t flags = 0;
    if (msg->extended) {
        flags |= 1;
    }
    if (msg->rtr) {
        flags |= 2;
    }
    if (msg->err) {
        flags |= 4;
    }
    buffer[4] = flags;
    buffer[5] = msg->data_size;
    int i;
    for (i = 0; i < msg->data_size; i++) {
        buffer[6 + i] = msg->data[i];
    }
}

void unpack_can_msg(uint8_t* buffer, CanMessage* msg)
{
    msg->id = unpack_u32(buffer);
    uint8_t flags = buffer[4];
    msg->extended = (flags & 1) ? 1 : 0;
    msg->rtr = (flags & 2) ? 1 : 0;
    msg->err = (flags & 4) ? 1 : 0;
    msg->data_size = buffer[5];

    int i;
    for (i = 0; i < msg->data_size; i++) {
        msg->data[i] = buffer[6 + i];
    }
}

}
