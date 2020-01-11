#!/usr/bin/python3
####################################################################################################
#
# Project           : CHEMION Decoder
#
# Program name      : decoder.py
#
# Author(s)         : Jurre Groenendijk
#                     Jilles Groenendijk
#
# Date created      : 20191220
#
# Purpose           : Convert a 128 character UART string to output on Glasses
#
# Revision History  :
#
# Date        Author      Version    Revision (Date in YYYYMMDD format) 
# 20191220    jurre       1.00       Initial release (Python2)
# 20191222    jilles      1.01       Converted to Python3 added parameter input and fault handling 
# 20200103    jilles      1.02       Messages are 128 nibbles = 64 bytes, added argparse
# 20200111    jilles      1.03       Last line was not interpreted, fixed
#
####################################################################################################

# On Android:
# 1. Install Chemion app
# 2. Developer options
#    [x] Enable Bluetooth HCI snoop log
#    [x] USB debugging
# 3. Connect to Chemion glasses
# 4. Upload animations
# 5. Disconnect from Chemion glasses
# 6. Developer options
#    [ ] Enable Bluetooth HCI snoop log
# 7. Developer options
#    [x] Enable Bluetooth HCI snoop log
#
# On linux:
# 1. $ apt install adb
# 2. $ adb shell
# 3. shell@device:/ $ cd /sdcard
# 4. shell@device:/ $ ls -l btsnoop_hci.log                            
#    -rw-rw---- root     sdcard_r       16 2020-01-03 15:34 btsnoop_hci.log
# 5. shell@device:/ $ exit
# 6. $ adb pull /sdcard/btsnoop_hci.log btsnoop_hci.pcap
#    /sdcard/btsnoop_hci.log: 1 file pulled. 0.0 MB/s (16 bytes in 0.002s)
# 7. $ echo $(echo $(tshark -r btsnoop_hci.pcap -T jsonraw) | sed -e 's/,/\n/g' | grep btgatt.nordic.uart_tx_raw| cut -d\" -f4  | sed -e 's/^fa/~fa/g') | sed -e 's/ //g' -e 's/~/\n/g' > uartstrings.txt
# 8. $ for f in $(cat uartstrings);do clear;./decoder.py $f -q;sleep .1;done
#                                                
#      X X X     X X       X X     X X       X       
#    X X       X X   X   X X   X   X X       X       
#    X X       X X   X   X X   X   X X       X       
#    X X       X X   X   X X   X   X X       X       
#    X X       X X   X   X X   X   X X               
#    X X       X X   X   X X   X   X X       X       
#      X X X     X X       X X     X X X X   X       
#

import sys
import argparse

line_length = 128

parser = argparse.ArgumentParser(description="decoder.py - v 1.03 by Jurre & Jilles Groenendijk")
parser.add_argument("-v", "--verbose", action="store_true")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("-q", "--quiet", action="store_true")
parser.add_argument("uartstring", type=str,help="the uartstring (128 character)")
args = parser.parse_args()

# Get string from stdin
data=args.uartstring
if( data=="-"):
  data = input()

if(len(data)!=line_length):
  if(not args.quiet):
    print("decoder.py: error: length of uartstring("+str(len(data))+") !=",line_length)
  quit()

# data = fa0300390100060000000000003f0f03c3c0c0f03ccf33c0c0f03ccf33c0c0f03ccf33c0c0f03ccf33c000f03ccf33c0c03f0f03c3fcc0000000000000cb55a9
header = data[:14]     # fa030039010006
body   = data[14:122]  # 0000000000003f0f03c3c0c0f03ccf33c0c0f03ccf33c0c0f03ccf33c0c0f03ccf33c000f03ccf33c0c03f0f03c3fcc0000000000000
crc    = data[122:124] # cb
footer = data[124:]    # 55a9
if(args.debug):
  print("DEBUG")
  print("Header: ", header," (",len(header),")")
  print("Body  : ", body," (",len(body),")")
  print("CRC   : ", crc," (",len(crc),")")
  print("Footer: ", footer," (",len(footer),")")
  print()

all_bits="".join(["0"*(4-len(str(bin(int(x,16)))[2:]))+str(bin(int(x,16)))[2:] for x in body])

all_pixels = [all_bits[x:x+2] for x in range(0, len(all_bits),2)]
  
for x in range(0, 9):
  if( args.verbose ): print( " ", x, "| " , end="" )
  for y in range(0, 24):
    pixel=all_pixels[x*24+y]
    if(pixel=="00"): print( "  ", end="" )
    if(pixel=="01"): print( "- ", end="")
    if(pixel=="10"): print( "x ", end="")
    if(pixel=="11"): print( "X ", end="")
  print()

if( args.verbose ):
  print( "    +------------------------------------------------" )

  print( "      ",end="" )
  for y in range(0, 24):
    print(int(y / 10),end=" ")
  print()

  print( "      ",end="" )
  for y in range(0, 24):
    print((y % 10),end=" ")
  print()

## EOF ##
