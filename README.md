# u8c  [![Build Status](https://travis-ci.org/rdentato/u8c.svg?branch=master)](https://travis-ci.org/rdentato/u8c) 

Fast validating utf-8 encoder/decoder for C

## API

```
 int u8next(char *s [, int *c]) --> Returns the number of bytes encoding the first
                                    codepoint in the string s.
                                    If c is not NULL, stores the codepoint in *c.
                                    If the encoding is not valid, returns -1 and stores
                                    in *c the first byte of the string.
                                    Note that c is optional (implies NULL).

 int u8strlen(char *s)          --> Returns the number of codepoints in the string s.
                                    Does NOT validate the string encoding!

 int u8codepoint(char *s)       --> Returns the codepoint encoded in s or the first
                                    byte if the encodinging is not valid.

 int u8encode(char *s, int c)   --> Stores the encoding of codepoint c in s and
                                    place a string terminator ('\0') at the end.
                                    There must be *at least* 5 bytes allocated in the
                                    string s.
 
 The decoding function is based on the work of Bjoern Hoehrmann:
    http://bjoern.hoehrmann.de/utf-8/decoder/dfa

 with the following differences:
   - the implementation is faster than what presented on the original site.
   - the code is clearer to read and to relate to the state machines
   - it has been extended to include C0 80 as the encoding for U+0000.
     (see https://en.wikipedia.org/wiki/UTF-8#Modified_UTF-8 )
     and treats '\0' as the string terminator.

 To use it, include u8c.h in your code and link against u8c.c
```
##  Compatibility with standard C functions

Strings encoded with UTF-8 are mostly compatible with the functions in
the standard C library:

```
strcpy()        Fully compatible.
  
strcat()        Fully compatible.
  
strstr()        Fully compatible.
  
strcmp()        Fully compatible (will return ordering based on the codepoints).
  
strchr()        Only works for ASCII codes (01..7F). Use strstr() instead.
  
strncat()       The length parameter 'n' is in byte. You must ensure the string
                is not split in the middle of a codepoint.
  
strncmp()       The length parameter 'n' is in byte. You must ensure the string
                is not split in the middle of a codepoint.
  
strncpy()       The length parameter 'n' is in byte. You must ensure the string
                is not split in the middle of a codepoint.
  
strlen()        Will return the size of the string in bytes.
  
strtok()        Only works for delimiters with ASCII codes (01..7F).
  
strcspn()       Only works for ASCII codes (01..7F).
  
strspn()        Only works for ASCII codes (01..7F).
  
strpbrk()       Only works for ASCII codes (01..7F).).
  
strrchr()       Only works for ASCII codes (01..7F).
  
[f]printf()     Fully compatible. Will output the string "as is"
[f]puts()       preserving the UTF encoding.

```

## FSM
Here is the decoder's Finite State Machine.

![fsm](https://user-images.githubusercontent.com/48629/60005257-6c5b4300-966e-11e9-81e7-4b8e25b53ddd.png)

Note that state LEN3 and LEN4 (dashed in the picture) are there just to ease the implementation, they do not 
consume any input.