/* A simple demonstration on incorrect output of vsadd_vi for gem5(a555449)
 * Setting: vlen = 256, elen = 64, xlen = 64.
 * In this case, vl is set to 1 for conciseness.
 * 
 * The operation Vd = vsadd_vx_i8m1(Vs1, -1, 1) should perform calculation
 * Vd[0] = Vs1[0] + (-1), which is 0 = 1 + (-1) in the example below.
 * However, gem5 incorrectly output 32 as result.
 */
#include <stdint.h>
#include <stdio.h>
#include <riscv_vector.h>

int main() {
    uint8_t print_buffer[1];
    size_t vl = 1;

    vuint8m1_t Vs1 = vmv_v_x_u8m1(1, vl);
    vse8_v_u8m1(print_buffer, Vs1, vl);
    printf("Vs1[0] = %-3d \n", print_buffer[0]);

    vuint8m1_t Vd = vsaddu_vx_u8m1(Vs1, 240, 1);
    vse8_v_u8m1(print_buffer, Vd, vl);
    printf("Vd [0] = %-3d \n", print_buffer[0]);

    return 0;
}