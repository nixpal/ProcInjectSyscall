# Process Injection using Windows SYSCALLS
## Presented at D3FC0N 30
## Author: Tarek Ahmed @Nixpal

<img width="600" height="300" alt="dBi" src="https://github.com/nixpal/ProcInjectSyscall/blob/main/defcon.jpg">



### INFO

- All SYSCALL numbers used in the shellcode are for the latest 3 Windows 10 builds. They all use the same SYSCALL numbers.
- The C file is used to execute the complete shellcode by injecting meterpreter into a process called CrypTool.exe
- If you want a different process, please change the string accordingly in the inline assembly.
- The complete sandwiched shellcode is separated in three chunks to make it easy for you to modify the meterpreter part.
- If your meterpreter code is larger than mine, you need to modify the jmp instruction e9 86 01 00 00   jmp    0x194
- So if your meterpreter size is larger or smaller, change the opcodes to match whether 0x195, 0x193, 0x196 or whatever the beginning of the shellcode.



