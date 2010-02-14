/*
This is LIRC, Linux Infra-red Remote Control support for CIR Port of chips 
"NCT677x" / "W836x7" of Nuvoton Technology Co. (or Winbond in old time).

It was produced by Nuvoton PS00 group. Website : http://www.nuvoton.com

The whole thing is licensed under the GPL v2 or later.

    This package is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this package; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

Adapted for usage in GeeXboX by the GeeXboX team.
*/

#define IR_IOC_MAGIC           'i'

/* output chip registers for debug */
#define IR_DUMPCIRREG    _IO(IR_IOC_MAGIC, 0x80)
#define IR_DUMPWAKEREG   _IO(IR_IOC_MAGIC, 0x81)


/* study wake key on MCE CIR controller */
#define IR_IOLEARNWAKE   _IO(IR_IOC_MAGIC, 0x82)

/* disable cir wake */
#define IR_IOUNSETWAKE   _IO(IR_IOC_MAGIC, 0x83)

/* enable cir wake */
#define IR_IOSETWAKE     _IO(IR_IOC_MAGIC, 0x84)


/* get carrier for study ir signal */
#define IR_IOGETCARRIER  _IOR(IR_IOC_MAGIC, 0x85, unsigned int)

/* set carrier for send ir signal */
#define IR_IOSETCARRIER  _IOW(IR_IOC_MAGIC, 0x86, unsigned int)

/* start/stop study key */
#define IR_IOSTARTSTUDY  _IO(IR_IOC_MAGIC, 0x88)
#define IR_IOSTOPSTUDY   _IO(IR_IOC_MAGIC, 0x89)

/* study key buf len */
#define IR_IOSTUDYLEN    _IOR(IR_IOC_MAGIC, 0x8A, unsigned int)

/* study key buf data */
#define IR_IOSTUDYBUF    _IOR(IR_IOC_MAGIC, 0x8B, unsigned char)

/* restudy key buf data */
#define IR_IORESTUDYBUF  _IO(IR_IOC_MAGIC, 0x8C)


/* clean data */
#define IR_IOCLEANDATA   _IO(IR_IOC_MAGIC, 0x8E)

/* chip id string, at most 7 characters */
char *chip_id = "w677hga";

/* check chip identification for the applications which are related to hardware */
#define IR_CHECKCHIP     _IOR(IR_IOC_MAGIC, 0x8F, unsigned long long)

