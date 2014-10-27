#!/bin/haserl
Content-type: text/html

<html xml:lang="en" xmlns="http://www.w3.org/1999/xhtml" lang="en">   
<%in funcs.cgi %>
<%

get_tz_sel(){
    if [ "$CUR_TZNUM" = "$1" ] ; then
        echo -n "selected"
    fi
}

get_year(){
    echo -n ${NOW:0:4}
}

get_month(){
    echo -n ${NOW:5:2}
}

get_day(){
    echo -n ${NOW:8:2}
}

get_hour(){
    echo -n ${NOW:11:2}
}

get_min(){
    echo -n ${NOW:14:2}
}

get_sec(){
    echo -n ${NOW:17:2}
}

set_tz(){
    case "$1" in
 	 1)
	tzposix=GMT0
	;;
	 2)
	tzposix=GMT0
	;;
	 3)
	tzposix=EAT-3
	;;
	 4)
	tzposix=CET-1
	;;
	 5)
	tzposix=EAT-3
	;;
	 6)
	tzposix=GMT0
	;;
	 7)
	tzposix=WAT-1
	;;
	 8)
	tzposix=GMT0
	;;
	 9)
	tzposix=GMT0
	;;
	 10)
	tzposix=CAT-2
	;;
	 11)
	tzposix=WAT-1
	;;
	 12)
	tzposix=CAT-2
	;;
	 13)
	tzposix=WET0
	;;
	 14)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 15)
	tzposix=GMT0
	;;
	 16)
	tzposix=GMT0
	;;
	 17)
	tzposix=EAT-3
	;;
	 18)
	tzposix=EAT-3
	;;
	 19)
	tzposix=WAT-1
	;;
	 20)
	tzposix=WET0
	;;
	 21)
	tzposix=GMT0
	;;
	 22)
	tzposix=CAT-2
	;;
	 23)
	tzposix=CAT-2
	;;
	 24)
	tzposix=SAST-2
	;;
	 25)
	tzposix=EAT-3
	;;
	 26)
	tzposix=EAT-3
	;;
	 27)
	tzposix=CAT-2
	;;
	 28)
	tzposix=WAT-1
	;;
	 29)
	tzposix=WAT-1
	;;
	 30)
	tzposix=WAT-1
	;;
	 31)
	tzposix=GMT0
	;;
	 32)
	tzposix=WAT-1
	;;
	 33)
	tzposix=CAT-2
	;;
	 34)
	tzposix=CAT-2
	;;
	 35)
	tzposix=WAT-1
	;;
	 36)
	tzposix=CAT-2
	;;
	 37)
	tzposix=SAST-2
	;;
	 38)
	tzposix=SAST-2
	;;
	 39)
	tzposix=EAT-3
	;;
	 40)
	tzposix=GMT0
	;;
	 41)
	tzposix=EAT-3
	;;
	 42)
	tzposix=WAT-1
	;;
	 43)
	tzposix=WAT-1
	;;
	 44)
	tzposix=GMT0
	;;
	 45)
	tzposix=GMT0
	;;
	 46)
	tzposix=WAT-1
	;;
	 47)
	tzposix=GMT0
	;;
	 48)
	tzposix=EET-2
	;;
	 49)
	tzposix=CET-1
	;;
	 50)
	tzposix=WAT-1WAST,M9.1.0,M4.1.0
	;;
	 51)
	tzposix=HAST10HADT,M3.2.0,M11.1.0
	;;
	 52)
	tzposix=AKST9AKDT,M3.2.0,M11.1.0
	;;
	 53)
	tzposix=AST4
	;;
	 54)
	tzposix=AST4
	;;
	 55)
	tzposix=BRT3
	;;
	 56)
	tzposix=ART3
	;;
	 57)
	tzposix=ART3
	;;
	 58)
	tzposix=ART3
	;;
	 59)
	tzposix=ART3
	;;
	 60)
	tzposix=ART3
	;;
	 61)
	tzposix=ART3
	;;
	 62)
	tzposix=ART3
	;;
	 63)
	tzposix=ART3
	;;
	 64)
	tzposix=ART3
	;;
	 65)
	tzposix=ART3
	;;
	 66)
	tzposix=ART3
	;;
	 67)
	tzposix=AST4
	;;
	 68)
	tzposix=PYT4PYST,M10.1.0/0,M4.2.0/0
	;;
	 69)
	tzposix=EST5
	;;
	 70)
	tzposix=BRT3
	;;
	 71)
	tzposix=AST4
	;;
	 72)
	tzposix=BRT3
	;;
	 73)
	tzposix=CST6
	;;
	 74)
	tzposix=AST4
	;;
	 75)
	tzposix=AMT4
	;;
	 76)
	tzposix=COT5
	;;
	 77)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 78)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 79)
	tzposix=AMT4AMST,M10.3.0/0,M2.3.0/0
	;;
	 80)
	tzposix=CST6CDT,M4.1.0,M10.5.0
	;;
	 81)
	tzposix=VET4:30
	;;
	 82)
	tzposix=GFT3
	;;
	 83)
	tzposix=EST5
	;;
	 84)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 85)
	tzposix=MST7MDT,M4.1.0,M10.5.0
	;;
	 86)
	tzposix=CST6
	;;
	 87)
	tzposix=AMT4AMST,M10.3.0/0,M2.3.0/0
	;;
	 88)
	tzposix=AST4
	;;
	 89)
	tzposix=GMT0
	;;
	 90)
	tzposix=PST8PDT,M3.2.0,M11.1.0
	;;
	 91)
	tzposix=MST7
	;;
	 92)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 93)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 94)
	tzposix=AST4
	;;
	 95)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 96)
	tzposix=AMT4
	;;
	 97)
	tzposix=CST6
	;;
	 98)
	tzposix=BRT3
	;;
	 99)
	tzposix=AST4ADT,M3.2.0,M11.1.0
	;;
	 100)
	tzposix=AST4ADT,M3.2.0/0:01,M11.1.0/0:01
	;;
	 101)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 102)
	tzposix=AST4
	;;
	 103)
	tzposix=AST4
	;;
	 104)
	tzposix=CST6
	;;
	 105)
	tzposix=ECT5
	;;
	 106)
	tzposix=GYT4
	;;
	 107)
	tzposix=AST4ADT,M3.2.0,M11.1.0
	;;
	 108)
	tzposix=CST5CDT,M3.2.0/0,M10.5.0/1
	;;
	 109)
	tzposix=MST7
	;;
	 110)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 111)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 112)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 113)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 114)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 115)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 116)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 117)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 118)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 119)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 120)
	tzposix=EST5
	;;
	 121)
	tzposix=AKST9AKDT,M3.2.0,M11.1.0
	;;
	 122)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 123)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 124)
	tzposix=BOT4
	;;
	 125)
	tzposix=PET5
	;;
	 126)
	tzposix=PST8PDT,M3.2.0,M11.1.0
	;;
	 127)
	tzposix=BRT3
	;;
	 128)
	tzposix=CST6
	;;
	 129)
	tzposix=AMT4
	;;
	 130)
	tzposix=AST4
	;;
	 131)
	tzposix=AST4
	;;
	 132)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 133)
	tzposix=MST7MDT,M4.1.0,M10.5.0
	;;
	 134)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 135)
	tzposix=CST6CDT,M4.1.0,M10.5.0
	;;
	 136)
	tzposix=CST6CDT,M4.1.0,M10.5.0
	;;
	 137)
	tzposix=PMST3PMDT,M3.2.0,M11.1.0
	;;
	 138)
	tzposix=AST4ADT,M3.2.0,M11.1.0
	;;
	 139)
	tzposix=CST6CDT,M4.1.0,M10.5.0
	;;
	 140)
	tzposix=UYT3UYST,M10.1.0,M3.2.0
	;;
	 141)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 142)
	tzposix=AST4
	;;
	 143)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 144)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 145)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 146)
	tzposix=AKST9AKDT,M3.2.0,M11.1.0
	;;
	 147)
	tzposix=FNT2
	;;
	 148)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 149)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 150)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 151)
	tzposix=EST5
	;;
	 152)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 153)
	tzposix=SRT3
	;;
	 154)
	tzposix=MST7
	;;
	 155)
	tzposix=AST4
	;;
	 156)
	tzposix=EST5
	;;
	 157)
	tzposix=AMT4
	;;
	 158)
	tzposix=AST4
	;;
	 159)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 160)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 161)
	tzposix=BRT3
	;;
	 162)
	tzposix=CST6
	;;
	 163)
	tzposix=AMT4
	;;
	 164)
	tzposix=PST8PDT,M4.1.0,M10.5.0
	;;
	 165)
	tzposix=BRT3
	;;
	 166)
	tzposix=AST4
	;;
	 167)
	tzposix=BRT3BRST,M10.3.0/0,M2.3.0/0
	;;
	 168)
	tzposix=EGT1EGST,M3.5.0/0,M10.5.0/1
	;;
	 169)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 170)
	tzposix=AST4
	;;
	 171)
	tzposix=NST3:30NDT,M3.2.0/0:01,M11.1.0/0:01
	;;
	 172)
	tzposix=AST4
	;;
	 173)
	tzposix=AST4
	;;
	 174)
	tzposix=AST4
	;;
	 175)
	tzposix=AST4
	;;
	 176)
	tzposix=CST6
	;;
	 177)
	tzposix=CST6
	;;
	 178)
	tzposix=AST4ADT,M3.2.0,M11.1.0
	;;
	 179)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 180)
	tzposix=PST8PDT,M3.2.0,M11.1.0
	;;
	 181)
	tzposix=EST5EDT,M3.2.0,M11.1.0
	;;
	 182)
	tzposix=AST4
	;;
	 183)
	tzposix=PST8PDT,M3.2.0,M11.1.0
	;;
	 184)
	tzposix=PST8PDT,M3.2.0,M11.1.0
	;;
	 185)
	tzposix=CST6CDT,M3.2.0,M11.1.0
	;;
	 186)
	tzposix=AKST9AKDT,M3.2.0,M11.1.0
	;;
	 187)
	tzposix=MST7MDT,M3.2.0,M11.1.0
	;;
	 188)
	tzposix=WST-8
	;;
	 189)
	tzposix=DAVT-7
	;;
	 190)
	tzposix=DDUT-10
	;;
	 191)
	tzposix=MIST-11
	;;
	 192)
	tzposix=MAWT-5
	;;
	 193)
	tzposix=NZST-12NZDT,M9.5.0,M4.1.0/3
	;;
	 194)
	tzposix=ROTT3
	;;
	 195)
	tzposix=NZST-12NZDT,M9.5.0,M4.1.0/3
	;;
	 196)
	tzposix=SYOT-3
	;;
	 197)
	tzposix=VOST-6
	;;
	 198)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 199)
	tzposix=AST-3
	;;
	 200)
	tzposix=ALMT-6
	;;
	 201)
	tzposix=ANAT-11ANAST,M3.5.0,M10.5.0/3
	;;
	 202)
	tzposix=AQTT-5
	;;
	 203)
	tzposix=AQTT-5
	;;
	 204)
	tzposix=TMT-5
	;;
	 205)
	tzposix=AST-3
	;;
	 206)
	tzposix=AST-3
	;;
	 207)
	tzposix=AZT-4AZST,M3.5.0/4,M10.5.0/5
	;;
	 208)
	tzposix=ICT-7
	;;
	 209)
	tzposix=EET-2EEST,M3.5.0/0,M10.5.0/0
	;;
	 210)
	tzposix=KGT-6
	;;
	 211)
	tzposix=BNT-8
	;;
	 212)
	tzposix=CHOT-8
	;;
	 213)
	tzposix=CST-8
	;;
	 214)
	tzposix=IST-5:30
	;;
	 215)
	tzposix=EET-2EEST,M4.1.5/0,M10.5.5/0
	;;
	 216)
	tzposix=BDT-6
	;;
	 217)
	tzposix=TLT-9
	;;
	 218)
	tzposix=GST-4
	;;
	 219)
	tzposix=TJT-5
	;;
	 220)
	tzposix=EET-2EEST,M3.5.6/0:01,M9.1.5
	;;
	 221)
	tzposix=CST-8
	;;
	 222)
	tzposix=ICT-7
	;;
	 223)
	tzposix=HKT-8
	;;
	 224)
	tzposix=HOVT-7
	;;
	 225)
	tzposix=IRKT-8IRKST,M3.5.0,M10.5.0/3
	;;
	 226)
	tzposix=WIT-7
	;;
	 227)
	tzposix=EIT-9
	;;
	 228)
	tzposix=AFT-4:30
	;;
	 229)
	tzposix=PETT-11PETST,M3.5.0,M10.5.0/3
	;;
	 230)
	tzposix=PKT-5
	;;
	 231)
	tzposix=CST-8
	;;
	 232)
	tzposix=NPT-5:45
	;;
	 233)
	tzposix=IST-5:30
	;;
	 234)
	tzposix=KRAT-7KRAST,M3.5.0,M10.5.0/3
	;;
	 235)
	tzposix=MYT-8
	;;
	 236)
	tzposix=MYT-8
	;;
	 237)
	tzposix=AST-3
	;;
	 238)
	tzposix=CST-8
	;;
	 239)
	tzposix=MAGT-11MAGST,M3.5.0,M10.5.0/3
	;;
	 240)
	tzposix=CIT-8
	;;
	 241)
	tzposix=PHT-8
	;;
	 242)
	tzposix=GST-4
	;;
	 243)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 244)
	tzposix=NOVT-6NOVST,M3.5.0,M10.5.0/3
	;;
	 245)
	tzposix=NOVT-6NOVST,M3.5.0,M10.5.0/3
	;;
	 246)
	tzposix=OMST-7
	;;
	 247)
	tzposix=ORAT-5
	;;
	 248)
	tzposix=ICT-7
	;;
	 249)
	tzposix=WIT-7
	;;
	 250)
	tzposix=KST-9
	;;
	 251)
	tzposix=AST-3
	;;
	 252)
	tzposix=QYZT-6
	;;
	 253)
	tzposix=MMT-6:30
	;;
	 254)
	tzposix=AST-3
	;;
	 255)
	tzposix=SAKT-10SAKST,M3.5.0,M10.5.0/3
	;;
	 256)
	tzposix=UZT-5
	;;
	 257)
	tzposix=KST-9
	;;
	 258)
	tzposix=CST-8
	;;
	 259)
	tzposix=SGT-8
	;;
	 260)
	tzposix=CST-8
	;;
	 261)
	tzposix=UZT-5
	;;
	 262)
	tzposix=GET-4
	;;
	 263)
	tzposix=BTT-6
	;;
	 264)
	tzposix=JST-9
	;;
	 265)
	tzposix=ULAT-8
	;;
	 266)
	tzposix=CST-8
	;;
	 267)
	tzposix=ICT-7
	;;
	 268)
	tzposix=VLAT-10VLAST,M3.5.0,M10.5.0/3
	;;
	 269)
	tzposix=YAKT-9YAKST,M3.5.0,M10.5.0/3
	;;
	 270)
	tzposix=YEKT-5YEKST,M3.5.0,M10.5.0/3
	;;
	 271)
	tzposix=AMT-4AMST,M3.5.0,M10.5.0/3
	;;
	 272)
	tzposix=AZOT1AZOST,M3.5.0/0,M10.5.0/1
	;;
	 273)
	tzposix=AST4ADT,M3.2.0,M11.1.0
	;;
	 274)
	tzposix=WET0WEST,M3.5.0/1,M10.5.0
	;;
	 275)
	tzposix=CVT1
	;;
	 276)
	tzposix=WET0WEST,M3.5.0/1,M10.5.0
	;;
	 277)
	tzposix=WET0WEST,M3.5.0/1,M10.5.0
	;;
	 278)
	tzposix=GMT0
	;;
	 279)
	tzposix=GST2
	;;
	 280)
	tzposix=GMT0
	;;
	 281)
	tzposix=FKT4FKST,M9.1.0,M4.3.0
	;;
	 282)
	tzposix=CST-9:30CST,M10.1.0,M4.1.0/3
	;;
	 283)
	tzposix=EST-10
	;;
	 284)
	tzposix=CST-9:30CST,M10.1.0,M4.1.0/3
	;;
	 285)
	tzposix=EST-10EST,M10.1.0,M4.1.0/3
	;;
	 286)
	tzposix=CST-9:30
	;;
	 287)
	tzposix=CWST-8:45
	;;
	 288)
	tzposix=EST-10EST,M10.1.0,M4.1.0/3
	;;
	 289)
	tzposix=EST-10
	;;
	 290)
	tzposix=LHST-10:30LHST-11,M10.1.0,M4.1.0
	;;
	 291)
	tzposix=EST-10EST,M10.1.0,M4.1.0/3
	;;
	 292)
	tzposix=WST-8
	;;
	 293)
	tzposix=EST-10EST,M10.1.0,M4.1.0/3
	;;
	 294)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 295)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 296)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 297)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 298)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 299)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 300)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 301)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 302)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 303)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 304)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 305)
	tzposix=GMT0IST,M3.5.0/1,M10.5.0
	;;
	 306)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 307)
	tzposix=GMT0BST,M3.5.0/1,M10.5.0
	;;
	 308)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 309)
	tzposix=GMT0BST,M3.5.0/1,M10.5.0
	;;
	 310)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 311)
	tzposix=GMT0BST,M3.5.0/1,M10.5.0
	;;
	 312)
	tzposix=EET-2EEST,M3.5.0,M10.5.0/3
	;;
	 313)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 314)
	tzposix=WET0WEST,M3.5.0/1,M10.5.0
	;;
	 315)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 0)
	tzposix=GMT0BST,M3.5.0/1,M10.5.0
	;;
	 317)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 318)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 319)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 320)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 321)
	tzposix=EET-2EEST,M3.5.0,M10.5.0/3
	;;
	 322)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 323)
	tzposix=MSK-4
	;;
	 324)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 325)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 326)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 327)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 328)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 329)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 330)
	tzposix=SAMT-3SAMST,M3.5.0,M10.5.0/3
	;;
	 331)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 332)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 333)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 334)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 335)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 336)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 337)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 338)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 339)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 340)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 341)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 342)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 343)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 344)
	tzposix=VOLT-3VOLST,M3.5.0,M10.5.0/3
	;;
	 345)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 346)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 347)
	tzposix=EET-2EEST,M3.5.0/3,M10.5.0/4
	;;
	 348)
	tzposix=CET-1CEST,M3.5.0,M10.5.0/3
	;;
	 349)
	tzposix=EAT-3
	;;
	 350)
	tzposix=IOT-6
	;;
	 351)
	tzposix=CXT-7
	;;
	 352)
	tzposix=CCT-6:30
	;;
	 353)
	tzposix=EAT-3
	;;
	 354)
	tzposix=TFT-5
	;;
	 355)
	tzposix=SCT-4
	;;
	 356)
	tzposix=MVT-5
	;;
	 357)
	tzposix=MUT-4
	;;
	 358)
	tzposix=EAT-3
	;;
	 359)
	tzposix=RET-4
	;;
	 360)
	tzposix=WST11
	;;
	 361)
	tzposix=NZST-12NZDT,M9.5.0,M4.1.0/3
	;;
	 362)
	tzposix=CHAST-12:45CHADT,M9.5.0/2:45,M4.1.0/3:45
	;;
	 363)
	tzposix=VUT-11
	;;
	 364)
	tzposix=PHOT-13
	;;
	 365)
	tzposix=TKT10
	;;
	 366)
	tzposix=FJT-12
	;;
	 367)
	tzposix=TVT-12
	;;
	 368)
	tzposix=GALT6
	;;
	 369)
	tzposix=GAMT9
	;;
	 370)
	tzposix=SBT-11
	;;
	 371)
	tzposix=ChST-10
	;;
	 372)
	tzposix=HST10
	;;
	 373)
	tzposix=HST10
	;;
	 374)
	tzposix=LINT-14
	;;
	 375)
	tzposix=KOST-11
	;;
	 376)
	tzposix=MHT-12
	;;
	 377)
	tzposix=MHT-12
	;;
	 378)
	tzposix=MART9:30
	;;
	 379)
	tzposix=SST11
	;;
	 380)
	tzposix=NRT-12
	;;
	 381)
	tzposix=NUT11
	;;
	 382)
	tzposix=NFT-11:30
	;;
	 383)
	tzposix=NCT-11
	;;
	 384)
	tzposix=SST11
	;;
	 385)
	tzposix=PWT-9
	;;
	 386)
	tzposix=PST8
	;;
	 387)
	tzposix=PONT-11
	;;
	 388)
	tzposix=PGT-10
	;;
	 389)
	tzposix=CKT10
	;;
	 390)
	tzposix=ChST-10
	;;
	 391)
	tzposix=TAHT10
	;;
	 392)
	tzposix=GILT-12
	;;
	 393)
	tzposix=TOT-13
	;;
	 394)
	tzposix=TRUT-10
	;;
	 395)
	tzposix=WAKT-12
	;;
	 396)
	tzposix=WFT-12
	;;
	 *)
	NOACTION=1
    esac
    
    if [ -n $NOACTION ] ; then
      set_config tznum "$1"
      set_config tzgeo "x"
      set_config tzposix $tzposix
      /usr/share/snake/settz 
    fi
   
}

if [ "${REQUEST_METHOD}" = "POST" ]
then
    USE_NTP=$(echo ${FORM_use_ntp} | cut -d ' ' -f 1)
    NTP_SERVER=$(echo ${FORM_ntp} | cut -d ' ' -f 1)
   	set_config ntp_enable ${USE_NTP:-0} 
    NEWTZ=$(echo ${FORM_tz} | cut -d ' ' -f 1)
    set_tz ${NEWTZ}
    if [ ${USE_NTP:-0} -eq 0 ] ; then
        YEAR=$(echo ${FORM_year} | cut -d ' ' -f 1)
        MONTH=$(echo ${FORM_month} | cut -d ' ' -f 1)
        DAY=$(echo ${FORM_day} | cut -d ' ' -f 1)
        HOUR=$(echo ${FORM_hour} | cut -d ' ' -f 1)
        MINUTE=$(echo ${FORM_minute} | cut -d ' ' -f 1)
        SECOND=$(echo ${FORM_second} | cut -d ' ' -f 1)
        date "$YEAR-$MONTH-$DAY $HOUR:$MINUTE:$SECOND" > /dev/null
    else
		set_config ntp_server $NTP_SERVER
        /usr/share/snake/setntp
    fi
fi
export TZ=$(cat /etc/TZ)
NOW=$(date -I'hours' -I'minutes' -I'seconds')
CUR_TZNUM=$(get_config tznum)

%>
<script language="JavaScript">
<!-- //
function checkNTP(){                                                           
    if ( <% get_use_ntp %> == 0 ){                                                                      
        document.forms[0].ntp.disabled = true;                             
    } 
    else{
        document.forms[0].year.readOnly = true;
        document.forms[0].month.readOnly = true;
        document.forms[0].day.readOnly = true;
        document.forms[0].hour.readOnly = true;
        document.forms[0].minute.readOnly = true;
        document.forms[0].second.readOnly = true;
    }                                                                          
}
function setReadNTP(obj){
    if(obj.checked)
    {
        document.forms[0].ntp.disabled = false;
        document.forms[0].year.readOnly = true;
        document.forms[0].month.readOnly = true;
        document.forms[0].day.readOnly = true;
        document.forms[0].hour.readOnly = true;
        document.forms[0].minute.readOnly = true;
        document.forms[0].second.readOnly = true;
        } 
    else {
        document.forms[0].ntp.disabled = true;
        document.forms[0].year.readOnly = false;
        document.forms[0].month.readOnly = false;
        document.forms[0].day.readOnly = false;
        document.forms[0].hour.readOnly = false;
        document.forms[0].minute.readOnly = false;
        document.forms[0].second.readOnly = false;
        }
} 

function daysInFebruary (year){
    return (((year % 4 == 0) && ( (!(year % 100 == 0)) || (year % 400 == 0))) ? 29 : 28 );
}
function DaysArray(n) {
	for (var i = 1; i <= n; i++) {
		this[i] = 31;
		if (i==4 || i==6 || i==9 || i==11) {
            this[i] = 30;
        }
		if (i==2) {
            this[i] = 29;
        }
   } 
   return this;
}

function isInteger(s){
	var i;
    for (i = 0; i < s.length; i++){   
        var c = s.charAt(i);
        if (((c < "0") || (c > "9"))) 
            return false;
    }
    return true;
}

function validateAction(form) {
    minyear = 2009;
    maxyear = 2037;
    var daysInMonth = DaysArray(12);
    year = form.year.value;
    month = form.month.value;
    day = form.day.value;
    hour = form.hour.value;
    minute = form.minute.value;
    second = form.second.value;
    if(form.use_ntp.checked){
        if ( form.ntp.value == ''){
            alert("The ntp server cannot be empty");
            return false;
        }
    }
    else
        {
        if ( isInteger(year) == false || year<minyear || year>maxyear){
    		alert("Please enter a valid year: from 2009 to 2037");
    		return false;
    	}
        if ( isInteger(month) == false || month<1 || month>12){
    		alert("Please enter a valid month");
    		return false;
    	}
        if ( isInteger(day) == false || day<1 || day>31 || (month==2 && day>daysInFebruary(year)) || day > daysInMonth[month]){
    		alert("Please enter a valid day");
    		return false;
    	}
        if ( isInteger(hour) == false || hour<0 || hour>23){
    		alert("Please enter a valid hour");
    		return false;
    	}
        if ( isInteger(minute) == false || minute<0 || minute>59){
    		alert("Please enter a valid minute");
    		return false;
    	}
        if ( isInteger(second) == false || second<0 || second>59){
    		alert("Please enter a valid second");
    		return false;
    	}
    }
    return true;
} 

// -->
</script>
</head>
<body onload="checkNTP();">   
<center>
<form action="<%= ${SCRIPT_NAME} %>" method="POST" onsubmit="return validateAction(this);">
<TABLE border="0">
<TR>
<TH>Select Timezone:</TH>
<TD>
<select name="tz">
<option value="1" <% get_tz_sel "1" %>>Africa/Abidjan</option>
<option value="2" <% get_tz_sel "2" %>>Africa/Accra</option>
<option value="3" <% get_tz_sel "3" %>>Africa/Addis Ababa</option>
<option value="4" <% get_tz_sel "4" %>>Africa/Algiers</option>
<option value="5" <% get_tz_sel "5" %>>Africa/Asmara</option>
<option value="6" <% get_tz_sel "6" %>>Africa/Bamako</option>
<option value="7" <% get_tz_sel "7" %>>Africa/Bangui</option>
<option value="8" <% get_tz_sel "8" %>>Africa/Banjul</option>
<option value="9" <% get_tz_sel "9" %>>Africa/Bissau</option>
<option value="10" <% get_tz_sel "10" %>>Africa/Blantyre</option>
<option value="11" <% get_tz_sel "11" %>>Africa/Brazzaville</option>
<option value="12" <% get_tz_sel "12" %>>Africa/Bujumbura</option>
<option value="13" <% get_tz_sel "13" %>>Africa/Casablanca</option>
<option value="14" <% get_tz_sel "14" %>>Africa/Ceuta</option>
<option value="15" <% get_tz_sel "15" %>>Africa/Conakry</option>
<option value="16" <% get_tz_sel "16" %>>Africa/Dakar</option>
<option value="17" <% get_tz_sel "17" %>>Africa/Dar es Salaam</option>
<option value="18" <% get_tz_sel "18" %>>Africa/Djibouti</option>
<option value="19" <% get_tz_sel "19" %>>Africa/Douala</option>
<option value="20" <% get_tz_sel "20" %>>Africa/El Aaiun</option>
<option value="21" <% get_tz_sel "21" %>>Africa/Freetown</option>
<option value="22" <% get_tz_sel "22" %>>Africa/Gaborone</option>
<option value="23" <% get_tz_sel "23" %>>Africa/Harare</option>
<option value="24" <% get_tz_sel "24" %>>Africa/Johannesburg</option>
<option value="25" <% get_tz_sel "25" %>>Africa/Kampala</option>
<option value="26" <% get_tz_sel "26" %>>Africa/Khartoum</option>
<option value="27" <% get_tz_sel "27" %>>Africa/Kigali</option>
<option value="28" <% get_tz_sel "28" %>>Africa/Kinshasa</option>
<option value="29" <% get_tz_sel "29" %>>Africa/Lagos</option>
<option value="30" <% get_tz_sel "30" %>>Africa/Libreville</option>
<option value="31" <% get_tz_sel "31" %>>Africa/Lome</option>
<option value="32" <% get_tz_sel "32" %>>Africa/Luanda</option>
<option value="33" <% get_tz_sel "33" %>>Africa/Lubumbashi</option>
<option value="34" <% get_tz_sel "34" %>>Africa/Lusaka</option>
<option value="35" <% get_tz_sel "35" %>>Africa/Malabo</option>
<option value="36" <% get_tz_sel "36" %>>Africa/Maputo</option>
<option value="37" <% get_tz_sel "37" %>>Africa/Maseru</option>
<option value="38" <% get_tz_sel "38" %>>Africa/Mbabane</option>
<option value="39" <% get_tz_sel "39" %>>Africa/Mogadishu</option>
<option value="40" <% get_tz_sel "40" %>>Africa/Monrovia</option>
<option value="41" <% get_tz_sel "41" %>>Africa/Nairobi</option>
<option value="42" <% get_tz_sel "42" %>>Africa/Ndjamena</option>
<option value="43" <% get_tz_sel "43" %>>Africa/Niamey</option>
<option value="44" <% get_tz_sel "44" %>>Africa/Nouakchott</option>
<option value="45" <% get_tz_sel "45" %>>Africa/Ouagadougou</option>
<option value="46" <% get_tz_sel "46" %>>Africa/Porto-Novo</option>
<option value="47" <% get_tz_sel "47" %>>Africa/Sao Tome</option>
<option value="48" <% get_tz_sel "48" %>>Africa/Tripoli</option>
<option value="49" <% get_tz_sel "49" %>>Africa/Tunis</option>
<option value="50" <% get_tz_sel "50" %>>Africa/Windhoek</option>
<option value="51" <% get_tz_sel "51" %>>America/Adak</option>
<option value="52" <% get_tz_sel "52" %>>America/Anchorage</option>
<option value="53" <% get_tz_sel "53" %>>America/Anguilla</option>
<option value="54" <% get_tz_sel "54" %>>America/Antigua</option>
<option value="55" <% get_tz_sel "55" %>>America/Araguaina</option>
<option value="56" <% get_tz_sel "56" %>>America/Argentina/Buenos Aires</option>
<option value="57" <% get_tz_sel "57" %>>America/Argentina/Catamarca</option>
<option value="58" <% get_tz_sel "58" %>>America/Argentina/Cordoba</option>
<option value="59" <% get_tz_sel "59" %>>America/Argentina/Jujuy</option>
<option value="60" <% get_tz_sel "60" %>>America/Argentina/La Rioja</option>
<option value="61" <% get_tz_sel "61" %>>America/Argentina/Mendoza</option>
<option value="62" <% get_tz_sel "62" %>>America/Argentina/Rio Gallegos</option>
<option value="63" <% get_tz_sel "63" %>>America/Argentina/Salta</option>
<option value="64" <% get_tz_sel "64" %>>America/Argentina/San Juan</option>
<option value="65" <% get_tz_sel "65" %>>America/Argentina/Tucuman</option>
<option value="66" <% get_tz_sel "66" %>>America/Argentina/Ushuaia</option>
<option value="67" <% get_tz_sel "67" %>>America/Aruba</option>
<option value="68" <% get_tz_sel "68" %>>America/Asuncion</option>
<option value="69" <% get_tz_sel "69" %>>America/Atikokan</option>
<option value="70" <% get_tz_sel "70" %>>America/Bahia</option>
<option value="71" <% get_tz_sel "71" %>>America/Barbados</option>
<option value="72" <% get_tz_sel "72" %>>America/Belem</option>
<option value="73" <% get_tz_sel "73" %>>America/Belize</option>
<option value="74" <% get_tz_sel "74" %>>America/Blanc-Sablon</option>
<option value="75" <% get_tz_sel "75" %>>America/Boa Vista</option>
<option value="76" <% get_tz_sel "76" %>>America/Bogota</option>
<option value="77" <% get_tz_sel "77" %>>America/Boise</option>
<option value="78" <% get_tz_sel "78" %>>America/Cambridge Bay</option>
<option value="79" <% get_tz_sel "79" %>>America/Campo Grande</option>
<option value="80" <% get_tz_sel "80" %>>America/Cancun</option>
<option value="81" <% get_tz_sel "81" %>>America/Caracas</option>
<option value="82" <% get_tz_sel "82" %>>America/Cayenne</option>
<option value="83" <% get_tz_sel "83" %>>America/Cayman</option>
<option value="84" <% get_tz_sel "84" %>>America/Chicago</option>
<option value="85" <% get_tz_sel "85" %>>America/Chihuahua</option>
<option value="86" <% get_tz_sel "86" %>>America/Costa Rica</option>
<option value="87" <% get_tz_sel "87" %>>America/Cuiaba</option>
<option value="88" <% get_tz_sel "88" %>>America/Curacao</option>
<option value="89" <% get_tz_sel "89" %>>America/Danmarkshavn</option>
<option value="90" <% get_tz_sel "90" %>>America/Dawson</option>
<option value="91" <% get_tz_sel "91" %>>America/Dawson Creek</option>
<option value="92" <% get_tz_sel "92" %>>America/Denver</option>
<option value="93" <% get_tz_sel "93" %>>America/Detroit</option>
<option value="94" <% get_tz_sel "94" %>>America/Dominica</option>
<option value="95" <% get_tz_sel "95" %>>America/Edmonton</option>
<option value="96" <% get_tz_sel "96" %>>America/Eirunepe</option>
<option value="97" <% get_tz_sel "97" %>>America/El Salvador</option>
<option value="98" <% get_tz_sel "98" %>>America/Fortaleza</option>
<option value="99" <% get_tz_sel "99" %>>America/Glace Bay</option>
<option value="100" <% get_tz_sel "100" %>>America/Goose Bay</option>
<option value="101" <% get_tz_sel "101" %>>America/Grand Turk</option>
<option value="102" <% get_tz_sel "102" %>>America/Grenada</option>
<option value="103" <% get_tz_sel "103" %>>America/Guadeloupe</option>
<option value="104" <% get_tz_sel "104" %>>America/Guatemala</option>
<option value="105" <% get_tz_sel "105" %>>America/Guayaquil</option>
<option value="106" <% get_tz_sel "106" %>>America/Guyana</option>
<option value="107" <% get_tz_sel "107" %>>America/Halifax</option>
<option value="108" <% get_tz_sel "108" %>>America/Havana</option>
<option value="109" <% get_tz_sel "109" %>>America/Hermosillo</option>
<option value="110" <% get_tz_sel "110" %>>America/Indiana/Indianapolis</option>
<option value="111" <% get_tz_sel "111" %>>America/Indiana/Knox</option>
<option value="112" <% get_tz_sel "112" %>>America/Indiana/Marengo</option>
<option value="113" <% get_tz_sel "113" %>>America/Indiana/Petersburg</option>
<option value="114" <% get_tz_sel "114" %>>America/Indiana/Tell City</option>
<option value="115" <% get_tz_sel "115" %>>America/Indiana/Vevay</option>
<option value="116" <% get_tz_sel "116" %>>America/Indiana/Vincennes</option>
<option value="117" <% get_tz_sel "117" %>>America/Indiana/Winamac</option>
<option value="118" <% get_tz_sel "118" %>>America/Inuvik</option>
<option value="119" <% get_tz_sel "119" %>>America/Iqaluit</option>
<option value="120" <% get_tz_sel "120" %>>America/Jamaica</option>
<option value="121" <% get_tz_sel "121" %>>America/Juneau</option>
<option value="122" <% get_tz_sel "122" %>>America/Kentucky/Louisville</option>
<option value="123" <% get_tz_sel "123" %>>America/Kentucky/Monticello</option>
<option value="124" <% get_tz_sel "124" %>>America/La Paz</option>
<option value="125" <% get_tz_sel "125" %>>America/Lima</option>
<option value="126" <% get_tz_sel "126" %>>America/Los Angeles</option>
<option value="127" <% get_tz_sel "127" %>>America/Maceio</option>
<option value="128" <% get_tz_sel "128" %>>America/Managua</option>
<option value="129" <% get_tz_sel "129" %>>America/Manaus</option>
<option value="130" <% get_tz_sel "130" %>>America/Marigot</option>
<option value="131" <% get_tz_sel "131" %>>America/Martinique</option>
<option value="132" <% get_tz_sel "132" %>>America/Matamoros</option>
<option value="133" <% get_tz_sel "133" %>>America/Mazatlan</option>
<option value="134" <% get_tz_sel "134" %>>America/Menominee</option>
<option value="135" <% get_tz_sel "135" %>>America/Merida</option>
<option value="136" <% get_tz_sel "136" %>>America/Mexico City</option>
<option value="137" <% get_tz_sel "137" %>>America/Miquelon</option>
<option value="138" <% get_tz_sel "138" %>>America/Moncton</option>
<option value="139" <% get_tz_sel "139" %>>America/Monterrey</option>
<option value="140" <% get_tz_sel "140" %>>America/Montevideo</option>
<option value="141" <% get_tz_sel "141" %>>America/Montreal</option>
<option value="142" <% get_tz_sel "142" %>>America/Montserrat</option>
<option value="143" <% get_tz_sel "143" %>>America/Nassau</option>
<option value="144" <% get_tz_sel "144" %>>America/New York</option>
<option value="145" <% get_tz_sel "145" %>>America/Nipigon</option>
<option value="146" <% get_tz_sel "146" %>>America/Nome</option>
<option value="147" <% get_tz_sel "147" %>>America/Noronha</option>
<option value="148" <% get_tz_sel "148" %>>America/North Dakota/Center</option>
<option value="149" <% get_tz_sel "149" %>>America/North Dakota/New Salem</option>
<option value="150" <% get_tz_sel "150" %>>America/Ojinaga</option>
<option value="151" <% get_tz_sel "151" %>>America/Panama</option>
<option value="152" <% get_tz_sel "152" %>>America/Pangnirtung</option>
<option value="153" <% get_tz_sel "153" %>>America/Paramaribo</option>
<option value="154" <% get_tz_sel "154" %>>America/Phoenix</option>
<option value="155" <% get_tz_sel "155" %>>America/Port of Spain</option>
<option value="156" <% get_tz_sel "156" %>>America/Port-au-Prince</option>
<option value="157" <% get_tz_sel "157" %>>America/Porto Velho</option>
<option value="158" <% get_tz_sel "158" %>>America/Puerto Rico</option>
<option value="159" <% get_tz_sel "159" %>>America/Rainy River</option>
<option value="160" <% get_tz_sel "160" %>>America/Rankin Inlet</option>
<option value="161" <% get_tz_sel "161" %>>America/Recife</option>
<option value="162" <% get_tz_sel "162" %>>America/Regina</option>
<option value="163" <% get_tz_sel "163" %>>America/Rio Branco</option>
<option value="164" <% get_tz_sel "164" %>>America/Santa Isabel</option>
<option value="165" <% get_tz_sel "165" %>>America/Santarem</option>
<option value="166" <% get_tz_sel "166" %>>America/Santo Domingo</option>
<option value="167" <% get_tz_sel "167" %>>America/Sao Paulo</option>
<option value="168" <% get_tz_sel "168" %>>America/Scoresbysund</option>
<option value="169" <% get_tz_sel "169" %>>America/Shiprock</option>
<option value="170" <% get_tz_sel "170" %>>America/St Barthelemy</option>
<option value="171" <% get_tz_sel "171" %>>America/St Johns</option>
<option value="172" <% get_tz_sel "172" %>>America/St Kitts</option>
<option value="173" <% get_tz_sel "173" %>>America/St Lucia</option>
<option value="174" <% get_tz_sel "174" %>>America/St Thomas</option>
<option value="175" <% get_tz_sel "175" %>>America/St Vincent</option>
<option value="176" <% get_tz_sel "176" %>>America/Swift Current</option>
<option value="177" <% get_tz_sel "177" %>>America/Tegucigalpa</option>
<option value="178" <% get_tz_sel "178" %>>America/Thule</option>
<option value="179" <% get_tz_sel "179" %>>America/Thunder Bay</option>
<option value="180" <% get_tz_sel "180" %>>America/Tijuana</option>
<option value="181" <% get_tz_sel "181" %>>America/Toronto</option>
<option value="182" <% get_tz_sel "182" %>>America/Tortola</option>
<option value="183" <% get_tz_sel "183" %>>America/Vancouver</option>
<option value="184" <% get_tz_sel "184" %>>America/Whitehorse</option>
<option value="185" <% get_tz_sel "185" %>>America/Winnipeg</option>
<option value="186" <% get_tz_sel "186" %>>America/Yakutat</option>
<option value="187" <% get_tz_sel "187" %>>America/Yellowknife</option>
<option value="188" <% get_tz_sel "188" %>>Antarctica/Casey</option>
<option value="189" <% get_tz_sel "189" %>>Antarctica/Davis</option>
<option value="190" <% get_tz_sel "190" %>>Antarctica/DumontDUrville</option>
<option value="191" <% get_tz_sel "191" %>>Antarctica/Macquarie</option>
<option value="192" <% get_tz_sel "192" %>>Antarctica/Mawson</option>
<option value="193" <% get_tz_sel "193" %>>Antarctica/McMurdo</option>
<option value="194" <% get_tz_sel "194" %>>Antarctica/Rothera</option>
<option value="195" <% get_tz_sel "195" %>>Antarctica/South Pole</option>
<option value="196" <% get_tz_sel "196" %>>Antarctica/Syowa</option>
<option value="197" <% get_tz_sel "197" %>>Antarctica/Vostok</option>
<option value="198" <% get_tz_sel "198" %>>Arctic/Longyearbyen</option>
<option value="199" <% get_tz_sel "199" %>>Asia/Aden</option>
<option value="200" <% get_tz_sel "200" %>>Asia/Almaty</option>
<option value="201" <% get_tz_sel "201" %>>Asia/Anadyr</option>
<option value="202" <% get_tz_sel "202" %>>Asia/Aqtau</option>
<option value="203" <% get_tz_sel "203" %>>Asia/Aqtobe</option>
<option value="204" <% get_tz_sel "204" %>>Asia/Ashgabat</option>
<option value="205" <% get_tz_sel "205" %>>Asia/Baghdad</option>
<option value="206" <% get_tz_sel "206" %>>Asia/Bahrain</option>
<option value="207" <% get_tz_sel "207" %>>Asia/Baku</option>
<option value="208" <% get_tz_sel "208" %>>Asia/Bangkok</option>
<option value="209" <% get_tz_sel "209" %>>Asia/Beirut</option>
<option value="210" <% get_tz_sel "210" %>>Asia/Bishkek</option>
<option value="211" <% get_tz_sel "211" %>>Asia/Brunei</option>
<option value="212" <% get_tz_sel "212" %>>Asia/Choibalsan</option>
<option value="213" <% get_tz_sel "213" %>>Asia/Chongqing</option>
<option value="214" <% get_tz_sel "214" %>>Asia/Colombo</option>
<option value="215" <% get_tz_sel "215" %>>Asia/Damascus</option>
<option value="216" <% get_tz_sel "216" %>>Asia/Dhaka</option>
<option value="217" <% get_tz_sel "217" %>>Asia/Dili</option>
<option value="218" <% get_tz_sel "218" %>>Asia/Dubai</option>
<option value="219" <% get_tz_sel "219" %>>Asia/Dushanbe</option>
<option value="220" <% get_tz_sel "220" %>>Asia/Gaza</option>
<option value="221" <% get_tz_sel "221" %>>Asia/Harbin</option>
<option value="222" <% get_tz_sel "222" %>>Asia/Ho Chi Minh</option>
<option value="223" <% get_tz_sel "223" %>>Asia/Hong Kong</option>
<option value="224" <% get_tz_sel "224" %>>Asia/Hovd</option>
<option value="225" <% get_tz_sel "225" %>>Asia/Irkutsk</option>
<option value="226" <% get_tz_sel "226" %>>Asia/Jakarta</option>
<option value="227" <% get_tz_sel "227" %>>Asia/Jayapura</option>
<option value="228" <% get_tz_sel "228" %>>Asia/Kabul</option>
<option value="229" <% get_tz_sel "229" %>>Asia/Kamchatka</option>
<option value="230" <% get_tz_sel "230" %>>Asia/Karachi</option>
<option value="231" <% get_tz_sel "231" %>>Asia/Kashgar</option>
<option value="232" <% get_tz_sel "232" %>>Asia/Kathmandu</option>
<option value="233" <% get_tz_sel "233" %>>Asia/Kolkata</option>
<option value="234" <% get_tz_sel "234" %>>Asia/Krasnoyarsk</option>
<option value="235" <% get_tz_sel "235" %>>Asia/Kuala Lumpur</option>
<option value="236" <% get_tz_sel "236" %>>Asia/Kuching</option>
<option value="237" <% get_tz_sel "237" %>>Asia/Kuwait</option>
<option value="238" <% get_tz_sel "238" %>>Asia/Macau</option>
<option value="239" <% get_tz_sel "239" %>>Asia/Magadan</option>
<option value="240" <% get_tz_sel "240" %>>Asia/Makassar</option>
<option value="241" <% get_tz_sel "241" %>>Asia/Manila</option>
<option value="242" <% get_tz_sel "242" %>>Asia/Muscat</option>
<option value="243" <% get_tz_sel "243" %>>Asia/Nicosia</option>
<option value="244" <% get_tz_sel "244" %>>Asia/Novokuznetsk</option>
<option value="245" <% get_tz_sel "245" %>>Asia/Novosibirsk</option>
<option value="246" <% get_tz_sel "246" %>>Asia/Omsk</option>
<option value="247" <% get_tz_sel "247" %>>Asia/Oral</option>
<option value="248" <% get_tz_sel "248" %>>Asia/Phnom Penh</option>
<option value="249" <% get_tz_sel "249" %>>Asia/Pontianak</option>
<option value="250" <% get_tz_sel "250" %>>Asia/Pyongyang</option>
<option value="251" <% get_tz_sel "251" %>>Asia/Qatar</option>
<option value="252" <% get_tz_sel "252" %>>Asia/Qyzylorda</option>
<option value="253" <% get_tz_sel "253" %>>Asia/Rangoon</option>
<option value="254" <% get_tz_sel "254" %>>Asia/Riyadh</option>
<option value="255" <% get_tz_sel "255" %>>Asia/Sakhalin</option>
<option value="256" <% get_tz_sel "256" %>>Asia/Samarkand</option>
<option value="257" <% get_tz_sel "257" %>>Asia/Seoul</option>
<option value="258" <% get_tz_sel "258" %>>Asia/Shanghai</option>
<option value="259" <% get_tz_sel "259" %>>Asia/Singapore</option>
<option value="260" <% get_tz_sel "260" %>>Asia/Taipei</option>
<option value="261" <% get_tz_sel "261" %>>Asia/Tashkent</option>
<option value="262" <% get_tz_sel "262" %>>Asia/Tbilisi</option>
<option value="263" <% get_tz_sel "263" %>>Asia/Thimphu</option>
<option value="264" <% get_tz_sel "264" %>>Asia/Tokyo</option>
<option value="265" <% get_tz_sel "265" %>>Asia/Ulaanbaatar</option>
<option value="266" <% get_tz_sel "266" %>>Asia/Urumqi</option>
<option value="267" <% get_tz_sel "267" %>>Asia/Vientiane</option>
<option value="268" <% get_tz_sel "268" %>>Asia/Vladivostok</option>
<option value="269" <% get_tz_sel "269" %>>Asia/Yakutsk</option>
<option value="270" <% get_tz_sel "270" %>>Asia/Yekaterinburg</option>
<option value="271" <% get_tz_sel "271" %>>Asia/Yerevan</option>
<option value="272" <% get_tz_sel "272" %>>Atlantic/Azores</option>
<option value="273" <% get_tz_sel "273" %>>Atlantic/Bermuda</option>
<option value="274" <% get_tz_sel "274" %>>Atlantic/Canary</option>
<option value="275" <% get_tz_sel "275" %>>Atlantic/Cape Verde</option>
<option value="276" <% get_tz_sel "276" %>>Atlantic/Faroe</option>
<option value="277" <% get_tz_sel "277" %>>Atlantic/Madeira</option>
<option value="278" <% get_tz_sel "278" %>>Atlantic/Reykjavik</option>
<option value="279" <% get_tz_sel "279" %>>Atlantic/South Georgia</option>
<option value="280" <% get_tz_sel "280" %>>Atlantic/St Helena</option>
<option value="281" <% get_tz_sel "281" %>>Atlantic/Stanley</option>
<option value="282" <% get_tz_sel "282" %>>Australia/Adelaide</option>
<option value="283" <% get_tz_sel "283" %>>Australia/Brisbane</option>
<option value="284" <% get_tz_sel "284" %>>Australia/Broken Hill</option>
<option value="285" <% get_tz_sel "285" %>>Australia/Currie</option>
<option value="286" <% get_tz_sel "286" %>>Australia/Darwin</option>
<option value="287" <% get_tz_sel "287" %>>Australia/Eucla</option>
<option value="288" <% get_tz_sel "288" %>>Australia/Hobart</option>
<option value="289" <% get_tz_sel "289" %>>Australia/Lindeman</option>
<option value="290" <% get_tz_sel "290" %>>Australia/Lord Howe</option>
<option value="291" <% get_tz_sel "291" %>>Australia/Melbourne</option>
<option value="292" <% get_tz_sel "292" %>>Australia/Perth</option>
<option value="293" <% get_tz_sel "293" %>>Australia/Sydney</option>
<option value="294" <% get_tz_sel "294" %>>Europe/Amsterdam</option>
<option value="295" <% get_tz_sel "295" %>>Europe/Andorra</option>
<option value="296" <% get_tz_sel "296" %>>Europe/Athens</option>
<option value="297" <% get_tz_sel "297" %>>Europe/Belgrade</option>
<option value="298" <% get_tz_sel "298" %>>Europe/Berlin</option>
<option value="299" <% get_tz_sel "299" %>>Europe/Bratislava</option>
<option value="300" <% get_tz_sel "300" %>>Europe/Brussels</option>
<option value="301" <% get_tz_sel "301" %>>Europe/Bucharest</option>
<option value="302" <% get_tz_sel "302" %>>Europe/Budapest</option>
<option value="303" <% get_tz_sel "303" %>>Europe/Chisinau</option>
<option value="304" <% get_tz_sel "304" %>>Europe/Copenhagen</option>
<option value="305" <% get_tz_sel "305" %>>Europe/Dublin</option>
<option value="306" <% get_tz_sel "306" %>>Europe/Gibraltar</option>
<option value="307" <% get_tz_sel "307" %>>Europe/Guernsey</option>
<option value="308" <% get_tz_sel "308" %>>Europe/Helsinki</option>
<option value="309" <% get_tz_sel "309" %>>Europe/Isle of Man</option>
<option value="310" <% get_tz_sel "310" %>>Europe/Istanbul</option>
<option value="311" <% get_tz_sel "311" %>>Europe/Jersey</option>
<option value="312" <% get_tz_sel "312" %>>Europe/Kaliningrad</option>
<option value="313" <% get_tz_sel "313" %>>Europe/Kiev</option>
<option value="314" <% get_tz_sel "314" %>>Europe/Lisbon</option>
<option value="315" <% get_tz_sel "315" %>>Europe/Ljubljana</option>
<option value="0" <% get_tz_sel "0" %>>Europe/London</option>
<option value="317" <% get_tz_sel "317" %>>Europe/Luxembourg</option>
<option value="318" <% get_tz_sel "318" %>>Europe/Madrid</option>
<option value="319" <% get_tz_sel "319" %>>Europe/Malta</option>
<option value="320" <% get_tz_sel "320" %>>Europe/Mariehamn</option>
<option value="321" <% get_tz_sel "321" %>>Europe/Minsk</option>
<option value="322" <% get_tz_sel "322" %>>Europe/Monaco</option>
<option value="323" <% get_tz_sel "323" %>>Europe/Moscow</option>
<option value="324" <% get_tz_sel "324" %>>Europe/Oslo</option>
<option value="325" <% get_tz_sel "325" %>>Europe/Paris</option>
<option value="326" <% get_tz_sel "326" %>>Europe/Podgorica</option>
<option value="327" <% get_tz_sel "327" %>>Europe/Prague</option>
<option value="328" <% get_tz_sel "328" %>>Europe/Riga</option>
<option value="329" <% get_tz_sel "329" %>>Europe/Rome</option>
<option value="330" <% get_tz_sel "330" %>>Europe/Samara</option>
<option value="331" <% get_tz_sel "331" %>>Europe/San Marino</option>
<option value="332" <% get_tz_sel "332" %>>Europe/Sarajevo</option>
<option value="333" <% get_tz_sel "333" %>>Europe/Simferopol</option>
<option value="334" <% get_tz_sel "334" %>>Europe/Skopje</option>
<option value="335" <% get_tz_sel "335" %>>Europe/Sofia</option>
<option value="336" <% get_tz_sel "336" %>>Europe/Stockholm</option>
<option value="337" <% get_tz_sel "337" %>>Europe/Tallinn</option>
<option value="338" <% get_tz_sel "338" %>>Europe/Tirane</option>
<option value="339" <% get_tz_sel "339" %>>Europe/Uzhgorod</option>
<option value="340" <% get_tz_sel "340" %>>Europe/Vaduz</option>
<option value="341" <% get_tz_sel "341" %>>Europe/Vatican</option>
<option value="342" <% get_tz_sel "342" %>>Europe/Vienna</option>
<option value="343" <% get_tz_sel "343" %>>Europe/Vilnius</option>
<option value="344" <% get_tz_sel "344" %>>Europe/Volgograd</option>
<option value="345" <% get_tz_sel "345" %>>Europe/Warsaw</option>
<option value="346" <% get_tz_sel "346" %>>Europe/Zagreb</option>
<option value="347" <% get_tz_sel "347" %>>Europe/Zaporozhye</option>
<option value="348" <% get_tz_sel "348" %>>Europe/Zurich</option>
<option value="349" <% get_tz_sel "349" %>>Indian/Antananarivo</option>
<option value="350" <% get_tz_sel "350" %>>Indian/Chagos</option>
<option value="351" <% get_tz_sel "351" %>>Indian/Christmas</option>
<option value="352" <% get_tz_sel "352" %>>Indian/Cocos</option>
<option value="353" <% get_tz_sel "353" %>>Indian/Comoro</option>
<option value="354" <% get_tz_sel "354" %>>Indian/Kerguelen</option>
<option value="355" <% get_tz_sel "355" %>>Indian/Mahe</option>
<option value="356" <% get_tz_sel "356" %>>Indian/Maldives</option>
<option value="357" <% get_tz_sel "357" %>>Indian/Mauritius</option>
<option value="358" <% get_tz_sel "358" %>>Indian/Mayotte</option>
<option value="359" <% get_tz_sel "359" %>>Indian/Reunion</option>
<option value="360" <% get_tz_sel "360" %>>Pacific/Apia</option>
<option value="361" <% get_tz_sel "361" %>>Pacific/Auckland</option>
<option value="362" <% get_tz_sel "362" %>>Pacific/Chatham</option>
<option value="363" <% get_tz_sel "363" %>>Pacific/Efate</option>
<option value="364" <% get_tz_sel "364" %>>Pacific/Enderbury</option>
<option value="365" <% get_tz_sel "365" %>>Pacific/Fakaofo</option>
<option value="366" <% get_tz_sel "366" %>>Pacific/Fiji</option>
<option value="367" <% get_tz_sel "367" %>>Pacific/Funafuti</option>
<option value="368" <% get_tz_sel "368" %>>Pacific/Galapagos</option>
<option value="369" <% get_tz_sel "369" %>>Pacific/Gambier</option>
<option value="370" <% get_tz_sel "370" %>>Pacific/Guadalcanal</option>
<option value="371" <% get_tz_sel "371" %>>Pacific/Guam</option>
<option value="372" <% get_tz_sel "372" %>>Pacific/Honolulu</option>
<option value="373" <% get_tz_sel "373" %>>Pacific/Johnston</option>
<option value="374" <% get_tz_sel "374" %>>Pacific/Kiritimati</option>
<option value="375" <% get_tz_sel "375" %>>Pacific/Kosrae</option>
<option value="376" <% get_tz_sel "376" %>>Pacific/Kwajalein</option>
<option value="377" <% get_tz_sel "377" %>>Pacific/Majuro</option>
<option value="378" <% get_tz_sel "378" %>>Pacific/Marquesas</option>
<option value="379" <% get_tz_sel "379" %>>Pacific/Midway</option>
<option value="380" <% get_tz_sel "380" %>>Pacific/Nauru</option>
<option value="381" <% get_tz_sel "381" %>>Pacific/Niue</option>
<option value="382" <% get_tz_sel "382" %>>Pacific/Norfolk</option>
<option value="383" <% get_tz_sel "383" %>>Pacific/Noumea</option>
<option value="384" <% get_tz_sel "384" %>>Pacific/Pago Pago</option>
<option value="385" <% get_tz_sel "385" %>>Pacific/Palau</option>
<option value="386" <% get_tz_sel "386" %>>Pacific/Pitcairn</option>
<option value="387" <% get_tz_sel "387" %>>Pacific/Ponape</option>
<option value="388" <% get_tz_sel "388" %>>Pacific/Port Moresby</option>
<option value="389" <% get_tz_sel "389" %>>Pacific/Rarotonga</option>
<option value="390" <% get_tz_sel "390" %>>Pacific/Saipan</option>
<option value="391" <% get_tz_sel "391" %>>Pacific/Tahiti</option>
<option value="392" <% get_tz_sel "392" %>>Pacific/Tarawa</option>
<option value="393" <% get_tz_sel "393" %>>Pacific/Tongatapu</option>
<option value="394" <% get_tz_sel "394" %>>Pacific/Truk</option>
<option value="395" <% get_tz_sel "395" %>>Pacific/Wake</option>
<option value="396" <% get_tz_sel "396" %>>Pacific/Wallis</option>
</select>
</TD>
</TR>
<TR><TH>Use NTP:</TH><TD><input type="checkbox" name="use_ntp" value="1" <% is_checked $(get_config ntp_enable) %> onclick="setReadNTP(this);"/></TD></TR>
<TR><TH>NTP server:</TH><TD><input type="text" name="ntp" size=20  value=<% get_config ntp_server %> title="Enter a NTP server name or IP address. Ex.: pool.ntp.org"></TD></TR>

<TR>
    <TD COLSPAN=2>Date:
    <INPUT class=text id=year maxLength=4 name=year size=4 value="<%= $(get_year)%>" title="Year">
    /
    <INPUT class=text id=month maxLength=2 name=month size=2 value="<%= $(get_month)%>" title="Month">
    / 
    <INPUT class=text id=day maxLength=2 name=day size=2 value="<%= $(get_day)%>" title="Day">
    (yyyy/mm/dd)</TD>
</TR>
<TR>
    <TD COLSPAN=2>Time:
    <INPUT class=text id=hour maxLength=2 name=hour size=2 value="<%= $(get_hour)%>" title="Hour">
    : 
    <INPUT class=text id=minute maxLength=2 name=minute size=2 value="<%= $(get_min)%>" title="Minute">
    :
    <INPUT class=text id=second maxLength=2 name=second size=2 value="<%= $(get_sec)%>" title="Second">
    (hh:mm:ss) </TD>
</TR>
<TR><TD COLSPAN=2><input type="submit" name="action" value="Apply"></TD></TR>
</TABLE>
</form>
</body>
</html>

