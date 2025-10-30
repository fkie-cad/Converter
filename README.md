# Converter
Various converter tools for daily tasks.




## Content
-  [Requirements](#requirements)
-  [Build](#build)
-  [CtlCode](#ctlcode)
-  [Num2Bin](#num2bin)
-  [NtStatus2DosError](#ntstatus2doserror)
-  [String2Arr](#string2arr)
-  [String2Hex](#string2hex)


## Requirements
- msbuild


## Build
```bash
$devcmd> build [/all]
```


## CtlCode
A ioctl code converter.
Converts a given ioctl code into its parts (DeviceType, Function, Method, Access), 
  or the parts into an ioctl code.

### version
1.0.2  
Last changed: 30.04.2024

### build
```bash
$devcmd> build /ctl
```

### examples
```bash
# convert ioctl code into parts
$ CtlCode.exe 0x10000
ioctl: 0x10000

deviceType: 0x1 (FILE_DEVICE_BEEP)
function: 0x0
method: 0x0 (METHOD_BUFFERED)
access: 0x0 (FILE_ANY_ACCESS)

# convert parts (<DeviceType> <Function> <Method> <Access>) given as ints into ioctl code.
$ CtlCode.exe 0x10 3 0 2
deviceType: 0x10 (FILE_DEVICE_MULTI_UNC_PROVIDER)
function: 0x3
method: 0x0 (METHOD_BUFFERED)
access: 0x2 (FILE_WRITE_DATA)

ioctl: 0x10800c

# convert parts (<DeviceType> <Function> <Method> <Access>) given as strings into ioctl code.
$ CtlCode.exe FILE_DEVICE_UCM 1 METHOD_NEITHER FILE_READ_DATA^|FILE_WRITE_ACCESS
deviceType: 0x57 (FILE_DEVICE_UCM)
function: 0x1
method: 0x3 (METHOD_NEITHER)
access: 0x3 (FILE_READ_DATA | FILE_WRITE_DATA)

ioctl: 0x57c007
```



## Num2Bin
A (decimal or hex) number to binary converter.
Converts a given number into binary format or a binary string to hex and decimal number.
Highlights selected bits.

### version
1.0.6  
Last changed: 30.10.2025

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



## String2Arr
Convert string into array of chars.

### version
1.0.0  
Last changed: 21.04.2023

### build
```bash
$devcmd> build /s2arr
```

### Usage
```
String2Arr <type> [-k <key>] string...
```

### Options
- type: 1: char, 2: wchar, 4: reversed
- -k: XOR encryption key given as a hex string
- strings: a set of strings to convert

### examples
```bash
$ String2Arr 1 bla // convert bla to char array
$ String2Arr 5 bla // convert bla to reversed char array
$ String2Arr 3 bla blub // convert bla and blub to char and wchar array
$ String2Arr 1 -k 41 bla // convert bla to xored char array and xor with 0x41
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
