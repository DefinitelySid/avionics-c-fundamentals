#include <stdio.h>
#include <stdint.h>

int count_ones(uint32_t word) {
    int count = 0;
    for (int i = 0; i < 32; i++) {
        if ((word >> i) & 1) {
            count++;
        }
    } 
    return count; 
}

// test data
//0000 0000 1100 0101 
//0001 1111 1111 1111 1111 1111 

// 0000 0000 1100 0101
// 0000 0000 0001 1100
// 0000 0000 0000 0100



uint32_t encode_arinc429(uint8_t label, uint32_t data, uint8_t ssm) {
    uint32_t word = 0;
    
   word |= (label & 0xFF);

   word |= ((data & 0x1FFFFF) << 8);

   word |= ((ssm & 0x03) << 29 );

   uint32_t packet = count_ones(word);
   
   if (packet % 2 == 0) {

    word |= (1U << 31);

   }
   

    return word;
}


// testing label word decode for example packet is 0x3103

// 0011 0001 0000 0011
// now mask with 0xFF to isolate
// 0011 0001 0000 0011
// 0000 0000 1111 1111
//now & operator
// 0000 0000 0000 0011
//HELP WHY DID WE GET THE SMM VALUE BAHAHAHAH LMAO



void decode_arinc429(uint32_t word) {

   uint32_t packetVal = count_ones(word);

   if (packetVal % 2 == 0) {

      printf("ERRORRR RETURNING THIS SHIT AND SENDING ANOTHERRR DWW <33");

      return;
   }

  uint8_t  label = word & 0xFF;            // 8 bits -> uint8_t is perfect!
  uint32_t data  = (word >> 8) & 0x1FFFFF; // 21 bits -> MUST be uint32_t!
  uint8_t  ssm   = (word >> 29) & 0x03;    // 2 bits -> uint8_t is perfect!

  printf("Parity OK! Unpacked Packet:\n");
printf("Label: 0x%02X\n", label);
printf("Data:  %u ft\n", data);
printf("SSM:   %d\n", ssm);


}

int main() {
    uint32_t packet = encode_arinc429(0x84, 35000, 3);
    // printf("Encoded Word: 0x%08X\n", packet);
    // printf("Total 1s:     %d\n", count_ones(packet));
    uint32_t corrupted = packet ^ (1U << 15);
    decode_arinc429(corrupted);

    return 0;
}
