# Test_DEV_0

- Collaborative repository implementing real-world test solutions in C/C++ 
across Linux-ARM platforms

Need to:

- Handle Data Streams (having issues getting valid ASCII data
when not testing locally)
- Create pthread test branch

Updates:
- CS_Build_v0.1 (master) -> Refactored Server instances to use only one common .h file - 111121
- CS_Build_v0.1 (master) -> No segmentation faults, but the data doesn't get replied back to client - 102821
- CS_Build_v0.1 (master) -> Need to allocate the memory for the struct elsewhere... - 102621
- CS_Build_v0.1 (master) -> Pass in struct as arg for thread handler (IP = success) - 101421
- CS_Build_v0.1 (master) -> FOR RPI4 DEPLOYMENT: USE COMMIT 1605a8b64d1681d9a21262de63761339984775a1
"Optimized Server code for all Server_vETHx instances - WORKING COMMIT (ready for deployment on RPi4 - 10142021-1330)"
- CS_Build_v0.1 (master) -> Pass in struct as arg for thread handler (start) - 100821
- CS_Build_v0.1 (master) -> Start copying code from working branch works now - 100621
- CS_Build_v0.1 (master) -> Reminder: Pass in structure as arg to thread handler - 100521
- CS_Build_v0.1 (revert-working-branch) -> Works...  - 100521
- CS_Build_v0.1 (master) -> Created a branch to revert to working version - 100421
- CS_Build_v0.1 (master) -> Problems with pre-compile conditionals - 100321
- CS_Build_v0.1 (master) -> Creating Multithreaded Client approach - 100121
- CS_Build_v0.1 (master) -> Creating Multithreaded Server approach - 100121
- CS_Build_v0.1 (master) -> Tested w/latest updates - 08262021
- CS_Build_v0.1 (master) -> Tested (string reply) from RPi4 to RPi4 over TCP/IP = OK 
- Merging RW-to-data-stream branch back to master, creating CS_Build_v0.1 (master)

Installation:
- Instructions in progress - 082021

Quick Notes:
- The data is replied only after running the Client code more than once?... 100521-0947