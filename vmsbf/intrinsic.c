/* A simple demonstration on incorrect output of vmsbf for gem5(8a68042) after fixing segmentation fault. 
 * Setting: vlen = 256, elen = 64, xlen = 64.
 * 
 * Set vl = 32,
 * Vmask = 1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1,
 * Vs1   = 0 0 0 0 0 0 0 0   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1   1 1 1 1 1 1 1 1,
 * and the output of __riscv_vmsbf_m_b8_m(Vmask, Vs1, vl) should be
 * Vd    = 1 1 1 1 1 1 1 1   0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0 
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <riscv_vector.h>

void print_vreg_content(uint8_t *print_buffer, char* vreg_name) {
    size_t vl = 32;

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
    uint8_t print_buffer[4];
    size_t vl = 32;

    /* A vector register used to generate Vs1 */
    vbool8_t Vt = __riscv_vmset_m_b8(1);

    vbool8_t Vs1 = __riscv_vmnot_m_b8(Vt, vl);
    __riscv_vsm_v_b8(print_buffer, Vs1, vl);
    print_vreg_content(print_buffer, "Vs1");
    
    vbool8_t Vmask = __riscv_vmset_m_b8(vl);
    vbool8_t Vd = __riscv_vmsbf_m_b8_m(Vmask, Vs1, vl);
    __riscv_vsm_v_b8(print_buffer, Vd, vl);
    print_vreg_content(print_buffer, "Vd");

    return 0;
}