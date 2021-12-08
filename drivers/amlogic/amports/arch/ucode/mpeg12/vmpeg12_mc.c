/*
 * drivers/amlogic/amports/arch/m8_m8m2/mpeg12/vmpeg12_mc.c
 *
 * Copyright (C) 2015 Amlogic, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
*/

#include "../firmware_def.h"

#define VERSTR "00000"
const u32 vmpeg12_mc[] __initconst = {
	0x0680f801, 0x06800000, 0x0d000001, 0x07400040, 0x0c011080,
	0x00000000, 0x080c0002, 0x080c2900, 0x06bffc00, 0x07c00440,
	0x06030400, 0x00400000, 0x0c012e80, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0c7ffe40,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x0c795e40, 0x00000000, 0x0c7919c0,
	0x00000000, 0x0c780480, 0x064c0007, 0x0c7921c0, 0x00000000,
	0x0c79a9c0, 0x00000000, 0x0c791740, 0x00000000, 0x0cc00000,
	0x00400000, 0x0cc00000, 0x00400000, 0x0cc00000, 0x00400000,
	0x00800000, 0x07800449, 0x0c7fff00, 0x06030409, 0x06bffd89,
	0x06030409, 0x00400000, 0x0a2e4047, 0x064c4609, 0x06802008,
	0x064c4309, 0x0b405209, 0x06800008, 0x06800048, 0x0c7ffc80,
	0x07c00e88, 0x07c00e88, 0x078001c9, 0x0aa14009, 0x0a608049,
	0x0c78d400, 0x0a608089, 0x0c78c4c0, 0x0a214807, 0x0c783c00,
	0x00000000, 0x0c78fbc0, 0x00000000, 0x07800787, 0x0b6f7fc7,
	0x07c00047, 0x07800747, 0x064c2908, 0x0bee4808, 0x07c00007,
	0x080c2841, 0x07800188, 0x064c2807, 0x0a2fc047, 0x05801347,
	0x0a61400d, 0x00000000, 0x080c2800, 0x0c7ff540, 0x080c0002,
	0x0aa0c008, 0x0c780840, 0x00000000, 0x0aa0c04d, 0x0c78f600,
	0x00000000, 0x080c2802, 0x064c2807, 0x0a2fc047, 0x058011c7,
	0x07c00607, 0x080c2800, 0x07800688, 0x0a60c008, 0x06800008,
	0x0fc01200, 0x07c006c8, 0x094a11c8, 0x0a210407, 0x060c2c07,
	0x0fc05200, 0x060c0a08, 0x07800708, 0x0aa10008, 0x080c2800,
	0x0c780dc0, 0x00000000, 0x080c2a00, 0x0c0148c0, 0x080c0004,
	0x0c0165c0, 0x00000000, 0x0c7fecc0, 0x00000000, 0x06800009,
	0x07c03e49, 0x0aa2404d, 0x06800008, 0x07800588, 0x0a614008,
	0x0c011940, 0x080c2c00, 0x0c7ffe00, 0x0440134d, 0x06800008,
	0x07c00588, 0x080c2802, 0x064c2807, 0x0a2fc047, 0x058011c7,
	0x07c00607, 0x06800009, 0x07800688, 0x0aa30048, 0x0a21c307,
	0x080c2800, 0x0fc02280, 0x0a61000a, 0x07c0064a, 0x0c7fe600,
	0x080c0002, 0x0aa10088, 0x0a20c0c7, 0x00000000, 0x0fc01240,
	0x07c006c9, 0x04000287, 0x094a1289, 0x07800649, 0x094c2289,
	0x060c2c0a, 0x0a210407, 0x080c2800, 0x0fc05200, 0x060c0a08,
	0x0a24c047, 0x07800708, 0x0a60c008, 0x0c781000, 0x080c2800,
	0x078004c8, 0x0aa0c0c8, 0x06800088, 0x06800048, 0x064c2c09,
	0x094c2248, 0x060c2c09, 0x080c2a00, 0x0c0139c0, 0x080c0000,
	0x0c0156c0, 0x00000000, 0x0c7fddc0, 0x00000000, 0x07800008,
	0x07c06108, 0x080c2800, 0x080c2a00, 0x0c0137c0, 0x080c0000,
	0x00000000, 0x064c2908, 0x00000000, 0x0be42008, 0x00000000,
	0x064c0008, 0x04841208, 0x0a6e5048, 0x07800008, 0x0a620008,
	0x04401208, 0x07800049, 0x0aa14009, 0x06800008, 0x04401249,
	0x07c00049, 0x07806188, 0x0c7fd780, 0x07c00008, 0x064c1008,
	0x07c03c08, 0x064c1108, 0x07c03c48, 0x064c1208, 0x07c03c88,
	0x064c1308, 0x07c03cc8, 0x064c1408, 0x07c03d08, 0x064c1508,
	0x07c03d48, 0x064c1608, 0x07c03d88, 0x064c1708, 0x07c03dc8,
	0x07c0f808, 0x0c013380, 0x00000000, 0x0c015640, 0x00000000,
	0x0c7fd1c0, 0x00000000, 0x080c2a00, 0x0c012c00, 0x080c0004,
	0x0c014900, 0x00000000, 0x0c7fd000, 0x00000000, 0x00000000,
	0x0c78d180, 0x00000000, 0x0683ffc8, 0x07c00748, 0x078013c8,
	0x09808008, 0x07c013c8, 0x0649cc09, 0x04001249, 0x0609cc09,
	0x06807fc9, 0x0aa18009, 0x06494707, 0x091081c7, 0x07800008,
	0x0a4fc207, 0x04401249, 0x0c000100, 0x00000000, 0x0c780d40,
	0x00000000, 0x064e0008, 0x083f0040, 0x083f0000, 0x06490b07,
	0x09807007, 0x09807087, 0x098073c7, 0x06090b07, 0x09c07007,
	0x09c07087, 0x09c073c7, 0x06090b07, 0x0683ffc9, 0x06498f07,
	0x0aa0c009, 0x0a6f8007, 0x04401249, 0x06498e07, 0x09807007,
	0x09807607, 0x09807647, 0x09807687, 0x09807747, 0x06098e07,
	0x09c07007, 0x09c07607, 0x09c07647, 0x09c07687, 0x09c07747,
	0x06098e07, 0x064c0007, 0x09c07187, 0x09807207, 0x060c0007,
	0x064c0807, 0x09807347, 0x060c0807, 0x083f0040, 0x083f0000,
	0x08090b65, 0x00000000, 0x08090b60, 0x0cc00000, 0x060e0008,
	0x0c796880, 0x00000000, 0x0c78c0c0, 0x00000000, 0x0a20d007,
	0x0c7fef00, 0x00000000, 0x0fc081c0, 0x046b3207, 0x0c502280,
	0x07803e89, 0x0aadc009, 0x046b8207, 0x0c57fcc0, 0x046b5207,
	0x0c503440, 0x04600207, 0x0c5067c0, 0x046b2207, 0x0c504940,
	0x046b7207, 0x0c505cc0, 0x07800888, 0x0aaac008, 0x07800788,
	0x0aaa7fc8, 0x07801688, 0x0ac0d207, 0x0a807207, 0x06800048,
	0x07c018c8, 0x078013c8, 0x09808008, 0x0c78b900, 0x07c013c8,
	0x078018c8, 0x0aa10008, 0x06800008, 0x0c78b7c0, 0x07c018c8,
	0x07c01687, 0x078000c8, 0x0acd8207, 0x04401207, 0x07800049,
	0x0a804209, 0x078013c9, 0x09809009, 0x07c013c9, 0x07c06088,
	0x0fc05200, 0x07c060c8, 0x0fc01200, 0x0aa0c008, 0x0c7fff80,
	0x0fc08200, 0x080c2841, 0x07800188, 0x064c2807, 0x0a2fc047,
	0x05801347, 0x0a60c00d, 0x0c78b1c0, 0x00000000, 0x044011cd,
	0x07800009, 0x0a8041c9, 0x078013c9, 0x09809009, 0x07c013c9,
	0x07c06047, 0x07800748, 0x0a405207, 0x07806087, 0x07800788,
	0x0a402207, 0x0c780e00, 0x07800007, 0x07806108, 0x0b403207,
	0x02408207, 0x07800008, 0x0b214008, 0x06800047, 0x0ae0c288,
	0x06800287, 0x040001c8, 0x07800008, 0x0b4031c8, 0x024081c8,
	0x06800008, 0x0c00f640, 0x00000000, 0x07800047, 0x07806088,
	0x0aa68008, 0x0b819207, 0x0ac06207, 0x07800007, 0x07806048,
	0x0b815207, 0x0aa08007, 0x0b013207, 0x06807807, 0x06800008,
	0x06808009, 0x020091c9, 0x0d07ff87, 0x07400248, 0x07806047,
	0x0440a1c7, 0x0b624007, 0x07806088, 0x04401208, 0x07c00048,
	0x07800088, 0x02008207, 0x0ba0c008, 0x06800007, 0x040001c8,
	0x07c00007, 0x07800047, 0x07806088, 0x0ac09207, 0x0b804207,
	0x07800007, 0x07806048, 0x0b405207, 0x0c00dd80, 0x00000000,
	0x0c7ffdc0, 0x00000000, 0x06800048, 0x07c00588, 0x080c2c00,
	0x064c0108, 0x09809048, 0x060c0109, 0x060c0108, 0x07806048,
	0x07c00008, 0x07806088, 0x07c00048, 0x078060c8, 0x060c0a08,
	0x0c7fa600, 0x00000000, 0x06a00007, 0x07c00d07, 0x07c03e87,
	0x0fc0c1c0, 0x0609c607, 0x0fc0c200, 0x0609c708, 0x0400f1c7,
	0x058041c7, 0x060c0907, 0x07c00087, 0x04401247, 0x07c06189,
	0x0400f208, 0x05804208, 0x07c000c8, 0x09508207, 0x0fc041c0,
	0x0fc04200, 0x07c00108, 0x05404208, 0x02c07207, 0x0609c407,
	0x0fc101c0, 0x0fc0f1c0, 0x06800009, 0x07c03e09, 0x064e0009,
	0x0a2fc089, 0x06800008, 0x0a22c047, 0x06800807, 0x06800009,
	0x06c00049, 0x060e0209, 0x0fc10240, 0x060e0209, 0x044011c7,
	0x0aef4007, 0x00000000, 0x04c10208, 0x0fc011c0, 0x0a22c047,
	0x06800807, 0x06800009, 0x06c00049, 0x060e0209, 0x0fc10240,
	0x060e0309, 0x044011c7, 0x0aef4007, 0x00000000, 0x04c08208,
	0x0c788f40, 0x060e0008, 0x06840047, 0x060c0107, 0x0649c409,
	0x098097c9, 0x0fc081c0, 0x0fc011c0, 0x09601247, 0x0fc021c0,
	0x0fc021c0, 0x09702247, 0x0fc021c0, 0x09742247, 0x0fc0d1c0,
	0x0fc091c0, 0x0fc021c0, 0x09622247, 0x0fc051c0, 0x09665247,
	0x0c788a40, 0x0609c409, 0x0c7889c0, 0x0fc041c0, 0x0fc041c0,
	0x0aaa4047, 0x0aa181c7, 0x0a63c0c7, 0x064e0008, 0x0fc011c0,
	0x0c7ff2c0, 0x00000000, 0x0fc101c0, 0x07c00487, 0x058041c7,
	0x07800d08, 0x0940c207, 0x07c00d08, 0x07800e47, 0x0405b1c7,
	0x0c788540, 0x074001c8, 0x0a6b0207, 0x00000000, 0x0fc101c0,
	0x060c0207, 0x0fc0c1c0, 0x060c0307, 0x09102247, 0x07c004c9,
	0x07c06309, 0x07803e08, 0x0649070b, 0x0980b0cb, 0x0a6140c9,
	0x05001308, 0x0680000c, 0x06800008, 0x09c0b0cb, 0x07c03e0c,
	0x078004ca, 0x0aa140ca, 0x0300a30a, 0x0500128a, 0x094e11ca,
	0x060c0307, 0x0a614048, 0x0780018a, 0x0a60c04a, 0x00000000,
	0x0980b10b, 0x0609070b, 0x0a6200c9, 0x06800088, 0x0a215007,
	0x06800089, 0x07c00649, 0x0c7800c0, 0x06800048, 0x06800008,
	0x07c00688, 0x090a1207, 0x07c00708, 0x0fc01240, 0x07c00549,
	0x07800d08, 0x095e1209, 0x09021247, 0x095c1209, 0x090e1247,
	0x095a1209, 0x078004c9, 0x0aa0c0c9, 0x06800049, 0x06800009,
	0x09581209, 0x07c00d08, 0x07800e49, 0x0405b249, 0x07400248,
	0x090e1247, 0x07c00509, 0x0c7875c0, 0x00000000, 0x06806349,
	0x06800007, 0x06801c88, 0x07400247, 0x0d07ffc8, 0x04001249,
	0x0cc00000, 0x00000000, 0x0649c907, 0x0befffc7, 0x00000000,
	0x0c07fd40, 0x00000000, 0x06806349, 0x06800008, 0x0680000a,
	0x064c2a07, 0x0a618007, 0x00000000, 0x064c3607, 0x058081c7,
	0x0aa3c047, 0x00000000, 0x0fc081c0, 0x0be18048, 0x00000000,
	0x04000287, 0x07400247, 0x0c780140, 0x00000000, 0x095081ca,
	0x07400247, 0x04001249, 0x04001208, 0x0b2bb888, 0x00000000,
	0x07806287, 0x040011c7, 0x07c06287, 0x0649cb0a, 0x06850008,
	0x0240a20a, 0x0603510a, 0x06806247, 0x024091c9, 0x04003249,
	0x05802249, 0x05402249, 0x06035209, 0x07806247, 0x0200a287,
	0x0240b1c8, 0x05401249, 0x04007249, 0x05803249, 0x05403249,
	0x07c06249, 0x020071c9, 0x0b405207, 0x06800008, 0x04000207,
	0x0603510a, 0x07c06248, 0x06a06249, 0x06035009, 0x06435009,
	0x0580f249, 0x0bef8049, 0x0a62c008, 0x0c07eec0, 0x00000000,
	0x0603510a, 0x058012cb, 0x0603520b, 0x06a06349, 0x06035009,
	0x06435009, 0x0580f249, 0x0bef8049, 0x06803f87, 0x07806248,
	0x09610207, 0x0609c908, 0x08007401, 0x0649c907, 0x0befffc7,
	0x00000000, 0x0c7809c0, 0x07800207, 0x0a608007, 0x0c780780,
	0x044011c7, 0x0aa0c087, 0x07800289, 0x07800309, 0x0ae380c9,
	0x00000000, 0x0405b1c9, 0x070001c7, 0x0609c507, 0x0403b1c9,
	0x070001c8, 0x040081c7, 0x070001c7, 0x09610207, 0x0609cd08,
	0x04001249, 0x0609c909, 0x08007401, 0x078002c9, 0x0ae281c9,
	0x00000000, 0x0649c907, 0x0a6fc007, 0x0405b1c9, 0x070001c7,
	0x0609c507, 0x04001249, 0x0609c909, 0x08007401, 0x06803fc9,
	0x07c00289, 0x07c002c9, 0x07c00309, 0x06800009, 0x07c00209,
	0x0c7855c0, 0x00000000, 0x07800889, 0x0a608009, 0x0c780700,
	0x07800789, 0x0aa0ffc9, 0x0c008a80, 0x00000000, 0x07800889,
	0x0aa58009, 0x0683ffc9, 0x0aa1c009, 0x06494707, 0x091081c7,
	0x07806188, 0x0aa40008, 0x0a4fb207, 0x04401249, 0x0683ffc8,
	0x0649bb07, 0x0900c1c7, 0x0aa10007, 0x0aa0c008, 0x0c7fff00,
	0x04401208, 0x0649bc07, 0x093e11c7, 0x0a2f8047, 0x00000000,
	0x0c078200, 0x00000000, 0x06800007, 0x07c01687, 0x07c018c7,
	0x064c5108, 0x064c4609, 0x064c4207, 0x064c400a, 0x02407287,
	0x040081c7, 0x0580a1c7, 0x0e000207, 0x02409289, 0x00000000,
	0x00000000, 0x0f0001c0, 0x0f010200, 0x097081c8, 0x0540a1c7,
	0x02007247, 0x064c4308, 0x02407207, 0x07c012c7, 0x058101c7,
	0x07c01307, 0x0c00c840, 0x00000000, 0x0fc0a1c0, 0x07c062c7,
	0x0fc031c0, 0x078062c8, 0x09543207, 0x07c062c8, 0x0ba08047,
	0x0b20c0c7, 0x0c7fef80, 0x00000000, 0x044011c7, 0x07c00187,
	0x060c0407, 0x06800008, 0x09808048, 0x0a234088, 0x06800049,
	0x07800848, 0x0b628088, 0x0aa10087, 0x0aa18007, 0x0a614008,
	0x00000000, 0x06800009, 0x0c784140, 0x07c00889, 0x04001208,
	0x07c00848, 0x07c00889, 0x0aa48007, 0x06800009, 0x064c040a,
	0x0fc10200, 0x0fc01200, 0x094a1288, 0x0fc03200, 0x09503248,
	0x09583248, 0x0a61c087, 0x09583248, 0x0fc01200, 0x09481288,
	0x0fc03200, 0x09403248, 0x09483248, 0x060c0209, 0x060c040a,
	0x06800008, 0x07c003c8, 0x07800088, 0x05404208, 0x078000c9,
	0x05404249, 0x09610248, 0x06090809, 0x06804008, 0x06090708,
	0x07803e07, 0x046011c7, 0x0c503780, 0x00000000, 0x0649c909,
	0x0aa10009, 0x06800087, 0x0c7f3500, 0x07c001c7, 0x07800207,
	0x0a608007, 0x0c780d00, 0x044011c7, 0x0aa0c087, 0x07800289,
	0x07800309, 0x0b2081c9, 0x0c780b80, 0x00000000, 0x078013c8,
	0x07800207, 0x044011c7, 0x0a610087, 0x00000000, 0x0be38148,
	0x0c780380, 0x0be100c8, 0x09808088, 0x09c08088, 0x0c780180,
	0x0be14048, 0x09808048, 0x0a60c007, 0x00000000, 0x09c08048,
	0x07c013c8, 0x0a20c0c8, 0x00000000, 0x098073c7, 0x07800e48,
	0x04050208, 0x07400207, 0x078013c8, 0x09c08008, 0x07c013c8,
	0x0405b1c9, 0x070001c7, 0x04050209, 0x07000208, 0x05410208,
	0x02c07207, 0x0609c507, 0x0403b1c9, 0x070001c8, 0x040081c7,
	0x070001c7, 0x09610207, 0x0609cd08, 0x04001249, 0x0609c909,
	0x08007401, 0x06800009, 0x07c00209, 0x06800009, 0x07c001c9,
	0x07800247, 0x02c07247, 0x0780090a, 0x078008e1, 0x0be0c047,
	0x0c780880, 0x06800008, 0x0780098a, 0x07800961, 0x0a278087,
	0x06800048, 0x07800a0a, 0x078009e1, 0x0a268107, 0x06800088,
	0x07800a8a, 0x07800a61, 0x0a258207, 0x068000c8, 0x07800b0a,
	0x07800ae1, 0x0a248407, 0x06800108, 0x07800b8a, 0x07800b61,
	0x0a238807, 0x06800148, 0x07800c4a, 0x07800c21, 0x0a229007,
	0x06800188, 0x07800cca, 0x07800ca1, 0x0a21a007, 0x068001c8,
	0x0809ce01, 0x06800047, 0x0c7f1d40, 0x07c001c7, 0x096102a1,
	0x07800387, 0x090041c7, 0x0a8061c8, 0x07800187, 0x0a620087,
	0x07800387, 0x090841c7, 0x0a4051c8, 0x06800047, 0x03407207,
	0x0c7ff380, 0x02c091c9, 0x0809ce00, 0x06800047, 0x07800349,
	0x0a60c008, 0x0c780600, 0x09401247, 0x0a60c048, 0x0c780540,
	0x09481247, 0x0a60c088, 0x0c780480, 0x09501247, 0x0a60c0c8,
	0x0c7803c0, 0x09581247, 0x07801609, 0x0a60c108, 0x0c780240,
	0x09401247, 0x0a60c148, 0x0c780180, 0x09481247, 0x0a60c188,
	0x0c7800c0, 0x09501247, 0x09581247, 0x0c7800c0, 0x07c01609,
	0x07c00349, 0x07800d09, 0x0405b1c8, 0x07c00e48, 0x074001c9,
	0x078012c9, 0x0403b1c8, 0x074001c9, 0x07801309, 0x040081c7,
	0x074001c9, 0x06800047, 0x03407207, 0x07800249, 0x02c091c9,
	0x07c00249, 0x0609b20a, 0x0609920a, 0x07800187, 0x04001247,
	0x07c00209, 0x0a608087, 0x0c780340, 0x078002c7, 0x07c00307,
	0x07c002c8, 0x06499109, 0x06099009, 0x07800389, 0x05404249,
	0x09404248, 0x07c00389, 0x0609910a, 0x0c7800c0, 0x00000000,
	0x07c00288, 0x08098002, 0x078004ce, 0x0a64c0ce, 0x00000000,
	0x06499011, 0x054103d1, 0x0581040f, 0x02c0f40f, 0x0609810f,
	0x06499111, 0x054103d1, 0x0581040f, 0x02c0f40f, 0x0609820f,
	0x080980f0, 0x078004ce, 0x0aa6c0ce, 0x00000000, 0x080980f4,
	0x0c780600, 0x00000000, 0x07803e0e, 0x0aab800e, 0x00000000,
	0x06499011, 0x054103d1, 0x0649b110, 0x05410410, 0x078004ce,
	0x0a61404e, 0x00000000, 0x058103cf, 0x0c7800c0, 0x00000000,
	0x05810410, 0x02c0f40f, 0x0609810f, 0x06499111, 0x054103d1,
	0x0581040f, 0x02c0f40f, 0x0609820f, 0x080980f4, 0x06800009,
	0x07c00009, 0x0c780080, 0x07c00049, 0x064c0009, 0x0a2fc049,
	0x00000000, 0x064c3a09, 0x09006249, 0x0aaec649, 0x00000000,
	0x080c2800, 0x080c0002, 0x0c7efb80, 0x00000000, 0x0c0011c0,
	0x00000000, 0x08011f00, 0x08002515, 0x08002605, 0x08002708,
	0x08002801, 0x08002a09, 0x08007601, 0x0649c003, 0x07c00903,
	0x058100c3, 0x07c008c3, 0x0649c103, 0x07c00983, 0x058100c3,
	0x07c00943, 0x0649c203, 0x07c00a03, 0x058100c3, 0x07c009c3,
	0x0649c303, 0x07c00a83, 0x058100c3, 0x07c00a43, 0x0649c403,
	0x07c00b03, 0x058100c3, 0x07c00ac3, 0x0649c503, 0x07c00b83,
	0x058100c3, 0x07c00b43, 0x0649c603, 0x07c00c43, 0x058100c3,
	0x07c00c03, 0x0649c703, 0x07c00cc3, 0x058100c3, 0x07c00c83,
	0x06498e03, 0x098037c3, 0x06098e03, 0x0649ca07, 0x0aa48007,
	0x00000000, 0x09010207, 0x092101c7, 0x0609c607, 0x0609c708,
	0x0400f1c7, 0x058041c7, 0x060c0907, 0x07c00087, 0x04401247,
	0x07c06189, 0x0400f208, 0x05804208, 0x07c000c8, 0x09508207,
	0x06800007, 0x07c00d07, 0x0cc00000, 0x00000000, 0x0c7eea40,
	0x00000000, 0x0cc00000, 0x00400000, 0x0c7ee940, 0x00000000,
	0x0684001f, 0x060c011f, 0x064c001f, 0x09c1f19f, 0x0981f21f,
	0x060c001f, 0x0680001f, 0x07c0021f, 0x07c0025f, 0x07c001df,
	0x07c0041f, 0x07c0085f, 0x07c0089f, 0x06803fe0, 0x07c002a0,
	0x07c002e0, 0x07c00320, 0x07c003a0, 0x068000a0, 0x07c00660,
	0x06800060, 0x07c006a0, 0x06800020, 0x07c00720, 0x068000e0,
	0x07c004e0, 0x06804020, 0x07c06020, 0x0cc00000, 0x00000000,
	0x0c7ff800, 0x00000000, 0x078003de, 0x0400179e, 0x07c003de,
	0x0649ce1e, 0x0a64401e, 0x064c411e, 0x064c4322, 0x0968c7a2,
	0x0a40d8de, 0x040008de, 0x064c471e, 0x0ba2a01e, 0x0ba26022,
	0x0780165e, 0x0400179e, 0x07c0165e, 0x0a61c21e, 0x00000000,
	0x0809cf01, 0x0c780000, 0x00000000, 0x0680001e, 0x07c0165e,
	0x0cc00000, 0x00400000, 0x0649c828, 0x0aa68028, 0x0a20e028,
	0x0c7805c0, 0x07c00428, 0x0aa3c068, 0x06bfffa9, 0x0aa340a8,
	0x06bfff69, 0x0aa2c0e8, 0x06bffee9, 0x0aa24128, 0x06bffde9,
	0x0aa1c168, 0x06bffbe9, 0x0aa141a8, 0x06bff7e9, 0x0aa0c1e8,
	0x06bfefe9, 0x06bfdfe9, 0x00800000, 0x0809ce00, 0x07800268,
	0x02828a68, 0x07c00268, 0x00400000, 0x0809c800, 0x078001e8,
	0x0a618028, 0x07800ea8, 0x0aa18028, 0x06802028, 0x064c4329,
	0x0b003a29, 0x06800128, 0x06030528, 0x0cc00000, 0x00000000,
	0x07800189, 0x0aa0c089, 0x06800208, 0x07c00608, 0x078004c8,
	0x0aa0c0c8, 0x06800088, 0x06800048, 0x07c00648, 0x0aa10089,
	0x06800008, 0x07c03c08, 0x07c03c48, 0x07c0f808, 0x06800048,
	0x07c03e48, 0x07800188, 0x0a614048, 0x0c004a00, 0x00000000,
	0x0c780100, 0x00000000, 0x0c005000, 0x00000000, 0x0c002c40,
	0x00000000, 0x07800009, 0x04001249, 0x07c00009, 0x07800088,
	0x0b809209, 0x06800009, 0x07c00009, 0x07800048, 0x04001208,
	0x078000c9, 0x0b803248, 0x07c00048, 0x0680008d, 0x0cc00000,
	0x00000000, 0x07800007, 0x07806108, 0x0b403207, 0x02408207,
	0x07800008, 0x0b214008, 0x06800047, 0x0ae0c288, 0x06800287,
	0x040001c8, 0x07800008, 0x0b4031c8, 0x024081c8, 0x06800008,
	0x0c0011c0, 0x00000000, 0x07800787, 0x0a60bfc7, 0x0cc00000,
	0x0c000100, 0x00000000, 0x0c7ffec0, 0x00000000, 0x078004c8,
	0x0aa0c0c8, 0x06800088, 0x06800048, 0x07c00648, 0x07800009,
	0x05402249, 0x06808008, 0x02009209, 0x07100248, 0x090011c8,
	0x05c01208, 0x07c03c48, 0x04001249, 0x07100248, 0x07c03c08,
	0x04001249, 0x07100248, 0x090012c8, 0x05c01208, 0x07c03d48,
	0x04001249, 0x07100248, 0x07c03d08, 0x054022cb, 0x094612c7,
	0x0a60c30b, 0x0c780240, 0x0680020b, 0x0a61c00b, 0x06800008,
	0x07c03c48, 0x07c03c08, 0x07c03d48, 0x07c03d08, 0x0680020b,
	0x07c0060b, 0x07c0f808, 0x06800048, 0x07c03e48, 0x0c003b80,
	0x00000000, 0x0c0017c0, 0x00000000, 0x07800009, 0x04001249,
	0x07c00009, 0x07800088, 0x0b80c209, 0x06800009, 0x07c00009,
	0x07800048, 0x04001208, 0x078000c9, 0x0b806248, 0x07c00048,
	0x06800009, 0x07c00049, 0x06803fc9, 0x07c00789, 0x0cc00000,
	0x00000000, 0x07800189, 0x0a640009, 0x07800049, 0x0540a249,
	0x0649cb07, 0x020091c9, 0x06035109, 0x07800089, 0x05402249,
	0x06035209, 0x06b08009, 0x06035009, 0x06435009, 0x0580f249,
	0x0bef8049, 0x0cc00000, 0x07c00008, 0x07906149, 0x0b403209,
	0x0cc00000, 0x07c00008, 0x044011c9, 0x07c06147, 0x05402249,
	0x06808007, 0x020091c9, 0x07806007, 0x0680400b, 0x0ac032c7,
	0x044041c7, 0x06805f07, 0x07c06007, 0x070001cb, 0x0740024b,
	0x040011c7, 0x04001249, 0x070001cb, 0x0740024b, 0x040011c7,
	0x04001249, 0x070001cb, 0x0740024b, 0x040011c7, 0x04001249,
	0x070001cb, 0x0c7ff4c0, 0x0740024b, 0x064c4414, 0x09101514,
	0x0a6f8014, 0x0c7ea500, 0x00000000, 0x0c0005c0, 0x00000000,
	0x07800007, 0x040011c7, 0x07c00747, 0x07800049, 0x07800088,
	0x0b403207, 0x0cc00000, 0x07c00789, 0x06800008, 0x07c00748,
	0x04001209, 0x078000c9, 0x0b403248, 0x0cc00000, 0x07c00788,
	0x06411f08, 0x04001208, 0x06011f08, 0x06803fc8, 0x0cc00000,
	0x07c00788, 0x07800009, 0x07c06149, 0x06808008, 0x05402249,
	0x02008248, 0x07806007, 0x07000209, 0x074001c9, 0x04001208,
	0x040011c7, 0x07000209, 0x074001c9, 0x04001208, 0x040011c7,
	0x07000209, 0x074001c9, 0x04001208, 0x040011c7, 0x07000209,
	0x074001c9, 0x04403208, 0x040011c7, 0x09c071c7, 0x07c06007,
	0x07800607, 0x0be4c047, 0x07903c49, 0x05401249, 0x090612c7,
	0x0940124b, 0x07400209, 0x04001208, 0x07903c09, 0x07400209,
	0x04001208, 0x07903d49, 0x05401249, 0x090412c7, 0x0940124b,
	0x07400209, 0x04001208, 0x07903d09, 0x0c780840, 0x07400209,
	0x07800009, 0x0a610009, 0x044041c8, 0x0c780700, 0x00000000,
	0x0780618b, 0x0b4102c9, 0x070001c9, 0x07400209, 0x040011c7,
	0x04001208, 0x070001c9, 0x07400209, 0x040011c7, 0x04001208,
	0x070001c9, 0x07400209, 0x040011c7, 0x04001208, 0x070001c9,
	0x0c7802c0, 0x07400209, 0x06800049, 0x07400209, 0x04001208,
	0x06800009, 0x07400209, 0x04001208, 0x07400209, 0x04001208,
	0x07400209, 0x07800189, 0x0a644049, 0x07800009, 0x07806188,
	0x0b80e209, 0x07800049, 0x0540a249, 0x0649cb08, 0x02009209,
	0x06035109, 0x07800089, 0x05402249, 0x06035209, 0x06a08009,
	0x06035009, 0x06435009, 0x0580f249, 0x0bef8049, 0x0cc00000,
	0x00000000, 0x06490604, 0x0aefcc04, 0x00000000, 0x07800004,
	0x07800045, 0x09508144, 0x06800084, 0x06c00004, 0x078004c6,
	0x04001186, 0x09562106, 0x06090604, 0x06090605, 0x06881044,
	0x09562106, 0x06090604, 0x0cc00000, 0x00000000, 0x06490604,
	0x0aefcc04, 0x00000000, 0x07800004, 0x07800045, 0x09508144,
	0x06900184, 0x06c00004, 0x078004c6, 0x04001186, 0x09562106,
	0x06090604, 0x06090605, 0x08090600, 0x06800004, 0x095e2106,
	0x06090604, 0x06985144, 0x06c00004, 0x09562106, 0x06090604,
	0x0cc00000, 0x00000000, 0x06800007, 0x07c06107, 0x07c00747,
	0x0cc00000, 0x07c00787, 0x06490604, 0x0aefcc04, 0x00000000,
	0x0680000a, 0x07800004, 0x07800049, 0x09508244, 0x06800087,
	0x07803e44, 0x0a60c044, 0x06c00007, 0x09807087, 0x07800606,
	0x0be10106, 0x00000000, 0x0c780140, 0x09807387, 0x0a20c206,
	0x098073c7, 0x09807387, 0x07800645, 0x078004cb, 0x0a67c0cb,
	0x0a208306, 0x0a618085, 0x06800008, 0x0c000e40, 0x00000000,
	0x0c780d40, 0x00000000, 0x0680000a, 0x0a6180c5, 0x0780018c,
	0x0a61404c, 0x00000000, 0x0c7800c0, 0x09807407, 0x0a638045,
	0x06800084, 0x095621c4, 0x06800044, 0x094a31c4, 0x0c000a40,
	0x06800088, 0x068000c4, 0x095621c4, 0x09c07047, 0x0c000900,
	0x068000c8, 0x0c780800, 0x00000000, 0x0c780000, 0x00000000,
	0x0aa0c04b, 0x06800084, 0x068000c4, 0x095621c4, 0x0a208306,
	0x0a614045, 0x0c000600, 0x06800008, 0x0c780500, 0x00000000,
	0x0a6240c5, 0x0780018c, 0x0a62004c, 0x00000000, 0x06800008,
	0x0c0003c0, 0x09807407, 0x0c7802c0, 0x00000000, 0x0a6ac085,
	0x06800044, 0x094a31c4, 0x0c000200, 0x06800088, 0x09807107,
	0x09c07047, 0x0c000100, 0x068000c8, 0x0cc00000, 0x00000000,
	0x0680000b, 0x06c0004b, 0x0aa0c0c8, 0x06090607, 0x06090609,
	0x091c2187, 0x0aa5c086, 0x07803c04, 0x0a6180c8, 0x07803c45,
	0x0a8042c7, 0x00000000, 0x07803c84, 0x07803cc5, 0x06090604,
	0x09181107, 0x091e1245, 0x02809244, 0x07803e4c, 0x0aa1004c,
	0x0a0042c7, 0x00000000, 0x068000c6, 0x09161247, 0x03405285,
	0x09601144, 0x095e1149, 0x0aa5c046, 0x06090605, 0x07803d04,
	0x0a6100c8, 0x07803d45, 0x07803d84, 0x07803dc5, 0x06090604,
	0x09181107, 0x091e1245, 0x02809244, 0x07803e4c, 0x0aa1804c,
	0x0a0062c7, 0x00000000, 0x09161247, 0x0c7800c0, 0x05001249,
	0x09161247, 0x03405285, 0x09601144, 0x095e1149, 0x06090605,
	0x09804347, 0x09804204, 0x09804184, 0x0aa0c088, 0x09c04044,
	0x09804004, 0x0cc00000, 0x06090604, 0x0c7e5840, 0x00000000,
	0x0609c42d, 0x00000000, 0x0649c42d, 0x0a6f802d, 0x00000000,
	0x0cc00000, 0x00000000, 0x0fc09200, 0x0fc101c0, 0x078061c9,
	0x0780620b, 0x07c061c7, 0x07c06208, 0x05410208, 0x02c081c8,
	0x054102cb, 0x02c0b24b, 0x0fc021c0, 0x0be10047, 0x0c780940,
	0x0a60800b, 0x0c7808c0, 0x09265248, 0x05406289, 0x05402249,
	0x0240a24a, 0x091a6248, 0x02009289, 0x05406289, 0x05402249,
	0x0240a24a, 0x090c6248, 0x02008289, 0x0926524b, 0x05406289,
	0x05402249, 0x0240a24a, 0x091a624b, 0x02009289, 0x05406289,
	0x05402249, 0x0240a24a, 0x090c624b, 0x0200b289, 0x0b8052c8,
	0x024082c8, 0x06800207, 0x0b4021c8, 0x0c780200, 0x06800007,
	0x07c00847, 0x07c00887, 0x06800008, 0x07c061c8, 0x07c06208,
	0x00000000, 0x0c7f4c80, 0x0fc071c0

};


#define FOR_VFORMAT VFORMAT_MPEG12

#define REG_FIRMWARE_ALL()\
		DEF_FIRMWARE_VER(vmpeg12_mc, VERSTR);\

INIT_DEF_FIRMWARE();

