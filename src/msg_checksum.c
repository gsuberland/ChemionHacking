#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

uint8_t checkSum(uint8_t *in) {
    uint8_t checkSum = 0x07;                // Check Sum Magic Number

    int i;
    for (i = 7; i <= 61; i ++) {            // Check Sum for 54Byte LED Matrix
        checkSum ^= in[i];                  // Each Byte is XORed ontop of the last
    }
    return checkSum;
}


int main() {
    uint8_t msg[64] = {0xFA, 0x03, 0x00, 0x39, 0x01, 0x00, 0x06,  // Starting Bytes
                       0xC0, 0X00, 0X00, 0X00, 0X00, 0X00,        // LED Matrix
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00,
                       0x00, 0X00, 0X00, 0X00, 0X00, 0X00};

    msg[62] = checkSum(msg);    // Get checkSum
    msg[63] = 0x55;             // Ending Bytes
    msg[64] = 0xa9;             // Ending Bytes

    // Check values
    int i;
    for(i = 0; i <= 64; i++) {
        printf("Msg %i: %i\n", i, msg[i]);
    } 
}
