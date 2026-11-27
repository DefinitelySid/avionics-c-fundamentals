#include <stdio.h>
#include <stdint.h>

#define FDR_SIZE 5

#define FLAG_NORMAL    0x00
#define FLAG_STALL     (1 << 0)  
#define FLAG_OVERSPEED (1 << 1)  
#define FLAG_CRASH     (1 << 7)  

typedef struct {
  
    uint32_t frame_id;
    uint32_t altitude;
    int16_t IAS;
    uint8_t status_flags;
    uint8_t checksum;   
  
} FDRdata;

typedef struct {
    FDRdata buffer[FDR_SIZE];
    uint8_t head;
    uint8_t count;
    uint8_t is_frozen;     // 0 = Recording active, 1 = LOCKED after crash!
} BlackBox;

uint8_t calculate_checksum(FDRdata frame) {
    
    uint8_t cs = 0x00;

    cs ^= (uint8_t)(frame.frame_id & 0xFF);         // Byte 0 (bits 0..7)
    cs ^= (uint8_t)((frame.frame_id >> 8) & 0xFF);  // Byte 1 (bits 8..15)
    cs ^= (uint8_t)((frame.frame_id >> 16) & 0xFF); // Byte 2 (bits 16..23)
    cs ^= (uint8_t)((frame.frame_id >> 24) & 0xFF); // Byte 3 (bits 24..31)

    cs ^= (uint8_t)((frame.altitude & 0xFF));
    cs ^= (uint8_t)((frame.altitude >> 8) & 0xFF);
    cs ^= (uint8_t)((frame.altitude >> 16) & 0xFF);
    cs ^= (uint8_t)((frame.altitude >> 24) & 0xFF);

    cs ^= (uint8_t)(frame.IAS & 0xFF);
    cs ^= (uint8_t)((frame.IAS >> 8) & 0xFF);

    cs ^= frame.status_flags;
    
    return cs;
}

void fdr_init(BlackBox *fdr) {

    fdr->head = 0;
    fdr->count = 0;
    fdr->is_frozen = 0;

    for (int i = 0; i < FDR_SIZE; i++) {
      
      fdr->buffer[i].altitude = 0;
      fdr->buffer[i].IAS = 0;
      fdr->buffer[i].frame_id = 0;
      fdr->buffer[i].checksum = 0;
      fdr->buffer[i].status_flags = 0;

    }

}

//but fdr push only fires after everywrap i think na?? then what if a crash occurs less than 5
//frames before it fully captures? or is it that iirc yesterday we did something for that and tested with a 2 second case i forgot.

void fdr_push(BlackBox *fdr, FDRdata frame) {

   if (fdr->is_frozen == 1) {
    printf("[FDR LOCKED] Ignoring post-crash frame #%u\n");
    return;
   }
   
   frame.checksum = calculate_checksum(frame);

   fdr->buffer[fdr->head] = frame;
   
   fdr->head = (fdr->head + 1) % FDR_SIZE;

   if (fdr->count < FDR_SIZE) { fdr->count++; }

   if (frame.status_flags & FLAG_CRASH) {
    fdr->is_frozen = 1;
    printf("!!! CRASH DETECTED ON FRAME #%u — FDR FROZEN !!!\n", frame.frame_id);
}



}

void fdr_dump_crash_log(const BlackBox *fdr) {

    if (fdr->count == 0) {

      printf("Couldn't log anything / flight data unavailable");
      return;

    }  

    uint8_t startIndex = (fdr->count < FDR_SIZE) ? 0 : fdr->head;

    printf("=== BLACK BOX CRASH DUMP (%u frames) ===\n", fdr->count);
   
    for (uint8_t i = 0; i < fdr->count; i++) {
        uint8_t idx = (startIndex + i) % FDR_SIZE;


        FDRdata frame = fdr->buffer[idx];

        uint8_t expected_cs = calculate_checksum(frame);
        uint8_t is_valid = (expected_cs == frame.checksum);

        printf("[Slot %u] Frame #%u | Alt: %u ft | IAS: %d kts | Flags: 0x%02X | CS: 0x%02X (%s)\n",
               idx, frame.frame_id, frame.altitude, frame.IAS, frame.status_flags,
               frame.checksum, is_valid ? "OK" : "CORRUPTED!");
            
    }


}


int main() {
    BlackBox fdr;
    fdr_init(&fdr);

    // 1. Normal flight frames 1 to 5 (fills buffer)
    for (uint32_t id = 1; id <= 5; id++) {
        FDRdata sample = {id, 10000 + (id * 500), 250 + (id * 5), FLAG_NORMAL, 0};
        fdr_push(&fdr, sample);
    }

    // 2. Frame 6 triggers a CRASH! (Should save Frame 6, then FREEZE the Black Box)
    FDRdata crash_frame = {6, 200, 390, FLAG_OVERSPEED | FLAG_CRASH, 0};
    fdr_push(&fdr, crash_frame);

    // 3. Post-impact electrical garbage frames 7 & 8 (Should be BLOCKED by is_frozen!)
    FDRdata noise1 = {7, 0, 999, FLAG_NORMAL, 0};
    FDRdata noise2 = {8, 88888, -50, FLAG_STALL, 0};
    fdr_push(&fdr, noise1);
    fdr_push(&fdr, noise2);

    // 4. Simulate a fire/radiation bit-flip inside Slot 3 AFTER saving!
    fdr.buffer[3].altitude ^= (1 << 10); // Flip bit 10 of altitude!

    // 5. Dump the crash log: verify Frames 7 & 8 were blocked AND Slot 3 flags CORRUPTED!
    fdr_dump_crash_log(&fdr);

    return 0;
}