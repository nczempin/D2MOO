// Auto-generated 1.12a D2Client link & address bridge
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include <D2DataTbls.h>
#include <Units/Units.h>
#include <Client.h>

extern "C" {

// 1.12a address-grouped global state buffers
uint8_t g_d2client_addr_6FB7D5FC[8192] = {0};
uint8_t g_d2client_addr_6FB7D61C[8192] = {0};
uint8_t g_d2client_addr_6FB84118[8192] = {0};
uint8_t g_d2client_addr_6FB8411C[8192] = {0};
uint8_t g_d2client_addr_6FB8C6E0[8192] = {0};
uint8_t g_d2client_addr_6FB8C6E4[8192] = {0};
uint8_t g_d2client_addr_6FB8CAC8[8192] = {0};
uint8_t g_d2client_addr_6FB8CACC[8192] = {0};
uint8_t g_d2client_addr_6FB8CAD0[8192] = {0};
uint8_t g_d2client_addr_6FB8CAD4[8192] = {0};
uint8_t g_d2client_addr_6FB8CAD8[8192] = {0};
uint8_t g_d2client_addr_6FB8CADC[8192] = {0};
uint8_t g_d2client_addr_6FB8CF28[8192] = {0};
uint8_t g_d2client_addr_6FB8CFA8[8192] = {0};
uint8_t g_d2client_addr_6FB8F368[8192] = {0};
uint8_t g_d2client_addr_6FB905EC[8192] = {0};
uint8_t g_d2client_addr_6FBA1BC0[8192] = {0};
uint8_t g_d2client_addr_6FBA2068[8192] = {0};
uint8_t g_d2client_addr_6FBA2090[8192] = {0};
uint8_t g_d2client_addr_6FBA2094[8192] = {0};
uint8_t g_d2client_addr_6FBA3B98[8192] = {0};
uint8_t g_d2client_addr_6FBA3B9C[8192] = {0};
uint8_t g_d2client_addr_6FBA3BA0[8192] = {0};
uint8_t g_d2client_addr_6FBA3BA4[8192] = {0};
uint8_t g_d2client_addr_6FBA61C8[8192] = {0};
uint8_t g_d2client_addr_6FBA6288[8192] = {0};
uint8_t g_d2client_addr_6FBA6351[8192] = {0};
uint8_t g_d2client_addr_6FBA675C[8192] = {0};
uint8_t g_d2client_addr_6FBA6778[8192] = {0};
uint8_t g_d2client_addr_6FBA67C8[8192] = {0};
uint8_t g_d2client_addr_6FBA6938[8192] = {0};
uint8_t g_d2client_addr_6FBA79A0[8192] = {0};
uint8_t g_d2client_addr_6FBA79FC[8192] = {0};
uint8_t g_d2client_addr_6FBA8AD4[8192] = {0};
uint8_t g_d2client_addr_6FBA9EC0[8192] = {0};
uint8_t g_d2client_addr_6FBAA220[8192] = {0};
uint8_t g_d2client_addr_6FBAA2D0[8192] = {0};
uint8_t g_d2client_addr_6FBAD104[8192] = {0};
uint8_t g_d2client_addr_6FBAD108[8192] = {0};
uint8_t g_d2client_addr_6FBAD10C[8192] = {0};
uint8_t g_d2client_addr_6FBAD110[8192] = {0};
uint8_t g_d2client_addr_6FBB0B78[8192] = {0};
uint8_t g_d2client_addr_6FBB0B94[8192] = {0};
uint8_t g_d2client_addr_6FBB0BC0[8192] = {0};
uint8_t g_d2client_addr_6FBB0BE8[8192] = {0};
uint8_t g_d2client_addr_6FBB0C70[8192] = {0};
uint8_t g_d2client_addr_6FBB0C88[8192] = {0};
uint8_t g_d2client_addr_6FBB0CA4[8192] = {0};
uint8_t g_d2client_addr_6FBB0CB8[8192] = {0};
uint8_t g_d2client_addr_6FBB0CD0[8192] = {0};
uint8_t g_d2client_addr_6FBB0CEC[8192] = {0};
uint8_t g_d2client_addr_6FBB0D10[8192] = {0};
uint8_t g_d2client_addr_6FBB0D20[8192] = {0};
uint8_t g_d2client_addr_6FBB0D24[8192] = {0};
uint8_t g_d2client_addr_6FBB0D28[8192] = {0};
uint8_t g_d2client_addr_6FBB11A0[8192] = {0};
uint8_t g_d2client_addr_6FBB1614[8192] = {0};
uint8_t g_d2client_addr_6FBB1618[8192] = {0};
uint8_t g_d2client_addr_6FBB161C[8192] = {0};
uint8_t g_d2client_addr_6FBB1620[8192] = {0};
uint8_t g_d2client_addr_6FBB1624[8192] = {0};
uint8_t g_d2client_addr_6FBB1628[8192] = {0};
uint8_t g_d2client_addr_6FBB162C[8192] = {0};
uint8_t g_d2client_addr_6FBB1630[8192] = {0};
uint8_t g_d2client_addr_6FBB1634[8192] = {0};
uint8_t g_d2client_addr_6FBB1638[8192] = {0};
uint8_t g_d2client_addr_6FBB163C[8192] = {0};
uint8_t g_d2client_addr_6FBB1640[8192] = {0};
uint8_t g_d2client_addr_6FBB1644[8192] = {0};
uint8_t g_d2client_addr_6FBB1648[8192] = {0};
uint8_t g_d2client_addr_6FBB1664[8192] = {0};
uint8_t g_d2client_addr_6FBB1668[8192] = {0};
uint8_t g_d2client_addr_6FBB166C[8192] = {0};
uint8_t g_d2client_addr_6FBB1670[8192] = {0};
uint8_t g_d2client_addr_6FBB1674[8192] = {0};
uint8_t g_d2client_addr_6FBB1678[8192] = {0};
uint8_t g_d2client_addr_6FBB2B58[8192] = {0};
uint8_t g_d2client_addr_6FBB2B5C[8192] = {0};
uint8_t g_d2client_addr_6FBB2B60[8192] = {0};
uint8_t g_d2client_addr_6FBB2B64[8192] = {0};
uint8_t g_d2client_addr_6FBB2B68[8192] = {0};
uint8_t g_d2client_addr_6FBB2B6C[8192] = {0};
uint8_t g_d2client_addr_6FBB2B70[8192] = {0};
uint8_t g_d2client_addr_6FBB2B7C[8192] = {0};
uint8_t g_d2client_addr_6FBB2B80[8192] = {0};
uint8_t g_d2client_addr_6FBB2B88[8192] = {0};
uint8_t g_d2client_addr_6FBB2B8C[8192] = {0};
uint8_t g_d2client_addr_6FBB2B90[8192] = {0};
uint8_t g_d2client_addr_6FBB2BA0[8192] = {0};
uint8_t g_d2client_addr_6FBB2BB0[8192] = {0};
uint8_t g_d2client_addr_6FBB2BB8[8192] = {0};
uint8_t g_d2client_addr_6FBB2BBC[8192] = {0};
uint8_t g_d2client_addr_6FBB2BC0[8192] = {0};
uint8_t g_d2client_addr_6FBB2BD4[8192] = {0};
uint8_t g_d2client_addr_6FBB2BE8[8192] = {0};
uint8_t g_d2client_addr_6FBB2BEC[8192] = {0};
uint8_t g_d2client_addr_6FBB32B0[8192] = {0};
uint8_t g_d2client_addr_6FBB32BC[8192] = {0};
uint8_t g_d2client_addr_6FBB32C0[8192] = {0};
uint8_t g_d2client_addr_6FBB32D4[8192] = {0};
uint8_t g_d2client_addr_6FBB330C[8192] = {0};
uint8_t g_d2client_addr_6FBB3310[8192] = {0};
uint8_t g_d2client_addr_6FBB3314[8192] = {0};
uint8_t g_d2client_addr_6FBB3318[8192] = {0};
uint8_t g_d2client_addr_6FBB33E0[8192] = {0};
uint8_t g_d2client_addr_6FBB5364[8192] = {0};
uint8_t g_d2client_addr_6FBB5490[8192] = {0};
uint8_t g_d2client_addr_6FBB54CC[8192] = {0};
uint8_t g_d2client_addr_6FBB5574[8192] = {0};
uint8_t g_d2client_addr_6FBBA6CC[8192] = {0};
uint8_t g_d2client_addr_6FBBA6D0[8192] = {0};
uint8_t g_d2client_addr_6FBBA6E8[8192] = {0};
uint8_t g_d2client_addr_6FBC21C0[8192] = {0};
uint8_t g_d2client_addr_6FBC21C8[8192] = {0};
uint8_t g_d2client_addr_6FBC21CC[8192] = {0};
uint8_t g_d2client_addr_6FBC21D0[8192] = {0};
uint8_t g_d2client_addr_6FBC21D8[8192] = {0};
uint8_t g_d2client_addr_6FBC22C8[8192] = {0};
uint8_t g_d2client_addr_6FBC23B8[8192] = {0};
uint8_t g_d2client_addr_6FBC24A8[8192] = {0};
uint8_t g_d2client_addr_6FBC2598[8192] = {0};
uint8_t g_d2client_addr_6FBC2798[8192] = {0};
uint8_t g_d2client_addr_6FBC57D0[8192] = {0};
uint8_t g_d2client_addr_6FBC57D8[8192] = {0};
uint8_t g_d2client_addr_6FBC97E0[8192] = {0};
uint8_t g_d2client_addr_6FBC9D60[8192] = {0};
uint8_t g_d2client_addr_6FBC9F60[8192] = {0};
uint8_t g_d2client_addr_6FBCA960[8192] = {0};
uint8_t g_d2client_addr_6FBCAB60[8192] = {0};
uint8_t g_d2client_addr_6FBCB160[8192] = {0};
uint8_t g_d2client_addr_6FBCB625[8192] = {0};
uint8_t g_d2client_addr_6FBCB629[8192] = {0};
uint8_t g_d2client_addr_6FBCB62D[8192] = {0};
uint8_t g_d2client_addr_6FBCB643[8192] = {0};
uint8_t g_d2client_addr_6FBCB647[8192] = {0};
uint8_t g_d2client_addr_6FBCB6CC[8192] = {0};
uint8_t g_d2client_addr_6FBCB6CF[8192] = {0};
uint8_t g_d2client_addr_6FBCB6D0[8192] = {0};
uint8_t g_d2client_addr_6FBCB6D4[8192] = {0};
uint8_t g_d2client_addr_6FBCB7C8[8192] = {0};
uint8_t g_d2client_addr_6FBCB7CC[8192] = {0};
uint8_t g_d2client_addr_6FBCB7CD[8192] = {0};
uint8_t g_d2client_addr_6FBCB7CE[8192] = {0};
uint8_t g_d2client_addr_6FBCB9B0[8192] = {0};
uint8_t g_d2client_addr_6FBCB9EC[8192] = {0};
uint8_t g_d2client_addr_6FBCB9F0[8192] = {0};
uint8_t g_d2client_addr_6FBCB9F4[8192] = {0};
uint8_t g_d2client_addr_6FBCB9F8[8192] = {0};
uint8_t g_d2client_addr_6FBCB9FC[8192] = {0};
uint8_t g_d2client_addr_6FBCBA00[8192] = {0};
uint8_t g_d2client_addr_6FBCBA04[8192] = {0};
uint8_t g_d2client_addr_6FBCBA08[8192] = {0};
uint8_t g_d2client_addr_6FBCBA0C[8192] = {0};
uint8_t g_d2client_addr_6FBCBA10[8192] = {0};
uint8_t g_d2client_addr_6FBCBA18[8192] = {0};
uint8_t g_d2client_addr_6FBCBA1C[8192] = {0};
uint8_t g_d2client_addr_6FBCBA20[8192] = {0};
uint8_t g_d2client_addr_6FBCBA24[8192] = {0};
uint8_t g_d2client_addr_6FBCBA28[8192] = {0};
uint8_t g_d2client_addr_6FBCBA2C[8192] = {0};
uint8_t g_d2client_addr_6FBCBA30[8192] = {0};
uint8_t g_d2client_addr_6FBCBA34[8192] = {0};
uint8_t g_d2client_addr_6FBCBA38[8192] = {0};
uint8_t g_d2client_addr_6FBCBA3C[8192] = {0};
uint8_t g_d2client_addr_6FBCBA40[8192] = {0};
uint8_t g_d2client_addr_6FBCBA44[8192] = {0};
uint8_t g_d2client_addr_6FBCBA68[8192] = {0};
uint8_t g_d2client_addr_6FBCBB48[8192] = {0};
uint8_t g_d2client_addr_6FBCBC24[8192] = {0};
uint8_t g_d2client_addr_6FBCBC28[8192] = {0};
uint8_t g_d2client_addr_6FBCBC30[8192] = {0};
uint8_t g_d2client_addr_6FBCBC3C[8192] = {0};
uint8_t g_d2client_addr_6FBCBC40[8192] = {0};
uint8_t g_d2client_addr_6FBCBC44[8192] = {0};
uint8_t g_d2client_addr_6FBCBC48[8192] = {0};
uint8_t g_d2client_addr_6FBCBC4C[8192] = {0};
uint8_t g_d2client_addr_6FBCBC50[8192] = {0};
uint8_t g_d2client_addr_6FBCBC54[8192] = {0};
uint8_t g_d2client_addr_6FBCBC60[8192] = {0};
uint8_t g_d2client_addr_6FBCBC64[8192] = {0};
uint8_t g_d2client_addr_6FBCBC68[8192] = {0};
uint8_t g_d2client_addr_6FBCBC70[8192] = {0};
uint8_t g_d2client_addr_6FBCBC74[8192] = {0};
uint8_t g_d2client_addr_6FBCBC78[8192] = {0};
uint8_t g_d2client_addr_6FBCBC7C[8192] = {0};
uint8_t g_d2client_addr_6FBCBC80[8192] = {0};
uint8_t g_d2client_addr_6FBCBC84[8192] = {0};
uint8_t g_d2client_addr_6FBCBC88[8192] = {0};
uint8_t g_d2client_addr_6FBCBC90[8192] = {0};
uint8_t g_d2client_addr_6FBCBC94[8192] = {0};
uint8_t g_d2client_addr_6FBCBC98[8192] = {0};
uint8_t g_d2client_addr_6FBCBC9C[8192] = {0};
uint8_t g_d2client_addr_6FBCBCA0[8192] = {0};
uint8_t g_d2client_addr_6FBCBCAC[8192] = {0};
uint8_t g_d2client_addr_6FBCBCB0[8192] = {0};
uint8_t g_d2client_addr_6FBCBCB4[8192] = {0};
uint8_t g_d2client_addr_6FBCBCBC[8192] = {0};
uint8_t g_d2client_addr_6FBCBCC0[8192] = {0};
uint8_t g_d2client_addr_6FBCBCC4[8192] = {0};
uint8_t g_d2client_addr_6FBCBCE0[8192] = {0};
uint8_t g_d2client_addr_6FBCBCE4[8192] = {0};
uint8_t g_d2client_addr_6FBCBCE8[8192] = {0};
uint8_t g_d2client_addr_6FBCBCF0[8192] = {0};
uint8_t g_d2client_addr_6FBCBCF4[8192] = {0};
uint8_t g_d2client_addr_6FBCBCFC[8192] = {0};
uint8_t g_d2client_addr_6FBCBD00[8192] = {0};
uint8_t g_d2client_addr_6FBCBD04[8192] = {0};
uint8_t g_d2client_addr_6FBCBD08[8192] = {0};
uint8_t g_d2client_addr_6FBCBD0C[8192] = {0};
uint8_t g_d2client_addr_6FBCBD10[8192] = {0};
uint8_t g_d2client_addr_6FBCBD14[8192] = {0};
uint8_t g_d2client_addr_6FBCBD18[8192] = {0};
uint8_t g_d2client_addr_6FBCBD28[8192] = {0};
uint8_t g_d2client_addr_6FBCBD2C[8192] = {0};
uint8_t g_d2client_addr_6FBCBD4C[8192] = {0};
uint8_t g_d2client_addr_6FBCBE50[8192] = {0};
uint8_t g_d2client_addr_6FBCBE58[8192] = {0};
uint8_t g_d2client_addr_6FBCBF78[8192] = {0};
uint8_t g_d2client_addr_6FBCBF88[8192] = {0};
uint8_t g_d2client_addr_6FBCBFE4[8192] = {0};
uint8_t g_d2client_addr_6FBCBFF4[8192] = {0};
uint8_t g_d2client_addr_6FBCBFF8[8192] = {0};
uint8_t g_d2client_addr_6FBCC004[8192] = {0};
uint8_t g_d2client_addr_6FBCC008[8192] = {0};
uint8_t g_d2client_addr_6FBCC00C[8192] = {0};
uint8_t g_d2client_addr_6FBCC010[8192] = {0};
uint8_t g_d2client_addr_6FBCC01C[8192] = {0};
uint8_t g_d2client_addr_6FBCC020[8192] = {0};
uint8_t g_d2client_addr_6FBCC028[8192] = {0};
uint8_t g_d2client_addr_6FBCC040[8192] = {0};
uint8_t g_d2client_addr_6FBCC044[8192] = {0};
uint8_t g_d2client_addr_6FBCC04C[8192] = {0};
uint8_t g_d2client_addr_6FBCC050[8192] = {0};
uint8_t g_d2client_addr_6FBCC054[8192] = {0};
uint8_t g_d2client_addr_6FBCC05C[8192] = {0};
uint8_t g_d2client_addr_6FBCC060[8192] = {0};
uint8_t g_d2client_addr_6FBCC0F4[8192] = {0};
uint8_t g_d2client_addr_6FBCC0F8[8192] = {0};
uint8_t g_d2client_addr_6FBCC118[8192] = {0};
uint8_t g_d2client_addr_6FBCC11C[8192] = {0};
uint8_t g_d2client_addr_6FBCC120[8192] = {0};
uint8_t g_d2client_addr_6FBCC158[8192] = {0};
uint8_t g_d2client_addr_6FBCC15C[8192] = {0};
uint8_t g_d2client_addr_6FBCC1C8[8192] = {0};
uint8_t g_d2client_addr_6FBCC1D0[8192] = {0};
uint8_t g_d2client_addr_6FBCC1D4[8192] = {0};
uint8_t g_d2client_addr_6FBCC1E0[8192] = {0};
uint8_t g_d2client_addr_6FBCC244[8192] = {0};
uint8_t g_d2client_addr_6FBCC248[8192] = {0};
uint8_t g_d2client_addr_6FBCC24C[8192] = {0};
uint8_t g_d2client_addr_6FBCC250[8192] = {0};
uint8_t g_d2client_addr_6FBCC254[8192] = {0};
uint8_t g_d2client_addr_6FBCC258[8192] = {0};
uint8_t g_d2client_addr_6FBCC25C[8192] = {0};
uint8_t g_d2client_addr_6FBCC260[8192] = {0};
uint8_t g_d2client_addr_6FBCC264[8192] = {0};
uint8_t g_d2client_addr_6FBCC268[8192] = {0};
uint8_t g_d2client_addr_6FBCC26C[8192] = {0};
uint8_t g_d2client_addr_6FBCC270[8192] = {0};
uint8_t g_d2client_addr_6FBCC274[8192] = {0};
uint8_t g_d2client_addr_6FBCC278[8192] = {0};
uint8_t g_d2client_addr_6FBCC2A0[8192] = {0};
uint8_t g_d2client_addr_6FBCC2A4[8192] = {0};
uint8_t g_d2client_addr_6FBCC2A8[8192] = {0};
uint8_t g_d2client_addr_6FBCC2AC[8192] = {0};
uint8_t g_d2client_addr_6FBCC2B0[8192] = {0};
uint8_t g_d2client_addr_6FBCC2B4[8192] = {0};
uint8_t g_d2client_addr_6FBCC2B8[8192] = {0};
uint8_t g_d2client_addr_6FBCC2D0[8192] = {0};
uint8_t g_d2client_addr_6FBCC2D8[8192] = {0};
uint8_t g_d2client_addr_6FBCC2DC[8192] = {0};
uint8_t g_d2client_addr_6FBCC2E8[8192] = {0};
uint8_t g_d2client_addr_6FBCC2EC[8192] = {0};
uint8_t g_d2client_addr_6FBCC2F4[8192] = {0};
uint8_t g_d2client_addr_6FBCC2F8[8192] = {0};
uint8_t g_d2client_addr_6FBCC2FC[8192] = {0};
uint8_t g_d2client_addr_6FBCC300[8192] = {0};
uint8_t g_d2client_addr_6FBCC328[8192] = {0};
uint8_t g_d2client_addr_6FBCC32C[8192] = {0};
uint8_t g_d2client_addr_6FBCC330[8192] = {0};
uint8_t g_d2client_addr_6FBCC334[8192] = {0};
uint8_t g_d2client_addr_6FBCC338[8192] = {0};
uint8_t g_d2client_addr_6FBCC34C[8192] = {0};
uint8_t g_d2client_addr_6FBCC35C[8192] = {0};
uint8_t g_d2client_addr_6FBCC360[8192] = {0};
uint8_t g_d2client_addr_6FBCC36C[8192] = {0};
uint8_t g_d2client_addr_6FBCC370[8192] = {0};
uint8_t g_d2client_addr_6FBCC374[8192] = {0};
uint8_t g_d2client_addr_6FBCC390[8192] = {0};
uint8_t g_d2client_addr_6FBCC3BC[8192] = {0};
uint8_t g_d2client_addr_6FBCC3C0[8192] = {0};
uint8_t g_d2client_addr_6FBCC3D0[8192] = {0};
uint8_t g_d2client_addr_6FBCC41C[8192] = {0};
uint8_t g_d2client_addr_6FBCC43C[8192] = {0};
uint8_t g_d2client_addr_6FBCC54C[8192] = {0};
uint8_t g_d2client_addr_6FBCD524[8192] = {0};
uint8_t g_d2client_addr_6FBCD528[8192] = {0};
uint8_t g_d2client_addr_6FBCF530[8192] = {0};
uint8_t g_d2client_addr_6FBD33F8[8192] = {0};
uint8_t g_d2client_addr_6FBD33FC[8192] = {0};
uint8_t g_d2client_addr_6FBD3400[8192] = {0};

// Additional named global state buffers
uint8_t g_d2client_var_gShrineClientTable[8192] = {0};
uint8_t g_d2client_var_g_apfnBodySlotClickHandlers[8192] = {0};
uint8_t g_d2client_var_g_apfnObjectClientFns[8192] = {0};
uint8_t g_d2client_var_gapfnObjectClientFunctions[8192] = {0};
uint8_t g_d2client_var_gbAct3ObjectSoundPreloaded[8192] = {0};
uint8_t g_d2client_var_gbEnvSoundPositionEnabled[8192] = {0};
uint8_t g_d2client_var_gbTowerTomeSoundPreloaded[8192] = {0};
uint8_t g_d2client_var_gnShrineClientTableCount[8192] = {0};
uint8_t g_d2client_var_gpLocalQuestFlags[8192] = {0};

// Stack-balanced fallback bridges
int32_t __cdecl bridge_ret_0(void) { return 0; }
int32_t __stdcall bridge_ret_4(int32_t a0) { (void)a0; return 0; }
int32_t __stdcall bridge_ret_8(int32_t a0, int32_t a1) { (void)a0; (void)a1; return 0; }
int32_t __stdcall bridge_ret_12(int32_t a0, int32_t a1, int32_t a2) { (void)a0; (void)a1; (void)a2; return 0; }
int32_t __stdcall bridge_ret_16(int32_t a0, int32_t a1, int32_t a2, int32_t a3) { (void)a0; (void)a1; (void)a2; (void)a3; return 0; }
int32_t __stdcall bridge_ret_20(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4) { (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; return 0; }
int32_t __stdcall bridge_ret_24(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5) { (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; return 0; }
int32_t __stdcall bridge_ret_28(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6) { (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; return 0; }
int32_t __stdcall bridge_ret_32(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7) { (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; return 0; }
int32_t __stdcall bridge_ret_36(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5, int32_t a6, int32_t a7, int32_t a8) { (void)a0; (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; (void)a7; (void)a8; return 0; }

} // extern "C"

// Authoritative Reconstructed Bridge Implementations (Milestone 3 Cutover)
extern D2UnitStrc* gpPlayerUnit;
int32_t __fastcall MSG_SendPacket(void* pPacketData, size_t nSize);
extern D2UnitStrc* gapServerSideUnitHashTables[6][128];
extern D2UnitStrc* gapClientSideUnitHashTables[6][128];
extern D2UnitStrc* __fastcall D2Client_FindServerSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType);
extern D2UnitStrc* __fastcall D2Client_FindClientSideUnit(uint32_t dwUnitGUID, uint32_t nUnitType);
extern void __stdcall D2Client_DestroyAndFreeUnit(D2UnitStrc* pUnit);

D2SkillsTxt* __fastcall DATATBLS_GetSkillsTxtRecord(int32_t nSkillId)
{
    if (!sgptDataTables || nSkillId < 0 || nSkillId >= sgptDataTables->nSkillsTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pSkillsTxt[nSkillId];
}

D2SkillsTxt* __fastcall SKILLS_GetSkillsTxtRecord(int32_t nSkillId)
{
    return DATATBLS_GetSkillsTxtRecord(nSkillId);
}

D2StatesTxt* __cdecl DATATBLS_GetStatesTxtRecord(int32_t nStateId)
{
    if (!sgptDataTables || nStateId < 0 || nStateId >= sgptDataTables->nStatesTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pStatesTxt[nStateId];
}

D2MissilesTxt* __fastcall DATATBLS_GetMissilesTxtRecord(int32_t nMissileId)
{
    if (!sgptDataTables || nMissileId < 0 || nMissileId >= sgptDataTables->nMissilesTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pMissilesTxt[nMissileId];
}

D2OverlayTxt* __fastcall DATATBLS_GetOverlayTxtRecord(int32_t nOverlayId)
{
    if (!sgptDataTables || nOverlayId < 0 || nOverlayId >= sgptDataTables->nOverlayTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pOverlayTxt[nOverlayId];
}

D2MonStatsTxt* __fastcall ENGINE_GetMonStatsTxtRecord(int32_t nClassId)
{
    if (!sgptDataTables || nClassId < 0 || nClassId >= sgptDataTables->nMonStatsTxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pMonStatsTxt[nClassId];
}

D2MonStats2Txt* __fastcall ENGINE_GetMonStats2TxtRecord(int32_t nMonStats2Id)
{
    if (!sgptDataTables || nMonStats2Id < 0 || nMonStats2Id >= sgptDataTables->nMonStats2TxtRecordCount)
    {
        return nullptr;
    }
    return &sgptDataTables->pMonStats2Txt[nMonStats2Id];
}

int32_t __fastcall ENGINE_GetMonStatsRecordCount(void)
{
    return sgptDataTables ? sgptDataTables->nMonStatsTxtRecordCount : 0;
}

D2UnitStrc* __stdcall PLAYER_GetLocalPlayer(void)
{
    return gpPlayerUnit;
}

D2UnitStrc* __fastcall UNIT_GetClientPlayer(void)
{
    return gpPlayerUnit;
}

D2UnitStrc* __fastcall UNIT_FindUnitById(int32_t nUnitGUID, int32_t nUnitType)
{
    D2UnitStrc* pUnit = D2Client_FindServerSideUnit(static_cast<uint32_t>(nUnitGUID), static_cast<uint32_t>(nUnitType));
    if (!pUnit)
    {
        pUnit = D2Client_FindClientSideUnit(static_cast<uint32_t>(nUnitGUID), static_cast<uint32_t>(nUnitType));
    }
    return pUnit;
}

void __fastcall D2Client_FreeClientUnit(D2UnitStrc* pUnit)
{
    if (pUnit)
    {
        D2Client_DestroyAndFreeUnit(pUnit);
    }
}

int32_t __fastcall UNITS_GetXPosition(D2UnitStrc* pUnit)
{
    return pUnit ? UNITS_GetClientCoordX(pUnit) : 0;
}

int32_t __fastcall UNITS_GetYPosition(D2UnitStrc* pUnit)
{
    return pUnit ? UNITS_GetClientCoordY(pUnit) : 0;
}

int32_t __fastcall UNIT_GetSubtileX(D2UnitStrc* pUnit)
{
    return pUnit ? UNITS_GetClientCoordX(pUnit) : 0;
}

int32_t __fastcall UNIT_GetSubtileY(D2UnitStrc* pUnit)
{
    return pUnit ? UNITS_GetClientCoordY(pUnit) : 0;
}

void __fastcall MSG_SendBytePacket(uint8_t nOpcode)
{
    MSG_SendPacket(&nOpcode, 1);
}

void __fastcall MSG_SendSingleBytePacket(uint8_t nOpcode)
{
    MSG_SendPacket(&nOpcode, 1);
}

void __fastcall MSG_SendByteWordPacket(uint8_t nOpcode, uint16_t wValue)
{
    uint8_t aPacket[3];
    aPacket[0] = nOpcode;
    *reinterpret_cast<uint16_t*>(&aPacket[1]) = wValue;
    MSG_SendPacket(aPacket, 3);
}

void __fastcall MSG_SendClientPacket(uint32_t nSize, const void* pPacket)
{
    if (pPacket && nSize > 0)
    {
        MSG_SendPacket(const_cast<void*>(pPacket), nSize);
    }
}

void __fastcall NET_SendByte(uint8_t nOpcode)
{
    MSG_SendPacket(&nOpcode, 1);
}

void __fastcall NET_SendByteAndWord(uint8_t nOpcode, uint16_t wArg)
{
    uint8_t aPacket[3];
    aPacket[0] = nOpcode;
    *reinterpret_cast<uint16_t*>(&aPacket[1]) = wArg;
    MSG_SendPacket(aPacket, 3);
}

void __fastcall NET_SendByteAndDword(uint8_t nOpcode, uint32_t dwArg)
{
    uint8_t aPacket[5];
    aPacket[0] = nOpcode;
    *reinterpret_cast<uint32_t*>(&aPacket[1]) = dwArg;
    MSG_SendPacket(aPacket, 5);
}

void __fastcall NET_SendByteAndTwoDwords(uint8_t nOpcode, uint32_t dwArg1, uint32_t dwArg2)
{
    uint8_t aPacket[9];
    aPacket[0] = nOpcode;
    *reinterpret_cast<uint32_t*>(&aPacket[1]) = dwArg1;
    *reinterpret_cast<uint32_t*>(&aPacket[5]) = dwArg2;
    MSG_SendPacket(aPacket, 9);
}

void __fastcall NET_SendByteAndThreeDwords(uint8_t nOpcode, uint32_t dwArg1, uint32_t dwArg2, uint32_t dwArg3)
{
    uint8_t aPacket[13];
    aPacket[0] = nOpcode;
    *reinterpret_cast<uint32_t*>(&aPacket[1]) = dwArg1;
    *reinterpret_cast<uint32_t*>(&aPacket[5]) = dwArg2;
    *reinterpret_cast<uint32_t*>(&aPacket[9]) = dwArg3;
    MSG_SendPacket(aPacket, 13);
}

void __fastcall NET_SendByteAndFourDwords(uint8_t nOpcode, uint32_t dwArg1, uint32_t dwArg2, uint32_t dwArg3, uint32_t dwArg4)
{
    uint8_t aPacket[17];
    aPacket[0] = nOpcode;
    *reinterpret_cast<uint32_t*>(&aPacket[1]) = dwArg1;
    *reinterpret_cast<uint32_t*>(&aPacket[5]) = dwArg2;
    *reinterpret_cast<uint32_t*>(&aPacket[9]) = dwArg3;
    *reinterpret_cast<uint32_t*>(&aPacket[13]) = dwArg4;
    MSG_SendPacket(aPacket, 17);
}

void __stdcall NET_SendC2SPacket(const void* pPacket, int32_t nSize)
{
    if (pPacket && nSize > 0)
    {
        MSG_SendPacket(const_cast<void*>(pPacket), static_cast<size_t>(nSize));
    }
}

// Global variable linker aliases
#pragma comment(linker, "/alternatename:?gAutomapColors@@3PAEA=_g_d2client_addr_6FBCC300")
#pragma comment(linker, "/alternatename:?gCharBarkSoundIndices_6FBA2068@@3PAHA=_g_d2client_addr_6FBA2068")
#pragma comment(linker, "/alternatename:?gCharVoiceTable_6FBA1BC0@@3PAUD2CharVoiceEntryStrc@@A=_g_d2client_addr_6FBA1BC0")
#pragma comment(linker, "/alternatename:?gCltMissileDoFuncTable@@3PAP6IXPAUD2UnitStrc@@@ZA=_g_d2client_addr_6FBA67C8")
#pragma comment(linker, "/alternatename:?gCltMissileHitFuncTable@@3PAP6IHPAUD2UnitStrc@@0HH@ZA=_g_d2client_addr_6FBA6938")
#pragma comment(linker, "/alternatename:?gDigitBitmap7x10_6FBA6351@@3PAEA=_g_d2client_addr_6FBA6351")
#pragma comment(linker, "/alternatename:?gKeyBindings@@3PAUD2KeyBindingStrc@@A=_g_d2client_addr_6FBB11A0")
#pragma comment(linker, "/alternatename:?gMonUModHandlers_6FBAA220@@3PAUD2MonUModHandlerStrc@@A=_g_d2client_addr_6FBAA220")
#pragma comment(linker, "/alternatename:?gMonsterModeCallbacks_6FBA61C8@@3PAUD2MonsterModeCallbackStrc@@A=_g_d2client_addr_6FBA61C8")
#pragma comment(linker, "/alternatename:?gPlayerUnits_6FBCB6D4@@3PAPAUD2UnitStrc@@A=_g_d2client_addr_6FBCB6D4")
#pragma comment(linker, "/alternatename:?gSavedKeyBindings@@3PAUD2KeyBindingStrc@@A=_g_d2client_addr_6FBB0D28")
#pragma comment(linker, "/alternatename:?gShrineClientTable@@3QBUD2ShrineClientTableStrc@@B=_g_d2client_var_gShrineClientTable")
#pragma comment(linker, "/alternatename:?gSoundChannelPool_6FBC57D8@@3PAUD2ClientSoundChannelStrc@@A=_g_d2client_addr_6FBC57D8")
#pragma comment(linker, "/alternatename:?gSpecialMonsterModeCallbacks_6FBA6288@@3PAUD2MonsterModeCallbackStrc@@A=_g_d2client_addr_6FBA6288")
#pragma comment(linker, "/alternatename:?gStateAppliedCallbacks@@3PAUD2ClientStateCallbackStrc@@A=_g_d2client_addr_6FB8CF28")
#pragma comment(linker, "/alternatename:?gStateRemovedCallbacks@@3PAUD2ClientStateCallbackStrc@@A=_g_d2client_addr_6FB8CFA8")
#pragma comment(linker, "/alternatename:?g_aClientUnitHashTables@@3PAY0IA@PAUD2UnitLightViewStrc@@A=_g_d2client_addr_6FBC9D60")
#pragma comment(linker, "/alternatename:?g_aServerUnitHashTables@@3PAY0IA@PAUD2UnitLightViewStrc@@A=_g_d2client_addr_6FBCA960")
#pragma comment(linker, "/alternatename:?g_apClientUnitHashTable@@3PAPAUD2UnitStrc@@A=_g_d2client_addr_6FBCAB60")
#pragma comment(linker, "/alternatename:?g_apClientUnitHashTables@@3PAPAUD2UnitStrc@@A=_g_d2client_addr_6FBC9F60")
#pragma comment(linker, "/alternatename:?g_apInvSlotBackCells@@3PAPAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA44")
#pragma comment(linker, "/alternatename:?g_apServerMonsterHashTable@@3PAPAUD2UnitStrc@@A=_g_d2client_addr_6FBCAB60")
#pragma comment(linker, "/alternatename:?g_apfnBodySlotClickHandlers@@3PAP6IHHPAUD2UnitStrc@@H@ZA=_g_d2client_var_g_apfnBodySlotClickHandlers")
#pragma comment(linker, "/alternatename:?g_apfnMonUModEvents@@3PAY04P6IXPAUD2UnitStrc@@H@ZA=_g_d2client_addr_6FBA9EC0")
#pragma comment(linker, "/alternatename:?g_apfnObjectClientFns@@3PAP6IXPAUD2UnitStrc@@@ZA=_g_d2client_var_g_apfnObjectClientFns")
#pragma comment(linker, "/alternatename:?g_atBodyLocRects@@3PAUD2InvBodyLocInfoStrc@@A=_g_d2client_addr_6FBCBA68")
#pragma comment(linker, "/alternatename:?g_atMercBodyLocRects@@3PAUD2InvBodyLocInfoStrc@@A=_g_d2client_addr_6FBCBB48")
#pragma comment(linker, "/alternatename:?g_awShopTabStringIds@@3PAGA=_g_d2client_addr_6FBCBC68")
#pragma comment(linker, "/alternatename:?g_bAct4PortalTransition@@3EA=_g_d2client_addr_6FBCB6D0")
#pragma comment(linker, "/alternatename:?g_bBaalPortalActive@@3HA=_g_d2client_addr_6FBCC010")
#pragma comment(linker, "/alternatename:?g_bChatInputOpen@@3HA=_g_d2client_addr_6FBB2B6C")
#pragma comment(linker, "/alternatename:?g_bClassicVictoryScreen@@3EA=_g_d2client_addr_6FBCB6CC")
#pragma comment(linker, "/alternatename:?g_bCutsceneActive@@3EA=_g_d2client_addr_6FBCB6CF")
#pragma comment(linker, "/alternatename:?g_bDiabloSealActive@@3HA=_g_d2client_addr_6FBCC41C")
#pragma comment(linker, "/alternatename:?g_bEscMenuActive@@3HA=_g_d2client_addr_6FBB5490")
#pragma comment(linker, "/alternatename:?g_bExpansionChar@@3HA=_g_d2client_addr_6FBB330C")
#pragma comment(linker, "/alternatename:?g_bExpansionMode@@3HA=_g_d2client_addr_6FBB330C")
#pragma comment(linker, "/alternatename:?g_bForceMediumLightQuality@@3HA=_g_d2client_addr_6FBA6778")
#pragma comment(linker, "/alternatename:?g_bHoradricAnimPlaying@@3HA=_g_d2client_addr_6FBCBA1C")
#pragma comment(linker, "/alternatename:?g_bHoradricCubeClosedLocally@@3HA=_g_d2client_addr_6FBCBCC4")
#pragma comment(linker, "/alternatename:?g_bHoradricCubeOpen@@3HA=_g_d2client_addr_6FBB2BBC")
#pragma comment(linker, "/alternatename:?g_bHoveredItemValid@@3HA=_g_d2client_addr_6FBCB9F0")
#pragma comment(linker, "/alternatename:?g_bHoveredSocketableItem@@3HA=_g_d2client_addr_6FBCB9EC")
#pragma comment(linker, "/alternatename:?g_bInteractedWithNpc@@3HA=_g_d2client_addr_6FBCB629")
#pragma comment(linker, "/alternatename:?g_bInvGoldBtnDown@@3HA=_g_d2client_addr_6FBCBC44")
#pragma comment(linker, "/alternatename:?g_bInvPanelOpen@@3HA=_g_d2client_addr_6FBB2B5C")
#pragma comment(linker, "/alternatename:?g_bMephistoBridgeActive@@3HA=_g_d2client_addr_6FBCC008")
#pragma comment(linker, "/alternatename:?g_bMercInvOpen@@3HA=_g_d2client_addr_6FBB2BC0")
#pragma comment(linker, "/alternatename:?g_bMercShopPanelOpen@@3HA=_g_d2client_addr_6FBB2BEC")
#pragma comment(linker, "/alternatename:?g_bMiniPanelOpen@@3HA=_g_d2client_addr_6FBB2B70")
#pragma comment(linker, "/alternatename:?g_bMusicSequenceActive@@3HA=_g_d2client_addr_6FBCC258")
#pragma comment(linker, "/alternatename:?g_bMusicSequencePhase1Pending@@3HA=_g_d2client_addr_6FBCC264")
#pragma comment(linker, "/alternatename:?g_bMusicSequencePhase2Pending@@3HA=_g_d2client_addr_6FBCC270")
#pragma comment(linker, "/alternatename:?g_bPanelBtnDragActive@@3HA=_g_d2client_addr_6FBCBCB4")
#pragma comment(linker, "/alternatename:?g_bQuestPanelOpenFlag@@3HA=_g_d2client_addr_6FBB2BA0")
#pragma comment(linker, "/alternatename:?g_bRepairAllHoverState@@3HA=_g_d2client_addr_6FBCBC90")
#pragma comment(linker, "/alternatename:?g_bShopActionBtnDown@@3HA=_g_d2client_addr_6FBCBC94")
#pragma comment(linker, "/alternatename:?g_bShopActionBtnPressed@@3HA=_g_d2client_addr_6FBCBC98")
#pragma comment(linker, "/alternatename:?g_bShopAutoSelectTabPending@@3HA=_g_d2client_addr_6FBCBA18")
#pragma comment(linker, "/alternatename:?g_bShopCloseBtnDown@@3HA=_g_d2client_addr_6FBCBCA0")
#pragma comment(linker, "/alternatename:?g_bShopCloseBtnPressed@@3HA=_g_d2client_addr_6FBCBC9C")
#pragma comment(linker, "/alternatename:?g_bShopConfirmPopupOpen@@3HA=_g_d2client_addr_6FBCBC78")
#pragma comment(linker, "/alternatename:?g_bShopSubBtnPressed@@3HA=_g_d2client_addr_6FBCBCB0")
#pragma comment(linker, "/alternatename:?g_bSkillHoverIsRight_6FBC21C0@@3HA=_g_d2client_addr_6FBC21C0")
#pragma comment(linker, "/alternatename:?g_bSkillTreeOrSelectOpen@@3HA=_g_d2client_addr_6FBCC008")
#pragma comment(linker, "/alternatename:?g_bStashOrTradeActionBtnDown@@3HA=_g_d2client_addr_6FBCBC30")
#pragma comment(linker, "/alternatename:?g_bTradeGoldBtnPressed@@3HA=_g_d2client_addr_6FBCBC24")
#pragma comment(linker, "/alternatename:?g_bTradeGoldTransferDisabled@@3HA=_g_d2client_addr_6FBCBC3C")
#pragma comment(linker, "/alternatename:?g_bTradePanelOpen@@3HA=_g_d2client_addr_6FBB2BB8")
#pragma comment(linker, "/alternatename:?g_bTradePartnerGoldHighlight@@3HA=_g_d2client_addr_6FBCBC28")
#pragma comment(linker, "/alternatename:?g_bTransmuteBtnDown@@3HA=_g_d2client_addr_6FBCBC50")
#pragma comment(linker, "/alternatename:?g_bTransmuteBtnPressed@@3HA=_g_d2client_addr_6FBCBC4C")
#pragma comment(linker, "/alternatename:?g_bWaitingForServerCursorSync@@3HA=_g_d2client_addr_6FBCB9F4")
#pragma comment(linker, "/alternatename:?g_bWeaponSwapActive@@3HA=_g_d2client_addr_6FBCBF78")
#pragma comment(linker, "/alternatename:?g_bbeltExpanded@@3HA=_g_d2client_addr_6FBB2B60")
#pragma comment(linker, "/alternatename:?g_dwBytesReceived@@3IA=_g_d2client_addr_6FBCBE50")
#pragma comment(linker, "/alternatename:?g_dwHoradricAnimTick@@3KA=_g_d2client_addr_6FBCBA24")
#pragma comment(linker, "/alternatename:?g_dwMusicSequenceEndTick@@3IA=_g_d2client_addr_6FBCC274")
#pragma comment(linker, "/alternatename:?g_dwMusicSequenceTriggerTick1@@3IA=_g_d2client_addr_6FBCC260")
#pragma comment(linker, "/alternatename:?g_dwMusicSequenceTriggerTick2@@3IA=_g_d2client_addr_6FBCC26C")
#pragma comment(linker, "/alternatename:?g_dwPacketsReceived@@3IA=_g_d2client_addr_6FBCBC90")
#pragma comment(linker, "/alternatename:?g_dwShakeStartTick@@3IA=_g_d2client_addr_6FBAD104")
#pragma comment(linker, "/alternatename:?g_dwShopHoverCooldownTick@@3KA=_g_d2client_addr_6FBCBA10")
#pragma comment(linker, "/alternatename:?g_nCurrentAct@@3HA=_g_d2client_addr_6FBCB643")
#pragma comment(linker, "/alternatename:?g_nCurrentClientTick@@3HA=_g_d2client_addr_6FBB32B0")
#pragma comment(linker, "/alternatename:?g_nCurrentInvPage@@3EA=_g_d2client_addr_6FBCBA0C")
#pragma comment(linker, "/alternatename:?g_nCurrentMusicLevelId@@3HA=_g_d2client_addr_6FBCC250")
#pragma comment(linker, "/alternatename:?g_nCurrentMusicSoundId@@3HA=_g_d2client_addr_6FBCC24C")
#pragma comment(linker, "/alternatename:?g_nCursorState@@3HA=_g_d2client_addr_6FBB1674")
#pragma comment(linker, "/alternatename:?g_nDiabloSealFlashCounter@@3HA=_g_d2client_addr_6FBA8AD4")
#pragma comment(linker, "/alternatename:?g_nFirstMusicSoundId@@3HA=_g_d2client_addr_6FBCC158")
#pragma comment(linker, "/alternatename:?g_nGameDifficulty@@3EA=_g_d2client_addr_6FBCBFF4")
#pragma comment(linker, "/alternatename:?g_nGoldDialogMode@@3HA=_g_d2client_addr_6FBCBC40")
#pragma comment(linker, "/alternatename:?g_nHoradricAnimFrame@@3IA=_g_d2client_addr_6FBCBA20")
#pragma comment(linker, "/alternatename:?g_nHoveredItemGridBottomY@@3HA=_g_d2client_addr_6FB8CADC")
#pragma comment(linker, "/alternatename:?g_nHoveredItemGridCenterX@@3HA=_g_d2client_addr_6FB8CAD8")
#pragma comment(linker, "/alternatename:?g_nHoveredItemInvPage@@3EA=_g_d2client_addr_6FBCBA08")
#pragma comment(linker, "/alternatename:?g_nHoveredItemInvRecordId@@3HA=_g_d2client_addr_6FBCBA04")
#pragma comment(linker, "/alternatename:?g_nHoveredItemSocketTop@@3HA=_g_d2client_addr_6FB8CAD4")
#pragma comment(linker, "/alternatename:?g_nHoveredItemSocketX@@3HA=_g_d2client_addr_6FB8CAD0")
#pragma comment(linker, "/alternatename:?g_nHoveredItemTooltipTop@@3HA=_g_d2client_addr_6FB8CACC")
#pragma comment(linker, "/alternatename:?g_nHoveredItemTooltipX@@3HA=_g_d2client_addr_6FB8CAC8")
#pragma comment(linker, "/alternatename:?g_nHoveredShopSubBtnIndex@@3HA=_g_d2client_addr_6FBCBCAC")
#pragma comment(linker, "/alternatename:?g_nHoveredSkillId_6FBBA6E8@@3HA=_g_d2client_addr_6FBBA6E8")
#pragma comment(linker, "/alternatename:?g_nInteractedNpcGUID@@3HA=_g_d2client_addr_6FBCB625")
#pragma comment(linker, "/alternatename:?g_nInvPanelMode@@3HA=_g_d2client_addr_6FBCB9F8")
#pragma comment(linker, "/alternatename:?g_nLastMouseDownX@@3HA=_g_d2client_addr_6FBB0B94")
#pragma comment(linker, "/alternatename:?g_nLastMouseDownY@@3HA=_g_d2client_addr_6FBB0D20")
#pragma comment(linker, "/alternatename:?g_nLastMusicSoundId@@3HA=_g_d2client_addr_6FBCC15C")
#pragma comment(linker, "/alternatename:?g_nLastNotifiedMusicLevelId@@3HA=_g_d2client_addr_6FBCC278")
#pragma comment(linker, "/alternatename:?g_nLeftPanelOffsetX@@3HA=_g_d2client_addr_6FBCBD28")
#pragma comment(linker, "/alternatename:?g_nLeftPanelOffsetY@@3HA=_g_d2client_addr_6FBCBD2C")
#pragma comment(linker, "/alternatename:?g_nMercTalkStringId@@3HA=_g_d2client_addr_6FBCBCBC")
#pragma comment(linker, "/alternatename:?g_nMouseX@@3HA=_g_d2client_addr_6FBB1638")
#pragma comment(linker, "/alternatename:?g_nMouseY@@3HA=_g_d2client_addr_6FBB1634")
#pragma comment(linker, "/alternatename:?g_nMusicCheckpointTick@@3HA=_g_d2client_addr_6FBCC254")
#pragma comment(linker, "/alternatename:?g_nMusicLastRestartTick@@3HA=_g_d2client_addr_6FBCC248")
#pragma comment(linker, "/alternatename:?g_nMusicLevelEnterTick@@3HA=_g_d2client_addr_6FBCC244")
#pragma comment(linker, "/alternatename:?g_nMusicSequenceSoundId1@@3HA=_g_d2client_addr_6FBCC25C")
#pragma comment(linker, "/alternatename:?g_nMusicSequenceSoundId2@@3HA=_g_d2client_addr_6FBCC268")
#pragma comment(linker, "/alternatename:?g_nOverrideAmbientBlue@@3EA=_g_d2client_addr_6FBCB7CE")
#pragma comment(linker, "/alternatename:?g_nOverrideAmbientGreen@@3EA=_g_d2client_addr_6FBCB7CD")
#pragma comment(linker, "/alternatename:?g_nOverrideAmbientIntensity@@3HA=_g_d2client_addr_6FBCB7C8")
#pragma comment(linker, "/alternatename:?g_nOverrideAmbientRed@@3EA=_g_d2client_addr_6FBCB7CC")
#pragma comment(linker, "/alternatename:?g_nPanelOffsetY@@3HA=_g_d2client_addr_6FBCBD2C")
#pragma comment(linker, "/alternatename:?g_nPlayerIsoY@@3HA=_g_d2client_addr_6FBCD528")
#pragma comment(linker, "/alternatename:?g_nRightPanelOffsetX@@3HA=_g_d2client_addr_6FBCBD28")
#pragma comment(linker, "/alternatename:?g_nScreenHeight@@3HA=_g_d2client_addr_6FB8C6E4")
#pragma comment(linker, "/alternatename:?g_nScreenOffsetX_6FBA79A0@@3HA=_g_d2client_addr_6FBA79A0")
#pragma comment(linker, "/alternatename:?g_nScreenWidth@@3HA=_g_d2client_addr_6FB8C6E0")
#pragma comment(linker, "/alternatename:?g_nSelectedShopTab@@3EA=_g_d2client_addr_6FBCBC60")
#pragma comment(linker, "/alternatename:?g_nShakeHoldMs@@3HA=_g_d2client_addr_6FBAD10C")
#pragma comment(linker, "/alternatename:?g_nShakeMaxAmplitude@@3HA=_g_d2client_addr_6FBCB9B0")
#pragma comment(linker, "/alternatename:?g_nShakeRampDownMs@@3HA=_g_d2client_addr_6FBAD110")
#pragma comment(linker, "/alternatename:?g_nShakeRampUpMs@@3HA=_g_d2client_addr_6FBAD108")
#pragma comment(linker, "/alternatename:?g_nShopPendingCost@@3HA=_g_d2client_addr_6FBCBC84")
#pragma comment(linker, "/alternatename:?g_nShopPendingCursorAction@@3HA=_g_d2client_addr_6FBCBC7C")
#pragma comment(linker, "/alternatename:?g_nShopPendingItemGUID@@3HA=_g_d2client_addr_6FBCBC80")
#pragma comment(linker, "/alternatename:?g_nShopPendingTabMode@@3HA=_g_d2client_addr_6FBCBC88")
#pragma comment(linker, "/alternatename:?g_nShopTabCount@@3HA=_g_d2client_addr_6FBCBC64")
#pragma comment(linker, "/alternatename:?g_nSkillDescTxtRecordCount_6FDD1C4C@@3HA=_g_d2client_addr_6FB7D61C")
#pragma comment(linker, "/alternatename:?g_nSkillHoverX_6FBBA6D0@@3HA=_g_d2client_addr_6FBBA6D0")
#pragma comment(linker, "/alternatename:?g_nSkillHoverY_6FBBA6CC@@3HA=_g_d2client_addr_6FBBA6CC")
#pragma comment(linker, "/alternatename:?g_nSkillTreeTab_6FBA79FC@@3HA=_g_d2client_addr_6FBA79FC")
#pragma comment(linker, "/alternatename:?g_nSmoothedFps@@3HA=_g_d2client_addr_6FBB32C0")
#pragma comment(linker, "/alternatename:?g_nSpeechCallbackParam@@3HA=_g_d2client_addr_6FBCC0F8")
#pragma comment(linker, "/alternatename:?g_nSpeedBarCount_6FBC21C8@@3HA=_g_d2client_addr_6FBC21C8")
#pragma comment(linker, "/alternatename:?g_nSpeedBarHoverIdx_6FBC21CC@@3HA=_g_d2client_addr_6FBC21CC")
#pragma comment(linker, "/alternatename:?g_nSpeedBarSkillFlags_6FBC22C8@@3PAHA=_g_d2client_addr_6FBC22C8")
#pragma comment(linker, "/alternatename:?g_nSpeedBarSkillIds_6FBC21D8@@3PAHA=_g_d2client_addr_6FBC21D8")
#pragma comment(linker, "/alternatename:?g_nSpeedBarSkillLevels_6FBC23B8@@3PAHA=_g_d2client_addr_6FBC23B8")
#pragma comment(linker, "/alternatename:?g_nSpeedBarSkillX_6FBC2598@@3PAHA=_g_d2client_addr_6FBC2598")
#pragma comment(linker, "/alternatename:?g_nSpeedBarSkillY_6FBC24A8@@3PAHA=_g_d2client_addr_6FBC24A8")
#pragma comment(linker, "/alternatename:?g_nStashObjectGUID@@3HA=_g_d2client_addr_6FBCBCC0")
#pragma comment(linker, "/alternatename:?g_nUiPanelCount@@3HA=_g_d2client_addr_6FB8F368")
#pragma comment(linker, "/alternatename:?g_nViewShiftState@@3HA=_g_d2client_addr_6FBCC1D0")
#pragma comment(linker, "/alternatename:?g_nViewportOffsetX@@3HA=_g_d2client_addr_6FBCC040")
#pragma comment(linker, "/alternatename:?g_nViewportOffsetY@@3HA=_g_d2client_addr_6FBCC044")
#pragma comment(linker, "/alternatename:?g_nViewportTargetOffsetX@@3HA=_g_d2client_addr_6FBCC05C")
#pragma comment(linker, "/alternatename:?g_nViewportTargetOffsetY@@3HA=_g_d2client_addr_6FBCC060")
#pragma comment(linker, "/alternatename:?g_pActiveConsumeTomeOrScroll@@3PAUD2UnitStrc@@A=_g_d2client_addr_6FBB1644")
#pragma comment(linker, "/alternatename:?g_pBankPanelCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA38")
#pragma comment(linker, "/alternatename:?g_pBuySellTabsCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBC74")
#pragma comment(linker, "/alternatename:?g_pCelBuySellBtn@@3PAXA=_g_d2client_addr_6FBCBD4C")
#pragma comment(linker, "/alternatename:?g_pClientAct@@3PAXA=_g_d2client_addr_6FBCC01C")
#pragma comment(linker, "/alternatename:?g_pGameQuestRecord@@3PAUD2BitBufferStrc@@A=_g_d2client_addr_6FBCB647")
#pragma comment(linker, "/alternatename:?g_pHireInvPanelCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA30")
#pragma comment(linker, "/alternatename:?g_pHoradricAnimCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA28")
#pragma comment(linker, "/alternatename:?g_pHoradricBoxCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA3C")
#pragma comment(linker, "/alternatename:?g_pHoveredItemUnit@@3PAUD2UnitStrc@@A=_g_d2client_addr_6FBCBA00")
#pragma comment(linker, "/alternatename:?g_pHoveredSocketHostItem@@3PAUD2UnitStrc@@A=_g_d2client_addr_6FBCB9FC")
#pragma comment(linker, "/alternatename:?g_pInvGoldBtnCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBC70")
#pragma comment(linker, "/alternatename:?g_pMusicTrackResumeOffsets@@3PAHA=_g_d2client_addr_6FBB5574")
#pragma comment(linker, "/alternatename:?g_pPlayerQuestRecord@@3PAUD2BitBufferStrc@@A=_g_d2client_addr_6FBCB643")
#pragma comment(linker, "/alternatename:?g_pPlayerUnit@@3PAUD2UnitStrc@@A=?gpPlayerUnit@@3PAUD2UnitStrc@@A")
#pragma comment(linker, "/alternatename:?g_pRecipeScrollCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA2C")
#pragma comment(linker, "/alternatename:?g_pSkillDescTxt_6FDD1C44@@3PAUD2SkillDescTxt@@A=_g_d2client_addr_6FB7D5FC")
#pragma comment(linker, "/alternatename:?g_pTradePanelCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA40")
#pragma comment(linker, "/alternatename:?g_pTransmogrifierBtnCell@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBA34")
#pragma comment(linker, "/alternatename:?g_pfnSpeechCallback@@3PAXA=_g_d2client_addr_6FBCC0F4")
#pragma comment(linker, "/alternatename:?g_rInvPanelRect@@3UtagRECT@@A=_g_d2client_addr_6FBB0BE8")
#pragma comment(linker, "/alternatename:?g_rNpcShopGridRect@@3UtagRECT@@A=_g_d2client_addr_6FBB0CA4")
#pragma comment(linker, "/alternatename:?g_rStashPanelRect@@3UtagRECT@@A=_g_d2client_addr_6FBB0C70")
#pragma comment(linker, "/alternatename:?g_rTradeBottomGridRect@@3UtagRECT@@A=_g_d2client_addr_6FBB0D10")
#pragma comment(linker, "/alternatename:?g_rTradeTopGridRect@@3UtagRECT@@A=_g_d2client_addr_6FBB0CEC")
#pragma comment(linker, "/alternatename:?g_tCubeClassicGridLayout@@3UD2InvGridLayoutStrc@@A=_g_d2client_addr_6FBB0C88")
#pragma comment(linker, "/alternatename:?g_tCubeLoDGridLayout@@3UD2InvGridLayoutStrc@@A=_g_d2client_addr_6FBB0BC0")
#pragma comment(linker, "/alternatename:?g_tInvGridLayout@@3UD2InvGridLayoutStrc@@A=_g_d2client_addr_6FBB0CD0")
#pragma comment(linker, "/alternatename:?g_tStashGridLayout@@3UD2InvGridLayoutStrc@@A=_g_d2client_addr_6FBB0B78")
#pragma comment(linker, "/alternatename:?g_tTradeTopGridLayout@@3UD2InvGridLayoutStrc@@A=_g_d2client_addr_6FBB0CB8")
#pragma comment(linker, "/alternatename:?gaAutomapCelGroups@@3PAHA=_g_d2client_addr_6FBC2798")
#pragma comment(linker, "/alternatename:?gaPanelStates@@3PAHA=_g_d2client_addr_6FBB2B58")
#pragma comment(linker, "/alternatename:?gabBeltSlotReady@@3PAEA=_g_d2client_addr_6FBCBF88")
#pragma comment(linker, "/alternatename:?gapClientItemHashTable@@3PAPAUD2UnitStrc@@A=_g_d2client_addr_6FBCB160")
#pragma comment(linker, "/alternatename:?gapCursorCellFiles@@3PAPAUD2CellFileStrc@@A=_g_d2client_addr_6FBB1648")
#pragma comment(linker, "/alternatename:?gapMonsterHashTable@@3PAPAUD2UnitStrc@@A=_g_d2client_addr_6FBCAB60")
#pragma comment(linker, "/alternatename:?gapfnObjectClientFunctions@@3PAP6IEPAUD2UnitStrc@@@ZA=_g_d2client_var_gapfnObjectClientFunctions")
#pragma comment(linker, "/alternatename:?gbAct3ObjectSoundPreloaded@@3EA=_g_d2client_var_gbAct3ObjectSoundPreloaded")
#pragma comment(linker, "/alternatename:?gbActLoadedFlag@@3HA=_g_d2client_addr_6FBCC004")
#pragma comment(linker, "/alternatename:?gbActUnloadedFlag@@3HA=_g_d2client_addr_6FBCC00C")
#pragma comment(linker, "/alternatename:?gbAlwaysRun@@3HA=_g_d2client_addr_6FBCC054")
#pragma comment(linker, "/alternatename:?gbAlwaysRunEnabled@@3HA=_g_d2client_addr_6FBCC050")
#pragma comment(linker, "/alternatename:?gbAutomapActive@@3HA=_g_d2client_addr_6FBB2B80")
#pragma comment(linker, "/alternatename:?gbAutomapCenters@@3HA=_g_d2client_addr_6FBA3B9C")
#pragma comment(linker, "/alternatename:?gbAutomapLeft@@3HA=_g_d2client_addr_6FBCC330")
#pragma comment(linker, "/alternatename:?gbAutomapLeftAlign2@@3HA=_g_d2client_addr_6FBCC334")
#pragma comment(linker, "/alternatename:?gbAutomapLeftAlign3@@3HA=_g_d2client_addr_6FBCC338")
#pragma comment(linker, "/alternatename:?gbAutomapLeftAlign@@3HA=_g_d2client_addr_6FBCC330")
#pragma comment(linker, "/alternatename:?gbAutomapLeftSaved1@@3HA=_g_d2client_addr_6FBCC334")
#pragma comment(linker, "/alternatename:?gbAutomapLeftSaved2@@3HA=_g_d2client_addr_6FBCC338")
#pragma comment(linker, "/alternatename:?gbAutomapMinimapMode@@3HA=_g_d2client_addr_6FBCC2A0")
#pragma comment(linker, "/alternatename:?gbAutomapShowParty@@3HA=_g_d2client_addr_6FBA3BA0")
#pragma comment(linker, "/alternatename:?gbAutomapShowPartyNames@@3HA=_g_d2client_addr_6FBA3BA4")
#pragma comment(linker, "/alternatename:?gbBeltShowOpen@@3HA=_g_d2client_addr_6FBB2BD4")
#pragma comment(linker, "/alternatename:?gbCharacterScreenOpen@@3HA=_g_d2client_addr_6FBB2B60")
#pragma comment(linker, "/alternatename:?gbChatBoxOpen@@3HA=_g_d2client_addr_6FBB2B6C")
#pragma comment(linker, "/alternatename:?gbCmdTblAltKeyHandlersRegistered@@3HA=_g_d2client_addr_6FBD33F8")
#pragma comment(linker, "/alternatename:?gbCmdTblInputEnabled@@3HA=_g_d2client_addr_6FBCBCC4")
#pragma comment(linker, "/alternatename:?gbCmdTblKeyHandlersRegistered@@3HA=_g_d2client_addr_6FBD33FC")
#pragma comment(linker, "/alternatename:?gbCursorTooltipLocked@@3HA=_g_d2client_addr_6FBB1630")
#pragma comment(linker, "/alternatename:?gbEnvSoundPositionEnabled@@3EA=_g_d2client_var_gbEnvSoundPositionEnabled")
#pragma comment(linker, "/alternatename:?gbEnvironmentIsNightCycle@@3HA=_g_d2client_addr_6FBCBCE0")
#pragma comment(linker, "/alternatename:?gbEquippedItemComponentChanged@@3HA=_g_d2client_addr_6FBCBCFC")
#pragma comment(linker, "/alternatename:?gbEscMenuOpen@@3HA=_g_d2client_addr_6FBCC008")
#pragma comment(linker, "/alternatename:?gbEscOptionsOpen@@3HA=_g_d2client_addr_6FBB2B7C")
#pragma comment(linker, "/alternatename:?gbExpansionMode@@3HA=_g_d2client_addr_6FBC97E0")
#pragma comment(linker, "/alternatename:?gbGoldDialogOpen@@3HA=_g_d2client_addr_6FBB5490")
#pragma comment(linker, "/alternatename:?gbGreenBloodEnabled@@3HA=_g_d2client_addr_6FBCBFE4")
#pragma comment(linker, "/alternatename:?gbHelpScreenOpen@@3HA=_g_d2client_addr_6FBB2B70")
#pragma comment(linker, "/alternatename:?gbHireableScreenOpen@@3HA=_g_d2client_addr_6FBB2BE8")
#pragma comment(linker, "/alternatename:?gbInventoryOpen@@3HA=_g_d2client_addr_6FBB2B5C")
#pragma comment(linker, "/alternatename:?gbIsExpansionClient@@3HA=_g_d2client_addr_6FBB330C")
#pragma comment(linker, "/alternatename:?gbIsLadderClient@@3HA=_g_d2client_addr_6FBB3310")
#pragma comment(linker, "/alternatename:?gbMessageLogOpen@@3HA=_g_d2client_addr_6FBB2BB8")
#pragma comment(linker, "/alternatename:?gbMultiplayerGame@@3HA=_g_d2client_addr_6FBCBFF8")
#pragma comment(linker, "/alternatename:?gbNpcMenuOpen@@3HA=_g_d2client_addr_6FBCB629")
#pragma comment(linker, "/alternatename:?gbNpcSpeechEnabled@@3HA=_g_d2client_addr_6FB905EC")
#pragma comment(linker, "/alternatename:?gbPacketItemActionTable@@3PAEA=_g_d2client_addr_6FBAA2D0")
#pragma comment(linker, "/alternatename:?gbPartyScreenOpen@@3HA=_g_d2client_addr_6FBB2BB0")
#pragma comment(linker, "/alternatename:?gbRunKeyHeld@@3HA=_g_d2client_addr_6FBCC04C")
#pragma comment(linker, "/alternatename:?gbRunLock@@3HA=_g_d2client_addr_6FBCC04C")
#pragma comment(linker, "/alternatename:?gbRunWalkToggle@@3HA=_g_d2client_addr_6FBCBE58")
#pragma comment(linker, "/alternatename:?gbShowGroundItems@@3HA=_g_d2client_addr_6FBB2B8C")
#pragma comment(linker, "/alternatename:?gbSkillSelectOpen@@3HA=_g_d2client_addr_6FBB2B64")
#pragma comment(linker, "/alternatename:?gbSkillSelectPopupOpen@@3HA=_g_d2client_addr_6FBD3400")
#pragma comment(linker, "/alternatename:?gbSkillTreeOpen@@3HA=_g_d2client_addr_6FBB2B68")
#pragma comment(linker, "/alternatename:?gbSoundDebugOverlay_6FBCC2D0@@3HA=_g_d2client_addr_6FBCC2D0")
#pragma comment(linker, "/alternatename:?gbSpeedBarOpen_6FBCBC50@@3HA=_g_d2client_addr_6FBCBC50")
#pragma comment(linker, "/alternatename:?gbSpeedBarRightSide_6FBCBC54@@3HA=_g_d2client_addr_6FBCBC54")
#pragma comment(linker, "/alternatename:?gbStandStillActive@@3HA=_g_d2client_addr_6FBCC054")
#pragma comment(linker, "/alternatename:?gbTowerTomeSoundPreloaded@@3EA=_g_d2client_var_gbTowerTomeSoundPreloaded")
#pragma comment(linker, "/alternatename:?gbTradeOpen@@3HA=_g_d2client_addr_6FBB2B88")
#pragma comment(linker, "/alternatename:?gbUseItemPanelOpen@@3HA=_g_d2client_addr_6FBCC120")
#pragma comment(linker, "/alternatename:?gbVendorInteractOpen@@3HA=_g_d2client_addr_6FBB2BBC")
#pragma comment(linker, "/alternatename:?gdwArenaFlags@@3IA=_g_d2client_addr_6FBA675C")
#pragma comment(linker, "/alternatename:?gdwClientGameFrame@@3IA=_g_d2client_addr_6FBB32B0")
#pragma comment(linker, "/alternatename:?gdwClientGameTick@@3HA=_g_d2client_addr_6FBB32B0")
#pragma comment(linker, "/alternatename:?gdwClientGameTick@@3IA=_g_d2client_addr_6FBB32B0")
#pragma comment(linker, "/alternatename:?gdwClientPing_6FBB32BC@@3IA=_g_d2client_addr_6FBB32BC")
#pragma comment(linker, "/alternatename:?gdwClientTickCount@@3IA=_g_d2client_addr_6FBB32B0")
#pragma comment(linker, "/alternatename:?gdwGameTickCounter@@3IA=_g_d2client_addr_6FBB32BC")
#pragma comment(linker, "/alternatename:?gdwLastCombatSoundTick_6FBCC35C@@3IA=_g_d2client_addr_6FBCC35C")
#pragma comment(linker, "/alternatename:?gdwLastPlayerSpeechTick_6FBA2090@@3IA=_g_d2client_addr_6FBA2090")
#pragma comment(linker, "/alternatename:?gdwLeftHandWeaponClass@@3IA=_g_d2client_addr_6FBCBCE8")
#pragma comment(linker, "/alternatename:?gdwNextSoundHandle_6FBCC374@@3IA=_g_d2client_addr_6FBCC374")
#pragma comment(linker, "/alternatename:?gdwReassignGraceTick@@3IA=_g_d2client_addr_6FBB32D4")
#pragma comment(linker, "/alternatename:?gdwRightHandWeaponClass@@3IA=_g_d2client_addr_6FBCBCE4")
#pragma comment(linker, "/alternatename:?gdwSavedLeftSkillOwnerGUID@@3HA=_g_d2client_addr_6FBCBD18")
#pragma comment(linker, "/alternatename:?gdwSavedRightSkillOwnerGUID@@3HA=_g_d2client_addr_6FBCBD10")
#pragma comment(linker, "/alternatename:?gdwSavedUnitMaxHP@@3HA=_g_d2client_addr_6FBCBD08")
#pragma comment(linker, "/alternatename:?gdwSavedUnitMaxMana@@3HA=_g_d2client_addr_6FBCBD04")
#pragma comment(linker, "/alternatename:?gdwSavedUnitMaxStamina@@3HA=_g_d2client_addr_6FBCBD00")
#pragma comment(linker, "/alternatename:?gdwScreenFadeEndTick@@3IA=_g_d2client_addr_6FBCC34C")
#pragma comment(linker, "/alternatename:?gdwSoundDuckStartTick_6FBCC370@@3IA=_g_d2client_addr_6FBCC370")
#pragma comment(linker, "/alternatename:?gdwaSkillDelayEndFrame@@3HA=_g_d2client_addr_6FBB3314")
#pragma comment(linker, "/alternatename:?ghClientAcceleratorTable@@3PAUHACCEL__@@A=_g_d2client_addr_6FBCC43C")
#pragma comment(linker, "/alternatename:?gnActiveNpcClassId@@3HA=_g_d2client_addr_6FBCB62D")
#pragma comment(linker, "/alternatename:?gnAutomapAllocatedCells@@3HA=_g_d2client_addr_6FBCC2AC")
#pragma comment(linker, "/alternatename:?gnAutomapDivisor@@3HA=_g_d2client_addr_6FBA3B98")
#pragma comment(linker, "/alternatename:?gnAutomapFadeDefault@@3HA=_g_d2client_addr_6FBCC2FC")
#pragma comment(linker, "/alternatename:?gnAutomapFadeMode@@3HA=_g_d2client_addr_6FBCC2F8")
#pragma comment(linker, "/alternatename:?gnAutomapMiniLeftX@@3HA=_g_d2client_addr_6FBCC32C")
#pragma comment(linker, "/alternatename:?gnAutomapMiniTopY@@3HA=_g_d2client_addr_6FBCC328")
#pragma comment(linker, "/alternatename:?gnAutomapMode@@3HA=_g_d2client_addr_6FBCC2A0")
#pragma comment(linker, "/alternatename:?gnAutomapOffsetX@@3HA=_g_d2client_addr_6FBCC2E8")
#pragma comment(linker, "/alternatename:?gnAutomapOffsetY@@3HA=_g_d2client_addr_6FBCC2EC")
#pragma comment(linker, "/alternatename:?gnAutomapPanX@@3HA=_g_d2client_addr_6FBCC2D8")
#pragma comment(linker, "/alternatename:?gnAutomapPanY@@3HA=_g_d2client_addr_6FBCC2DC")
#pragma comment(linker, "/alternatename:?gnAutomapRevealDelayTicks@@3HA=_g_d2client_addr_6FBCC2F4")
#pragma comment(linker, "/alternatename:?gnAutomapSavedFadeMode@@3HA=_g_d2client_addr_6FBCC2FC")
#pragma comment(linker, "/alternatename:?gnCameraShakeOffsetX@@3HA=_g_d2client_addr_6FBCC3BC")
#pragma comment(linker, "/alternatename:?gnCameraShakeOffsetY@@3HA=_g_d2client_addr_6FBCC3C0")
#pragma comment(linker, "/alternatename:?gnClientLoopCount@@3HA=_g_d2client_addr_6FBB32B0")
#pragma comment(linker, "/alternatename:?gnClientSkillUseState@@3HA=_g_d2client_addr_6FBCBC48")
#pragma comment(linker, "/alternatename:?gnCurrentEnvironmentLevelId@@3HA=_g_d2client_addr_6FB84118")
#pragma comment(linker, "/alternatename:?gnCurrentEnvironmentSoundId@@3HA=_g_d2client_addr_6FB8411C")
#pragma comment(linker, "/alternatename:?gnCursorAnimId@@3HA=_g_d2client_addr_6FBB1664")
#pragma comment(linker, "/alternatename:?gnCursorFrameFixed8@@3HA=_g_d2client_addr_6FBB1668")
#pragma comment(linker, "/alternatename:?gnCursorHoverUnitId@@3HA=_g_d2client_addr_6FBB163C")
#pragma comment(linker, "/alternatename:?gnCursorHoverUnitType@@3HA=_g_d2client_addr_6FBB1640")
#pragma comment(linker, "/alternatename:?gnCursorItemColorIndex@@3EA=_g_d2client_addr_6FBB1678")
#pragma comment(linker, "/alternatename:?gnCursorSoundId@@3HA=_g_d2client_addr_6FBB166C")
#pragma comment(linker, "/alternatename:?gnCursorSpriteType@@3HA=_g_d2client_addr_6FBB1674")
#pragma comment(linker, "/alternatename:?gnCursorState@@3HA=_g_d2client_addr_6FBB1674")
#pragma comment(linker, "/alternatename:?gnCursorTargetMode@@3HA=_g_d2client_addr_6FBB1664")
#pragma comment(linker, "/alternatename:?gnCursorTargetSubType@@3HA=_g_d2client_addr_6FBB1668")
#pragma comment(linker, "/alternatename:?gnEnvironmentLoopSoundHandle@@3HA=_g_d2client_addr_6FBCBCF0")
#pragma comment(linker, "/alternatename:?gnEnvironmentRainSoundHandle@@3HA=_g_d2client_addr_6FBCBCF4")
#pragma comment(linker, "/alternatename:?gnEscMenuOpenFlag@@3HA=_g_d2client_addr_6FBB5490")
#pragma comment(linker, "/alternatename:?gnForceTileSoundType_6FBCC360@@3HA=_g_d2client_addr_6FBCC360")
#pragma comment(linker, "/alternatename:?gnGameFrame@@3HA=_g_d2client_addr_6FBCC390")
#pragma comment(linker, "/alternatename:?gnHoveredGroundItemId@@3HA=_g_d2client_addr_6FBCC11C")
#pragma comment(linker, "/alternatename:?gnHoveredGroundItemType@@3HA=_g_d2client_addr_6FBCC118")
#pragma comment(linker, "/alternatename:?gnMouseScreenX@@3HA=_g_d2client_addr_6FBB1634")
#pragma comment(linker, "/alternatename:?gnMouseScreenY@@3HA=_g_d2client_addr_6FBB1638")
#pragma comment(linker, "/alternatename:?gnMouseX@@3HA=_g_d2client_addr_6FBB1634")
#pragma comment(linker, "/alternatename:?gnMouseY@@3HA=_g_d2client_addr_6FBB1638")
#pragma comment(linker, "/alternatename:?gnPanelCount@@3HA=_g_d2client_addr_6FB8F368")
#pragma comment(linker, "/alternatename:?gnPanelShiftMode@@3HA=_g_d2client_addr_6FBCC1D0")
#pragma comment(linker, "/alternatename:?gnPanelShiftOffset@@3HA=_g_d2client_addr_6FBCC1D4")
#pragma comment(linker, "/alternatename:?gnPendingQuestSpeechSoundId_6FBA2094@@3IA=_g_d2client_addr_6FBA2094")
#pragma comment(linker, "/alternatename:?gnRealmStatusValue@@3HA=_g_d2client_addr_6FBCC020")
#pragma comment(linker, "/alternatename:?gnRightPanelOpenFlag@@3HA=_g_d2client_addr_6FBB2B90")
#pragma comment(linker, "/alternatename:?gnSavedLeftSkillId@@3HA=_g_d2client_addr_6FBCBD14")
#pragma comment(linker, "/alternatename:?gnSavedRightSkillId@@3HA=_g_d2client_addr_6FBCBD0C")
#pragma comment(linker, "/alternatename:?gnScreenHeight@@3HA=_g_d2client_addr_6FB8C6E4")
#pragma comment(linker, "/alternatename:?gnShrineClientTableCount@@3HA=_g_d2client_var_gnShrineClientTableCount")
#pragma comment(linker, "/alternatename:?gnSoundMaxVoices_6FBCC1C8@@3HA=_g_d2client_addr_6FBCC1C8")
#pragma comment(linker, "/alternatename:?gnTooltipUnitGUID@@3HA=_g_d2client_addr_6FBCC11C")
#pragma comment(linker, "/alternatename:?gnTooltipUnitType@@3HA=_g_d2client_addr_6FBCC118")
#pragma comment(linker, "/alternatename:?gnViewportAnchorX@@3HA=_g_d2client_addr_6FBC21D0")
#pragma comment(linker, "/alternatename:?gnViewportAnchorY@@3HA=_g_d2client_addr_6FBC21CC")
#pragma comment(linker, "/alternatename:?gnViewportHeight@@3HA=_g_d2client_addr_6FBCD528")
#pragma comment(linker, "/alternatename:?gnViewportOffsetX_6FBC21D0@@3HA=_g_d2client_addr_6FBC21D0")
#pragma comment(linker, "/alternatename:?gnViewportOffsetY_6FBC21CC@@3HA=_g_d2client_addr_6FBC21CC")
#pragma comment(linker, "/alternatename:?gnViewportShiftMode@@3HA=_g_d2client_addr_6FBCC1D0")
#pragma comment(linker, "/alternatename:?gnViewportShiftOffsetX@@3HA=_g_d2client_addr_6FBCC1D4")
#pragma comment(linker, "/alternatename:?gnViewportTopOffset@@3HA=_g_d2client_addr_6FBCC028")
#pragma comment(linker, "/alternatename:?gnViewportWidth@@3HA=_g_d2client_addr_6FBCD524")
#pragma comment(linker, "/alternatename:?gpActiveSoundList_6FBCC36C@@3PAUD2ClientSoundChannelStrc@@A=_g_d2client_addr_6FBCC36C")
#pragma comment(linker, "/alternatename:?gpAutomapCellPages@@3PAUD2AutomapCellPageStrc@@A=_g_d2client_addr_6FBCC2A8")
#pragma comment(linker, "/alternatename:?gpAutomapLayerList@@3PAUD2AutomapLayerStrc@@A=_g_d2client_addr_6FBCC2B0")
#pragma comment(linker, "/alternatename:?gpAutomapSerializeCursor@@3PAFA=_g_d2client_addr_6FBCC2A4")
#pragma comment(linker, "/alternatename:?gpAutomapSprites@@3PAPAUD2CellFileStrc@@A=_g_d2client_addr_6FBCC2B8")
#pragma comment(linker, "/alternatename:?gpBuySellBtnCel@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBCBD4C")
#pragma comment(linker, "/alternatename:?gpChatMessageList@@3PAXA=_g_d2client_addr_6FBB54CC")
#pragma comment(linker, "/alternatename:?gpClientPlayer@@3PAUD2UnitStrc@@A=?gpPlayerUnit@@3PAUD2UnitStrc@@A")
#pragma comment(linker, "/alternatename:?gpClientViewport@@3PAXA=_g_d2client_addr_6FBCC1E0")
#pragma comment(linker, "/alternatename:?gpCurrentAutomapLayer@@3PAUD2AutomapLayerStrc@@A=_g_d2client_addr_6FBCC2B4")
#pragma comment(linker, "/alternatename:?gpCursorItemCellFile@@3PAUD2CellFileStrc@@A=_g_d2client_addr_6FBB1670")
#pragma comment(linker, "/alternatename:?gpCursorLockedItem@@3PAUD2UnitStrc@@A=_g_d2client_addr_6FBB1644")
#pragma comment(linker, "/alternatename:?gpLocalPlayerUnit@@3PAUD2UnitStrc@@A=?gpPlayerUnit@@3PAUD2UnitStrc@@A")
#pragma comment(linker, "/alternatename:?gpLocalQuestFlags@@3PAUD2BitBufferStrc@@A=_g_d2client_var_gpLocalQuestFlags")
#pragma comment(linker, "/alternatename:?gpPlayerUnit_6FBCC3D0@@3PAUD2UnitStrc@@A=?gpPlayerUnit@@3PAUD2UnitStrc@@A")
#pragma comment(linker, "/alternatename:?gpQuestMonster_6FBC57D0@@3PAUD2UnitStrc@@A=_g_d2client_addr_6FBC57D0")
#pragma comment(linker, "/alternatename:?gpfnClientLoopHook@@3PAXA=_g_d2client_addr_6FBCC54C")
#pragma comment(linker, "/alternatename:?gpfnMouseLButtonDown@@3P6IXXZA=_g_d2client_addr_6FBB0D24")
#pragma comment(linker, "/alternatename:?gpfnMouseLButtonUp@@3P6IXXZA=_g_d2client_addr_6FBB1624")
#pragma comment(linker, "/alternatename:?gpfnMouseMButtonDown@@3P6IXXZA=_g_d2client_addr_6FBB1628")
#pragma comment(linker, "/alternatename:?gpfnMouseMButtonUp@@3P6IXXZA=_g_d2client_addr_6FBB1618")
#pragma comment(linker, "/alternatename:?gpfnMouseRButtonDown@@3P6IXXZA=_g_d2client_addr_6FBB162C")
#pragma comment(linker, "/alternatename:?gpfnMouseRButtonUp@@3P6IXXZA=_g_d2client_addr_6FBB1614")
#pragma comment(linker, "/alternatename:?gpfnMouseWheelDown@@3P6IXXZA=_g_d2client_addr_6FBB1620")
#pragma comment(linker, "/alternatename:?gpfnMouseWheelUp@@3P6IXXZA=_g_d2client_addr_6FBB161C")
#pragma comment(linker, "/alternatename:?gpfnPrevClientLoopHook@@3PAXA=_g_d2client_addr_6FBB5364")
#pragma comment(linker, "/alternatename:?gszAccountOrCharName@@3PADA=_g_d2client_addr_6FBB33E0")
#pragma comment(linker, "/alternatename:?gszRealmName@@3PADA=_g_d2client_addr_6FBB3318")
#pragma comment(linker, "/alternatename:?gwszKeyCharBuf@@3PAUUnicode@@A=_g_d2client_addr_6FBCF530")

// Function linker aliases
#pragma comment(linker, "/alternatename:?ARCHIVE_CloseMpqaFile@@YIXPAX@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?ARCHIVE_LoadCellFile@@YIPAXPBDH@Z=?ARCHIVE_LoadCellFile@@YIPAUD2CellFileStrc@@PBDH@Z")
#pragma comment(linker, "/alternatename:?ARCHIVE_LoadFile@@YIPAXIPBDPAKHH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?ARCHIVE_LoadFileToBuffer@@YIPAXPBDPAXPAK@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?ARCHIVE_LoadFileToMemory@@YIPAXPAUHD2ARCHIVE__@@PBDPAIH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?ARCHIVE_OpenMpqaFile@@YIHPBDPAPAXH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?ARCHIVE_ReadMpqaFile@@YIHPAXH0@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?AUTOMAP_CenterOnPlayer@@YIXH@Z=?AUTOMAP_CenterView@@YIXH@Z")
#pragma comment(linker, "/alternatename:?AUTOMAP_SetFadeMode@@YIXH@Z=?AUTOMAP_SetFadeMode@@YIHH@Z")
#pragma comment(linker, "/alternatename:?CLIENTSIDE_FindUnit@@YIPAUD2UnitStrc@@PAPAUD2UnitLightViewStrc@@HH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?CMDTBL_DrawModalPrompt@@YGXXZ=?CURSOR_DrawLockedItemTooltip@@YIXXZ")
#pragma comment(linker, "/alternatename:?CMDTBL_PickUpSlotItemToCursor@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?CUNIT_HasActiveInteract@@YIHXZ=?sub_6FACEAC0@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?CUNIT_HasCursorItem@@YIHXZ=?sub_6FACEAC0@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?CUNIT_IterateClientUnitsOfType@@YIXHP6IXPAUD2UnitStrc@@@Z@Z=?sub_6FACEA20@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_AllocLightMap_6FB0EAF0@@YGPAXHHHHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?D2Client_AllocateClientUnit@@YIPAUD2UnitStrc@@HHHHHHHH@Z=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:?D2Client_ApplyMonsterShapeshiftMode@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_ApplyUnitStateRemovedClientSide_6FACEBC0@@YIXPAUD2UnitStrc@@H@Z=?sub_6FACEBC0@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_AreUnitsHostile_6FB08B60@@YIHPAUD2UnitStrc@@0@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_AssignSkillHotkeyFromPopup@@YIXH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_CanStartChanneledSkill@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_ChangeObjectAnimMode_6FAEB3C0@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_CheckPlayerCanOperateObject_6FAF0C30@@YIHH@Z=?D2Client_CanUnitInterruptCurrentMode@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_CheckPlayerRosterFlag_6FAE0830@@YIHIIH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_ClearPlayerMapWaypoints_6FAE0490@@YIXPAUD2UnitStrc@@@Z=?D2Client_AccumulatePerfCounters@@YAXHH@Z")
#pragma comment(linker, "/alternatename:?D2Client_CreateClientMissile_6FAD4B20@@YIPAUD2UnitStrc@@PAUD2ClientMissileParamStrc@@@Z=?D2Client_CreateClientMissile@@YIPAUD2UnitStrc@@PAUD2ClientMissileInitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_DispatchItemModeChange_6FB45810@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_DispatchMonsterModeChange_6FAE48D0@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_DispatchMonsterMoveOrSkill_6FAE5100@@YIXPAUD2UnitStrc@@HHIHH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?D2Client_DispatchPlayerModeChange_6FB0A820@@YIXPAUD2UnitStrc@@HHHIHHH@Z=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:?D2Client_DispatchUnitModeChange_6FAF1450@@YIHHPAUD2UnitStrc@@PAXH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_FindClientSideUnit@@YIPAUD2UnitStrc@@HH@Z=?D2Client_FindClientSideUnit@@YIPAUD2UnitStrc@@II@Z")
#pragma comment(linker, "/alternatename:?D2Client_FindRoomAtCoords@@YIPAUD2ActiveRoomStrc@@HH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_FindServerSideUnit@@YIPAUD2UnitStrc@@HH@Z=?D2Client_FindServerSideUnit@@YIPAUD2UnitStrc@@II@Z")
#pragma comment(linker, "/alternatename:?D2Client_FindServerSideUnit@@YIPAUD2UnitStrc@@IH@Z=?D2Client_FindServerSideUnit@@YIPAUD2UnitStrc@@II@Z")
#pragma comment(linker, "/alternatename:?D2Client_ForcePlayerToNeutralMode_6FB09AA0@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_FreeClientUnit@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_FreeLightMap_6FB0E500@@YIXPAX@Z=?LIGHTMAP_DestroyLightNode@@YIXPAUD2LightNodeStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_FreeUnitOverlays_6FB40ED0@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_GetLocalPlayer@@YIPAUD2UnitStrc@@XZ=?D2Client_GetLocalPlayer@@YGPAUD2UnitStrc@@XZ")
#pragma comment(linker, "/alternatename:?D2Client_GetMouseX_6FB55BC0@@YAHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_GetMouseY_6FB55BD0@@YAHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_GetPlayerSkillActionMode_6FB0B5B0@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_GetPlayerUnit@@YIPAUD2UnitStrc@@XZ=?D2Client_GetLocalPlayerUnit@@YGPAUD2UnitStrc@@XZ")
#pragma comment(linker, "/alternatename:?D2Client_GetUniqueClientUnitId@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_GetUnitX_6FAB1590@@YIHPAUD2UnitStrc@@@Z=?D2Client_GetUnitX@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_GetUnitY_6FAB15C0@@YIHPAUD2UnitStrc@@@Z=?D2Client_GetUnitY@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_InteractWithNPC_6FAE30C0@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_IsInTownOrTownSkillRestricted_6FB0B900@@YIHPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_IsPlayerMouseButtonHeld_6FB08BF0@@YAHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_IsSoundLooping_6FAE20B0@@YIHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_IsSoundPaused_6FB70190@@YGEXZ=?QUESTLOG_IsStaffTombSmokeUnitValid@@YGEXZ")
#pragma comment(linker, "/alternatename:?D2Client_IsUnitSoundChannelActive_6FAE11E0@@YIHPAUD2UnitStrc@@H@Z=?D2Client_FindActiveSpeechChannelForUnit@@YIPAUD2ClientSoundChannelStrc@@PAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_NotifySoundSystemPlayerTeleported_6FAE1240@@YAXXZ=?D2Client_FindSoundChannelHandleBySoundId@@YIIH@Z")
#pragma comment(linker, "/alternatename:?D2Client_OnItemAnimSequenceFinished_6FB450B0@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_OnMissileAnimSequenceFinished_6FAD2F50@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_OnMonsterAnimSequenceFinished_6FAE5D10@@YIXPAUD2UnitStrc@@@Z=?D2Client_SpawnMonsterAttackFrameMissile@@YIXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:?D2Client_OnObjectAnimEnd@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_OnPlayerAnimSequenceFinished_6FB0CF70@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_OpenCorpseInventory_6FB08730@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_OpenSpecialQuestDialog_6FB1EBF0@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_OperateObjectClientSide@@YIXPAUD2UnitStrc@@0@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_PlayDelayedSound@@YIXHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_PlayGlobalSound_6FAE1470@@YIXHH@Z=?D2Client_SetSoundChannelVolume@@YIXHI@Z")
#pragma comment(linker, "/alternatename:?D2Client_PlayObjectOperateSound_6FAE8170@@YGXPAUD2UnitStrc@@@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_PlayPositionalSoundAtCoords_6FAE12B0@@YIXHHHHHH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?D2Client_PlaySoundAtCoords@@YIXHHH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_PlaySoundCentered@@YIXH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_PlayUnitPositionalSound@@YIHPAUD2UnitStrc@@HIHH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?D2Client_PlayUnitSound@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_PlayUnitSound_6FAE1FA0@@YGXHPAUD2UnitStrc@@HHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?D2Client_PreloadSound_6FAE1220@@YIXH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_PreloadSound_6FB41F40@@YGXHH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_ProcessUnitBlazeMissileTrail_6FB515F0@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_ProcessUnitInfernoState_6FB516D0@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RebuildPlayerCompositedUnits@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RebuildUnitCompositeAppearance@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RefreshPlayerSpeedAndWalkRun@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RemoveUnitFromHashTable_6FAD0620@@YIXPAPAUD2UnitStrc@@HHH@Z=?D2Client_RemoveUnitFromHashTable@@YIXPAPAUD2UnitStrc@@III@Z")
#pragma comment(linker, "/alternatename:?D2Client_RemoveUnitFromRoomAndFree@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RemoveUnitFromServerHashTable@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RemoveUnitOverlay_6FB41600@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RemoveUnitSoundNode_6FAE19F0@@YIXPAUD2UnitStrc@@PAUD2SoundNodeStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_RollRandomRange_6FAB2E60@@YIHPAUD2SeedStrc@@HH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_ScreenToWorldCoords_6FB23AD0@@YIXHHPAH0@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_SelectHotkeyedSkill@@YIXH@Z=?D2Client_BindSkillHotkeyFromSpeedBarOrActive@@YAHH@Z")
#pragma comment(linker, "/alternatename:?D2Client_SendClientPacket0@@YIXE@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SendLocationSkillPacket_6FB6EC60@@YIXPAUD2UnitStrc@@HH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_SendPacket01_WalkToLocation_6FB6ED00@@YIXHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SendPacket02_WalkToEntity_6FB6E730@@YIXHI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SendPacket03_RunToLocation_6FB6ED30@@YIXHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SendPacket04_RunToEntity_6FB6E770@@YIXHI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SendPacket13_UnitInteract_6FB6E7B0@@YIXHI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SendTargetedSkillPacket_6FB6E5F0@@YIXPAUD2UnitStrc@@HI@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_SendWeaponSwitchPacket@@YIXH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SetDelayedQuestSpeechTicks_6FB08DC0@@YIXI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SetLeftSkillForLocalPlayer@@YIXHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SetLightMapRadius_6FB0EAB0@@YIXPAXH@Z=?LIGHTMAP_SetLightNodeTargetRadius@@YIXHPAUD2LightNodeStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_SetRightSkillForLocalPlayer@@YIXHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SetUnitOverlay_6FB430E0@@YGXHPAUD2UnitStrc@@HHHHHH@Z=_bridge_ret_32@32")
#pragma comment(linker, "/alternatename:?D2Client_SetUnitSelectionHighlight@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_SetUnitSkillId_6FB51590@@YIXPAUD2SkillStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_ShakeScreen_6FAC5E60@@YGXHH@Z=?sub_6FAC5E60@@YGHPAUD2UnitStrc@@G@Z")
#pragma comment(linker, "/alternatename:?D2Client_ShouldPlayerRun_6FB08840@@YAHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_StartChanneledSkillAnim@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@HH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_StartInfernoArcticSkill@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@HH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_StepChanneledSkillAnim@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@HH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_StepInfernoArcticSkill@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@HH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_ToggleUnitFlag_6FAB10C0@@YIXPAUD2UnitStrc@@IH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_TryCastFriendlyTargetedSkill_6FB0BE20@@YIHPAUD2UnitStrc@@HI@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?D2Client_TryPickUpGroundItem_6FB0A2D0@@YIXHI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_TryTakeWarpTile_6FB09140@@YIXHI@Z=?D2Client_CloseSpeedBarIfOpen@@YGXXZ")
#pragma comment(linker, "/alternatename:?D2Client_UpdateEnvironmentSoundCoords_6FB71700@@YGXHH@Z=?QUESTLOG_SpawnStaffTombSmokeObjects@@YGXHH@Z")
#pragma comment(linker, "/alternatename:?D2Client_UpdateItemUnitPerTick_6FB468F0@@YIXPAUD2UnitStrc@@@Z=?D2Client_UpdateItemUnitPerTick@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_UpdateMissileUnitPerTick_6FAD3520@@YIXPAUD2UnitStrc@@@Z=?D2Client_DispatchMissileCltDoFunc@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_UpdateMonsterUnitPerTick_6FAE6100@@YIXPAUD2UnitStrc@@@Z=?D2Client_UpdateMonsterUnitPerTick@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_UpdateObjectPerTick@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?D2Client_UpdatePlayerUnitPerTick_6FB0D140@@YIXPAUD2UnitStrc@@@Z=?D2Client_UpdatePlayerUnitPerTick@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_UpdateUnitAnimGraphics_6FB42000@@YGXPAUD2UnitStrc@@H@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?D2Client_UpdateUnitGfxAndAnim_6FB42000@@YIXPAUD2UnitStrc@@@Z=?GFX_InitUnitCompositeGfx@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?D2Client_UseBeltItemSlot@@YIXHH@Z=?PANEL_UseBeltItemInSlot@@YIHH@Z")
#pragma comment(linker, "/alternatename:?DATATBLS_GetMissilesTxtRecord@@YIPAUD2MissilesTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?DATATBLS_GetOverlayTxtRecord@@YIPAUD2OverlayTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?DATATBLS_GetSkillsTxtRecord@@YIPAUD2SkillsTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?DATATBLS_GetStatesTxtRecord@@YAPAUD2StatesTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?DEBUG_GetUnitFromTypeAndGUID@@YGPAUD2UnitStrc@@II@Z=?DEBUG_GetUnitFromTypeAndGUID@@YGPAUD2UnitStrc@@HH@Z")
#pragma comment(linker, "/alternatename:?DIALOG_Create@@YGPAXHHPAXHHHHHHHH@Z=?sub_6FB13600@@YGPAUD2DialogStrc@@HHP6AHXZ0HHHHHHH@Z")
#pragma comment(linker, "/alternatename:?DIALOG_Destroy@@YIXPAX@Z=?sub_6FB13290@@YAXXZ")
#pragma comment(linker, "/alternatename:?DIALOG_Draw@@YIXPAX@Z=?sub_6FB12F40@@YIXPAUD2DialogStrc@@@Z")
#pragma comment(linker, "/alternatename:?DIALOG_SetLineText@@YIXPAXHPBUUnicode@@HHHHH@Z=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:?DLIGHTMAP_RemoveUnitLight@@YIXPAUD2GfxLightStrc@@@Z=?LIGHTMAP_DestroyLightNode@@YIXPAUD2LightNodeStrc@@@Z")
#pragma comment(linker, "/alternatename:?DUNGEON_FindRoomBySubtile@@YIPAUD2ActiveRoomStrc@@PAU1@HH@Z=?ENGINE_FindRoomBySubtileCoords@@YIPAUD2RoomStrc@@PAU1@HH@Z")
#pragma comment(linker, "/alternatename:?ENGINE_GetMonStats2TxtRecord@@YIPAUD2MonStats2Txt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?ENGINE_GetMonStatsRecordCount@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?ENGINE_GetMonStatsTxtRecord@@YIPAUD2MonStatsTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_ClearAllOpenPanels@@YIXXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_HasFourButtonMouse@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_HasMouseWheelA@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_HasMouseWheelB@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_HasThreeButtonMouse@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_LoadAct@@YIXEIGI@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?GAME_ShouldHideCursor@@YIHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_TriggerDiabloSealScreenShake@@YGXXZ=_g_d2client_addr_6FBCB6CC")
#pragma comment(linker, "/alternatename:?GAME_TriggerhoradricAltarEffect@@YGXXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GAME_UpdateCursorFromMsg@@YIXPAUD2StormMsgStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?GFXUTIL_BuildItemCelContext@@YGHPAUD2UnitStrc@@PAUD2GfxDataStrc@@PAH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?GFXUTIL_GetItemSpriteSize@@YGXPAUD2UnitStrc@@HHPAH1@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?GFX_DrawItemSprite@@YGXHH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?GFX_PlayUnitOverlayOnPlayer@@YGXH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?HIREABLES_OnTogglePortraitsDown@@YIXXZ=?HIREABLES_CyclePortraitDisplayMode@@YGXXZ")
#pragma comment(linker, "/alternatename:?INPUT_RegisterCallbacks@@YIXHPAUHWND__@@PAUD2InputCallbackStrc@@@Z=?WALL2_RegisterMsgHandlerTable@@YIXIPAUHWND__@@PBUD2SMsgRegEntryStrc@@@Z")
#pragma comment(linker, "/alternatename:?INPUT_UnregisterCallbacks@@YIXHPAUHWND__@@PAUD2InputCallbackStrc@@@Z=?WALL2_UnregisterMsgHandlerTable@@YIXIPAUHWND__@@PBUD2SMsgRegEntryStrc@@@Z")
#pragma comment(linker, "/alternatename:?INVENTORY_RemoveItem@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?INV_DrawItemHoverTooltip@@YIXPAUD2UnitStrc@@HHHH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?INV_GetItemGridScreenRect@@YGXPAUD2UnitStrc@@0PAUtagRECT@@H@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?INV_HandleLButtonDown@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?INV_HandleLButtonUp@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?INV_HitTestCloseButton@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?INV_SendSwapWeaponsPacket@@YIXXZ=?INV_SendWeaponSwapPacket@@YGXXZ")
#pragma comment(linker, "/alternatename:?INV_UpdateItemHoverTooltip@@YIXPAUD2StormMsgStrc@@@Z=?INV_HitTestNpcShopGridRect@@YIHHH@Z")
#pragma comment(linker, "/alternatename:?ITEMS_IsItemLockedInInventory@@YIHPAUD2UnitStrc@@@Z=?D2Client_IsItemIdentifiedOrCursor@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?ITEMS_LockItemInInventory@@YIXPAUD2UnitStrc@@@Z=?D2Client_ApplyState54ToItemUnit@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?ITEM_BuildGroundLabelText@@YIXPAUD2UnitStrc@@PAUUnicode@@H@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?ITEM_FreeClientItem@@YIXPAUD2UnitStrc@@@Z=?D2Client_FreeItemUnitClientSide@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?ITEM_GetCursorCellFile@@YIPAUD2CellFileStrc@@PAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?ITEM_GetPaletteShiftTable@@YGPAEPAUD2UnitStrc@@0PAEH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?ITEM_InitClientItem@@YIXPAUD2UnitStrc@@HHHH@Z=?D2Client_SetupItemUnitAnimModeAndPath@@YIHPAUD2UnitStrc@@HHHH@Z")
#pragma comment(linker, "/alternatename:?ITEM_PlayPickupDropSound@@YGHPAUD2UnitStrc@@HHHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?LIGHTMAP_AttachOverlayLight@@YIHPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?LIGHTMAP_GetSubTilePackedRGB@@YIIPAUD2ActiveRoomStrc@@HH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?LIGHTMAP_GetViewportShiftX@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?LIGHTMAP_GetViewportShiftY@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?LIGHTMAP_SampleSubTileRGB@@YIXPAUD2ActiveRoomStrc@@PAE11HH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?MISSILE_CreateClientMissile@@YGPAUD2UnitStrc@@HPAU1@HHHHHHH@Z=_bridge_ret_36@36")
#pragma comment(linker, "/alternatename:?MISSILE_DrawSpecial@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?MISSILE_FreeClientMissile@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?MISSILE_InitClientMissile@@YIXPAUD2UnitStrc@@PAUD2ActiveRoomStrc@@HH@Z=?D2Client_InitMissileUnitDataAndPath@@YIHPAUD2UnitStrc@@HHI@Z")
#pragma comment(linker, "/alternatename:?MONSTER_CanBeConsumed@@YIHPAUD2UnitStrc@@@Z=?D2Client_IsNormalSwitchableAIMonster@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?MONSTER_FreeClientMonster@@YIXPAUD2UnitStrc@@@Z=?D2Client_FreeMonsterUnitClientSide@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?MONSTER_InitClientMonsterData@@YIXPAUD2UnitStrc@@H@Z=?D2Client_InitMonsterDataFields@@YIXPAUD2UnitStrc@@G@Z")
#pragma comment(linker, "/alternatename:?MONSTER_ReinitVisualSeed@@YIXPAUD2UnitStrc@@@Z=?D2Client_UpdateMonsterLightRadiusAndColor@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?MONSTER_SetInitPacketData@@YGXPAUD2UnitStrc@@PBX@Z=?D2Client_InitHirelingMonsterUnit@@YIXPAUD2UnitStrc@@GHG@Z")
#pragma comment(linker, "/alternatename:?MONSTER_SpawnClientMonster@@YIXPAUD2UnitStrc@@PAUD2ActiveRoomStrc@@HHHH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?MONSTER_SpawnDeathOverlayOrMissile@@YIXPAUD2UnitStrc@@HHHH@Z=?D2Client_PlaySound@@YIHHPAUD2UnitStrc@@HHI@Z")
#pragma comment(linker, "/alternatename:?MONSTER_TeleportToCoords@@YGXPAUD2UnitStrc@@HH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?MSG_SendBytePacket@@YIXE@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?MSG_SendByteWordPacket@@YIXEG@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?MSG_SendClientPacket@@YIXIPBX@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?MSG_SendHostilePacket@@YIXIEH@Z=?MSG_SendPartyAction_5D@@YIHEEI@Z")
#pragma comment(linker, "/alternatename:?MSG_SendLeavePartyPacket@@YGXXZ=?MSG_ResetCounters@@YIXXZ")
#pragma comment(linker, "/alternatename:?MSG_SendPacket@@YGXPBX@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?MSG_SendRawPacket@@YGXPBX@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?MSG_SendSingleBytePacket@@YIXE@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?NET_SendByte@@YIXE@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?NET_SendByteAndDword@@YIXEI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?NET_SendByteAndFourDwords@@YIXEIIII@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?NET_SendByteAndThreeDwords@@YIXEIII@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?NET_SendByteAndTwoDwords@@YIXEII@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?NET_SendByteAndWord@@YIXEG@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?NET_SendC2SPacket@@YGXPBXH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?NET_SendStatButtonPacket@@YIHEGG@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?NET_SendVariablePacket@@YIXEPBXH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?NPCMENU_IsActive@@YIHXZ=?sub_6FAC7C90@@YAXXZ")
#pragma comment(linker, "/alternatename:?NPCMENU_ResetState@@YIXXZ=?sub_6FACB4C0@@YAXXZ")
#pragma comment(linker, "/alternatename:?NPCMENU_SendCancelUpgrade@@YIXXZ=?sub_6FACA430@@YAHXZ")
#pragma comment(linker, "/alternatename:?NPCMENU_SendQuestUpgradeRequest@@YIXI@Z=?sub_6FAC8410@@YAXXZ")
#pragma comment(linker, "/alternatename:?NPCMENU_TriggerCainRescueUpdate@@YIXH@Z=?GAME_SetExitAppTimer@@YAXI@Z")
#pragma comment(linker, "/alternatename:?OBJECT_FreeClientObject@@YIXPAUD2UnitStrc@@@Z=?D2Client_FreeObjectData@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?OBJECT_InitClientObject@@YIXHPAUD2UnitStrc@@PAUD2ActiveRoomStrc@@HHE@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?OBJECT_InitShrineVisual@@YIXPAUD2UnitStrc@@@Z=?D2Client_ApplyShrineInitialEffect@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?OBJECT_PostCreateInit@@YIXPAUD2UnitStrc@@@Z=?D2Client_ApplyInitialObjectOverlays@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?OBJECT_UpdatePeriodOfDayLight@@YIXPAUD2UnitStrc@@@Z=?D2Client_UpdateObjectPeriodOfDay@@YIHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:?PALSHIFT_GetMonsterPalette@@YIPAEHH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PANEL_UseBeltPotionSlot@@YIXPAUD2InventoryStrc@@PAUD2UnitStrc@@HH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?PARTICLE_FreeList@@YIXPAPAX@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PARTICLE_SpawnCorpseExplosionGore@@YGXPAUD2UnitStrc@@HH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?PARTICLE_SpawnOverlayOrEffect@@YGXPAUD2UnitStrc@@HHH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?PARTICLE_SpawnRegurgitatorCorpseEffect@@YGXPAUD2UnitStrc@@0H@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?PARTICLE_SpawnSuicideMinionGore@@YGXPAUD2UnitStrc@@HH@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?PARTICLE_UpdateUnitParticles@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PARTYUI_MarkRosterDirty@@YIXXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PLAYERLIST_AllocList@@YIPAUD2PlayerRelationListStrc@@XZ=?D2Client_AllocPlayerList@@YGPAUD2PlayerListStrc@@XZ")
#pragma comment(linker, "/alternatename:?PLAYERLIST_CheckRelationshipFlag@@YIHIH@Z=?D2Client_TestPlayerListRelationFlag@@YAIIII@Z")
#pragma comment(linker, "/alternatename:?PLAYERLIST_EnsureRelationEntry@@YIXII@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PLAYERLIST_SetPartyStateForTarget@@YIXIIG@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?PLAYERLIST_SetRelationFlag@@YIXIIIH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?PLAYERLIST_TestRelationFlag@@YIHIII@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?PLAYER_ActivateSkillHotkey@@YIXH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PLAYER_AssignSkillHotkeyFromBar@@YGXH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?PLAYER_CycleActiveSkillHotkey@@YGXH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?PLAYER_FreeClientPlayer@@YIXPAUD2UnitStrc@@@Z=?D2Client_FreePlayerUnitClientSide@@YAXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?PLAYER_GetLocalPlayer@@YGPAUD2UnitStrc@@XZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?PLAYER_InitClientPlayer@@YIXPAUD2UnitStrc@@PAUD2ActiveRoomStrc@@HH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?PLAYER_ResetSkillState@@YIXPAUD2UnitStrc@@H@Z=?D2Client_TriggerPlayerUsedSkillEnd@@YAXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?PLAYER_StartMoveToTarget@@YGXPAUD2UnitStrc@@HHHHH@Z=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:?ROSTERPETS_GetOwnerGUIDForPet@@YIII@Z=?ROSTERPETS_GetPetOwnerId@@YIII@Z")
#pragma comment(linker, "/alternatename:?ROSTER_CheckHostilePvPFlag@@YIHII@Z=?ROSTER_CanPlayerLootCorpse@@YIHII@Z")
#pragma comment(linker, "/alternatename:?ROSTER_FindByUnitGuid@@YIPAUD2RosterUnitStrc@@H@Z=?ROSTER_FindPlayerByUnitId@@YIPAUD2RosterUnitStrc@@I@Z")
#pragma comment(linker, "/alternatename:?ROSTER_FindEntryByGUID@@YIPAXI@Z=?ROSTER_FindPlayerByUnitId@@YIPAUD2RosterUnitStrc@@I@Z")
#pragma comment(linker, "/alternatename:?ROSTER_GetPartyId@@YIGPAUD2RosterUnitStrc@@@Z=?ROSTER_GetActivePartyId@@YIGPBUD2RosterUnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?SELECT_SetSelectedGroundItem@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?SKILLS_GetSkillsTxtRecord@@YIPAUD2SkillsTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?SKILLS_GetUnitTargetCoords@@YIHPAUD2UnitStrc@@PAH1@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?SKILLS_LaunchBaalNovaMissiles@@YGXPAUD2UnitStrc@@HHHHH@Z=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:?SKILLS_LaunchMissileAtCoords@@YGXPAUD2UnitStrc@@HHHHHH@Z=_bridge_ret_28@28")
#pragma comment(linker, "/alternatename:?SKILLS_LaunchMissileFromOffset@@YGXPAUD2UnitStrc@@HHHHHH@Z=_bridge_ret_28@28")
#pragma comment(linker, "/alternatename:?SKILLS_LaunchMissileWithVelocity@@YGXPAUD2UnitStrc@@HHHHHH@Z=_bridge_ret_28@28")
#pragma comment(linker, "/alternatename:?SKILLS_LaunchNovaMissiles@@YGXPAUD2UnitStrc@@HHHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?SKILLS_RotateVector2D@@YGXPAH0H@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?SKILLS_SaveTargetCoordsToSkill@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?SKILLS_SaveTargetUnitToSkill@@YIHPAUD2UnitStrc@@PAUD2SkillStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?SKILLS_SpawnHomingMissileFromSkill@@YGXPAUD2UnitStrc@@PAUD2SkillStrc@@HHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?SMSG_RegisterWindowMsg@@YIXPAUHWND__@@IIPAX@Z=?WALL2_RegisterSingleMsgHandler@@YIXPAUHWND__@@IIP6GXPAUSMsgWindowMessage@@@Z@Z")
#pragma comment(linker, "/alternatename:?SMSG_RunModalLoop@@YIXPAX@Z=?GAME_FrameLoop@@YIHH@Z")
#pragma comment(linker, "/alternatename:?SMSG_UnregisterWindowMsg@@YIXPAUHWND__@@IIPAX@Z=?WALL2_UnregisterSingleMsgHandler@@YIXPAUHWND__@@IIP6GXPAUSMsgWindowMessage@@@Z@Z")
#pragma comment(linker, "/alternatename:?SOUND_PickRandomVariant@@YIHH@Z=?SOUNDHDR_ResolveRandomGroupSoundId@@YIHH@Z")
#pragma comment(linker, "/alternatename:?SOUND_PlaySoundEffect@@YGXHHHH@Z=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:?SOUND_PlaySoundEffect@@YGXIPAUD2UnitStrc@@HHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?SOUND_PlayUiClick@@YIXH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?SOUND_PlayUnitSpeech@@YGXPAUD2UnitStrc@@I@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?TEXT_CloseQuestSpeechBox@@YGXXZ=?sub_6FB22E40@@YGHXZ")
#pragma comment(linker, "/alternatename:?TEXT_DisplaySystemMessage@@YGXPBUUnicode@@E@Z=?sub_6FB21740@@YGXPBGE@Z")
#pragma comment(linker, "/alternatename:?TEXT_PlayQuestSpeechBox@@YGXGH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?UI_CancelActiveCursorState@@YGXXZ=?UI_SendCloseInteractPacket27@@YIXXZ")
#pragma comment(linker, "/alternatename:?UI_ClearGoldDialogFocus@@YGXXZ=?UI_StartPanelCloseClickCooldown@@YIXXZ")
#pragma comment(linker, "/alternatename:?UI_ClearTooltipRegions@@YGXXZ=?UI_StartPanelCloseClickCooldown@@YIXXZ")
#pragma comment(linker, "/alternatename:?UI_CloseAllOpenPanels@@YGHH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UI_CloseConflictingPanels@@YGXHH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?UI_CloseEscOptionsMenu@@YGXH@Z=?sub_6FB05560@@YGHH@Z")
#pragma comment(linker, "/alternatename:?UI_CloseGoldDialog@@YGXXZ=?UI_CloseGoldDialog@@YIXXZ")
#pragma comment(linker, "/alternatename:?UI_HandleBeltMouseClick@@YGHPAUD2WinMsgStrc@@@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UI_IsAnyModalOrShopOpen@@YIHXZ=?UI_DrawDebugPlayersList@@YIXXZ")
#pragma comment(linker, "/alternatename:?UI_LoadPanelCelAsset@@YIPAUD2CellFileStrc@@PBD@Z=?UI_LoadGlobalUICellFile@@YIPAUD2CellFileStrc@@PBD@Z")
#pragma comment(linker, "/alternatename:?UI_LoadPanelCellFile@@YIPAUD2CellFileStrc@@PBD@Z=?UI_LoadGlobalUICellFile@@YIPAUD2CellFileStrc@@PBD@Z")
#pragma comment(linker, "/alternatename:?UI_OpenEscOptionsMenu@@YIXHH@Z=?sub_6FB05610@@YIHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:?UI_OpenGoldDialog@@YGXH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UI_ResetRightPanelLayout@@YGXXZ=?UI_StartPanelCloseClickCooldown@@YIXXZ")
#pragma comment(linker, "/alternatename:?UI_SetPanelVisible@@YIXHHH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UI_TogglePanelWithRule@@YIXHHH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UNITMODE_ClearActionFrame@@YIXPAUD2UnitStrc@@@Z=?D2Client_ValidatePathTargetUnit@@YIHPAUD2DynamicPathStrc@@@Z")
#pragma comment(linker, "/alternatename:?UNITMODE_DispatchClientEvent@@YIHPAUD2UnitStrc@@HHH0HH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?UNITMODE_OnModeChangeComplete@@YIXHPAUD2UnitStrc@@@Z=?D2Client_ApplyElementalHitOverlay@@YIXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?UNITMODE_QueuePendingPlayerAction@@YIXPAUD2UnitStrc@@HHH0@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?UNITMODE_SetPlayerMoveMode@@YIXH@Z=?D2Client_ChangeUnitModeWithHitOverlayAndSound@@YIXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:?UNITMODE_UpdateUnitFrame@@YIXPAUD2UnitStrc@@@Z=?D2Client_UpdateSingleUnitPerTick@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?UNITSND_DeinitUnitAudio@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNITSND_InitMonsterAudio@@YIXPAUD2UnitStrc@@@Z=?D2Client_PlayMonsterSkillModeSound@@YIXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:?UNITSND_PlayUiOrVoiceSound@@YGXHHHHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?UNITSND_PlayUnitModeSound@@YIXPAUD2UnitStrc@@HH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UNITSND_StopUnitLoopingSound@@YIXHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNITS_GetMonStats2TxtRecordFromMonsterId@@YIPAUD2MonStats2Txt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNITS_GetXPosition@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNITS_GetYPosition@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_AssignClientMonster@@YIPAUD2UnitStrc@@IGHHEPBEH0PBDH@Z=_bridge_ret_32@32")
#pragma comment(linker, "/alternatename:?UNIT_AssignClientObject@@YIXPAE@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_AssignClientPlayer@@YIXPAE@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_AssignClientWarp@@YIXPAUD2UnitStrc@@GHH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?UNIT_FindClientUnit@@YIPAUD2UnitStrc@@HH@Z=?sub_6FACE550@@YIPAUD2UnitStrc@@HH@Z")
#pragma comment(linker, "/alternatename:?UNIT_FindUnitById@@YIPAUD2UnitStrc@@HH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_FreeUnitGfxData@@YIXPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_GetClientPlayer@@YIPAUD2UnitStrc@@XZ=_g_d2client_addr_6FBCC1D4")
#pragma comment(linker, "/alternatename:?UNIT_GetMonsterPaletteIndex@@YIIPAUD2UnitStrc@@@Z=?D2Client_GetUnitGfxField38@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?UNIT_GetSubtileX@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_GetSubtileY@@YIHPAUD2UnitStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_IsCorpseOrFloorUnit@@YIHPAUD2UnitStrc@@@Z=?D2Client_ShouldDrawUnitBeforeEntities@@YIHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:?UNIT_LoadMonsterPaletteAssets@@YIXHHPAE@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UNIT_PlaySound@@YIXPAUD2UnitStrc@@G@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_RemoveClientUnit@@YIXEI@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?UNIT_SetClientUnitPosition@@YIXPAUD2UnitStrc@@HH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UNIT_TriggerHitHighlight@@YGXPAUD2UnitStrc@@@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?UNIT_UpdateMercenaryHostileOverlay@@YIXPAUD2UnitStrc@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?VIEWPORT_GetOffsetX@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?VIEWPORT_GetOffsetY@@YGHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?VIEW_ScreenToIsometric@@YIXPAXPAH1@Z=?VIEW_ScreenToIsometric@@YIXPAUD2ViewportStrc@@PAH1@Z")
#pragma comment(linker, "/alternatename:?VIEW_ScreenToWorldCoords@@YIXPAXHHPAH1@Z=?VIEW_ScreenToWorldCoords@@YIXPAUD2ViewportStrc@@HHPAH1@Z")
#pragma comment(linker, "/alternatename:?VIEW_SetViewportRect@@YIXHHHHH@Z=?VIEW_SetViewportRect@@YIXPAUD2ViewportStrc@@HHHH@Z")
#pragma comment(linker, "/alternatename:?WALL2_BuildViewportRoomGrid@@YIXPAUD2ViewportStrc@@@Z=?WALL2_BuildViewportRoomGrid@@YIXPAUD2WallViewportStrc@@H@Z")
#pragma comment(linker, "/alternatename:?sub_6FAE1FA0@@YGXHHHHH@Z=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:?sub_6FAEBDB0@@YIHHPAXH@Z=?UI_HitTestNewSkillBtn640@@YIHHHH@Z")
#pragma comment(linker, "/alternatename:?sub_6FAEBE00@@YIHHPAX@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FAFCC20@@YIHPAUD2UnitStrc@@H0@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FAFCD70@@YIHPAUD2UnitStrc@@H0@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FAFCE90@@YIHPAUD2UnitStrc@@H0@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FAFE930@@YAHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FAFF090@@YAEXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FAFF3D0@@YAPAUD2UnitStrc@@XZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FAFF7E0@@YIPAUD2SkillsTxt@@H@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FB02DD0@@YIHH@Z=?sub_6FB02DD0@@YIHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:?sub_6FB1D0A0@@YIXHPAUHWND__@@PAX@Z=?WALL2_UnregisterMsgHandlerTable@@YIXIPAUHWND__@@PBUD2SMsgRegEntryStrc@@@Z")
#pragma comment(linker, "/alternatename:?sub_6FB1D100@@YIXHPAUHWND__@@PAX@Z=?WALL2_RegisterMsgHandlerTable@@YIXIPAUHWND__@@PBUD2SMsgRegEntryStrc@@@Z")
#pragma comment(linker, "/alternatename:?sub_6FB200F0@@YIPB_WH@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FB201A0@@YIXPA_WPB_WH1@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?sub_6FB201D0@@YIXPA_WPB_WH1@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?sub_6FB20200@@YIXPA_WPB_W11@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?sub_6FB20280@@YIXPB_WPA_WH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FB202B0@@YIXPB_WPA_WH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FB202E0@@YIXPB_WPA_W0@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FB20310@@YIXPA_WHPB_W@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FB20340@@YIXPA_WHPB_W@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FB26790@@YIXEHH@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:?sub_6FB267B0@@YIXEH@Z=?RECORD_IsPlaybackActive@@YIHXZ")
#pragma comment(linker, "/alternatename:?sub_6FB267C0@@YIXEHHH@Z=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:?sub_6FB267D0@@YIXEHHHI@Z=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:?sub_6FB2A520@@YAHXZ=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FB3AA10@@YAXXZ=?UI_StartPanelCloseClickCooldown@@YIXXZ")
#pragma comment(linker, "/alternatename:?sub_6FB3AB50@@YIXPAUtagRECT@@@Z=?UI_DrawBoxPiecesFrame@@YIXPAUtagRECT@@@Z")
#pragma comment(linker, "/alternatename:?sub_6FB4FA40@@YIGHH@Z=?CMDTBL_GetBoundKey@@YIGHH@Z")
#pragma comment(linker, "/alternatename:?sub_6FB50740@@YIHHH@Z=?CMDTBL_HasKeyBound@@YIHHH@Z")
#pragma comment(linker, "/alternatename:?sub_6FB57830@@YIPAUD2UnitStrc@@PAUD2InventoryStrc@@@Z=_bridge_ret_0")
#pragma comment(linker, "/alternatename:?sub_6FB6EF70@@YIPAUD2CellFileStrc@@PBDH@Z=?ARCHIVE_LoadCellFileDefault@@YIPAUD2CellFileStrc@@PBDH@Z")
#pragma comment(linker, "/alternatename:?sub_6FD91E80@@YIHPAUD2UnitStrc@@00@Z=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@D2GFX_SetViewMatrixAdjust@4=?D2GFX_SetViewMatrixAdjust@@YIXH@Z")
#pragma comment(linker, "/alternatename:@D2GFX_SetViewportShift@4=?D2GFX_SetViewMatrixAdjust@@YIXH@Z")
#pragma comment(linker, "/alternatename:@D2LANG_GetStringByReferenceIndex@4=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@D2LANG_GetStringFromTblIndex@4=?D2LANG_GetStringFromTblIndex@@YIPBUUnicode@@F@Z")
#pragma comment(linker, "/alternatename:@D2LANG_UnicodeStrCat@8=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@D2LANG_UnicodeStrCpy@8=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@D2LANG_UnicodeStrLen@4=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@D2LANG_UnicodeStrNCpy@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@D2LANG_UnicodeToAnsi@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@D2LANG_UnicodeToUpper@4=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@D2WIN_DestroyEditBox@4=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@D2WIN_DrawFramedText@20=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:@D2WIN_DrawText@20=?D2Win_10117_DrawText@@YIXPBUUnicode@@HHHH@Z")
#pragma comment(linker, "/alternatename:@D2WIN_GetEditBoxText@4=?ARCHIVE_FreeArchives@@YIXXZ")
#pragma comment(linker, "/alternatename:@D2WIN_SetFont@4=?D2WIN_SetFont@@YA?AW4Font@@H@Z")
#pragma comment(linker, "/alternatename:@D2Win_10005@16=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:@D2Win_10014@0=?D2Win_GetArchive@@YGPAUHD2ARCHIVE__@@XZ")
#pragma comment(linker, "/alternatename:@D2Win_10034_MixRGB@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@D2Win_10117_DrawText@20=?D2Win_10117_DrawText@@YIXPBUUnicode@@HHHH@Z")
#pragma comment(linker, "/alternatename:@D2Win_10121_GetTextWidth@4=?D2Win_10121_GetTextWidth@@YIHPBUUnicode@@@Z")
#pragma comment(linker, "/alternatename:@D2Win_10124_DrawHoverText@24=?SCROLLBAR_Create@@YIPAUD2WinScrollBarStrc@@HHHHPAUD2CellFileStrc@@P6GHPAUSMSGHANDLER_PARAMS@@@Z@Z")
#pragma comment(linker, "/alternatename:@D2Win_10127_SetFont@4=?D2Win_10127_SetFont@@YI?AW4Font@@W41@@Z")
#pragma comment(linker, "/alternatename:@D2Win_10129_DrawFramedText@20=?D2Win_10129_DrawFramedText@@YIXPBUUnicode@@HHHH@Z")
#pragma comment(linker, "/alternatename:@D2Win_10130_DrawFramedTextEx@20=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:@D2Win_10132@24=?D2Win_10132@@YIXPBUUnicode@@HHKW4DrawMode@@H@Z")
#pragma comment(linker, "/alternatename:@D2Win_10178@4=?D2Win_10178@@YIXPAUD2ShadowLightGammaPaletteStrc@@@Z")
#pragma comment(linker, "/alternatename:@D2Win_10199_WrapTextLines@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@FOG_Alloc@16=?FOG_Alloc@@YIPAXHPBDHH@Z")
#pragma comment(linker, "/alternatename:@FOG_AllocClientMemory@16=?FOG_AllocClientMemory@@YIPAXHPBDHH@Z")
#pragma comment(linker, "/alternatename:@FOG_AllocPool@20=?FOG_AllocPool@@YIPAXPAXHPBDHH@Z")
#pragma comment(linker, "/alternatename:@FOG_CloseFile@4=?FOG_CloseFile@@YIHPAX@Z")
#pragma comment(linker, "/alternatename:@FOG_CreateFileA@28=?FOG_CreateFileA@@YIPAXPBDKKPAU_SECURITY_ATTRIBUTES@@KKPAX@Z")
#pragma comment(linker, "/alternatename:@FOG_CreatePathHierarchy@4=?FOG_CreatePathHierarchy@@YIHPAD@Z")
#pragma comment(linker, "/alternatename:@FOG_FCloseFile@4=?FOG_FCloseFile@@YIXPAUHSFILE__@@@Z")
#pragma comment(linker, "/alternatename:@FOG_FOpenFile@8=?FOG_FOpenFile@@YIHPBDPAPAUHSFILE__@@@Z")
#pragma comment(linker, "/alternatename:@FOG_Free@16=?FOG_Free@@YIXPAXPBDHH@Z")
#pragma comment(linker, "/alternatename:@FOG_FreeClientMemory@16=?FOG_FreeClientMemory@@YIXPAXPBDHH@Z")
#pragma comment(linker, "/alternatename:@FOG_FreePool@20=?FOG_FreePool@@YIXPAX0PBDHH@Z")
#pragma comment(linker, "/alternatename:@FOG_GetInstallPath@8=?FOG_GetInstallPath@@YIHPADI@Z")
#pragma comment(linker, "/alternatename:@FOG_GetSavePath@8=?FOG_GetSavePath@@YIIPADI@Z")
#pragma comment(linker, "/alternatename:@FOG_IsExpansion@0=?FOG_IsExpansion@@YIHXZ")
#pragma comment(linker, "/alternatename:@FOG_ReadFile@20=?FOG_ReadFile@@YIHPAX0KPAKPAU_OVERLAPPED@@@Z")
#pragma comment(linker, "/alternatename:@FOG_SetMemoryPoolSystemMode@4=?FOG_SetMemoryPoolSystemMode@@YAXPAXH@Z")
#pragma comment(linker, "/alternatename:@FOG_WriteFile@20=?FOG_WriteFile@@YIHPAXPBXKPAKPAU_OVERLAPPED@@@Z")
#pragma comment(linker, "/alternatename:@Fog_10042_AllocClientMemory@16=?FOG_Alloc@@YIPAXHPBDHH@Z")
#pragma comment(linker, "/alternatename:@Fog_10043_FreeClientMemory@16=?FOG_Free@@YIXPAXPBDHH@Z")
#pragma comment(linker, "/alternatename:@Fog_10046_FreePool@16=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:@Fog_10101_OpenFile@8=?FOG_MPQSetConfig@@YIHHH@Z")
#pragma comment(linker, "/alternatename:@Fog_10103_CloseFile@4=?FOG_FCloseFile@@YIXPAUHSFILE__@@@Z")
#pragma comment(linker, "/alternatename:@Fog_10104_ReadFile@28=?FOG_FReadFile@@YIHPAUHSFILE__@@PAXIPAHIII@Z")
#pragma comment(linker, "/alternatename:@Fog_10105_GetFileSize@8=?FOG_FGetFileSize@@YIIPAUHSFILE__@@PAI@Z")
#pragma comment(linker, "/alternatename:@Fog_10106_AsyncReadFile@28=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:@Fog_10107_GetAsyncSearchResult@4=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@Fog_10109_CancelAsyncOp@8=?FOG_CloseFile@@YIHPAX@Z")
#pragma comment(linker, "/alternatename:@Fog_10110_WaitAsyncOp@4=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@SERVER_GetServerPacketSize@12=?SERVER_GetServerPacketSize@@YIHPAUD2PacketBufferStrc@@IPAH@Z")
#pragma comment(linker, "/alternatename:@STRTABLE_GetLanguage@0=?STRTABLE_GetLanguage@@YA?AW4ELANGUAGE@@XZ")
#pragma comment(linker, "/alternatename:@STRTABLE_GetLanguageInfo@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@STRTABLE_GetLanguageInfo@8=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@Unicode_strcat@8=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@Unicode_strcpy@8=_bridge_ret_0")
#pragma comment(linker, "/alternatename:@Unicode_strncpy@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@Unicode_toUnicode@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@Unicode_win2utf8@12=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:@UnimplementedOrdinal_10144@4=?UnimplementedOrdinal_10144@@YAXXZ")
#pragma comment(linker, "/alternatename:@UnimplementedOrdinal_10191@4=?UnimplementedOrdinal_10191@@YAXXZ")
#pragma comment(linker, "/alternatename:_CLIENT_Send@12=?CLIENT_Send@@YGHHPBEH@Z")
#pragma comment(linker, "/alternatename:_COLLISION_CheckMask@16=?COLLISION_CheckMask@@YGGPAUD2ActiveRoomStrc@@HHG@Z")
#pragma comment(linker, "/alternatename:_COLLISION_CheckMaskWithSize@20=?COLLISION_CheckMaskWithSize@@YGGPAUD2ActiveRoomStrc@@HHHG@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelFileFreeHardware@4=?D2CMP_CelFileFreeHardware@@YGHPAUD2CellFileStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelGetHandle@4=?D2CMP_CelGetHandle@@YGPAUD2CellFileStrc@@PAUD2GfxDataStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelGetHeight@4=?D2CMP_CelGetHeight@@YGHPAUD2CellFileStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelGetOffsetX@4=?D2CMP_CelGetOffsetX@@YGHPAUD2CellFileStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelGetOffsetY@4=?D2CMP_CelGetOffsetY@@YGHPAUD2CellFileStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelGetWidth@4=?D2CMP_CelGetWidth@@YGHPAUD2CellFileStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2CMP_CelHitTestPixel@16=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:_D2CMP_DeleteCellFile@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2CMP_GetCelBoundingBox@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_D2CMP_GetPaletteRemapTable@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_D2CMP_SpriteCacheGetAsyncLoadingCount@0=?D2CMP_SpriteCacheGetAsyncLoadingCount@@YGHXZ")
#pragma comment(linker, "/alternatename:_D2CMP_SpriteFreeAsyncLoads@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2CMP_SpriteValidate@12=?D2CMP_SpriteValidate@@YGHPAUD2GfxDataStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_10170_PathSetTargetPos@12=?D2COMMON_10170_PathSetTargetPos@@YGXPAUD2DynamicPathStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_11017_CheckUnitIfConsumeable@8=?D2COMMON_11017_CheckUnitIfConsumeable@@YGHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_11018@4=?D2COMMON_11018@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_11018_CheckUnitInteractable@4=?D2COMMON_11018@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_11019@4=?D2COMMON_11019@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_11021@4=?D2COMMON_11021@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_CheckItemFlag@16=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:_D2COMMON_CheckItemType@8=?MISSILE_SetSkill@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_GetBeltItem@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_D2COMMON_GetBeltTypeBoxes@4=?UNITROOM_UpdatePath@@YGXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_GetFirstInventoryItem@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetGemsTxtRecord@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetItemBodyLoc@4=?UNITS_ResetLightMap@@YGPAUD2GfxLightStrc@@PAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_GetItemFileIndex@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetItemPage@4=?STATES_IsUnitShapeShifted@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_GetItemsTxtRecord@4=?DATATBLS_GetSeqFramePointsCount@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2COMMON_GetLeftSkill@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetNextInventoryItem@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetRightSkill@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetSkillId@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2COMMON_GetSkillsTxtRecord@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2COMMON_GetStatFromStatList@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2COMMON_GetUnitStatUInt@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2COMMON_VerifyUnitIsItem@4=?ITEMS_GetItemFlags@@YGIPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2Common_10085_GetRoomTileGrid@4=?STATES_CheckStateMaskHideOnUnit@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2Common_10235_PATH_UpdateRiderPath@8=?D2Common_10235_PATH_UpdateRiderPath@@YGXPAUD2UnitStrc@@0@Z")
#pragma comment(linker, "/alternatename:_D2Common_10269_COMPOSIT@20=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:_D2Common_10519_GetUnitStatSigned@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2Common_10550_GetUnitDefense@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2Common_10552_GetUnitToHitFactor@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2Common_10554_GetUnitBlockingPercent@8=?ITEMS_CheckItemCMDFlag@@YGHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_D2Common_10576_CompileTxtFile@20=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:_D2Common_10611@12=?STATLIST_GetStatListFromUnitStateAndFlag@@YGPAUD2StatListStrc@@PAUD2UnitStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_D2Common_10776_GetNextInventoryItem@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2Common_10777_UnitIsItem@4=?STATLIST_GetMaxStaminaFromUnit@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2Common_10795_GetFirstInventoryItem@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2Common_10797_GetItemBodyLoc@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_D2Common_10801_CheckItemType@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_D2Common_10844_CheckItemRequirements@24=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:_D2Common_10862_GetMissileTrans@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2Common_10885_COMPOSIT@32=_bridge_ret_32@32")
#pragma comment(linker, "/alternatename:_D2Common_10899_COMPOSIT_IsArmorComponent@4=?ITEMS_IsNotQuestItem@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2Common_10920@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2Common_11020@4=?D2Common_11020@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_D2Common_11026_GetAttackRate@32=_bridge_ret_32@32")
#pragma comment(linker, "/alternatename:_D2Common_11037_GetStateColor@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_D2Common_11066@16=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:_D2Common_11076@8=?MISSILE_SetTotalFrames@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_D2GFX_CheckLowQuality@0=?D2GFX_CheckLowQuality@@YGHXZ")
#pragma comment(linker, "/alternatename:_D2GFX_CheckPerspective@0=?D2GFX_CheckPerspective@@YGHXZ")
#pragma comment(linker, "/alternatename:_D2GFX_CheckPerspectiveMode@0=_bridge_ret_0")
#pragma comment(linker, "/alternatename:_D2GFX_DebugDraw@16=?D2GFX_DebugDraw@@YGXPAI0HH@Z")
#pragma comment(linker, "/alternatename:_D2GFX_DrawBox@24=?D2GFX_DrawBox@@YGXHHHHIW4DrawMode@@@Z")
#pragma comment(linker, "/alternatename:_D2GFX_DrawCellContextEx@24=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:_D2GFX_DrawFloorTile@20=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:_D2GFX_DrawLine@24=?D2GFX_DrawLine@@YGXHHHHEE@Z")
#pragma comment(linker, "/alternatename:_D2GFX_DrawRect@24=?D2GFX_DrawBox@@YGXHHHHIW4DrawMode@@@Z")
#pragma comment(linker, "/alternatename:_D2GFX_FindBestPaletteIndex@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_D2GFX_GetResolutionMode@0=?D2GFX_GetResolutionMode@@YG?AW4D2GameResolutionMode@@XZ")
#pragma comment(linker, "/alternatename:_D2GFX_ScalePerspectivePosition@24=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:_D2Gfx_10012_CelDrawHilight@16=?TEXTURE_CelDrawHilight@@YGXPAUD2GfxDataStrc@@HHE@Z")
#pragma comment(linker, "/alternatename:_D2Gfx_10024_CelDraw@24=?TEXTURE_CelDraw@@YGXPAUD2GfxDataStrc@@HHIW4DrawMode@@PAE@Z")
#pragma comment(linker, "/alternatename:_D2Gfx_10030_CelDrawShadow@12=?TEXTURE_CelDrawShadow@@YGXPAUD2GfxDataStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_D2Gfx_10042_TransformPerspectiveCoords@20=?TILE_ShadowTileDraw@@YGHPAUD2TileLibraryEntryStrc@@HHW4DrawMode@@H@Z")
#pragma comment(linker, "/alternatename:_D2Gfx_10043_IsPerspectiveEnabled@0=?D2GFX_GetResolutionMode@@YG?AW4D2GameResolutionMode@@XZ")
#pragma comment(linker, "/alternatename:_D2Gfx_10080_DrawFloorTile@20=_bridge_ret_20@20")
#pragma comment(linker, "/alternatename:_D2LANG_UnicodeSPrintf=_bridge_ret_0")
#pragma comment(linker, "/alternatename:_D2NET_10023_GetSendQueueBacklog@0=_bridge_ret_0")
#pragma comment(linker, "/alternatename:_D2SOUND_GetMasterVolume@0=?D2SOUND_GetMasterVolume@@YIHXZ")
#pragma comment(linker, "/alternatename:_D2SOUND_GetMusicVolume@0=?D2SOUND_GetMusicVolume@@YIHXZ")
#pragma comment(linker, "/alternatename:_D2WIN_SetEditBoxText@8=?DATATBLS_GetMissileVelocityFromMissilesTxt@@YGHHH@Z")
#pragma comment(linker, "/alternatename:_D2Win_10034_MixRGB@12=?D2Win_10034_MixRGB@@YGEEEE@Z")
#pragma comment(linker, "/alternatename:_D2Win_GetArchive@0=?D2Win_GetArchive@@YGPAUHD2ARCHIVE__@@XZ")
#pragma comment(linker, "/alternatename:_D2Win_TakeScreenshot@0=?D2Win_10168_WINMAIN_CreateScreenshot@@YGXXZ")
#pragma comment(linker, "/alternatename:_DATATBLS_GetItemsTxtRecord@4=?DATATBLS_GetItemsTxtRecord@@YGPAUD2ItemsTxt@@H@Z")
#pragma comment(linker, "/alternatename:_DATATBLS_GetLevelsTxtRecord@4=?DATATBLS_GetLevelsTxtRecord@@YGPAUD2LevelsTxt@@H@Z")
#pragma comment(linker, "/alternatename:_DATATBLS_GetMonSeqTxtRecordFromUnit@4=?DATATBLS_GetMonSeqTxtRecordFromUnit@@YGPAUD2AnimSeqTxt@@PAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_DATATBLS_GetMonsterTxtRecord@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_DATATBLS_GetMudFromLevelsTxt@4=?DATATBLS_GetMudFromLevelsTxt@@YGEH@Z")
#pragma comment(linker, "/alternatename:_DATATBLS_GetObjectsTxtRecord@4=?DATATBLS_GetObjectsTxtRecord@@YGPAUD2ObjectsTxt@@H@Z")
#pragma comment(linker, "/alternatename:_DATATBLS_GetOverlaysTxtRecord@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_DATATBLS_GetRainFromLevelsTxt@4=?DATATBLS_GetRainFromLevelsTxt@@YGEH@Z")
#pragma comment(linker, "/alternatename:_DEBUG_GetUnitFromTypeAndGUID@8=?DEBUG_GetUnitFromTypeAndGUID@@YGPAUD2UnitStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_DRLGACTIVATE_Update@4=?DRLGACTIVATE_Update@@YGXPAUD2DrlgStrc@@@Z")
#pragma comment(linker, "/alternatename:_DRLG_GetLOSDrawFromLevelId@4=?DRLG_GetLOSDrawFromLevelId@@YGHH@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_FindRoomBySubtileCoordinates@12=?DUNGEON_FindRoomBySubtileCoordinates@@YGPAUD2ActiveRoomStrc@@PAUD2DrlgActStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GameSubtileToClientCoords@8=?DUNGEON_GameSubtileToClientCoords@@YGXPAH0@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GameToClientCoords@8=?DUNGEON_GameToClientCoords@@YGXPAH0@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GetAdjacentRoomsListFromRoom@12=?DUNGEON_GetAdjacentRoomsListFromRoom@@YGXPAUD2ActiveRoomStrc@@PAPAPAU1@PAH@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GetLevelIdFromRoom@4=?DUNGEON_GetLevelIdFromRoom@@YGHPAUD2ActiveRoomStrc@@@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GetRGB_IntensityFromRoom@20=?DUNGEON_GetRGB_IntensityFromRoom@@YGXPAUD2ActiveRoomStrc@@PAE111@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GetRoomCoordinates@8=?DUNGEON_GetRoomCoordinates@@YGXPAUD2ActiveRoomStrc@@PAUD2DrlgCoordsStrc@@@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_GetRoomFromAct@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_DUNGEON_GetWarpDestinationLevel@8=?DUNGEON_GetWarpDestinationLevel@@YGHPAUD2ActiveRoomStrc@@H@Z")
#pragma comment(linker, "/alternatename:_DUNGEON_SetActCallbackFunc@8=?DUNGEON_SetActCallbackFunc@@YGXPAUD2DrlgActStrc@@P6IXPAUD2ActiveRoomStrc@@@Z@Z")
#pragma comment(linker, "/alternatename:_ENVIRONMENT_GetIntensityFromAct@4=?ENVIRONMENT_GetIntensityFromAct@@YGHPAUD2DrlgActStrc@@@Z")
#pragma comment(linker, "/alternatename:_ENVIRONMENT_GetLightColorFromAct@16=?ENVIRONMENT_GetLightColorFromAct@@YGXPAUD2DrlgActStrc@@PAE11@Z")
#pragma comment(linker, "/alternatename:_FOG_10055_GetSyncTime=?FOG_10055_GetSyncTime@@YIHXZ")
#pragma comment(linker, "/alternatename:_FOG_DisplayAssert=?FOG_DisplayAssert@@YAXPBD0H@Z")
#pragma comment(linker, "/alternatename:_FOG_DisplayHalt=?FOG_DisplayHalt@@YAXPBDH@Z")
#pragma comment(linker, "/alternatename:_FOG_GetErrorLocation=?UnimplementedOrdinal_10265@@YAXXZ")
#pragma comment(linker, "/alternatename:_FOG_GetErrorLocation@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_FOG_GetErrorString=?UnimplementedOrdinal_10265@@YAXXZ")
#pragma comment(linker, "/alternatename:_FOG_GetInstallPath@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_FOG_IsExpansion@0=?FOG_IsExpansion@@YIHXZ")
#pragma comment(linker, "/alternatename:_FOG_Trace=?FOG_Trace@@YAXPBDZZ")
#pragma comment(linker, "/alternatename:_Fog_10023_Error=?FOG_DisplayAssert@@YAXPBD0H@Z")
#pragma comment(linker, "/alternatename:_Fog_10227_IsExpansion@0=?FOG_IsExpansion@@YIHXZ")
#pragma comment(linker, "/alternatename:_Fog_10265=_bridge_ret_0")
#pragma comment(linker, "/alternatename:_GAME_SetInitSeed@4=?GAME_SetInitSeed@@YGXH@Z")
#pragma comment(linker, "/alternatename:_INVENTORY_GetCursorItem@4=?INVENTORY_GetCursorItem@@YGPAUD2UnitStrc@@PAUD2InventoryStrc@@@Z")
#pragma comment(linker, "/alternatename:_INVENTORY_GetFirstItem@4=?INVENTORY_GetFirstItem@@YGPAUD2UnitStrc@@PAUD2InventoryStrc@@@Z")
#pragma comment(linker, "/alternatename:_INVENTORY_GetItemFromInventory@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_INVENTORY_GetNextItem@4=?INVENTORY_GetNextItem@@YGPAUD2UnitStrc@@PAU1@@Z")
#pragma comment(linker, "/alternatename:_INVENTORY_RemoveInventoryItems@4=?INVENTORY_RemoveInventoryItems@@YGXPAUD2InventoryStrc@@@Z")
#pragma comment(linker, "/alternatename:_INVENTORY_UnitIsItem@4=?INVENTORY_UnitIsItem@@YGPAUD2UnitStrc@@PAU1@@Z")
#pragma comment(linker, "/alternatename:_INV_GetFirstItem@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_INV_GetItemFromBodyLoc@8=?INVENTORY_GetItemFromBodyLoc@@YGPAUD2UnitStrc@@PAUD2InventoryStrc@@H@Z")
#pragma comment(linker, "/alternatename:_INV_GetNextItem@4=?INVENTORY_GetNextItem@@YGPAUD2UnitStrc@@PAU1@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_CanBeRare@4=?ITEMS_CanBeRare@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_CheckIfSocketable@4=?ITEMS_CheckIfSocketable@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_CheckIfThrowable@4=?ITEMS_CheckIfThrowable@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_CheckItemFlag@16=?ITEMS_CheckItemFlag@@YGHPAUD2UnitStrc@@IHPBD@Z")
#pragma comment(linker, "/alternatename:_ITEMS_CheckItemTypeId@8=?ITEMS_CheckItemTypeId@@YGHPBUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_ITEMS_CheckNotSocketable@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_ITEMS_GetBodyLocation@4=?ITEMS_GetBodyLocation@@YGEPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_GetColor@16=?ITEMS_GetColor@@YGPAEPAUD2UnitStrc@@0PAEH@Z")
#pragma comment(linker, "/alternatename:_ITEMS_GetFileIndex@4=?ITEMS_GetFileIndex@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_GetItemQuality@4=?ITEMS_GetItemQuality@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_GetTransactionCost@24=?ITEMS_GetTransactionCost@@YGHPAUD2UnitStrc@@0W4D2C_Difficulties@@PAUD2BitBufferStrc@@HW4D2C_TransactionTypes@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_HasDurability@4=?ITEMS_HasDurability@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_ITEMS_IsNotQuestItem@4=?ITEMS_IsNotQuestItem@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_MISSILE_SetCurrentFrame@8=?MISSILE_SetCurrentFrame@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_MISSILE_SetTargetX@8=?MISSILE_SetTargetX@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_MISSILE_SetTargetY@8=?MISSILE_SetTargetY@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_MISSILE_SetTotalFrames@8=?MISSILE_SetTotalFrames@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_MONSTERAI_GetMonsterComponents@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_MONSTERAI_UpdateAiState@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_MONSTERS_GetCompInfo@24=_bridge_ret_24@24")
#pragma comment(linker, "/alternatename:_MONSTERS_GetMinionSpawnInfo@16=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:_MONSTERS_IsDead@4=?MONSTERS_IsDead@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_MONSTER_HasComponents@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_PATH_GetMaxDistance@4=?PATH_GetMaxDistance@@YGEPAUD2DynamicPathStrc@@@Z")
#pragma comment(linker, "/alternatename:_PATH_GetTargetType@4=?ITEMS_GetVarGfxIndex@@YGEPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_PATH_GetTargetUnit@4=?PATH_GetTargetUnit@@YGPAUD2UnitStrc@@PAUD2DynamicPathStrc@@@Z")
#pragma comment(linker, "/alternatename:_PATH_GetType@4=?PATH_GetType@@YGHPAUD2DynamicPathStrc@@@Z")
#pragma comment(linker, "/alternatename:_PATH_GetXPosition@4=?PATH_GetXPosition@@YGHPAUD2DynamicPathStrc@@@Z")
#pragma comment(linker, "/alternatename:_PATH_GetYPosition@4=?PATH_GetYPosition@@YGHPAUD2DynamicPathStrc@@@Z")
#pragma comment(linker, "/alternatename:_PATH_SetFootprintCollisionMask@8=?PATH_SetFootprintCollisionMask@@YGXPAUD2DynamicPathStrc@@H@Z")
#pragma comment(linker, "/alternatename:_PATH_SetTargetPos@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_QUESTRECORD_AllocRecord@4=?QUESTRECORD_AllocRecord@@YGPAUD2BitBufferStrc@@PAX@Z")
#pragma comment(linker, "/alternatename:_QUESTRECORD_CopyBufferToRecord@16=?QUESTRECORD_CopyBufferToRecord@@YGXPAUD2BitBufferStrc@@PAEGH@Z")
#pragma comment(linker, "/alternatename:_QUESTRECORD_FreeRecord@8=?QUESTRECORD_FreeRecord@@YGXPAXPAUD2BitBufferStrc@@@Z")
#pragma comment(linker, "/alternatename:_QUESTRECORD_GetQuestState@12=?QUESTRECORD_GetQuestState@@YGHPAUD2BitBufferStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_SCALE_AdjustPerspectivePosition@20=?SCALE_AdjustPerspectivePosition@@YGXHHHPAH0@Z")
#pragma comment(linker, "/alternatename:_SCALE_CheckPerspectivePosition@8=?SCALE_CheckPerspectivePosition@@YGHHH@Z")
#pragma comment(linker, "/alternatename:_SCALE_GetFactor@0=?SCALE_GetFactor@@YGHXZ")
#pragma comment(linker, "/alternatename:_SCALE_SetDefaultFactor@0=?SCALE_SetDefaultFactor@@YGXXZ")
#pragma comment(linker, "/alternatename:_SCALE_SetDefaultFactorPerspectiveFactor@0=?SCALE_SetDefaultFactorPerspectiveFactor@@YGXXZ")
#pragma comment(linker, "/alternatename:_SCALE_SetPerspectiveScale@8=?SCALE_SetPerspectiveScale@@YGXHH@Z")
#pragma comment(linker, "/alternatename:_SCALE_UpdateFactor@0=?SCALE_UpdateFactor@@YGXXZ")
#pragma comment(linker, "/alternatename:_SCmdProcess@16=?SCmdProcess@@YGHPAPBDHHH@Z")
#pragma comment(linker, "/alternatename:_SCmdRegisterArgList@8=?SCmdRegisterArgList@@YGHHH@Z")
#pragma comment(linker, "/alternatename:_SFileCloseFile@4=?SFileCloseFile@@YGHPAX@Z")
#pragma comment(linker, "/alternatename:_SFileGetFileSize@8=?SFileGetFileSize@@YGHHPAK@Z")
#pragma comment(linker, "/alternatename:_SFileOpenFileEx@16=?SFileOpenFileEx@@YGHHPADDH@Z")
#pragma comment(linker, "/alternatename:_SFileReadFile@20=?SFileReadFile@@YGHHHHHJ@Z")
#pragma comment(linker, "/alternatename:_SKILLS_EvaluateSkillFormula@16=?SKILLS_EvaluateSkillFormula@@YGHPAUD2UnitStrc@@IHH@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetFlags@4=?SKILLS_GetFlags@@YGHPAUD2SkillStrc@@@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetHighestLevelSkillFromUnitAndId@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_SKILLS_GetLevel@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_SKILLS_GetParam1@4=?SKILLS_GetParam1@@YGHPAUD2SkillStrc@@@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetParam2@4=?SKILLS_GetParam2@@YGHPAUD2SkillStrc@@@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetParam3@4=?SKILLS_GetParam3@@YGHPAUD2SkillStrc@@@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetParam4@4=?SKILLS_GetParam4@@YGHPAUD2SkillStrc@@@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetSkillIdFromSkill@12=?SKILLS_GetSkillIdFromSkill@@YGHPAUD2SkillStrc@@PBDH@Z")
#pragma comment(linker, "/alternatename:_SKILLS_GetSkillsTxtRecordFromSkill@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_SKILLS_SetFlags@8=?SKILLS_SetFlags@@YGXPAUD2SkillStrc@@H@Z")
#pragma comment(linker, "/alternatename:_SKILLS_SetParam1@8=?SKILLS_SetParam1@@YGXPAUD2SkillStrc@@H@Z")
#pragma comment(linker, "/alternatename:_SKILLS_SetParam2@8=?SKILLS_SetParam2@@YGXPAUD2SkillStrc@@H@Z")
#pragma comment(linker, "/alternatename:_SKILLS_SetParam3@8=?SKILLS_SetParam3@@YGXPAUD2SkillStrc@@H@Z")
#pragma comment(linker, "/alternatename:_SKILLS_SetParam4@8=?SKILLS_SetParam4@@YGXPAUD2SkillStrc@@H@Z")
#pragma comment(linker, "/alternatename:_SMemAlloc@16=?SMemAlloc@@YGPAXIPBDHI@Z")
#pragma comment(linker, "/alternatename:_SMsgBreakHandlerChain@4=?SMsgBreakHandlerChain@@YGHPAUSMSGHANDLER_PARAMS@@@Z")
#pragma comment(linker, "/alternatename:_SRegLoadString@20=?SRegLoadString@@YGHPBD0IPADI@Z")
#pragma comment(linker, "/alternatename:_SRegLoadValue@16=?SRegLoadValue@@YGHPBD0IPAK@Z")
#pragma comment(linker, "/alternatename:_SRegSaveValue@16=?SRegSaveValue@@YGHPBD0EK@Z")
#pragma comment(linker, "/alternatename:_SStrChr@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_SStrCmp@12=?SStrCmp@@YGHPBD0I@Z")
#pragma comment(linker, "/alternatename:_SStrCmpI@12=?SStrCmpI@@YGHPBD0I@Z")
#pragma comment(linker, "/alternatename:_SStrCopy@12=?SStrCopy@@YGIPADPBDI@Z")
#pragma comment(linker, "/alternatename:_SStrHash@12=?SStrHash@@YGKPBDII@Z")
#pragma comment(linker, "/alternatename:_SStrPrintf=?SStrPrintf@@YAHPADIPBDZZ")
#pragma comment(linker, "/alternatename:_STATES_CheckState@8=?STATES_CheckState@@YGHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_STATES_CheckStateMaskGreenOnUnit@4=?STATES_CheckStateMaskGreenOnUnit@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_STATES_ToggleState@12=?STATES_ToggleState@@YGXPAUD2UnitStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_STATLIST_FreeModStats@4=?STATLIST_FreeModStats@@YGXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_STATLIST_GetMaxDurabilityFromUnit@4=?STATLIST_GetMaxDurabilityFromUnit@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_STATLIST_GetMaxLifeFromUnit@4=?STATLIST_GetMaxLifeFromUnit@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_STATLIST_GetUnitAlignment@4=?STATLIST_GetUnitAlignment@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_STATLIST_SetBaseStat2@20=?STATLIST_SetBaseStat2@@YGXPAUD2StatListStrc@@HHGPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_STATLIST_SetUnitStat@16=?STATLIST_SetUnitStat@@YGXPAUD2UnitStrc@@HHG@Z")
#pragma comment(linker, "/alternatename:_STATLIST_UnitGetItemStatOrSkillStatValue@12=?STATLIST_UnitGetItemStatOrSkillStatValue@@YGHPAUD2UnitStrc@@HG@Z")
#pragma comment(linker, "/alternatename:_STATLIST_UnitGetStatValue@12=?STATLIST_UnitGetStatValue@@YGHPBUD2UnitStrc@@HG@Z")
#pragma comment(linker, "/alternatename:_Storm_495_SMemCmp@12=?SMemCmp@@YGHPAX0I@Z")
#pragma comment(linker, "/alternatename:_Storm_501_SStrCopy@12=?SStrCopy@@YGIPADPBDI@Z")
#pragma comment(linker, "/alternatename:_Storm_508_SStrCmpI@12=?SStrCmp@@YGHPBD0I@Z")
#pragma comment(linker, "/alternatename:_Storm_604_SBigDel@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_Storm_606_SBigFromBinary@12=_bridge_ret_12@12")
#pragma comment(linker, "/alternatename:_Storm_624_SBigNew@4=?SBigNew@@YGHPAPAX@Z")
#pragma comment(linker, "/alternatename:_Storm_628_SBigPowMod@16=?SBigPowMod@@YGHPAXHHH@Z")
#pragma comment(linker, "/alternatename:_Storm_636_SBigToBinaryBuffer@16=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:_TEXTURE_CelDraw@24=?TEXTURE_CelDraw@@YGXPAUD2GfxDataStrc@@HHIW4DrawMode@@PAE@Z")
#pragma comment(linker, "/alternatename:_TEXTURE_CelDrawColor@24=?TEXTURE_CelDrawColor@@YGXPAUD2GfxDataStrc@@HHIW4DrawMode@@H@Z")
#pragma comment(linker, "/alternatename:_TEXTURE_CelFlatSpriteDraw@28=?TEXTURE_CelFlatSpriteDraw@@YGXPAUD2GfxDataStrc@@HHIW4DrawMode@@HPAE@Z")
#pragma comment(linker, "/alternatename:_TEXT_FreeTextHeader@4=?TEXT_FreeTextHeader@@YGXPAUD2TextHeaderStrc@@@Z")
#pragma comment(linker, "/alternatename:_TEXT_GetMenuType2NodeCount@4=?TEXT_GetMenuType2NodeCount@@YGHPAUD2TextHeaderStrc@@@Z")
#pragma comment(linker, "/alternatename:_TEXT_GetStringIdOfMenuType1Or2Node@8=?TEXT_GetStringIdOfMenuType1Or2Node@@YGFPAUD2TextHeaderStrc@@H@Z")
#pragma comment(linker, "/alternatename:_TEXT_GetStringIdOfMenuType2Node@8=?TEXT_GetStringIdOfMenuType2Node@@YGFPAUD2TextHeaderStrc@@H@Z")
#pragma comment(linker, "/alternatename:_TILE_ShadowTileDraw@20=?TILE_ShadowTileDraw@@YGHPAUD2TileLibraryEntryStrc@@HHW4DrawMode@@H@Z")
#pragma comment(linker, "/alternatename:_UNITFINDS_FindUnitInNeighboredRooms@12=?UNITFINDS_FindUnitInNeighboredRooms@@YGPAUD2UnitStrc@@PAUD2ActiveRoomStrc@@HH@Z")
#pragma comment(linker, "/alternatename:_UNITROOM_RemoveUnitFromRoom@4=?UNITROOM_RemoveUnitFromRoom@@YGXPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_ChangeAnimMode@8=?UNITS_ChangeAnimMode@@YGHPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetClientCoordX@4=?UNITS_GetClientCoordX@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetClientCoordY@4=?UNITS_GetClientCoordY@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetClientCoords@8=?UNITS_GetClientCoords@@YGXPAUD2UnitStrc@@PAUD2CoordStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetCoords@8=?UNITS_GetCoords@@YGXPAUD2UnitStrc@@PAUD2CoordStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetCurrentLifePercentage@4=?UNITS_GetCurrentLifePercentage@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetDir@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_UNITS_GetDistanceToOtherUnit@8=?UNITS_GetDistanceToOtherUnit@@YGHPAUD2UnitStrc@@0@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetObjectTxtRecordFromObject@4=?UNITS_GetObjectTxtRecordFromObject@@YGPAUD2ObjectsTxt@@PAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetPrecisionX@4=?UNITS_GetPrecisionX@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetPrecisionY@4=?UNITS_GetPrecisionY@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetRoom@4=?UNITS_GetRoom@@YGPAUD2ActiveRoomStrc@@PAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetShiftedFrameMetrics@12=?UNITS_GetShiftedFrameMetrics@@YGXPAUD2UnitStrc@@PAH1@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetSubtileX@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_UNITS_GetSubtileY@4=?D2Common_11028@@YGHH@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetUsedSkill@4=?UNITS_GetUsedSkill@@YGPAUD2SkillStrc@@PAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_GetWeaponClass@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_UNITS_IsInMovingMode@4=?UNITS_IsInMovingMode@@YGHPAUD2UnitStrc@@@Z")
#pragma comment(linker, "/alternatename:_UNITS_SetAnimActionFrame@8=?UNITS_SetAnimActionFrame@@YGXPAUD2UnitStrc@@H@Z")
#pragma comment(linker, "/alternatename:_UNITS_SetTargetUnitForDynamicUnit@8=?UNITS_SetTargetUnitForDynamicUnit@@YGXPAUD2UnitStrc@@0@Z")
#pragma comment(linker, "/alternatename:_UNITS_TestCollisionWithUnit@12=?UNITS_TestCollisionWithUnit@@YGHPAUD2UnitStrc@@0H@Z")
#pragma comment(linker, "/alternatename:_Unicode_sprintf=_bridge_ret_0")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10060@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10061@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10062@16=_bridge_ret_16@16")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10063@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10064@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10065@8=_bridge_ret_8@8")
#pragma comment(linker, "/alternatename:_UnimplementedOrdinal_10265@4=_bridge_ret_4@4")
#pragma comment(linker, "/alternatename:_WINDOW_GetDimensions@8=?WINDOW_GetDimensions@@YGHPAH0@Z")
#pragma comment(linker, "/alternatename:_WINDOW_GetWindow@0=?WINDOW_GetWindow@@YGPAUHWND__@@XZ")
