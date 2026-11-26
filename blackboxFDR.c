#include <stdio.h>
#include <stdint.h>

#define FDR_SIZE 5

typedef struct {

    uint32_t frame_id;
    uint32_t altitude;
    int16_t IAS;
    uint8_t status_flags;

} FDRdata;

typedef struct {
    FDRdata buffer[FDR_SIZE];
    uint8_t head;
    uint8_t count;
} BlackBox;

void fdr_init(BlackBox *fdr) {

  fdr->head = 0;
  fdr->count = 0;

  for (uint8_t i = 0; i < FDR_SIZE; i++) {
  
    fdr->buffer[i].frame_id = 0;
    fdr->buffer[i].altitude = 0;
    fdr->buffer[i].IAS = 0;
    fdr->buffer[i].status_flags = 0;

  }


}

void fdr_push(BlackBox *fdr, FDRdata frame) {

    fdr->buffer[fdr->head] = frame;

    fdr->head = (fdr->head + 1) % FDR_SIZE;

    if (fdr->count < FDR_SIZE) {
        fdr->count++;
    }
}


void fdr_dump_crash_log(const BlackBox *fdr) {

    if (fdr->count == 0)
    {
      printf("FLIGHT LOG EMPTY / UNAVAILABLE");

      return;  
    }

    uint8_t start_index = (fdr->count < FDR_SIZE) ? 0 : fdr->head;

    printf("=== BLACK BOX CRASH DUMP (%u frames) ===\n", fdr->count);

    for (uint8_t i = 0; i < fdr->count; i++) {
        uint8_t idx = (start_index + i) % FDR_SIZE;


        FDRdata frame = fdr->buffer[idx];

        printf("[Slot %u] Frame #%u | Alt: %u ft | IAS: %d kts | Flags: 0x%02X\n",
               idx, frame.frame_id, frame.altitude, frame.IAS, frame.status_flags);
    }
}

int main() {
    BlackBox fdr;
    fdr_init(&fdr);

    for (uint32_t id = 1; id <= 7; id++) {
        FDRdata sample = {
            .frame_id = id,
            .altitude = 10000 + (id * 500),
            .IAS = 250 + (id * 5),
            .status_flags = 0x00
        };
        fdr_push(&fdr, sample);

        if (id == 2) {
            fdr_dump_crash_log(&fdr);
        }
    }

    fdr_dump_crash_log(&fdr);

    return 0;
}
