#!/usr/bin/python3
####################################################################################################
#
# Project           : CHEMION Encoder
#
# Program name      : encoder.py
#
# Author(s)         : Jilles Groenendijk
#
# Date created      : 202001011
#
# Purpose           : Convert an image textfile to an 128 character UART string to output on Glasses
#
# Revision History  :
#
# Date        Author      Version    Revision (Date in YYYYMMDD format) 
# 20200111    jilles      1.00       Initial release
# 20200111    jilles      1.01       New version to include Arduino structs
#
####################################################################################################

"""
1. Create image file (9 rows by 24 cols, " " = off, "-" = 25%, "x" = 75%, "X" = 100%

$ cat << EOF > image.txt

 XXX X  X XXX  XXXX XXX
X    X  X X  X X    X  X
X    X  X X  X X    X  X
X    XXXX XXX  XXX  XXX
X     XX  X  X X    X  X
X     XX  X  X X    X  X
 XXX  XX  XXX  XXXX X  X

EOF

2. Run the encoder

$ ./encoder.py image.txt

fa0300390100060000000000003f30cfc3fcfcc030cc3300c3c030cc3300c3c03fcfc3f0fcc00f0c3300c3c00f0c3300c33f0f0fc3fcc30000000000003855a9

3. Run the decoder to verify the string

$ ./decoder.py fa0300390100060000000000003f30cfc3fcfcc030cc3300c3c030cc3300c3c03fcfc3f0fcc00f0c3300c3c00f0c3300c33f0f0fc3fcc30000000000003855a9

  X X X   X     X   X X X     X X X X   X X X
X         X     X   X     X   X         X     X
X         X     X   X     X   X         X     X
X         X X X X   X X X     X X X     X X X
X           X X     X     X   X         X     X
X           X X     X     X   X         X     X
  X X X     X X     X X X     X X X X   X     X

"""
import sys
import os.path
from os import path
import argparse

parser = argparse.ArgumentParser(description="encoder.py - v 1.01 by  Jilles Groenendijk")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("filename", type=str,help="the filename containing the design to encode to an uartstring")
args = parser.parse_args()

if(not path.exists(args.filename)):
  if(not args.quiet):
    print("encoder.py: error: file \""+args.filename+"\" does not exists")
  quit()

if(args.debug):
  print("DEBUG")
  print("Filename: ", args.filename)
  print("")
  print("Image:")
  print("")
  linenr=0
  with open(args.filename,"r") as fh:
    for line in fh.readlines():
      print( " ", linenr, "|" , line.strip())
      linenr+=1

  print( "    +-------------------------" )

  print( "      ",end="" )
  for y in range(0, 24):
    print(int(y / 10),end="")
  print()

  print( "      ",end="" )
  for y in range(0, 24):
    print((y % 10),end="")
  print()

header = "fa030039010006"
crc    = 7
footer = "55a9"

ledstring = ""

with open(args.filename,"r") as fh:
  for ledline in fh.readlines():
    ledstring+=ledline.strip("\n")+" "*(24-len(ledline.strip("\n")))

uartstring = header

binarystring = ""

for nibble in ledstring:
  if(  nibble==" "): binarystring+="00"
  elif(nibble=="-"): binarystring+="01"
  elif(nibble=="x"): binarystring+="10"
  elif(nibble=="X"): binarystring+="11"
  if(len(binarystring)==8):
    byte=int(binarystring,2)
    crc^=byte
    uartstring+="{:02x}".format(byte)
    binarystring=""

uartstring += "{:02x}".format(crc)
uartstring += footer

if(args.debug):
  print("")
  print("uartstring:")

print(uartstring)
print("")

print("    uint8_t msg_static[4][20] = {{",end="")
for f in range(63):
  print("0x"+uartstring[(f*2):(f*2)+2]+",",end="")

  if(((f+1)%20)==0):
    print("},\n                                 {",end="")
f+=1
print("0x"+uartstring[(f*2):(f*2)+2]+"}};\n")

print("    uint8_t msg_save[8][20] = {{0xfa,0x01,0x00,0x03,0x01,0x00,0x05,0x04,0x55,0xa9},")
print("                               {0xfa,0x01,0x00,0x04,0x01,0x00,0x14,0x05,0x10,0x55,0xa9},")
print("                               {0xfa,0x01,0x00,0x06,0x01,0x00,0x0b,0x05,0x00,0x01,0x0e,0x55,0xa9},")

uartstring="fa01003b01000d010a"+uartstring[14:129]
print("                               {",end="")

for f in range(65):
  print("0x"+uartstring[(f*2):(f*2)+2]+",",end="")

  if(((f+1)%20)==0):
    print("},\n                               {",end="")
f+=1
print("0x"+uartstring[(f*2):(f*2)+2]+"},")
print("                               {0xfa,0x01,0x00,0x03,0x01,0x00,0x0c,0x0d,0x55,0xa9}};")
