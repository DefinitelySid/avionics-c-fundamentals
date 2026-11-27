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


void decode_arinc429(uint32_t word) {

   uint32_t packetVal = count_ones(word);

   if (packetVal % 2 == 0) {

      printf("ERRORRR RETURNING THIS SHIT AND SENDING ANOTHERRR DWW <33");

      return;
   }

  uint8_t  label = word & 0xFF;           
  uint32_t data  = (word >> 8) & 0x1FFFFF; 
  uint8_t  ssm   = (word >> 29) & 0x03;    

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
