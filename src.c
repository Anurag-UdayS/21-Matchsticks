// ==================================================================================== //
//                                    21 Matchsticks                                    //
// ==================================================================================== //
/*!
	@Author: Uday Shankar
	@Date: 14/08/2025

	21 Matchsticks is a Terminal-based game, written completely in standard C 
							(except `sleep` function, which is OS-dependent).
	It includes various graphical elements, and is best supported in 
	an ANSI-based terminal like `xterm` or `Windows Terminal`.

	OBJECTIVE:
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

	RUNNING IN WINDOWS:
		1. Install Windows Terminal (or any ANSI-suppported terminal): 
				https://apps.microsoft.com/detail/9n0dx20hk701?hl=en-US&gl=US
		2. Download the Game.
		3. Double-click the game. Blue "Windows Defender" window opens.
		4. Click "More info"
		5. Click "Run Anyway"
		6. See if Windows Terminal opens.
		7. Enjoy!
!*/
// ------------------------------------------------------------------------------------ //
/* 
Stress Test Checklist:
	F: Exit Safely -- done
	F: Player is a normie -- done
	SF: Player is a super normie -- done
	F: Normie wins -- done
	SF: Normie Rejection -- done
	F: Hide user input when "Press enter to continue..." -- done
	F: Reject Play -- done
	F: Computer Loses -- done
	F: Computer Wins from beginning -- done
	F: Computer Wins in the middle -- done
	F: Computer Wins at the very end -- done
*/

// ==================================================================================== //
//                                    Notes Section                                     //
// ==================================================================================== //
/*
	These notes have been taken while making the game and learning new tricks.
	They may help in understanding the code.

// ------------------------------------------------------------------------------------ //

	- Preprocessor: 
		- There are two operators for Preprocessors.
			Unary # is the Stringize operator. It escapes \s and creates "".
			#define R(a) #a;; R("\ asd \w") -> "\"\\asd\\w\""
		
		- Binary ## is the token concat operator. 
			It can use macro args or literal chars also.
			#define T(a,b) a##b##c=3;; T(abc, def) -> abcdefc=3;
			It does NOT use the computed value. Just the symbols.
			
// ------------------------------------------------------------------------------------ //

	- Syntax:
		- {} initializers are used ONLY on the declaration - initialization statement.
			They cannot be used for redefining arrays / structs, or defining them
			after they are declared.
			Example:
			
				int hello[2];
				hello = {1,2}; // ERROR

			We cannot pass it in-place.
			Example:
					int sum(int* nums);
					sum({1,2,3}) // ERROR;
					
		// NOTE: . 
		// NOTE: Also, format specifier for a pointer (or array variable) is %p.
		// So the following statements are valid:
		// int a[4] = {0};
		// 3[a] = 1;

		- To understand pre/post incrementation:
			Read i++ as "Post-return-increment", and ++i as "pre-return increment".

		- Avoid `strcat`, in general, for it loops through the array to count it 
				despite us usually already having the length.
				When you have the length known, use a manual copying functionality.

		- Funny story about concating literals...
			"asd" "bfg" will automatically concat the literal string...
			This causes lots of debugging mayhem.

		- Style Guide:
			- Use ALL CAPS for Enums and Macros ONLY.
				
// ------------------------------------------------------------------------------------ //

	- Tokens and Types:
		
		- In C, static variables have to be initialized with CONSTANT expressions.
			They cannot use runtime-function calls to retrieve values.
			This holds true for global-level variables too.

		- The only thing that can be outside of functions is declarations.
			`a[3] = 0;` would fail.

		- `extern` is used when objects exist across translation units (i.e. files)
			It does not create the object, only makes a link.
			Object needs to be created at exactly one place.

		- To make a constant array of constant strings, use: 
				`const char* const MESSAGES[nMSG];`
			Though, at the global level it is not really possible, 
			as we cannot initialize it.

		- `typedef` defines the last lexeme as an alias type for the preceding ones.
			You CANNOT attach additional types to typedefs.
			Example:

				typedef int integer;
				unsigned integer a; // ERROR.

		- Arrays and Pointers are (almost) equivalent.
			char a* = malloc(3);
			*(a+1) === a[1] === 1[a];

		- Enums:
			- If we already define an ENUM in enum type, then
				the same name cannot be used in another enum type.
				Exameple:
					`enum E1 {A, B}; enum E2 {A, B}; // Invalid`

			- Enumerations start by default at 0 and then just increment by 1, 
				unless explicit value is specified.

// ------------------------------------------------------------------------------------ //

	- Memory Management:
		- After free(), always set pointer to NULL to avoid double-freeing. 
			free(NULL) is safe.

		- free() takes `void*` but rejects `const void*`.
			void* implies unknown type pointer.
			We still need to free constant pointers.
			Casting to (void*) implies casting to an unknown type pointer.
			Hence, it is useful to free mallocated const *ptrs.

		- For allocating strings, ALWAYS use null-terminator.
			Hence, size is strlen + 1.

		-  STORAGE SEGMENTS: malloc() and calloc() allocate on heap. 
			The following go to stack:
				Local variables within functions 
					(not allocated by calloc() / malloc() / realloc())
				Function arguments

			Global variables go on a separate segment called `static storage segment`.
			It stores the data for execution during program.
			Like Stack and Heap, it's in RAM too.
			However, we don't need to bother freeing it.
				
// ------------------------------------------------------------------------------------ //	
*/



// ------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------ //



// ==================================================================================== //
//                                   Translation Unit                                   //
// ==================================================================================== //

#include <stdio.h>		// printf, getchar, putchar, puts, fprintf, fopen, fclose, 
						// stdin, stdout, stderr
											 
#include <stdlib.h>		// malloc, free, exit
#include <stdarg.h>		// va_list, va_arg, va_start, va_end
#include <stdbool.h>	// bool, true, false.

#include <string.h>		// strlen
#include <math.h>		// log10, srand, rand
#include <time.h>		// time

// sleep() is an os function. So we need to handle it with care.
#ifdef _WIN32
	#include <windows.h> // Sleep
	#define sleep(x) Sleep(x*1000)
	#define sleepms(x) Sleep(x)
	#define itoa(x) itoa_(x) // Apparently x86_64-w64-migw32-gcc's stdlib CONTAINS itoa...
#else
	#include <unistd.h> // sleep, sleepms
	#define sleepms(x) usleep(x)
#endif

#ifdef DEBUG
unsigned sleep(unsigned) {return 0;}
#undef sleepms
#define sleepms(x) sleep(x)
#endif

// ------------------------------------------------------------------------------------ //

// Macros
#define R(str) #str // Get the stringized value. Basically, load the rawstring.
#define INF_LOOP while(1)

// Preprocessor-level Constants
#define nMSG 32
#define nDANCES 8

// Debug Directive (Disabled)
// Use gcc -D DEBUG
// #define DEBUG

// ==================================================================================== //
//                                        Types                                         //
// ==================================================================================== //

// `char` is also a numeric type and for our purposes, it will save memory.
typedef char tiny; 

typedef enum {
	HUMAN = 0,
	COMPUTER = 1
} PLAYER;

// These make it easy to find the correct messages, instead of remembering magic values.
// Also makes alterations handled well.
typedef enum {
	// 7
	emj_smile,
	emj_happy,
	emj_angry,
	emj_evil,
	emj_taunt,
	emj_sad,
	emj_cry,

	// 5
	greeting,
	alt_greeting,
	mode_choice,
	nerfed_mode_choice,
	color_choice_msg,

	// 4
	goodbye,
	normie,
	guts,
	true_normie,

	// 5
	invalid_choice,
	invalid_choice_guts,
	good_choice,
	bad_choice,
	get_out,

	// 6
	plr_pref_choice,
	plr_choice_4,
	plr_choice_3,
	plr_choice_2,
	plr_choice_1,
	cmp_choice,

	// 5
	dance_msg,
	replay,
	true_normie_win,
	true_normie_loss,
	normie_max
		
} MESSAGE_IDX ;

// ------------------------------------------------------------------------------------ //

// Foreground colors for ANSI terminal.
typedef enum {
	FG_DEFAULT = 0,
	FG_BLACK = 30,
	FG_RED = 31,
	FG_GREEN = 32,
	FG_YELLOW = 33,
	FG_BLUE = 34,
	FG_PURPLE = 35,
	FG_CYAN = 36,
	FG_WHITE = 37,
	FG__RGB = 38,
	FG_BRIGHT_BLACK = 90,
	FG_BRIGHT_RED = 91,
	FG_BRIGHT_GREEN = 92,
	FG_BRIGHT_YELLOW = 93,
	FG_BRIGHT_BLUE = 94,
	FG_BRIGHT_PURPLE = 95,
	FG_BRIGHT_CYAN = 96,
	FG_BRIGHT_WHITE = 97
} FG_COLOR;

// Background colors for ANSI terminal.
typedef enum {
	BG_DEFAULT = 0,
	BG_BLACK = 40,
	BG_RED = 41,
	BG_GREEN = 42,
	BG_YELLOW = 43,
	BG_BLUE = 44,
	BG_PURPLE = 45,
	BG_CYAN = 46,
	BG_WHITE = 47,
	BG__RGB = 48,
	BG_BRIGHT_BLACK = 100,
	BG_BRIGHT_RED = 101,
	BG_BRIGHT_GREEN = 102,
	BG_BRIGHT_YELLOW = 103,
	BG_BRIGHT_BLUE = 104,
	BG_BRIGHT_PURPLE = 105,
	BG_BRIGHT_CYAN = 106,
	BG_BRIGHT_WHITE = 107
} BG_COLOR;

// Special effect characters for ANSI terminal.
typedef enum {
	RESET = 0,
	BOLD = 1,
	FAINT = 2,
	ITALIC = 3,
	UNDERLINE = 4,
	BLINK = 5,
	RAPID_BLINK = 6,
	REVERSE = 7,
	HIDE = 8,
	STRIKE = 9
} MODIFIER;



// ==================================================================================== //
//                                      Constants                                       //
// ==================================================================================== //

MODIFIER *modheavy, *modlight;
const char *MESSAGES[nMSG], *DANCES[nDANCES];
const char *SMILE = "\U0001F600", *TONGUE = "\U0001F61B"; // Unicode Emojis.
short ncache = 0;
tiny normieness = 0;
void *cache[512];

// I've obfuscated these >:). 
// TOOL: ChatGPT
const long TARGETS[5] = {
	(0xAB12CDL ^ 0xAB12CDL ^ (0x123450L & 0x000000L) ^ 0x0F1E2DL ^ 0x0F1E2DL ^ (0x000001L | 0x000000L)),
	(0x1A2B3CL ^ 0x1A2B3CL ^ (0x123456L & 0x000000L) ^ 0xABCDEFL ^ 0xABCDEFL ^ (0x000005L | 0x000000L)),
	(0xC001D0L ^ 0xC001D0L ^ (0x654321L & 0x000000L) ^ 0xF0E1D2L ^ 0xF0E1D2L ^ (0x00000AL | 0x000000L)),
	(0x0FABCDL ^ 0x0FABCDL ^ (0xDEAD10L & 0x000000L) ^ 0x111222L ^ 0x111222L ^ (0x00000FL | 0x000000L)),
	(0xA1B2C3L ^ 0xA1B2C3L ^ (0xB2C3D4L & 0x000000L) ^ 0xC3D4E5L ^ 0xC3D4E5L ^ (0x000014L | 0x000000L))
};



// ==================================================================================== //
//                                 Function Prototypes                                  //
// ==================================================================================== //

// String Functions
char* itoa(int i);
char* joinstr(tiny n, ...);
char* trimquotes(const char rawstr[]);
char* emojify(char* rawstr, const char* emoji, char echar);
char* strnice(const char* rawstr, FG_COLOR fg, BG_COLOR bg, MODIFIER* mod, tiny nmod);

// ------------------------------------------------------------------------------------ //

// MEMORY
void _gc(void);
void _gc_full_(void);

// ------------------------------------------------------------------------------------ //

// Declaration
void setmessages(void);
void setmessages_customcolor(FG_COLOR player_color, FG_COLOR computer_color);

// ------------------------------------------------------------------------------------ //

// Terminal I/O
tiny getn(void);
void cls(void);
void loading(tiny nloops, const char* loading_txt, const char* dots, bool newln);
void printsticks(tiny choices[21], tiny choice_sum, FG_COLOR player_color, FG_COLOR computer_color);
void dance(const char *message, tiny nloop);

// ------------------------------------------------------------------------------------ //

// Game Functionality
tiny computer_choose(tiny choice_sum, bool random); 
void REFUSE(void);
void NORMIE(void);
void wrong_input(bool guts);
void normal_mode(void);
void impossible_mode(bool is_true_normie);

// ------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------ //



// ==================================================================================== //
//                          Function Implementations - Helpers                          //
// ==================================================================================== //

// ------------------------------------------------------------------------------------ //
//                           Subsection: String Functionality                           //
// ------------------------------------------------------------------------------------ //

char* itoa(int i) {
	/* 
		Converts an integer (i) to a string (a).

		@param int i:	The integer to convert to string.
		@return char*:	The required string.
	*/
	
	tiny digs = log10(i) + 1;
	char* a = malloc(digs + 1);
	for (tiny n = digs - 1; n >= 0; n--) {
		*(a + n) = (i % 10) + '0';
		i /= 10;
	}
	*(a + digs) = 0;
	cache[ncache++] = a;
	return a;
}

// ------------------------------------------------------------------------------------ //

char* joinstr(tiny n, ...) {
	/*
		Gets the concatenated string. 
		Substitute of `strcat`.
		
		Note that the use of pointer notation 
			*(ptr + i) = val;
		is intentional for demonstrating its capabilities. 
		It is not a good practice and the sugar
			ptr[i] = val;
		is preferred.

		@param tiny n: 	Number of strings to concatenate.
		@vararg: 		Said strings to concatenate.
		@return char*: 	Concatenated string.
	*/

	va_list args;
	va_start(args, n); // Initialize varargs to va_list. It stores all varargs.
	
	int len = 0, idx = 0;
	for (tiny _ = 0; _<n; _++) {
		char* str = va_arg(args, char*);

		#ifdef DEBUG
			fprintf(stderr, "Concatting string: %s\n", str);
		#endif
		if (!str) continue;
		len += strlen(str);
	};
	
	char* finalstr = malloc(len + 1); // +1 for NULL

	// Restart varargs. Starting without ending causes undefined behaviour.
	va_end(args);
	va_start(args, n);

	// va_arg retrieves and casts arg. va_list should not be indexed.
	for (tiny _ = 0; _<n; _++) {
		char* str = va_arg(args, char*);
		if (!str) continue;
		char f;
		int c = 0;

		// Read till NULL terminator.
		do *(finalstr + idx++) = f = *(str + c++); while (f);
		idx--; // The last 0 also gets added. So we overwrite it in the next loop.
	}
	// *(finalstr + idx) = 0; // NULL terminator of string.
	va_end(args);
	return finalstr; 
}

// ------------------------------------------------------------------------------------ //

char* trimquotes(const char rawstr[]) {
	/*
		Removes the quotes of a rawstring.
		Useful after the #str macro. 

		@param const char rawstr[]: Raw string, provided by the macro.
		@return char*:	Trimmed string.
	*/
	
	int len = strlen(rawstr) - 2;
	char *trimstr = malloc(len + 1);

	for (int i = 1; i <= len; i++) trimstr[i - 1] = rawstr[i];
	trimstr[len] = 0; // NULL terminator of string.

	cache[ncache++] = trimstr; // Caching for GC.
	return trimstr; 
}

// ------------------------------------------------------------------------------------ //

char* emojify(char* rawstr, const char* emoji, char echar) {
	/*
		Replaces a certain character with provided unicode emoji. 

		@param char* rawstr: String in which we wish to replace the emoji.
		@param const char* emoji:	The Emoji string.
		@param char echar:	 The character which gets replaced by the emoji.
		@return char*:		 Emojified string.
	*/
	
	int n = strlen(rawstr), ne = strlen(emoji);
	char *emojified = malloc(n + ne);
	char c;
	int r = 0, ef = 0;
	
	while (r < n) 
		if ((c = rawstr[r++]) != echar) emojified[ef++] = c; // Copying char.
			// Copying emoji.
		else for (int e = 0; e < ne; e++, ef++) emojified[ef] = emoji[e];
		 
	emojified[ef] = 0; // NULL terminated string.
	
	cache[ncache++] = emojified; // Caching for GC.
	return emojified;
}

// ------------------------------------------------------------------------------------ //

char* strnice(const char* rawstr, FG_COLOR fg, BG_COLOR bg, MODIFIER* mod, tiny nmod) {
	/*
		Colorizes and applies special effects to strings.
		Based on ANSI escape sequences.
		
		@param char* rawstr:	Plain String.
		@param FG_COLOR fg:		ANSI Foreground Color.
		@param BG_COLOR bg:		ANSI Background Color.
		@param MODIFIER* mod:	List of ANSI special effects.
		@return char*:			Colorized string.
	*/

	// Invalid Case,
	if (!fg && !bg && !nmod) {
		char* raw = joinstr(1, rawstr);	
		cache[ncache++] = raw; // Caching for GC
		return raw;
	}

	int rawlen = strlen(rawstr);
	tiny esclen = 3; // \033[m

	// !! converts non-zero value to 1.
	tiny ncolor = !!fg + !!bg;
	tiny nsemi = ncolor + nmod - 1;

	// +1 for 0 in \033[0m; +1 for NULL.
	// \033[1;7;31;42mHello\033[0m
	char* filled = malloc(esclen*2 + ncolor*2 + nmod + nsemi + rawlen + 1 + 1); 
	int i = 0, c;
	char f;
	filled[i++] = 033;
	filled[i++] = '[';

	// Add fg and bg colors.
	if (fg) {
		char* fgstr = itoa(fg);
		c = 0;	

		// Copy till NULL terminator.
		do filled[i++] = f = fgstr[c++]; while (f);
		filled[i-1] = ';'; // Replace NULL byte 0;
	}

	if (bg) {
		char* bgstr = itoa(bg);
		c = 0;
		
		// Copy till NULL terminator.
		do filled[i++] = f = bgstr[c++]; while (f);
		filled[i-1] = ';'; // Last NULL is saved above. So we overwrite it.
	}

	
	// Deal with modifiers separately.
	for (short idx = 0; idx < nmod; idx++) {
		filled[i++] = mod[idx] + '0';
		filled[i++] = ';';
	}

	// Replace last ; with m.
	filled[i - 1] = 'm';

	// Add remaining string. 
	c = 0;
	while (c < rawlen) filled[i++] = rawstr[c++];

	// Put closing formatter.
	filled[i++] = 033;
	filled[i++] = '[';
	filled[i++] = '0';
	filled[i++] = 'm';
	filled[i++] = 0;
	
	cache[ncache++] = filled; // Caching for GC.
	return filled;
}


// ------------------------------------------------------------------------------------ //
//                            Subsection: Memory Management                             //
// ------------------------------------------------------------------------------------ //

void _gc(void) {
	/*
		Cleares the cached pointers.
	*/ 

	#ifdef DEBUG
		fprintf(stderr, "Attempting to clear cache:\n");
		for (short n = 0; n < ncache; n++) fprintf(stderr, "\t[%d]: %p\n", n, cache[n]);
	#endif
	
	for (short n = 0; n < ncache; n++) {
		// TODO Memleak here.
		free(cache[n]);
		cache[n] = NULL;
	}

	#ifdef DEBUG
		fprintf(stderr, "CLEARED CACHE (%d ITEMS)\n", ncache);
	#endif
	ncache = 0;
}

// ------------------------------------------------------------------------------------ //

void _gc_full_(void) {
	/*
		Cleares all pointers in memory. Used before exiting.
	*/ 
	_gc();
	free(modheavy);
	free(modlight);
	for (tiny i = 0; i < nMSG; i++) {
		#ifdef DEBUG
			fprintf(stderr, "Attempting to free MESSAGES[%i]...\n", i);
		#endif
		char* msg = (char*) MESSAGES[i];
		if (msg) free(msg);
	}

	// Hardcoded. Will be modified if changed later.
	free((void*) DANCES[0]);
	free((void*) DANCES[1]);
	free((void*) DANCES[4]);
	free((void*) DANCES[5]);
}

// ------------------------------------------------------------------------------------ //

void setmessages(void) {
	/*
		Sets the messages that need to be displayed.
		Basically a declaration, to be performed at the start of main();
	*/
		
	// -------------------------------------------------------------------------------- //
	//                                 Portion: Dances                                  //
	// -------------------------------------------------------------------------------- //
	/*
		When the computer wins, it will dance and taunt the player.
		These strings are used to set said dance poses.
		Dance Sequence: L R L R P O P O 
	*/

	
	char* pose_L = joinstr(7*2,
	                            "_          "     ,              "\n",
		emojify(trimquotes(R(   " \   O     "   )), SMILE, 'O'), "\n",
		                        "  ---|---  "     ,              "\n",
		        trimquotes(R(   "     |   \_"   )),              "\n",
		                        "     |     "     ,              "\n",
		        trimquotes(R(   "    / \    "   )),              "\n",
		        trimquotes(R(   "   /   \   "   )),              "\n"	
	);
	char* pose_R = joinstr(7*2,
		                        "           _"    ,             "\n",
		emojify(trimquotes(R(   "     O   / "   )), SMILE, 'O'), "\n",
		                        "  ---|---  "     ,              "\n",
		        trimquotes(R(   "_/   |     "   )),              "\n",
		                        "     |     "     ,              "\n",
		        trimquotes(R(   "    / \    "   )),              "\n",
		        trimquotes(R(   "   /   \   "   )),              "\n"	
	);
	char* pose_P = joinstr(7*2, 
	                            "_          _"    ,              "\n",
		emojify(trimquotes(R(   " \   P   / "   )), TONGUE, 'P'),"\n",
		                        "  ---|---  "     ,              "\n",
		        trimquotes(R(   "     |     "   )),              "\n",
		                        "     |     "     ,              "\n",
		        trimquotes(R(   "    / \    "   )),              "\n",
		        trimquotes(R(   "   /   \   "   )),              "\n"	
	);
	char* pose_O = joinstr(7*2,
	                            "_          _"    ,              "\n",
		emojify(trimquotes(R(   " \   O   / "   )), SMILE, 'O'), "\n",
		                        "  ---|---  "     ,              "\n",
		        trimquotes(R(   "     |     "   )),              "\n",
		                        "     |     "     ,              "\n",
		        trimquotes(R(   "    / \    "   )),              "\n",
		        trimquotes(R(   "   /   \   "   )),              "\n"	
	);
	
	DANCES[0] = pose_L;
	DANCES[1] = pose_R;
	DANCES[2] = pose_L;
	DANCES[3] = pose_R;
	
	DANCES[4] = pose_P;
	DANCES[5] = pose_O;
	DANCES[6] = pose_P;
	DANCES[7] = pose_O;

	// Many objects have built up in cache. Good idea to clear them here.
	_gc();

	// -------------------------------------------------------------------------------- //
	//                                Portion: Messages                                 //
	// -------------------------------------------------------------------------------- //
	/*
		Many messages exist in the game that are displayed repeatedly.
		This will store them.
	*/

	MESSAGES[emj_smile] = joinstr(1, strnice(":)", FG_GREEN, BG_DEFAULT, modheavy, 1));
	MESSAGES[emj_happy] = joinstr(1, strnice(":D", FG_GREEN, BG_DEFAULT, modheavy, 1));
	MESSAGES[emj_angry] = joinstr(1, strnice(">:(", FG_RED, BG_DEFAULT, modheavy, 1));
	MESSAGES[emj_evil]  = joinstr(1, strnice(">:)", FG_PURPLE, BG_DEFAULT, modheavy, 1));
	MESSAGES[emj_taunt] = joinstr(1, strnice(":P", FG_YELLOW, BG_DEFAULT, modheavy, 1));
	MESSAGES[emj_sad]   = joinstr(1, strnice(":(", FG_RED, BG_DEFAULT, modheavy, 1));
	MESSAGES[emj_cry]   = joinstr(1, strnice(":'(", FG_BLUE, BG_DEFAULT, modheavy, 1));

	#ifdef DEBUG
		fprintf(stderr, "Saved Emojis.\n");
		fprintf(stderr, "Like this emoji: %s\n", strnice(":)", FG_GREEN, BG_DEFAULT, modheavy, 1));
	#endif

	// -------------------------------------------------------------------------------- //

	MESSAGES[greeting] = joinstr(9, 
		"Hi! Welcome to my game! ", 
		MESSAGES[emj_smile],
		"\nLet me explain the rules...\n",
		"\t1. We have 21 matchsticks in the pool.\n",
		"\t2. Each player can pick 1,2,3 or 4 matchsticks in their turn.\n",
		"\t3. The player to pick the last matchstick loses.\n",
		strnice("Lets Begin! ", FG_GREEN, BG_DEFAULT, modheavy, 0),
		MESSAGES[emj_happy],
		"\n"	
	);

	MESSAGES[alt_greeting] = joinstr(10, 
		"... \n", 
		"Let me explain the rules again... ", 
		MESSAGES[emj_taunt], 
		"\n",
		"\t1. We have 21 matchsticks in the pool.\n",
		"\t2. Each player can pick 1,2,3 or 4 matchsticks in their turn.\n",
		"\t3. The player to pick the last matchstick loses.\n",
		strnice("Let's go again! ", FG_GREEN, BG_DEFAULT, modheavy, 0),
		MESSAGES[emj_taunt],
		"\n"
	);

	// -------------------------------------------------------------------------------- //
	
	MESSAGES[mode_choice] = joinstr(5,
		"Choose Mode:",
		"\n\t0. Exit",
		"\n\t",
		strnice("1. Normal Mode", FG_GREEN, BG_WHITE, modlight, 1), 
		strnice("\n\t2. IMPOSSIBLE MODE\n", FG_RED, BG_DEFAULT, modheavy, 2)
	);

	MESSAGES[nerfed_mode_choice] = joinstr(3,
		"Choose Mode:",
		"\n\t0. Exit",
		strnice("\n\t2. IMPOSSIBLE MODE\n", FG_RED, BG_DEFAULT, modheavy, 2)
	);

	MESSAGES[color_choice_msg] = joinstr(8, 
		"Let us choose our colors...\nMy Color is: ", 
		strnice("\n\t6. CYAN", FG_CYAN, BG_DEFAULT, modheavy, 1),
		"\nChoose Yours:",
		strnice("\n\t1. RED", FG_RED, BG_DEFAULT, modheavy, 1),
		strnice("\n\t2. GREEN", FG_GREEN, BG_DEFAULT, modheavy, 1),
		strnice("\n\t3. YELLOW", FG_YELLOW, BG_DEFAULT, modheavy, 1),
		strnice("\n\t4. BLUE", FG_BLUE, BG_DEFAULT, modheavy, 1),
		strnice("\n\t5. PURPLE", FG_PURPLE, BG_DEFAULT, modheavy, 1)
	);

	// -------------------------------------------------------------------------------- //

	MESSAGES[goodbye] = joinstr(7, 
		MESSAGES[emj_cry], 
		strnice(" So sorry to say goodbye!\n", FG_BLUE, BG_DEFAULT, modlight, 1),
		MESSAGES[emj_sad],
		strnice(" I see that you have work to attend to...\n", FG_RED, BG_DEFAULT, modlight, 1),
		MESSAGES[emj_happy],
		strnice(" Hope to see you again!\n", FG_GREEN, BG_DEFAULT, modlight, 1),
		"Bye~\n"
	);

	MESSAGES[normie] = joinstr(6, 
		MESSAGES[emj_taunt], 
		strnice(" HAHA LOSER!\n", FG_RED, BG_DEFAULT, modheavy, 1),
		strnice("This game is not for normies.", FG_GREEN, BG_WHITE, modlight, 2),
		"\n",
		MESSAGES[emj_evil],
		strnice("  CHOOSE IMPOSSIBLE MODE OR...", FG_RED, BG_WHITE, modheavy, 2)
	);

	MESSAGES[guts] = joinstr(2, 
		MESSAGES[emj_evil],
		strnice(" Yeah!! Now we're talking! Let's Go!", FG_CYAN, BG_DEFAULT, modheavy, 0)
	);

	MESSAGES[true_normie] = joinstr(8,
		MESSAGES[emj_taunt],
		strnice(" It seems that you ", FG_BLUE, BG_DEFAULT, modlight, 0),
		strnice("really", FG_RED, BG_DEFAULT, modheavy, 2),
		strnice(" want to be a ", FG_BLUE, BG_DEFAULT, modlight, 0),
		strnice("normie", FG_BRIGHT_BLACK, BG_WHITE, modheavy, 2),
		strnice(" so why would I stop you? ", FG_BLUE, BG_DEFAULT, modlight, 0),
		MESSAGES[emj_taunt],
		"\n"
	);
	
	// -------------------------------------------------------------------------------- //
	
	MESSAGES[invalid_choice] = joinstr(7, 
		MESSAGES[emj_sad], 
		strnice("\nYou chose a wrong input!\n", FG_CYAN, BG_DEFAULT, modheavy, 0),
		strnice("Please choose again.\n", FG_DEFAULT, BG_DEFAULT, modlight, 1),
		"\n",
		MESSAGES[emj_evil],
		strnice("CHOOSE IMPOSSIBLE MODE OR ...", FG_RED, BG_WHITE, modheavy, 2),
		"\n"
	);

	MESSAGES[invalid_choice_guts] = joinstr(4, 
		MESSAGES[emj_sad], 
		strnice(" You chose a wrong input!\n", FG_CYAN, BG_DEFAULT, modheavy, 0),
		strnice("Please choose again.\n", FG_DEFAULT, BG_DEFAULT, modlight, 1),
		"\n"
	);

	MESSAGES[good_choice] = joinstr(2,
		MESSAGES[emj_evil], 
		strnice(" Good Choice", FG_PURPLE, BG_DEFAULT, modheavy, 1)
	);
	
	MESSAGES[bad_choice] = joinstr(2, 
		MESSAGES[emj_angry],
		strnice(" You think you're smart huh?", FG_PURPLE, BG_DEFAULT, modheavy, 1)
	);
	
	MESSAGES[get_out] = joinstr(3,
		MESSAGES[emj_angry],
		strnice(" I won't be playing with you.\n", FG_PURPLE, BG_DEFAULT, modheavy, 1),
		strnice("GET OUT!", FG_RED, BG_DEFAULT, modheavy, 2)
	);

	// -------------------------------------------------------------------------------- //

	MESSAGES[dance_msg] = joinstr(3,
		MESSAGES[emj_evil],
		strnice(" HAHA LOSER!! I WON!", FG_PURPLE, BG_DEFAULT, modheavy, 2),
		"\n"
	);

	MESSAGES[replay] = joinstr(3,
		strnice("Let's Play Again! ", FG_BLUE, BG_DEFAULT, modheavy, 0),
		MESSAGES[emj_happy],
		"\n"
	);
	
	MESSAGES[true_normie_win] = joinstr(2,
		MESSAGES[emj_smile],
		strnice(" Congrats on your win! You truly deserve it", FG_GREEN, BG_DEFAULT, modlight, 0)
	);

	MESSAGES[true_normie_loss] = joinstr(4,
		MESSAGES[emj_taunt],
		"\n",
		MESSAGES[emj_evil],
		strnice("  JUST KIDDING!!!", FG_RED, BG_DEFAULT, modlight, 0)
	);

	MESSAGES[normie_max] = joinstr(2, 
		strnice("NORMIE", FG_BRIGHT_BLACK, BG_WHITE, modheavy, 2),
		"     "
	);
}

// ------------------------------------------------------------------------------------ //

void updatemessage(MESSAGE_IDX idx, char* msg) {
	/*
		Clears the messages if they exist and puts in a new one.

		@param MESSAGE_IDX idx:	Location of message in array.
		@param char* msg:		The new message to replace with.
	*/
	if (MESSAGES[idx]) free((void*) MESSAGES[idx]);
	MESSAGES[idx] = msg;
}

// ------------------------------------------------------------------------------------ //

void setmessages_customcolor(FG_COLOR player_color, FG_COLOR computer_color){
	/*
		Resets messages according to player and computer color.
		We use the helper `updatemessage` as messages need to be 
		re-created every time.

		@param FG_COLOR player_color:	Color chosen by player.
		@param FG_COLOR computer_color:	Color of computer.
	*/	

	updatemessage(plr_pref_choice, joinstr(4, 
		"\nDo you want to go first? ", 
		strnice("\n\t1. YES, I (human) will go first.", 
			player_color, BG_DEFAULT, modheavy, 1),
		strnice("\n\t2. NO, You (computer) will go first.", 
			computer_color, BG_DEFAULT, modheavy, 1),
		"\n"
	));

	updatemessage(plr_choice_4, joinstr(2,
		strnice("Now you get to pick certain number of sticks.\n\t", 
			player_color, BG_DEFAULT, modheavy, 0),
		strnice("Valid Choices: 1, 2, 3 or 4.\n", 
			player_color, BG_DEFAULT, modheavy, 1)
	));

	updatemessage(plr_choice_3, joinstr(2,
		strnice("Now you get to pick certain number of sticks.\n\t", 
			player_color, BG_DEFAULT, modheavy, 0),
		strnice("Valid Choices: 1, 2 or 3.\n", 
			player_color, BG_DEFAULT, modheavy, 1)
	));
		
	updatemessage(plr_choice_2, joinstr(2,
		strnice("Now you get to pick certain number of sticks.\n\t", 
			player_color, BG_DEFAULT, modheavy, 0),
		strnice("Valid Choices: 1 or 2.\n", player_color, BG_DEFAULT, modheavy, 1)
	));
		
	updatemessage(plr_choice_1, joinstr(2,
		strnice("Now you get to pick certain number of sticks.\n\t", 
			player_color, BG_DEFAULT, modheavy, 0),
		strnice("Valid Choices: ONLY 1.\n", player_color, BG_DEFAULT, modheavy, 1)
	));

	updatemessage(cmp_choice, joinstr(1,
		strnice("Now it's my turn to choose.\n\t", computer_color, BG_DEFAULT, modheavy, 0)
	));

};


// ------------------------------------------------------------------------------------ //
//                               Subsection: Terminal I/O                               //
// ------------------------------------------------------------------------------------ //

tiny getn(void) {
	/*
		Reads the first character from stdin and returns it as a tiny.

		@return tiny:	Numeric face value of character entered.
	*/ 
	char _, n = getchar();
	if (n == '\n' || n == EOF) return -1;
	while ((_ = getchar()) != '\n' && _ != EOF); // FLUSH stdin
	return n - '0';
}

// ------------------------------------------------------------------------------------ //

void cls(void){
	/*
		Clears the ANSI terminal, using escape sequences.
		These are the codes decided by ANSI.
			2J -> CLS;
			H -> RESET cursor to HOME;
	*/
	#ifndef DEBUG
		fflush(stdout);
		printf("\033[2J\033[H");
		fflush(stdout);
	#endif
}

// ------------------------------------------------------------------------------------ //

void loading(tiny nloops, const char* loading_txt, const char* dots, bool newln) {
	/*
		Streams the input. Shows the text and then loads the dots gradually.
		Fake loading screen.

		@param tiny nloops:		How many times to show the dots / ellipses after deleting.
		@param const char* loading_txt:		The text that is shown before the ellpises.
		@param const char* dots:			The ellipses. The dots to stream.
		@param bool newln:		Whether to append newline at the end.
	*/
	if (!nloops) return;
	tiny ndots = strlen(dots);
	printf("%s ", loading_txt);
	fflush(stdout);
	char load;
	
	while (nloops--) {
		for(tiny i = 0; i < ndots; i++) {
			load = dots[i];
			putchar(load);
			fflush(stdout);
			#ifndef DEBUG
				if (load == '.' || (load >= 'A' && load <= 'z')) sleep(1);
			#endif
		}

		// ANSI Escape codes to move cursor.
		// nD = Move n to the left. 0K = Delete from cursor to end of screen.	
		if (nloops) printf("\033[%hdD\033[0K", ndots), fflush(stdout); 
	}
	if (newln) putchar('\n'), fflush(stdout);
}

// ------------------------------------------------------------------------------------ //

void printsticks(tiny choices[21], tiny choice_sum, FG_COLOR player_color, FG_COLOR computer_color) {
	/*
		Used to print the sticks selected & remaining.
		Colors them as necessary.
	
		@param tiny choices[21]:		Choices of the player / computer.
		@param tiny choice_sum:			Sum of all choices.
		@param FG_COLOR player_color:	Color that describes the player.
		@param FG_COLOR computer_color:	Color that describes the computer.
	*/
		
	tiny cidx = 0, baridx = 0, choice;
	char *bars;
		
	while ((choice = choices[cidx]) && cidx < 21) {
		char bar; // Bar to use for current choice. Computer uses \, Player /, Unused |
		
		// Read masked bit to determine choice.
		FG_COLOR color_choice = choice & 0b1000 ? (bar='\\', computer_color) : (bar='/', player_color);
		choice &= 0b111; // Reject mask.

		bars = malloc(choice + 1);
		for(baridx = 0; baridx < choice; baridx++) bars[baridx] = bar;
		bars[baridx] = 0; // NULL terminator.
		
		printf(strnice(bars, color_choice, BG_DEFAULT, modheavy, 0));

		cidx++; // Next choice.
		free(bars);
	}

	// Unused bars.
	bars = malloc(21 - choice_sum + 1);	
	for(baridx = 0; baridx < 21 - choice_sum; baridx++) bars[baridx] = '|';
	bars[baridx] = 0; // NULL Terminator.
	puts(bars);
	free(bars);
	_gc();
}

// ------------------------------------------------------------------------------------ //

void dance(const char* message, tiny nloop) {
	/*
		Simple implementation of dancing mechanism.
		
		@param char* message:	Message to show above dancing man.
		@param tiny nloop: 		Number of times to loop dance.
	*/
	
	while (nloop--) {	
		for (int i = 0; i<nDANCES; i++) {
			cls();
			puts(message);
			puts(DANCES[i]);
			fflush(stdout);
			sleep(1);
		}
		cls();
	}
}

// ------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------ //



// ==================================================================================== //
//                          Function Implementations - Primary                          //
// ==================================================================================== //


// ------------------------------------------------------------------------------------ //
//                                  Subsection: Logic                                   //
// ------------------------------------------------------------------------------------ //

void REFUSE(void) {
	/*
		REFUSE to play with the player.
		Persistence is implemented using simple file I/O but can be extended.
	*/

	char* no = strnice("no", FG_RED, BG_DEFAULT, modheavy, 1);
	char* no_caps = strnice("NO", FG_RED, BG_DEFAULT, modheavy, 1);
	char* no_max = strnice("NO!  ", FG_RED, BG_DEFAULT, modheavy, 1);

	puts("");
	loading(1, no, ".....", true);
	sleep(1);
	loading(1, no_caps, "..........", true);
	sleep(1);

	for (unsigned short n = 10000; n--;) {
		printf(no_max);
		sleepms(25);
	};
	puts("");

	// Persistence: This enables us to REFUSE whenever player has won once.
	FILE* noplay = fopen("./noplay", "w");
	fputs("0", noplay);
	fclose(noplay);

	// GC;
	_gc_full_();

	cls();
	exit(0);
};

// ------------------------------------------------------------------------------------ //

void NORMIE(void) {
	/*
		Callout the NORMIE!!
		Persistence is implemented using simple file I/O but can be extended.
	*/
	const char* normax = MESSAGES[normie_max];
	loading(1, "You, you are a ", normax, true);
	for (unsigned short n = 10000; n--;) {
		printf(normax);
		sleepms(25);
	};
	puts("");

	// Persistence: This enables us to REFUSE whenever player has won once.
	FILE* norfile = fopen("./normie", "w");
	fputs("0", norfile);
	fclose(norfile);

	// GC
	_gc_full_();
	cls();
	exit(0);
};

// ------------------------------------------------------------------------------------ //

tiny computer_choose(tiny choice_sum, bool random) {
	/*
		Algorithm for the best possible choice.

		@param tiny choice_sum:	Current Sum of all choices made by both players.
		@return tiny:			Next choice of computer.
	*/


	srand(time(0)); // SEED random.
	tiny max = (21 - choice_sum < 4) ? 21 - choice_sum : 4;
	tiny target, tidx = 0;
	tiny random_choice = rand() % max + 1;

	// NORMIE
	if (random) return random_choice;

	// NOT NORMIE
	if (choice_sum == 20) REFUSE(); // Computer is about to lose.
	
	// Find the next target.
	while (tidx < 5 && (target = (tiny) TARGETS[tidx]) <= choice_sum) tidx++;
	
	tiny choice = target - choice_sum;
	if (choice >= 5) {
		printf(MESSAGES[emj_angry]);
		return random_choice;
	}

	printf(MESSAGES[emj_evil]);
	return choice;
}


// ------------------------------------------------------------------------------------ //
//                               Subsection: Subroutines                                //
// ------------------------------------------------------------------------------------ //

void wrong_input(bool guts) {
	/* 
		#subroutine
		Error message shown to user when invalid option chosen.

		@param bool guts:	Whether player has guts (i.e. has chosen Impossible mode).
	*/
	cls();
	printf(MESSAGES[guts ? invalid_choice_guts : invalid_choice]);
	sleep(3);
	cls();
}

// ------------------------------------------------------------------------------------ //

void normal_mode(void) {
	/* 
		#subroutine
		Function to run when `normal` is chosen by user.
	*/
	
	// Mask the bits.
	// If player enters normal_mode 5 times (despite program saying it doesn't exist)
	// Player actually gets to play normal mode.
	if (!normieness) normieness = 1;
	normieness <<= 1;
	if (normieness & 0b100000) {
		normieness >>= 1; // Once a normie, always a normie.
		return impossible_mode(true);
	}
	else if (normieness & 0b011100) return wrong_input(false);
	
	cls();
	loading(3, "Loading Normal Mode sources", "...", true);
	loading(1, "Normalizing Vectors", "...", true);
	loading(1, "Finding Normals of all circles in sight", "...", true);
	loading(5, "Running Heavy (but normal) Code", "...", true);
	loading(1, "Almost There", "........", true);
	sleep(1);
	cls();
	sleep(3);
	printf(MESSAGES[normie]);
	sleep(7);
	cls();
}

// ------------------------------------------------------------------------------------ //

void impossible_mode(bool is_true_normie) {
	/* 
		#subroutine
		Function to run when `impossible` is chosen by user.
		This ensures computer always(?) wins.
	*/

	if (is_true_normie) {
		puts(MESSAGES[true_normie]);
		sleep(4);
		cls();
	}

	cls();
	if (!is_true_normie) {
		puts(MESSAGES[guts]);
		loading(1,strnice("LOADING IMPOSSIBLE MODE", FG_PURPLE, BG_DEFAULT, modheavy, 1), "......", 1);
		sleep(2);
		_gc();
	}
	cls();

	
	tiny choices[21] = {0}, choice_sum = 0, cidx = 0;
	tiny color_choice, plrchoice; 
	bool start_with_computer;
	PLAYER currentplr;

	// Player selects color.
	INF_LOOP {
		puts(MESSAGES[color_choice_msg]);
		#ifdef DEBUG
			fprintf(stderr, "HERE\n");
		#endif
		printf("Choice: ");
		color_choice = getn();
		if (color_choice > 0 && color_choice <= 5) break;
		else wrong_input(true); 
	}

	FG_COLOR player_color = color_choice + 30;
	FG_COLOR computer_color = FG_CYAN;
	setmessages_customcolor(player_color, computer_color);

	// Player order preference. Whether player or computer goes first.
	pref_loop: INF_LOOP {
		puts(MESSAGES[plr_pref_choice]);
		printf("Choice: ");
		start_with_computer = getn() - 1;
		switch((tiny) start_with_computer) {
			case HUMAN:
			case COMPUTER:
				break pref_loop;
			default:
				wrong_input(true);
				break;
		}
	}

	if (!is_true_normie) {
		if (start_with_computer) puts(MESSAGES[bad_choice]);
		else puts(MESSAGES[good_choice]);
		sleep(2);
	}
	
	currentplr = (start_with_computer ? COMPUTER : HUMAN); // Unnecessary but discrete.
	while (choice_sum < 21) {
		cls();
		if (currentplr == HUMAN) {
			tiny remaining = 21 - choice_sum;

			// Get player choice.
			INF_LOOP {
				#ifdef DEBUG 
					printf("Sticks Collected: %d\t", choice_sum);
				#endif
				printf("Sticks Remaining: %d\t\t\t\tSticks:\t", remaining);
				printsticks(choices, choice_sum, player_color, computer_color);
				puts("");

				// Prompt w/ valid choices.
				if (remaining == 3) puts(MESSAGES[plr_choice_3]);
				else if (remaining == 2) puts(MESSAGES[plr_choice_2]);
				else if (remaining == 1) puts(MESSAGES[plr_choice_1]);
				else puts(MESSAGES[plr_choice_4]);

				printf("Choice: ");
				plrchoice = getn();

				if (plrchoice <= 0 || plrchoice > 4 || plrchoice + choice_sum > 21) {
					wrong_input(true); 
					cls(); 
					continue;
				}
				break;
			}

			choices[cidx++] = plrchoice;
			choice_sum += plrchoice;

			// Switch Player
			currentplr = !currentplr; 
			
		} else if (currentplr == COMPUTER) {
			printf("Sticks Remaining: %d\t\t\t\tSticks:\t", 21 - choice_sum);
			printsticks(choices, choice_sum, player_color, computer_color);
			puts("");
			printf(MESSAGES[cmp_choice]);

			char* ichoose = strnice("I Choose", computer_color, BG_DEFAULT, modheavy, 0);
			plrchoice = computer_choose(choice_sum, is_true_normie); // May REFUSE if needed. Random for normies.
			puts("");
			loading(1, ichoose, ".....", false);
			printf(" %s", strnice(itoa(plrchoice), computer_color, BG_DEFAULT, modheavy, 1));

			// Hide the user input.
			printf("\nPress Enter to continue...\033[%hdm", HIDE);
			getn(); // FLUSH
			puts("\033[0m");

			choices[cidx++] = plrchoice | 0b1000; // MASKing computer inputs.
			choice_sum += plrchoice;

			// Switch Player
			currentplr = !currentplr;
			
			// We are creating strings here; good idea to GC. 
			_gc();
		}
	}

	
	if (choice_sum == 21 && currentplr == COMPUTER) {
		// Computer has won! 
		dance(MESSAGES[dance_msg], 3);
		puts(MESSAGES[replay]);
		loading(1,"Resetting", "...", true);
		_gc();
		cls();
	} else if (choice_sum == 21 && currentplr == HUMAN) {
		loading(1, MESSAGES[true_normie_win], "..........", true);
		cls();
		sleep(1);
		puts(MESSAGES[true_normie_loss]);
		sleep(2);
		NORMIE();
	}
}
// ------------------------------------------------------------------------------------ //
// ------------------------------------------------------------------------------------ //


// |==================================================================================| //
// |==================================================================================| //
// |                                       MAIN                                       | //
// |==================================================================================| //
// |==================================================================================| //

int main(void) {

	#ifdef DEBUG
		fprintf(stderr, "DEBUG MODE ON.\n");
	#endif

	// Initialization
	modheavy = malloc(sizeof(MODIFIER) * 2);
	*modheavy = BOLD;
	modheavy[1] = UNDERLINE;
		
	modlight = malloc(sizeof (MODIFIER) * 2);
	*modlight = FAINT;
	modlight[1] = STRIKE;

	setmessages();

	// If said file exists, then computer will REFUSE to play.
	FILE* noplay = fopen("./noplay", "r");
	if (noplay) {
		fclose(noplay);
		cls();
		sleep(1);
		puts(MESSAGES[get_out]);
		sleep(5);
		cls();
		_gc_full_();
		exit(0);
	}

	// Same with normie.
	FILE* norfile = fopen("./normie", "r");
	if (norfile) {
		fclose(norfile);
		cls();
		sleep(1);
		NORMIE();
		cls();
		_gc_full_();
		exit(0);
	}
	
	cls();
	tiny choice;
	bool started = false;
		
	game: INF_LOOP {
		// Greeting. If started, greet again.
		started ? puts(MESSAGES[alt_greeting]) : puts(MESSAGES[greeting]);
		if (!started) started = true;

		puts(MESSAGES[normieness ? nerfed_mode_choice : mode_choice]);
		fflush(stdout);
		printf("Choice: ");
		choice = getn();	
		switch(choice) {
			case 0:
				break game;
			case 1:
				normal_mode();
				break;
			case 2:
				impossible_mode(false);
				break;
			default:
				wrong_input(false);
				break;
		}
	}
	cls();
	if (choice == 0) { // Discrete
		puts(MESSAGES[goodbye]);
		fflush(stdout);
		sleep(5);
		cls();
	}
	_gc_full_();
	return 0;
}
