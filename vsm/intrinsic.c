/* A simple demonstration on incorrect output of vsm for gem5(76c3c02). 
 * Setting: vlen = 256, elen = 64, xlen = 64.
 * 
 * Set vl = 64,
 * Expected:
 * Vd  = 1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1 
 * 
 * Actual:
 * Vd  = 1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0 
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <riscv_vector.h>

const int vl = 64;

void print_vreg_content(uint8_t *print_buffer, char* vreg_name) {
    printf("%-3s = ", vreg_name);
    for (int i = 0; i < (vl / 8); ++i) {
        for (int j = 0; j < 8; ++j) {
            bool bit = print_buffer[i] & 1;
            print_buffer[i] = print_buffer[i] >> 1;
            printf("%-2d", bit);
        }
        printf("  ");
    }
    printf("\n");
}

int main() {
    uint8_t print_buffer[8] = {0};

    printf("\n");
    print_vreg_content(print_buffer, "Vd");
    printf("Storing Vd using vsm: \n");
    
    vbool4_t Vd = __riscv_vmset_m_b4(vl);
    __riscv_vsm_v_b4(print_buffer, Vd, vl);
    print_vreg_content(print_buffer, "Vd");

    return 0;
}