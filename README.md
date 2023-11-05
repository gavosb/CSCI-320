# CSCI-320
Assignments for CSCI-320: Operating Systems

# Project 4

This project defined a kernel function vmprint() and a syscall, pgaccess.

Files Modified:

Makefile
- referenced user/pgaccessTest.c

kernel/defs.h
- defined pgaccess

kernel/riscv.h
- defined PTE_A
  
kernel/vm.c
- implemented vmprint()
- implemented pgaccess(), returns buffer to user space
  
kernel/sysproc.c
- defined pgaccess syscall
  
user/user.h
user/usys.pl
- connected syscall to user space

user/pgAccessTest.c
- added

Output:

![output](https://github.com/gavosb/CSCI-320/assets/75707967/e86eb82b-0674-409c-b58f-2c5ddd199f34)
