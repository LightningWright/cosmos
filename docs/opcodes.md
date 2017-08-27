# **COSMOS** _Docs_

## Virtual Machine
### Opcodes


| OPCODE|                 | Description |
|-------|---------------|-----|
| 00E0  | CLS           |Clear the display|
| 00EE  | RET           |Return from a subroutine|
| 0NNN  | SYS  NNN      |Jump to a machine code routine at NNN|
| 1NNN  | JP    NNN     |Jump to location NNN|
| 2NNN  | CALL  NNN     |Call subroutine at NNN|
| 3XNN  | SE  VX, NN    |Skip next instruction if VX = NN|
| 4XNN  | SNE VX, NN    |Skip next instruction if VX != NN|
| 5XY0  | SE  VX, VY    |Skip next instruction if VX = VY|
| 6XNN  | LD  VX, NN    |Set VX = NN|
| 7XNN  | ADD VX, NN    |Set VX = VX + NN|
| 8XY0  | LD  VX, VY    |Set VX = VY|
| 8XY1  | OR  VX, VY    |Set VX = VX OR VY|
| 8XY2  | AND VX, VY    |Set VX = VX AND VY|
| 8XY3  | XOR VX, VY    |Set VX = VX XOR VY|
| 8XY4  | ADD VX, VY    |Set VX = VX + VY, set VF = carry|
| 8XY5  | SUB VX, VY    |Set VX = VX - VY, set VF = NOT borrow|
| 8XY6  | SHR VX  {, VY}|Set VX = VX >> 1|
| 8XY7  | SUBN  VX, VY  |Set VX = VY - VX, set VF = NOT borrow|
| 8XYE  | SHL VX {, VY} |Set VX = VX << 1|
| 9XY0  | SNE VX, VY    |Skip next instruction if VX != VY|
| ANNN  | LD  I,  NNN   |Set I = NNN|
| BNNN  | JP  V0, NNN   |Jump to location NNN + V0|
| CXNN  | RND VX, NN    |Set VX = random byte AND NN|
| DXYN  | DRW VX, VY, N |Display n-byte sprite starting at memory location I at (VX, VY), set VF = collision|
| EX9E  | SKP VX        |Skip next instruction if key with the value of VX is pressed|
| EXA1  | SKNP  VX      |Skip next instruction if key with the value of VX is not pressed|
| FX07  | LD  VX, DT    |Set VX = delay timer value|
| FX0A  | LD  VX, K     |Wait for a key press, store the value of the key in VX|
| FX15  | LD  DT, VX    |Set delay timer = VX|
| FX18  | LD  ST, VX    |Set sound timer = VX|
| FX1E  | ADD I,  VX    |Set I = I + VX|
| FX29  | LD  F,  VX    |Set I = location of sprite for digit VX|
| FX33  | LD  B,  VX    |Store BCD representation of VX in memory locations I, I+1, and I+2|
| FX55  | LD  I,  VX    |Store registers V0 through VX in memory starting at location I|
| FX65  | LD  VX, I     |Read registers V0 through VX from memory starting at location I|

* * *
