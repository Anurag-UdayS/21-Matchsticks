# 21 Matchsticks                     

> Author: Uday Shankar
> Date: 14/08/2025

21 Matchsticks is a Terminal-based game, written completely in standard C 
						(except `sleep` function, which is OS-dependent).
It includes various graphical elements, and is best supported in 
an ANSI-based terminal like `xterm` or `Windows Terminal`.

---

## OBJECTIVE:
You are up against the computer, who is not ready to lose and will make
fun of you for even trying.
It will never accept defeat and will go upto any extent...

The basic objective of the game is quite simple.
1. We have 21 matchsticks in the pool.
2. Each player can pick 1,2,3 or 4 matchsticks in their turn.
3. The player to pick the last matchstick loses.

Anyways, there are various easter eggs hidden within the game, that
makes it entertaining for, well, atleast a short period of time.

I mainly wanted to learn various features of C in the process of making 
this game. 

If you would like to help me improve the memory management or safety of 
the game, I would love your suggestions!

	
## Installation

### RUNNING IN WINDOWS:
1. Install Windows Terminal (or any ANSI-suppported terminal): 
		https://apps.microsoft.com/detail/9n0dx20hk701?hl=en-US&gl=US
2. Download the Game.
3. Double-click the game. Blue "Windows Defender" window opens.
4. Click ["More info"](./assets/windows/warn1.jpeg)
5. Click ["Run Anyway"](./assets/windows/warn2.jpeg)
6. See if Windows Terminal opens.
7. Enjoy!


### RUNNING IN UNIX-systems (MacOS, Linux, BSD)
1. Open your ANSI-supported Terminal.
2. Download the game.
3. Add execution permissions.
```bash
	sudo chmod +x /path/to/file.bin
```
4. Execute the game.
```bash
	exec /path/to/file.bin
```

### Build from source
```bash
curl https://raw.githubusercontent.com/Anurag-UdayS/21-Matchsticks/refs/heads/main/src.c -o "game.c"
gcc -lm -fsanitize=address -Wall -Wno-char-subscripts -o game.bin
sudo chmod +x game.bin
exec game.bin
```
