// #include <stdio.h>
// #include <stdint.h> // Gives us uint8_t (8-bit unsigned integer)

// // Define which bit position each aircraft warning lives on
// #define STALL_BIT       0
// #define OVERSPEED_BIT   1
// #define LOW_FUEL_BIT    2
// #define PULL_UP_BIT     3
// #define ENGINE_FIRE_BIT 4

// // Helper function to print our 8-bit register in binary so you can see the coins!
// void print_register(uint8_t reg) {
//     printf("Register Binary: ");
//     for (int i = 7; i >= 0; i--) {
//         printf("%d", (reg >> i) & 1);
//     }
//     printf("  (Decimal Value: %d)\n", reg);
// }

// int main() {
//     // Start with all 8 warning switches OFF (00000000)
//     uint8_t warning_reg = 0; 
//     printf("--- AIRCRAFT NORMAL ---\n");
//     print_register(warning_reg);

//     // 1. Trigger LOW FUEL (Bit 2) and PULL UP (Bit 3) using Bitwise OR (|=)
//     printf("\n[!] ALERT: Low Fuel and Terrain Pull Up!\n");
//     warning_reg |= (1 << LOW_FUEL_BIT);
//     warning_reg |= (1 << PULL_UP_BIT);
//     print_register(warning_reg); 
//     // Notice: Bit 3 (8) + Bit 2 (4) = Decimal 12 (00001100)!

//     // 2. Check if PULL UP is active using Bitwise AND (&)
//     if (warning_reg & (1 << PULL_UP_BIT)) {
//         printf("AUDIO CALLOUT: TERRAIN! PULL UP!\n");
//     }

//     // 3. Pilot climbs to safety -> Clear PULL UP (Bit 3) using AND + NOT (&= ~)
//     printf("\n[OK] Safe altitude reached. Clearing Pull Up alert...\n");
//     warning_reg &= ~(1 << PULL_UP_BIT);
//     print_register(warning_reg);
//     // Notice: Only Bit 2 (4) remains ON (00000100)!

//     return 0;
// }


/// DAY 2

#include <stdio.h>
#include <stdint.h>

int main() {
    // Incoming 16-bit packet from aircraft altimeter sensor:
    // Top 4 bits = 0x3 (Status: OK) | Bottom 12 bits = 0x4D2 (1234 meters)
    uint16_t sensor_packet = 0x34D2;

    // 1. Unpack Status (Bits 15..12)
    uint8_t sensor_status = (sensor_packet >> 12) & 0x0F;

    // 2. Unpack Altitude (Bits 11..0)
    uint16_t altitude_m = sensor_packet & 0x0FFF;

    printf("Raw Packet:    0x%04X\n", sensor_packet);
    printf("Sensor Status: %d (0x%X)\n", sensor_status, sensor_status);
    printf("Altitude:      %d meters\n", altitude_m);

    if (sensor_status != 3) {
        printf("[WARNING] Altimeter Sensor Fault!\n");
    } else {
        printf("[OK] Valid Altitude Data.\n");
    }

    return 0;
}


0011 0001 0000 0000 
0000 1111 1111 1111

0000 0001 0000 0000

0x0100


1111 1001 1001 1001
0000 0000 0000 1111


0x005A 

0000 0000 0101 1010

