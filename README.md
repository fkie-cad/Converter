# Converter
Various converter tools for daily tasks.




## Content
-  [Requirements](#requirements)
-  [Build](#build)
-  [CtlCode](#ctlcode)
-  [Num2Bin](#num2bin)
-  [NtStatus2DosError](#ntstatus2doserror)
-  [String2Hex](#string2hex)


## Requirements
- msbuild


## Build
```bash
$devcmd> build [/all]
```


## CtlCode
A ctl code converter.
Converts a given ctl code into it's parts (,DeviceType Function, Method, <Access>), or the parts into a ctl code.

### version
1.0.0  
Last changed: 21.04.2023

### build
```bash
$devcmd> build /ioctl
```

### examples
```bash
$ CtlCode.exe 0x10000 // convert ioctl code into parts
$ CtlCode.exe 0x10 0 0 0 // convert parts (<DeviceType> <Function> <Method> <Access>) into ioctl code.
```



## Num2Bin
A (decimal or hex) number to binary converter.
Converts a given number into binary format or a binary string to hex and decimal number.
Highlights selected bits.

### version
1.0.4  
Last changed: 22.04.2024

### build
```bash
$devcmd> build /n2b
```

### options
- -n: Input number to convert to binary.
- -x: Input hex number to convert to binary. No prefix needed.
- -b: Input binary string to convert to a hex and decimal number.
- -m: Highlight bits at given offsets. Offsets begin with 0 from right to left. Separate with comma (1,3,4), give ranges with colon (4:7,12:15).
- -w: Width of displayed bits. Valid values are multiples of 8 <= 64. Default: 32.
- -s: Size of bit blocks. Valid values are powers of 2 <= 64. Default: 4.

`-w`, `-s` and `-m` only take effect if input is from `-n` or `-x`.

### examples
```bash
$ Num2Bin.exe -n 13 // convert 13 to binary format 
$ Num2Bin.exe -n 0xd -w 8 -s 2 // convert 13 to binary format just showing 8 bits in 2 bit sized blocks
$ Num2Bin.exe -x d -m 0,2:4,8:16 // convert 13 to binary, highlighting bit 0, 2 to 4 and 8 to 16
$ Num2Bin.exe -b 11001101 // convert 11001101 to hex and dec
$ Num2Bin.exe -b "1100 1101" -m 7:4 // convert 11001101 to hex and dec and highlight bits 4 to 7
```



## NtStatus2DosError
Convert an nt status code into a dos error code.

### version
1.0.0  
Last changed: 21.04.2023

### build
```bash
$devcmd> build /s2dE
```

### examples
```bash
$ NtStatus2DosError 0xC0000005
```



## String2Hex
Convert strings into hex or hex into string.

### version
1.0.0  
Last changed: 21.04.2023

### build
```bash
$devcmd> build /s2h
```

### Options
- -a Convert ascii string into hex.
- -u Convert unicode (utf-16) string into hex.
- -na Convert hex byte string into ascii string/chars. If preceded with '0x', it will be interpreted as an uint64 (i.e. mostly as little endian).
- -nu Convert hex byte string into unicode (utf-16) string/chars. If preceded with '0x', it will be interpreted as an uint64 (i.e. mostly as little endian).
- -f Number format of output. Used with the -a or -u option. 1: bytes, 2: uint16, 4: uint32, 8: uint64

### examples
```bash
$ String2hex -a bla // ascii string to hex uint8 => 62 6c 61
$ String2hex -a blub -f 2 // ascii string to hex uint16 => 6c62 6275
$ String2hex -u blob -f 4 // utf-16 string to hex uint32 => 006c0062 0062007f
$ String2hex -na 414243 // hex byte string into ascii => string: ABC, chars: A B C
$ String2hex -na 0x434241 // uint into ascii => string: ABC, chars: A B C (on little endian machines)
$ String2hex -nu 42006c006100 // hex byte string unicode => string: Bla, chars: B l a 
$ String2hex -nu 0x0061006c0042 // uint into unicode => string: Bla, wchars: B l a  (on little endian machines)
```


## Copyright, Credits & Contact
Published under [GNU GENERAL PUBLIC LICENSE](LICENSE).

### author
- Henning Braun ([henning.braun@fkie.fraunhofer.de](henning.braun@fkie.fraunhofer.de)) 
