#include <stdio.h>
#include <stdint.h>
#include <windows.h> 

#define STALL_BIT     0
#define OVERSPEED_BIT 1

typedef struct 
{
    uint32_t altitude;
    uint16_t airspeed;
    uint8_t status;
} flightData;

void climb_step(flightData *plane, uint32_t climb_ft, int16_t ias) {

    plane->altitude += climb_ft;
    plane->airspeed += ias;

}


void check_flight_envelope(flightData *plane) {

    if (plane->airspeed < 150) {

        plane->status |= (1 << STALL_BIT);
        printf("STALL!\n");

    } else {
        plane->status &= ~(1 << STALL_BIT);

    }


    if (plane->airspeed > 350) {

        plane->status |= (1 << OVERSPEED_BIT);
        printf("OVERSPEED!\n");

    } else {
        plane->status &= ~(1 << OVERSPEED_BIT);

  
    }
}

uint16_t seed = 44257; 

uint16_t math_random(uint16_t min, uint16_t max) {
    seed ^= seed << 7;
    seed ^= seed >> 9;
    seed ^= seed << 8;
    return (seed % (max - min + 1)) + min;
}

int main() {

  flightData airbus = {10000, 250, 0};

  printf("Before climb: %u ft\n", airbus.altitude);

  climb_step(&airbus, 1000, 30);

  printf("After climb:  %u ft | %u kts\n", airbus.altitude, airbus.airspeed);
   
  climb_step(&airbus, 0, 100);

  check_flight_envelope(&airbus);

  climb_step(&airbus, 100, -250);

  check_flight_envelope(&airbus);



    while (1) {
    airbus.airspeed = math_random(120, 370); 
    check_flight_envelope(&airbus);         

    printf("Speed: %u kts | Status bits: %u\n", airbus.airspeed, airbus.status); 

    Sleep(500);
  }

  return 0;

}





