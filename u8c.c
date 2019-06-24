/* 
**  (C) by Remo Dentato (rdentato@gmail.com)
**
** This software is distributed under the terms of the MIT license:
**  https://opensource.org/licenses/MIT
**
*/

#define fsm           goto fsm_state_START;
#define fsmGOTO(x)    goto fsm_state_##x
#define fsmSTATE(x)   fsm_state_##x :

// Returns the length of the next UTF8 character and stores in ch
// (if it's not null) the corresponding codepoint.
// It is based on a work by Bjoern Hoehrmann:
//   http://bjoern.hoehrmann.de/utf-8/decoder/dfa
// but it's faster and has been extended to include C0 80 as the
// encoding for U+0000 

/*
digraph finite_state_machine {
  rankdir=LR;
  node [style = dashed]; LEN3; LEN4;
  node [style = solid];
  node [shape = ellipse]; START ;
  node [shape = ellipse];

  START -> END [ label = "00..00 / len=0" ];
  START -> END [ label = "01..7F / len=1" ];
  START -> NULL [ label = "C0..C0 / len=2" ];

  START -> LEN2_0 [ label = "C2..DF / len=2" ];

  START -> LEN3_0 [ label = "E0..E0 / len=3" ];
  START -> LEN3_1 [ label = "E1..EC,EE..EF / len=3" ];
  START -> LEN3_2 [ label = "ED..ED / len=3" ];

  START -> LEN4_0  [ label = "F0..F0 / len=4" ];
  START -> LEN4_1  [ label = "F1..F3 / len=4" ];
  START -> LEN4_2  [ label = "F4..F4 / len=4" ];
  
  NULL -> END [label = "80..80" ];
        
  LEN2_0 -> END   [ label = "80..BF" ];

  LEN3_0 -> LEN3  [ label = "A0..BF" ];
  LEN3_1 -> LEN3  [ label = "80..BF" ];
  LEN3_2 -> LEN3  [ label = "80..9F" ];
  LEN3 -> LEN2_0 [style=dashed ];

  LEN4_0 -> LEN4 [ label = "90..BF" ];
  LEN4_1 -> LEN4 [ label = "80..BF" ];
  LEN4_2 -> LEN4 [ label = "80..8F" ];

  LEN4 -> LEN3_1 [style=dashed ];
 }
 */

int u8next_(const char *txt, int *ch)
{
  int len = 0;
  unsigned char *s = (unsigned char *)txt;
  char first = *s;
  int val = 0;
  
  if (first) {
    val = first;
    fsm {
      fsmSTATE(START) {
        if (*s <  0x80) {              len = 1; fsmGOTO(end);     }
        if (*s == 0xC0) {              len = 2; fsmGOTO(null);    }
        if (*s <= 0xC1) {                       fsmGOTO(invalid); }
        if (*s <= 0xDF) { val &= 0x1F; len = 2; fsmGOTO(len2_0);  }
        if (*s == 0xE0) { val &= 0x0F; len = 3; fsmGOTO(len3_0);  }
        if (*s <= 0xEC) { val &= 0x0F; len = 3; fsmGOTO(len3_1);  }
        if (*s == 0xED) { val &= 0x0F; len = 3; fsmGOTO(len3_2);  }
        if (*s <= 0xEF) { val &= 0x0F; len = 3; fsmGOTO(len3_1);  }
        if (*s == 0xF0) { val &= 0x07; len = 4; fsmGOTO(len4_0);  }
        if (*s <= 0xF3) { val &= 0x07; len = 4; fsmGOTO(len4_1);  }
        if (*s == 0xF4) { val &= 0x07; len = 4; fsmGOTO(len4_2);  }
        fsmGOTO(invalid);
      } 
      
      fsmSTATE(null) {  // Modified UTF-8 encodes NULL (U+0000) to C0 80
        val = 0;
        s++; if ( *s != 0x80) fsmGOTO(invalid);
        fsmGOTO(end);
      }

      fsmSTATE(len4_0) {
        s++; if ( *s < 0x90 || 0xBF < *s) fsmGOTO(invalid);
        fsmGOTO(len4);
      }
      
      fsmSTATE(len4_1) {
        s++; if ( *s < 0x80 || 0xBF < *s) fsmGOTO(invalid);
        fsmGOTO(len4);
      }
      
      fsmSTATE(len4_2) {
        s++; if ( *s < 0x80 || 0x8F < *s) fsmGOTO(invalid);
        fsmGOTO(len4);
      }

      fsmSTATE(len4) {
        val = (val << 6) | (*s & 0x3F);
        fsmGOTO(len3_1);
      }
      
      fsmSTATE(len3_0) {
        s++; if ( *s < 0xA0 || 0xBF < *s) fsmGOTO(invalid);
        fsmGOTO(len3);
      }
      
      fsmSTATE(len3_1) {
        s++; if ( *s < 0x80 || 0xBF < *s) fsmGOTO(invalid);
        fsmGOTO(len3);
      }
  
      fsmSTATE(len3_2) {
        s++; if ( *s < 0x80 || 0x9F < *s) fsmGOTO(invalid);
        fsmGOTO(len3);
      }
      
      fsmSTATE(len3) {
        val = (val << 6) | (*s & 0x3F);
        fsmGOTO(len2_0);
      }

      fsmSTATE(len2_0) {
        s++; if ( *s < 0x80 || 0xBF < *s) fsmGOTO(invalid);
        val = (val << 6) | (*s & 0x3F);
        fsmGOTO(end);
      }
      
      // Returns -1 if the sequence is invalid  and put the first byte in ch
      fsmSTATE(invalid) {val = first; len = -1;}
      
      fsmSTATE(end)   { }
    }
  }
  if (ch) *ch = val;
  return len;
}

/* ACTUALLY NOT FASTER!!! DO NOT USE! */
int u8next_FAST(const char *txt, int *ch)
{
  int len = 0;
  unsigned char *s = (unsigned char *)txt;
  unsigned char first = *s;
  int val = 0;
  
  if (first) {
    val = first; 
    if (first > 0x7F) {
       val = *++s & 0x3F;
            if ((first & 0xF8) == 0xF0) { val |= (first & 0x07)<<6; goto L4;}
       else if ((first & 0xF0) == 0xE0) { val |= (first & 0x0F)<<6; goto L3;}
       else                             { val |= (first & 0x1F)<<6; goto L2;}
  
       L4: val <<= 6; if (*s) s++; val |= (*s & 0x3F);
       L3: val <<= 6; if (*s) s++; val |= (*s & 0x3F);
       L2: ; 
    }
    len = 1+(s-(unsigned char *)txt);
  }      

  if (ch) *ch = val;
  return len;
}

int u8strlen(const char *s)
{
  int len=0;
  while (*s) {
    if ((*s & 0xC0) != 0x80) len++ ;
    s++;
  }
  return len;
}

int u8encode(char *s, int ch)
{
  int len = -1;
  if (ch == 0) {
    len=2;
    *s++ = 0xC0;
    *s++ = 0x80;
  }
  else if (ch < 0x80) {
    len = 1;
    *s++ = ch;
  }
  else if (ch < 0x0800) {
    len = 2;
    *s++ = 0xC0 | (ch >> 6);
    *s++ = 0x80 | (ch         & 0x3F);
  }
  else if (ch < 0x10000) {
    len = 3;
    *s++ = 0xE0 |  (ch >> 12);
    *s++ = 0x80 | ((ch >> 6 ) & 0x3F);
    *s++ = 0x80 |  (ch        & 0x3F);
  } else if (ch < 0x110000) {
    len = 4;
    *s++ = 0xF0 |  (ch >> 18);
    *s++ = 0x80 | ((ch >> 12) & 0x3F);
    *s++ = 0x80 | ((ch >> 6 ) & 0x3F);
    *s++ = 0x80 |  (ch        & 0x3F);
  }
  *s = '\0';
  return len;
}
