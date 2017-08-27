# **COSMOS** _Docs_

## Virtual Machine

- #### Memory
 - Most of the microcomputers running CHIP-8 language had a 4KB memory. The original interpreter was stored in the first 512 bytes of the memory, (0x000 - 0x200). Also the uppermost 256 bytes were reserved for the display refresh.
  - In this implementation only the font data is stored in (0x000 - 0x04F) and the ROM is stored in (0x200 - 0xFFF).
- #### Stack
 - When a subroutine is called the return adress is stored here. It has 16 levels of nesting.

- #### Registers
 -  V Registers. There are 16 of them and they are 8-bit-sized. V0 -VF. VF acts as a carry/borrow flag or with the instruction DRAW as a pixel collision flag.
 - I Special Register. It is 16-bit-sized and is used in several memory operations.

- #### Timers
 - The timers count down at 60 Hz until they reach 0.
 - Sound Timer. A 'beep' sound is made when this timer is nonzero.
 - Delay Timer. It handles time events.

- #### [Opcodes](opcodes.md)
 - CHIP-8 has a total of 35 Opcodes

- #### [Display](display.md)
 - CHIP-8 has a monochrome 64x32 pixels display. Sprites are used to draw everything on the screen, and they are 8 pixels wide and its height is variable, up to 15 pixels.
 When the pixel is flipped from set to unset the register VF acts as a Carry Flag and its value is set to 1, otherwise is set to 0.
 -  _(In games VF Carry Flag acts as a pixel collision detector used in many games.)_


* * *

> Special thanks to [Cowgod's Chip-8 Technical Reference]()

* * *

#### Coded with <3 by ![](https://avatars3.githubusercontent.com/u/6288559?v=2&s=32) [LightningWright](https://github.com/LightningWright)
