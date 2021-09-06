#include "CharBits.h"

CharBits get_char_bits(char c) {
  // Return unprintable chars as questionmark
  if (c <= 31 || c == 127) return char_table[63 - 32];
  return char_table[c - 32]; // Return all other chars from lookup table
}

CharBits char_table[95] = {
// 0 .. 31
/*  32  (0) */ {0x0, 0x0},
/*  33  (1) */ {0x2, 0x10842008},
/*  34  (2) */ {0x5, 0x28000000},
/*  35  (3) */ {0x5, 0x2beafa94},
/*  36  (4) */ {0x2, 0x3e0e0f88},
/*  37  (5) */ {0xc, 0xe84442e6},
/*  38  (6) */ {0x2, 0x288d949a},
/*  39  (7) */ {0x2, 0x10000000},
/*  40  (8) */ {0x1, 0x11084104},
/*  41  (9) */ {0x4, 0x10421110},
/*  42 (10) */ {0x0, 0x1445000},
/*  43 (11) */ {0x0, 0x8e2000},
/*  44 (12) */ {0x0, 0x110},
/*  45 (13) */ {0x0, 0xe0000},
/*  46 (14) */ {0x0, 0x8},
/*  47 (15) */ {0x0, 0x88444220},
/*  48 (16) */ {0x7, 0x4675cc5c},
/*  49 (17) */ {0x2, 0x3284213e},
/*  50 (18) */ {0x7, 0x4426443e},
/*  51 (19) */ {0x7, 0x44260c5c},
/*  52 (20) */ {0x1, 0x9531f842},
/*  53 (21) */ {0xf, 0xc3c10c5c},
/*  54 (22) */ {0x3, 0xa21e8c5c},
/*  55 (23) */ {0xf, 0xc4222108},
/*  56 (24) */ {0x7, 0x462e8c5c},
/*  57 (25) */ {0x7, 0x462f0898},
/*  58 (26) */ {0x0, 0x802000},
/*  59 (27) */ {0x0, 0x802200},
/*  60 (28) */ {0x0, 0x8882080},
/*  61 (29) */ {0x0, 0x1c07000},
/*  62 (30) */ {0x0, 0x20822200},
/*  63 (31) */ {0x7, 0x44222008},
/*  64 (32) */ {0x7, 0x46f5bc1c},
/*  65 (33) */ {0x7, 0x463f8c62},
/*  66 (34) */ {0xf, 0x463e8c7c},
/*  67 (35) */ {0x7, 0x4610845c},
/*  68 (36) */ {0xf, 0x46318c7c},
/*  69 (37) */ {0xf, 0xc21f843e},
/*  70 (38) */ {0xf, 0xc21f8420},
/*  71 (39) */ {0x7, 0xc2168c5c},
/*  72 (40) */ {0x8, 0xc63f8c62},
/*  73 (41) */ {0x2, 0x10842108},
/*  74 (42) */ {0x0, 0x84210c5c},
/*  75 (43) */ {0x8, 0xca98a4a2},
/*  76 (44) */ {0x8, 0x4210843e},
/*  77 (45) */ {0x8, 0xeeb18c62},
/*  78 (46) */ {0x8, 0xe6b38c62},
/*  79 (47) */ {0x7, 0x46318c5c},
/*  80 (48) */ {0xf, 0x463e8420},
/*  81 (49) */ {0x7, 0x46318c9a},
/*  82 (50) */ {0xf, 0x463e8c62},
/*  83 (51) */ {0x7, 0xc1c10c5c},
/*  84 (52) */ {0xf, 0x90842108},
/*  85 (53) */ {0x8, 0xc6318c5c},
/*  86 (54) */ {0x8, 0xc6315288},
/*  87 (55) */ {0x8, 0xc631ad54},
/*  88 (56) */ {0x8, 0xc5445462},
/*  89 (57) */ {0x8, 0xc54a2108},
/*  90 (58) */ {0xf, 0x8444443e},
/*  91 (59) */ {0x7, 0x2108421c},
/*  92 (60) */ {0x8, 0x21041082},
/*  93 (61) */ {0x7, 0x842109c},
/*  94 (62) */ {0x2, 0x28000000},
/*  95 (63) */ {0x0, 0x3e},
/*  96 (64) */ {0x2, 0x8000000},
/*  97 (65) */ {0x0, 0x1c17c5e},
/*  98 (66) */ {0x8, 0x42d98c7c},
/*  99 (67) */ {0x0, 0x1d1845c},
/* 100 (68) */ {0x0, 0x85b38c5e},
/* 101 (69) */ {0x0, 0x1d1fc1e},
/* 102 (70) */ {0x1, 0x91e42108},
/* 103 (71) */ {0x0, 0x3e31787c},
/* 104 (72) */ {0x8, 0x42d98c62},
/* 105 (73) */ {0x2, 0x842108},
/* 106 (74) */ {0x1, 0x421498},
/* 107 (75) */ {0x8, 0x4254c524},
/* 108 (76) */ {0x4, 0x21084288},
/* 109 (78) */ {0x0, 0x355ac62},
/* 110 (79) */ {0x0, 0x3d18c62},
/* 111 (80) */ {0x0, 0x1d18c5c},
/* 112 (81) */ {0x0, 0x3d18fa0},
/* 113 (82) */ {0x0, 0x1b38bc2},
/* 114 (83) */ {0x0, 0x2d98420},
/* 115 (84) */ {0x0, 0x1f0707c},
/* 116 (85) */ {0x2, 0x11c42104},
/* 117 (86) */ {0x0, 0x2318c5e},
/* 118 (87) */ {0x0, 0x2318a88},
/* 119 (88) */ {0x0, 0x231ad5e},
/* 120 (89) */ {0x0, 0x22a22a2},
/* 121 (90) */ {0x0, 0x231d9b8},
/* 122 (91) */ {0x0, 0x3e2223e},
/* 123 (92) */ {0x1, 0x10882104},
/* 124 (93) */ {0x2, 0x10842108},
/* 125 (94) */ {0x4, 0x10822110},
/* 126 (95) */ {0x0, 0x2e8000}
// 127
};