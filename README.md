# RunningShellCode

RunningShellCode.exe forces the specific binary to run in the debugger.
The debugger(eg. ollydbg, x64dbg, etc ...) must enable a JIT(just-in-time) mode.

RunningShellCode.exe wrties 0xCC(software break) before the specific binary that you pass as an argument.
Then, the debugger with JIT mode set will be stopped in front of the binary.
You can do the analysis binary in the debugger.

USAGE: RunningShellCode.exe [ToExecuteShellCode]

