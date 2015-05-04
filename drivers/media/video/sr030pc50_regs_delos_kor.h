/* drivers/media/video/sr030pc50_regs_1_1.h
 *
 * Driver for sr030pc50 (VGA Camera) from Siliconfile
 *
 * Copyright (C) 2012, SAMSUNG ELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


/* ========== version info ==========*/
//2012.06.13 jwryu
//26MHz (50Hz)
/* ============================*/

#ifndef __EOSCAMERASR030PC50_REG_H
#define __EOSCAMERASR030PC50_REG_H

//#define START_BURST_MODE 0x0F12
//#define DELAY_SEQ 0xFF

// Frame Auto
static const unsigned short sr030pc50_set_init_regs[]={
0x0300,
0x0171,
0x0173,
0x0171,


// PLL Setting
0x0300,

0x080f, //Parallel NO Output_PAD Out
0x1000,
0x1190,
0x1200,
0x1488,

0x0baa,
0x0caa,
0x0daa,

0xc095,
0xc118,
0xc291,
0xc300,
0xc401,

0x0320, //page 20
0x100c, //ae off
0x0322, //page 22
0x107b, //awb off

0x0300,
0x1200,
0x2000,
0x2104,
0x2200,
0x2304,

0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 129
0x4381,

//BLC
0x802e, //don't touch
0x817e, //don't touch
0x8290, //don't touch
0x8330, //don't touch
0x842c, //don't touch
0x854b, //don't touch
0x8601, //don't touch
0x8847, //don't touch
//0x8948,//BLC hold

0x900f, //BLC_TIME_TH_ON
0x910f, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x9838, //don't touch
0x9940, //Out BLC
0xa040, //Dark BLC
0xa844, //Normal BLC 44

//Page2 Last Update 12_01_20
0x0302,
0x1000,
0x1100,
0x1340,
0x1404,
0x181c,
0x1900, //01
0x1a00,
0x1b08,
0x1c9c,
0x1d03,
0x2033,
0x2177,
0x22a7,
0x2332,
0x2433,
0x2b40,
0x2d32,
0x3199,
0x3200,
0x3300,
0x343c,
0x350d,
0x3b60, //80

//timing control 1 // //don't touch
0x5021,
0x511c,
0x52aa,
0x535a,
0x5430,
0x5510,
0x560c,
0x5800,
0x590f,

//timing control 2 // //don't touch
0x6034,
0x613a,
0x6234,
0x6339,
0x6434,
0x6539,
0x7235,
0x7338,
0x7435,
0x7538,
0x8002,
0x812e,
0x820d,
0x8310,
0x840d,
0x8510,
0x921d,
0x9320,
0x941d,
0x9520,
0xa003,
0xa12d,
0xa42d,
0xa503,
0xa812,
0xa91b,
0xaa22,
0xab2b,
0xac10,
0xad0e,
0xb833,
0xb935,
0xbc0c,
0xbd0e,
0xc03a,
0xc13f,
0xc23a,
0xc33f,
0xc43a,
0xc53e,
0xc63a,
0xc73e,
0xc83a,
0xc93e,
0xca3a,
0xcb3e,
0xcc3b,
0xcd3d,
0xce3b,
0xcf3d,
0xd033,
0xd13f,

//Page 10
0x0310,
0x1001, //03, //ISPCTL1, YUV ORDER(FIX)
0x1143,
0x1230, //Y offet, dy offseet enable
0x4080,
0x4102, //00 DYOFS  00->10  _100318
0x4885, //Contrast  88->84  _100318
0x5048, //AGBRT

0x6001, //7f //7c
0x6100, //Use default
0x627e, //80 SATB_B  (1.4x)
0x637c, //80 SATR_R  (1.2x)
0x6448, //AGSAT
0x6690, //wht_th2
0x6736, //wht_gain  Dark (0.4x), Normal (0.75x)

0x8000,

//Page 11
//LPF
0x0311,
0x1025,
0x1107,
0x2000,
0x2160,
0x230a,
0x6013,
0x6185,
0x6200,
0x6300,
0x6400,

0x6770,
0x6824,
0x6904,

//Page 12
//2D
0x0312,
0x40d3,
0x4109,
0x5018,
0x5124,
0x701f,
0x7100,
0x7200,
0x7300,
0x7412,
0x7512,
0x7620,
0x7780,
0x7888,
0x7918,

///////////////////////
0x903d,
0x9134,
0x9928,
0x9c05, //14 For defect
0x9d08, //15 For defect
0x9e28,
0x9f28,

0xb07d, //75 White Defect
0xb544,
0xb682,
0xb752,
0xb844,
0xb915,
///////////////////////

//Edge
0x0313,
0x1001,
0x1189,
0x1214,
0x1319,
0x1408,
0x2003,
0x2104,
0x2325,
0x2421,
0x2508,
0x2640,
0x2700,
0x2808,
0x2950,
0x2ae0,
0x2b10,
0x2c28,
0x2d40,
0x2e00,
0x2f00,
0x3011,
0x8005,
0x8107,
0x9004,
0x9105,
0x9200,
0x9330,
0x9430,
0x9510,

0x0314,
0x1001,

0x2244,  //54,
0x2334,  //41,
0x2432,  //40, //30

0x2758, //60 RXCEN
0x2880, //RYCEN
0x2958, //60 GXCEN
0x2a80, //GYCEN
0x2b58, //60 BXCEN
0x2c80, //BYCEN

//15page//////////////////////////
0x0315,
0x1003,

0x1452,
0x163a,
0x172f,

//CMC
0x30f1,
0x3171,
0x3200,
0x331f,
0x34e1,
0x3542,
0x3601,
0x3731,
0x3872,
//CMC OFS
0x4090,
0x4182,
0x4212,
0x4386,
0x4492,
0x4518,
0x4684,
0x4702,
0x4802,

0x0316,
0x1001,
0x3000,
0x310d,
0x321b,
0x332b,  //2d,
0x3452,  //4e,
0x3576,  //6f,
0x368e,  //86,
0x37a3,  //9d,
0x38b7,  //af,
0x39c7,  //c0,
0x3ad5,  //cf,
0x3be7,  //e2,
0x3cf3,  //f0,
0x3df9,
0x3eff,

//Page 17 AE
0x0317,
0xc43c,
0xc532,

//Page 20 AE
0x0320,
0x100c,
0x1104,

0x2001,
0x2827,
0x29a1,

0x2af0,
0x2bf4,
0x2c2b,

0x30f8,

0x3b22,
0x3cde,

0x3922,
0x3ade,
0x3b22, //23->22 _10_04_06 hhzin
0x3cde,

0x6070, //70
0x6100, //11 //22

0x6270,
0x6300, //11 //22

0x682c,// x no Flip 34
0x696e,// x no Flip 66
0x6A27,
0x6Bbb,

0x7034,//Y Targe 32

0x7611, //Unlock bnd1
0x7772, //Unlock bnd2 02->a2 _10_04_06 hhzin

0x7812, //Yth 1
0x7926, //Yth 2 26->27 _10_04_06 hhzin
0x7a23, //Yth 3

0x7c17, //1c->1d _10_04_06 hhzin
0x7d22,

//60Hz
0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8600, //EXPMin 7500.00 fps
0x87c8,

0x8802, //EXP Max(120Hz) 8.00 fps
0x89dc,
0x8a6c,
0xa002, //EXP Max(100Hz) 8.33 fps
0xa1bf,
0xa220,

0x8B3a, //EXP100
0x8C98,
0x8D30, //EXP120
0x8Ed4,

0x988C,
0x9923,

0x9c07, //EXP Limit 833.33 fps
0x9d08,
0x9e00, //EXP Unit
0x9fc8,

0xb01d,
0xb114,
0xb2a0, //80
0xb317, //AGLVL //17
0xb417,
0xb53e,
0xb62b,
0xb724,
0xb821,
0xb91f,
0xba1e,
0xbb1d,
0xbc1c,
0xbd1b,

0xc01a,
0xc348,
0xc448,

//Page 22 AWB
0x0322,
0x10e2,
0x112e, //2e
0x2075, //01 //69
0x2180,
0x24fe,

0x3080, //Cb
0x3180, //Cr
0x3811, //Lock Boundary //13
0x3933,
0x40e3, //f0
0x4143,
0x4233,
0x43f3,
0x4444,
0x4566,
0x4608,
0x4763,

0x803d, //3a //3c
0x8120,
0x8240,

0x835a, //RMAX 5a
0x8420, //22 RMIN 23
0x8556, //BMAX 5a
0x8624, //BMIN

0x8748, //42
0x883a,
0x893e,
0x8a34,

0x8b00, //OUT TH
0x8d24, //21 //22
0x8e41, //41

0x8f63,
0x9062,
0x915e,
0x925a,
0x9350,
0x9442,
0x953b,
0x9634,
0x972d,
0x982b,
0x9929,
0x9a27,
0x9b0b,
0xb47f,

///////////////////////////// Page 48
0x0348,

0x1005,
0x1100, //async_fifo off
0x1200,

0x16c4,
0x1700,
0x1900,
0x1a00,
0x1c02,
0x1d04,
0x1e07,
0x1f06,
0x2000,
0x21b8,
0x2200,
0x2301,

0x3005,
0x3100,
0x3402,
0x3206,
0x3503,
0x3601,
0x3703,
0x3800,
0x394a,
0x3c00,
0x3dfa,
0x3f10,
0x4000,
0x4120,
0x4200,

0x0322,
0x10fb,

0x0320,
0x108c,
};

static const unsigned short sr030pc50_stream_off[]={
0x0300,
0x0171, 
};

static const unsigned short sr030pc50_stream_on[]={
0x0300,
0x0170, 
};

//==========================================================
//  EFFECT(6)
//==========================================================

static const unsigned short sr030pc50_effect_normal_regs[] =
{
0x0310,
0x1103,
0x1230,
0x0313,
0x103b,
0x2002,
};
static const unsigned short sr030pc50_effect_negative_regs[] =
{
0x0310,
0x1103,
0x1238,
0x0313,
0x103b,
0x2002,
};
static const unsigned short sr030pc50_effect_sepia_regs[] =
{
0x0310,
0x1103,
0x1230,
0xff03,
0x4470,
0x4598,
0x1233,
0x0313,
0x103b,
0x2002,
};
static const unsigned short sr030pc50_effect_mono_regs[] =
{
0x0310,
0x1103,
0x1230,
0xff03,
0x4480,
0x4580,
0x1233,
0x0313,
0x103b,
0x2002,
};
static const unsigned short sr030pc50_effect_aqua_regs[] =
{
0x0310,
0x1103,
0x1233,
0x44b0,
0x4540,
0x0313,
0x103b,
0x2002,
};
static const unsigned short sr030pc50_effect_sharpen_regs[] =
{
0xFF00,  //END
};
static const unsigned short sr030pc50_effect_solarization_regs[] =
{
0xFF00,  //END
};
static const unsigned short sr030pc50_effect_black_white_regs[] =
{
0xFF00,  //END
};
//==========================================================
//  EFFECT(6)  -END
//==========================================================



//==========================================================
// WB(5) -START
//==========================================================


static const unsigned short sr030pc50_wb_auto_regs[] =
{
0x0322,
0x10e2,
0x112e,
0x803d, //3a //3c
0x8120,
0x8240,
0x835a, //RMAX
0x8420, //(0216_kjh20>>23) RMIN
0x8556, //(0216_kjh5c>>5a) BMAX //51
0x8624, //BMIN //20
0x10fb,

};

static const unsigned short sr030pc50_wb_daylight_regs[] =
{
0x0322, //MWB Sunny
0x107b,
0x1126,
0x803f, //52 R Gain
0x8120, //G Gain
0x8230, //B Gain
0x833a, //R Max //41
0x843a, //R Min
0x8538, //B Max
0x862a, //B Min //2f
0x10fb,
};

static const unsigned short sr030pc50_wb_cloudy_regs[] =
{
0x0322, //MWB Cloudy
0x107b,
0x1126,
0x804f, //R Gain
0x8120, //G Gain
0x8225, //B Gain
0x8353, //R Max
0x8448, //R Min
0x8535, //B Max
0x862b, //B Min
0x10fb,
};
static const unsigned short sr030pc50_wb_incandescent_regs[] =
{
0x0322,
0x107b,
0x1126,
0x8020,
0x8120,
0x8257,
0x8321,
0x841d,
0x8559,
0x8656,
0x10fb,

};

static const unsigned short sr030pc50_wb_fluorescent_regs[] =
{
0x0322,
0x107b,
0x1126,
0x8042,
0x8120,
0x8251,
0x834a,
0x843a,
0x8555,
0x8645,
0x10fb,
};

//==========================================================
// WB(5)  --END
//==========================================================



//==========================================================
//METERING()
//==========================================================

static const unsigned short sr030pc50_metering_matrix_regs[] =
{
0x0320,
0x6011,
0x6111,
0x6211,
0x6311,

};

static const unsigned short sr030pc50_metering_center_regs[] =
{
0x0320,
0x6070, //70
0x6110, //11 //22

0x6270,
0x6310, //11 //22
};

static const unsigned short sr030pc50_metering_spot_regs[] =
{
0x0320,
0x6070,
0x6100,
0x6270,
0x6300,
};
//==========================================================
//METERING() - END
//==========================================================


//==========================================================
// BRIGHTNES(9) - START
//==========================================================

static const unsigned short sr030pc50_ev_minus_4_regs[] =
{
0x0310,
0x40D0,
};

static const unsigned short sr030pc50_ev_minus_3_regs[] =
{
0x0310,
0x40B0,
};

static const unsigned short sr030pc50_ev_minus_2_regs[] =
{
 0x0310,
 0x40A0,

};

static const unsigned short sr030pc50_ev_minus_1_regs[] =
{
 0x0310,
 0x4090,
};

static const unsigned short sr030pc50_ev_default_regs[] =
{
 0x0310,
 0x4080,

};

static const unsigned short sr030pc50_ev_plus_1_regs[] =
{
 0x0310,
 0x4010,

};

static const unsigned short sr030pc50_ev_plus_2_regs[] =
{
 0x0310,
 0x4020,

};

static const unsigned short sr030pc50_ev_plus_3_regs[] =
{
 0x0310,
 0x4030,
};

static const unsigned short sr030pc50_ev_plus_4_regs[] =
{
 0x0310,
 0x4050,
};
// ==========================================================
// BRIGHTNES(9) - END
//==========================================================

//==========================================================
// contrast(5) - START
//==========================================================

static const unsigned short sr030pc50_contrast_minus_2_regs[] =
{
0x0310,
0x48a0,
};

static const unsigned short sr030pc50_contrast_minus_1_regs[] =
{
0x0310,
0x4890,
};

static const unsigned short sr030pc50_contrast_default_regs[] =
{
0x0310,
0x4885,
};

static const unsigned short sr030pc50_contrast_plus_1_regs[] =
{
0x0310,
0x4870,
};

static const unsigned short sr030pc50_contrast_plus_2_regs[] =
{
0x0310,
0x4860,
};

static const unsigned short sr030pc50_fps_auto_regs[] =
{
  0xFF00,  //END
};

static const unsigned short sr030pc50_fps_auto_normal_regs[] =
{
0x0300,
0x0171, //sleep

0x0300,
0x1190, //fixed fps disable
0x4200, //Vblank 129
0x4381,

0x900f, //BLC_TIME_TH_ON
0x910f, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320,
0x100c, //ae off
0x2af0, //antibanding
0x2bf4,
0x30f8,

0x8802, //EXP Max(120Hz) 8.00 fps 
0x89dc, 
0x8a6c,
0xa002, //EXP Max(100Hz) 8.33 fps 
0xa1bf, 
0xa220,

0x0320,
0x108c, //ae on

0x0300,
0x0170,

0xff28, //delay 400ms
};

static const unsigned short sr030pc50_fps_auto_dark_regs[] =
{
0x0300,
0x0171, //sleep

0x0300,
0x1190, //fixed fps disable
0x4200, //Vblank 129
0x4381,

0x900f, //BLC_TIME_TH_ON
0x910f, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320,
0x100c, //ae off
0x2af0, //antibanding
0x2bf4,
0x30f8,

0x8302, //exp normal 8.00fps
0x84dc,
0x856c,

0x8802, //EXP Max(120Hz) 8.00 fps 
0x89dc, 
0x8a6c, 
0xa002, //EXP Max(100Hz) 8.33 fps 
0xa1bf,
0xa220,

0x0320,
0x108c, //ae on

0x0300,
0x0170,

0xff28, //delay 400ms
};

//==========================================================
//FPS()
//==========================================================

static const unsigned short sr030pc50_fps_5_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 20
0x4314,

0x9017, //BLC_TIME_TH_ON
0x9117, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8804, //EXP Max(120Hz) 5.22 fps
0x8963,
0x8a0c,


0x9104, //EXP Fix 5.00 fps
0x9293,
0x93e0,

0x108c,
0x0300,
0x1194,

0x0300,
0x5000,
};
static const unsigned short sr030pc50_fps_7_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 20
0x4314,

0x9011, //BLC_TIME_TH_ON
0x9111, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8803, //EXP Max(120Hz) 7.06 fps
0x893e,
0x8a14,

0x9103, //EXP Fix 7.00 fps
0x9244,
0x93b8,

0x108c,
0x0300,
0x1194,

0x0300,
0x5000,
};

static const unsigned short sr030pc50_fps_10_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 20
0x4314,

0x900b, //BLC_TIME_TH_ON
0x910b, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8802, //EXP Max(120Hz) 10.91 fps
0x8919,
0x8a1c,

0x9102, //EXP Fix 10.00 fps
0x9249,
0x93f0,

0x108c,
0x0300,
0x1194,

0x0300,
0x5000,
};

static const unsigned short sr030pc50_fps_15_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 20
0x4314,

0x9007, //BLC_TIME_TH_ON
0x9107, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8801, //EXP Max(120Hz) 17.14 fps
0x8955,
0x8acc,

0x9101, //EXP Fix 15.00 fps
0x9286,
0x93a0,

0x108c,
0x0300,
0x1194,

0x0300,
0x5000,
};

static const unsigned short sr030pc50_fps_20_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 20
0x4314,

0x9005, //BLC_TIME_TH_ON
0x9105, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF


0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8800, //EXP Max(120Hz) 24.00 fps
0x89f4,
0x8a24,


0x9101, //EXP Fix 20.00 fps
0x9224,
0x93f8,

0x108c,
0x0300,
0x1194,

0x0300,
0x5000,
};
static const unsigned short sr030pc50_fps_25_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 20
0x4314,

0x9004, //BLC_TIME_TH_ON
0x9104, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP Normal 30.00 fps
0x84c3,
0x8550,

0x8800, //EXP Max(120Hz) 30.00 fps
0x89c3,
0x8a50,


0x9100, //EXP Fix 24.00 fps
0x92f4,
0x9324,

0x108c,
0x0300,
0x1194,
};

static const unsigned short sr030pc50_fps_30_regs[] =
{
0x0300,
0x0171,
0x1190,
0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 2
0x4302,

0x9003, //BLC_TIME_TH_ON
0x9103, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x0320, //Page 20
0x100c,

0x2a90,
0x2bf5,
0x30f8,

0x8300, //EXP normal 40.00 fps
0x8492,
0x857c,

0x8800, //EXP Max(120Hz) 40.00 fps
0x8992,
0x8a7c,

0x9100, //EXP Fix 30.00 fps
0x92c3,
0x9350,

0x108c,
0x0300,
0x1194,

0x0300,
0x5000,
};

//==========================================================
//AE - START
//==========================================================

static const unsigned short sr030pc50_ae_lock_regs[] =
{
  0xFF00,  //END
};
static const unsigned short sr030pc50_ae_unlock_regs[] =
{
  0xFF00,  //END
};

//==========================================================
//AE - END
//==========================================================



//==========================================================
//AWB - START
//==========================================================

static const unsigned short sr030pc50_awb_lock_regs[] =
{
  0xFF00,  //END
};
static const unsigned short sr030pc50_awb_unlock_regs[] =
{
  0xFF00,  //END
};

//==========================================================
//AWB - END
//==========================================================

// Frame Fixed
static const unsigned short sr030pc50_vt_mode_regs[] =
{
  
0x0171,
0x0173,
0x0171,
0x0300,
0x080f,
0x1000,
0x1194,
0x1200,
0x1488,
0x0baa,
0x0caa,
0x0daa,
0xc095,
0xc118,
0xc291,
0xc300,
0xc401,
0x0320,
0x100c,
0x0322,
0x107b,
0x0300,
0x1200,
0x2000,
0x2104,
0x2200,
0x2304,
0x4000, //Hblank 144
0x4190,
0x4206, //Vblank 20
0x435c,

//BLC
0x802e,
0x817e,
0x8290,
0x8330,
0x842c,
0x854b,
0x8601,
0x8847,
0x9011,
0x9111,
0x92f0,
0x93e8,
0x9838,
0x9940,
0xa040,
0xa842,
0x0302,
0x1000,
0x1100,
0x1340,
0x1404,
0x181c,
0x1900,
0x1a00,
0x1b08,
0x1c9c,
0x1d03,
0x2033,
0x2177,
0x22a7,
0x2332,
0x2433,
0x2b40,
0x2d32,
0x3199,
0x3200,
0x3300,
0x343c,
0x350d,
0x3b60,
0x5021,
0x511c,
0x52aa,
0x535a,
0x5430,
0x5510,
0x560c,
0x5800,
0x590f,
0x6034,
0x613a,
0x6234,
0x6339,
0x6434,
0x6539,
0x7235,
0x7338,
0x7435,
0x7538,
0x8002,
0x812e,
0x820d,
0x8310,
0x840d,
0x8510,
0x921d,
0x9320,
0x941d,
0x9520,
0xa003,
0xa12d,
0xa42d,
0xa503,
0xa812,
0xa91b,
0xaa22,
0xab2b,
0xac10,
0xad0e,
0xb833,
0xb935,
0xbc0c,
0xbd0e,
0xc03a,
0xc13f,
0xc23a,
0xc33f,
0xc43a,
0xc53e,
0xc63a,
0xc73e,
0xc83a,
0xc93e,
0xca3a,
0xcb3e,
0xcc3b,
0xcd3d,
0xce3b,
0xcf3d,
0xd033,
0xd13f,
0x0310,
0x1001,
0x1143,
0x1230,
0x4080,
0x4120, //DYOFS 02
0x4880,
0x50f0,
0x6001,
0x6100,
0x6280,
0x6380,
0x6448,
0x6690,
0x6736,
0x8000,
0x0311,
0x1025,
0x1107,
0x2000,
0x2160,
0x230a,
0x6013,
0x6185,
0x6200,
0x6300,
0x6400,
0x6770,
0x6824,
0x6904,
0x0312,
0x40d3,
0x4109,
0x5018,
0x5124,
0x701f,
0x7100,
0x7200,
0x7300,
0x7412,
0x7512,
0x7620,
0x7780,
0x7888,
0x7918,
0x903d,
0x9134,
0x9928,
0x9c05,
0x9d08,
0x9e28,
0x9f28,
0xb07d,
0xb544,
0xb682,
0xb752,
0xb844,
0xb915,
0x0313,
0x1001,
0x1189,
0x1214,
0x1319,
0x1408,
0x2003,
0x2104,
0x2325,
0x2421,
0x2508,
0x2640,
0x2700,
0x2808,
0x2950,
0x2ae0,
0x2b10,
0x2c28,
0x2d40,
0x2e00,
0x2f00,
0x3011,
0x8005,
0x8107,
0x9004,
0x9105,
0x9200,
0x9330,
0x9430,
0x9510,
0x0314,
0x1001,
0x2254,
0x2341,
0x2440,
0x2760,
0x2880,
0x2960,
0x2a80,
0x2b60,
0x2c80,
0x0315,
0x1003,
0x1452,
0x163a,
0x172f,
0x30f1,
0x3171,
0x3200,
0x331f,
0x34e1,
0x3542,
0x3601,
0x3731,
0x3872,
0x4090,
0x4182,
0x4212,
0x4386,
0x4492,
0x4518,
0x4684,
0x4702,
0x4802,
0x0316,
0x1001,
0x3000,
0x310c,
0x3220,
0x3335,
0x3458,
0x3575,
0x368e,
0x37a3,
0x38b4,
0x39c3,
0x3acf,
0x3be2,
0x3cf0,
0x3df9,
0x3eff,
0x0317,
0xc43c,
0xc532,
0x0320,
0x100c,
0x1104,
0x2001,
0x2827,
0x29a1,
0x2a90,
0x2bf5,
0x2c2b,

0x30f8,
0x3b22,
0x3cde,
0x3922,
0x3ade,
0x3b22,
0x3cde,
0x6070,
0x6110,
0x6270,
0x6310,
0x6830,
0x696a,
0x6A27,
0x6Bbb,
0x7042,
0x7622,
0x7781,
0x7822,
0x7927,
0x7a23,
0x7c17,
0x7d22,
0x8300,
0x84c3,
0x8550,
0x8600,
0x87c8,
0x8803, //EXP Max(120Hz) 7.06 fps
0x893e,
0x8a14,
0x8B3a,
0x8C98,
0x8D30,
0x8Ed4,
0x9103, //EXP Fix 7.00 fps
0x9244,
0x93b8,
0x988C,
0x9923,
0x9c05,
0x9d78,
0x9e00,
0x9fc8,
0xb01d,
0xb114,
0xb2f8,
0xb317,
0xb417,
0xb53e,
0xb62b,
0xb724,
0xb821,
0xb91f,
0xba1e,
0xbb1d,
0xbc1c,
0xbd1b,
0xc01a,
0xc348,
0xc448,
0x0322,
0x10e2,
0x112e,
0x2041,
0x2140,
0x24fe,
0x3080,
0x3180,
0x3812,
0x3933,
0x40f3,
0x4143,
0x4233,
0x43f3,
0x4444,
0x4566,
0x4608,
0x4763,
0x803d,
0x8120,
0x8240,
0x835a,
0x8422,
0x8557,
0x8624,
0x8741,
0x8833,
0x893e,
0x8a34,
0x8b03,
0x8d22,
0x8e21,
0x8f58,
0x9056,
0x9153,
0x924e,
0x9347,
0x9441,
0x953b,
0x9633,
0x972f,
0x982b,
0x9929,
0x9a27,
0x9b06,
0x0348,
0x1005,
0x1100,
0x1200,
0x16c4,
0x1700,
0x1900,
0x1a00,
0x1c02,
0x1d04,
0x1e07,
0x1f06,
0x2000,
0x21b8,
0x2200,
0x2301,
0x3005,
0x3100,
0x3402,
0x3206,
0x3503,
0x3601,
0x3703,
0x3800,
0x394a,
0x3c00,
0x3dfa,
0x3f10,
0x4000,
0x4120,
0x4200,
0x0322,
0x10fb,
0x0320,
0x108c,
0x0170,  

};

static const unsigned short sr030pc50_init_regs_smart_stay[] = {

0x0300,
0x0171,
0x0173,
0x0171,


// PLL Setting
0x0300,

0x080f, //Parallel NO Output_PAD Out
0x1000,
0x1190,
0x1200,
0x1488,

0x0baa,
0x0caa,
0x0daa,

0xc095,
0xc118,
0xc291,
0xc300,
0xc401,

0x0320, //page 20
0x100c, //ae off
0x0322, //page 22
0x107b, //awb off

0x0300,
0x1200,
0x2000,
0x2104,
0x2200,
0x2304,

0x4000, //Hblank 144
0x4190,
0x4200, //Vblank 129
0x4381,

//BLC
0x802e, //don't touch
0x817e, //don't touch
0x8290, //don't touch
0x8330, //don't touch
0x842c, //don't touch
0x854b, //don't touch
0x8601, //don't touch
0x8847, //don't touch
//0x8948,//BLC hold

0x900f, //BLC_TIME_TH_ON
0x910f, //BLC_TIME_TH_OFF
0x9298, //BLC_AG_TH_ON
0x9390, //BLC_AG_TH_OFF

0x9838, //don't touch
0x9940, //Out BLC
0xa040, //Dark BLC
0xa844, //Normal BLC 44

//Page2 Last Update 12_01_20
0x0302,
0x1000,
0x1100,
0x1340,
0x1404,
0x181c,
0x1900, //01
0x1a00,
0x1b08,
0x1c9c,
0x1d03,
0x2033,
0x2177,
0x22a7,
0x2332,
0x2433,
0x2b40,
0x2d32,
0x3199,
0x3200,
0x3300,
0x343c,
0x350d,
0x3b60, //80

//timing control 1 // //don't touch
0x5021,
0x511c,
0x52aa,
0x535a,
0x5430,
0x5510,
0x560c,
0x5800,
0x590f,

//timing control 2 // //don't touch
0x6034,
0x613a,
0x6234,
0x6339,
0x6434,
0x6539,
0x7235,
0x7338,
0x7435,
0x7538,
0x8002,
0x812e,
0x820d,
0x8310,
0x840d,
0x8510,
0x921d,
0x9320,
0x941d,
0x9520,
0xa003,
0xa12d,
0xa42d,
0xa503,
0xa812,
0xa91b,
0xaa22,
0xab2b,
0xac10,
0xad0e,
0xb833,
0xb935,
0xbc0c,
0xbd0e,
0xc03a,
0xc13f,
0xc23a,
0xc33f,
0xc43a,
0xc53e,
0xc63a,
0xc73e,
0xc83a,
0xc93e,
0xca3a,
0xcb3e,
0xcc3b,
0xcd3d,
0xce3b,
0xcf3d,
0xd033,
0xd13f,

//Page 10
0x0310,
0x1001, //03, //ISPCTL1, YUV ORDER(FIX)
0x1143,
0x1230, //Y offet, dy offseet enable
0x4080,
0x4102, //00 DYOFS  00->10  _100318
0x4885, //Contrast  88->84  _100318
0x5048, //AGBRT

0x6001, //7f //7c
0x6100, //Use default
0x627e, //80 SATB_B  (1.4x)
0x637c, //80 SATR_R  (1.2x)
0x6448, //AGSAT
0x6690, //wht_th2
0x6736, //wht_gain  Dark (0.4x), Normal (0.75x)

0x8000,

//Page 11
//LPF
0x0311,
0x1025,
0x1107,
0x2000,
0x2160,
0x230a,
0x6013,
0x6185,
0x6200,
0x6300,
0x6400,

0x6770,
0x6824,
0x6904,

//Page 12
//2D
0x0312,
0x40d3,
0x4109,
0x5018,
0x5124,
0x701f,
0x7100,
0x7200,
0x7300,
0x7412,
0x7512,
0x7620,
0x7780,
0x7888,
0x7918,

///////////////////////
0x903d,
0x9134,
0x9928,
0x9c05, //14 For defect
0x9d08, //15 For defect
0x9e28,
0x9f28,

0xb07d, //75 White Defect
0xb544,
0xb682,
0xb752,
0xb844,
0xb915,
///////////////////////

//Edge
0x0313,
0x1001,
0x1189,
0x1214,
0x1319,
0x1408,
0x2003,
0x2104,
0x2325,
0x2421,
0x2508,
0x2640,
0x2700,
0x2808,
0x2950,
0x2ae0,
0x2b10,
0x2c28,
0x2d40,
0x2e00,
0x2f00,
0x3011,
0x8005,
0x8107,
0x9004,
0x9105,
0x9200,
0x9330,
0x9430,
0x9510,

0x0314,
0x1001,

0x2244,  //54,
0x2334,  //41,
0x2432,  //40, //30

0x2758, //60 RXCEN
0x2880, //RYCEN
0x2958, //60 GXCEN
0x2a80, //GYCEN
0x2b58, //60 BXCEN
0x2c80, //BYCEN

//15page//////////////////////////
0x0315,
0x1003,

0x1452,
0x163a,
0x172f,

//CMC
0x30f1,
0x3171,
0x3200,
0x331f,
0x34e1,
0x3542,
0x3601,
0x3731,
0x3872,
//CMC OFS
0x4090,
0x4182,
0x4212,
0x4386,
0x4492,
0x4518,
0x4684,
0x4702,
0x4802,

0x0316,
0x1001,
0x3000,
0x310d,
0x321b,
0x332b,  //2d,
0x3452,  //4e,
0x3576,  //6f,
0x368e,  //86,
0x37a3,  //9d,
0x38b7,  //af,
0x39c7,  //c0,
0x3ad5,  //cf,
0x3be7,  //e2,
0x3cf3,  //f0,
0x3df9,
0x3eff,

//Page 17 AE
0x0317,
0xc43c,
0xc532,

0x0320,
0x100c,
0x1104,
0x2001,
0x2827,
0x29a1,
0x2af0,
0x2b04,
0x2c2b,
0x2e23,
0x30f8,
0x3b22,
0x3cde,
0x3922,
0x3ade,
0x3b22,
0x3cde,
0x6040,
0x6131,
0x6240,
0x6331,
0x6826,
0x6976,
0x6A3c,
0x6Bb4,
0x7048,
0x7621,
0x7791,
0x7822,
0x792b,
0x7a23,
0x7c24,
0x7d22,
0x8300,
0x8461,
0x85a8,
0x8600,
0x87c8,
0x8801,
0x8986,
0x8aa0,
0x8B7e,
0x8C90,
0x8D61,
0x8Ea8,
0x988C,
0x9923,
0x9c06,
0x9d40,
0x9e00,
0x9fc8,
0xb038,
0xb138,
0xb2ff,
0xb340,
0xb440,
0xb570,
0xb62b,
0xb724,
0xb821,
0xb91f,
0xba1e,
0xbb1d,
0xbc1c,
0xbd1b,
0xc01a,
0xc348,
0xc448,

//Page 22 AWB
0x0322,
0x10e2,
0x112e, //2e
0x2075, //01 //69
0x2180,
0x24fe,

0x3080, //Cb
0x3180, //Cr
0x3811, //Lock Boundary //13
0x3933,
0x40e8, //f0
0x4143,
0x4233,
0x43f3,
0x4444,
0x4566,
0x4608,
0x4763,

0x803d, //3a //3c
0x8120,
0x8240,

0x835a, //RMAX 5a
0x8420, //22 RMIN 23
0x8556, //BMAX 5a
0x8624, //BMIN

0x8748, //42
0x883a,
0x893e,
0x8a34,

0x8b00, //OUT TH
0x8d24, //21 //22
0x8e41, //41

0x8f63,
0x9062,
0x915e,
0x925a,
0x9350,
0x9442,
0x953b,
0x9634,
0x972d,
0x982b,
0x9929,
0x9a27,
0x9b0b,
0xb4d5,

///////////////////////////// Page 48
0x0348,

0x1005,
0x1100, //async_fifo off
0x1200,

0x16c4,
0x1700,
0x1900,
0x1a00,
0x1c02,
0x1d04,
0x1e07,
0x1f06,
0x2000,
0x21b8,
0x2200,
0x2301,

0x3005,
0x3100,
0x3402,
0x3206,
0x3503,
0x3601,
0x3703,
0x3800,
0x394a,
0x3c00,
0x3dfa,
0x3f10,
0x4000,
0x4120,
0x4200,

0x0322,
0x10fb,

0x0320,
0x108c,
};

static const unsigned short sr030pc50_640_480_size_regs[] =
{
0x0300,
0x2000,
0x2104,
0x2200,
0x2304,

0x2401,
0x25e0, //480
0x2602,
0x2780, //640

0x0348,
0x3005,
0x3100,
};

static const unsigned short sr030pc50_528_432_size_regs[] =
{
0x0300,
0x2000,
0x211c,
0x2200,
0x233c,

0x2401,
0x25b0, //432
0x2602,
0x2710, //528

0x0348,
0x3004,
0x3120,
};

#endif /* __EOSCAMERASR030PC50_REG_H */
