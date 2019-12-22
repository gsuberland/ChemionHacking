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
# Purpose           : Convert a 132 character UART string to output on Glasses
#
# Revision History  :
#
# Date        Author      Version    Revision (Date in YYYYMMDD format) 
# 20191220    jurre       1.00       Initial release (Python2)
# 20191222    jilles      1.01       Converted to Python3 added parameter input and fault handling 
#
####################################################################################################

import sys

def display(data=""):

  # Get string from parameter
  if( not( data ) and len(sys.argv)>1 ):
    data = sys.argv[1]

  # Get string from stdin
  if( not( data ) ):
    data = input()

  print()
  print( "decoder.py - v1.01 by Jurre & Jilles Groenendijk" )
  print()

  # Incorrect data? -> Show error
  if( len( data ) != 132 ):
    print( "Error:" )
    print( "  Invalid length ",len( data )," should be 132")
    print()
    print( "Syntax:" )
    print( "  echo <string>|decoder.py" )
    print( "  decoder.py <string>"  )
    quit()

  # data = fa01003b01000d010a0000000000003c00003c003cf30000f300c3f33c0cf300c3f33cccf3000cf33cccf3000cf33cccf300003c0f303c000c000000000000c855a9

  header = data[:18]    # fa01003b01000d010a
  body   = data[18:126] # 0000000000003c00003c003cf30000f300c3f33c0cf300c3f33cccf3000cf33cccf3000cf33cccf300003c0f303c000c000000000000
  crc    = data[126:2]  # c8
  footer = data[128:]   # 55a9

  all_bits="".join(["0"*(4-len(str(bin(int(x,16)))[2:]))+str(bin(int(x,16)))[2:] for x in body])

  all_pixels = [all_bits[x:x+2] for x in range(0, len(all_bits),2)]
  
  for x in range(0, 8):
    print( " ", x, "| " , end="" )
    for y in range(0, 24):
      pixel=all_pixels[x*24+y]
      if(pixel=="00"): print( "  ", end="" )
      if(pixel=="01"): print( "- ", end="")
      if(pixel=="10"): print( "x ", end="")
      if(pixel=="11"): print( "X ", end="")
    print()

  print( "    +------------------------------------------------" )

  print( "      ",end="" )
  for y in range(0, 24):
    print(int(y / 10),end=" ")
  print()

  print( "      ",end="" )
  for y in range(0, 24):
    print((y % 10),end=" ")
  print()

display()