# Intel 8086 disassembler
The disassembler converts machine code to Intel 8086/8088 instructions. It was written as a university project during the 1st semester of Software Engineering at Vilnius University (2022).

## Usage
The disassembler is designed for Turbo Assembler (TASM). It accepts input files in COM format. To generate a COM file, run the following commands:
```
tasm [INPUT_FILE].asm
tlink /t [INPUT_FILE].obj
```

Then disassemble it:
```
tasm disasm.asm
tlink disasm.obj
disasm.exe [INPUT_FILE].com [OUTPUT_FILE].asm
```