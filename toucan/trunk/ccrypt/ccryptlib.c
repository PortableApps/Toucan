/* Copyright (C) 2000-2009 Peter Selinger.
   This file is part of ccrypt. It is free software and it is covered
   by the GNU general public license. See the file COPYING for details. */

/* ccryptlib.c: library for encrypting/decrypting a character stream */
/* $Id: ccryptlib.c 258 2009-08-26 17:46:10Z selinger $ */

/* NOTE: this library is copyrighted under the GNU General Public
   License, *not* under the GNU Library General Public License.  This
   means, among other things, that you cannot use this library in
   closed-source software. */

/* ccryptlib implements a stream cipher based on the block cipher
   Rijndael, the candidate for the AES standard. */

#include <stdlib.h>
#if defined(_MSC_VER)
#include <winsock2.h>
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>  /* generated by configure */
#endif

#include "ccryptlib.h"
#include "rijndael.h"
#include "platform.h"

#define MAGIC "c051"   /* magic string for this version of ccrypt */

/* private struct, not visible by applications */
struct ccrypt_state_s {  
  int n;          /* number of keys. n=1 for encryption, n>=1 for decryption */
  roundkey *rkks; /* array of n keys */
  int ak;         /* rkks[ak] is the active key */
  int iv;         /* are we reading/writing the IV? */
  int bufindex;   /* in bytes */
  word32 buf[8];  /* current buffer; partly ciphertext, partly mask */
  int flags;      /* flags determining behavior */
};
typedef struct ccrypt_state_s ccrypt_state_t;

int ccrypt_errno;

static void ccrypt_state_free(ccrypt_state_t *st) {
  if (st) {
    free(st->rkks);
  }
  free(st);
}

/* ---------------------------------------------------------------------- */
/* some private functions dealing with hashes, keys, and nonces */

/* hash a keystring into a 256-bit cryptographic random value. */
static void hashstring(const char *keystring, word32 hash[8]) {
  int i;
  roundkey rkk;
  word32 key[8];      /* rijndael key */

  for (i=0; i<8; i++) 
    key[i] = hash[i] = 0;
  
  do {
    for (i=0; i<32; i++) {
      if (*keystring != 0) {
	((word8 *)key)[i] ^= *keystring;
	keystring++;
      }
    }
    xrijndaelKeySched(key, 256, 256, &rkk);
    xrijndaelEncrypt(hash, &rkk);
  } while (*keystring != 0);
}

/* return a 256-bit value that is practically unique */
static void make_nonce(word32 nonce[8]) {
  char acc[512], host[256];
  struct timeval tv;
  static int count=0;
  
  gethostname(host, 256);
  gettimeofday(&tv, NULL);
  sprintf(acc, "%s,%ld,%ld,%ld,%d", host, (long)tv.tv_sec, (long)tv.tv_usec,
	  (long)getpid(), count++);
  hashstring(acc, nonce); 
}

/* ---------------------------------------------------------------------- */
/* core functions for encryption */

int ccencrypt_init(ccrypt_stream_t *b, const char *key) {
  word32 keyblock[8];
  ccrypt_state_t *st;
  roundkey *rkks;

  b->state = NULL;
  
  st = malloc(sizeof(ccrypt_state_t));
  if (st == NULL) {
    return -1;
  }
  rkks = malloc(sizeof(roundkey));
  if (!rkks) {
    free(st);
    return -1;
  }

  st->n = 1;
  st->rkks = rkks;
  st->ak = 0; /* not used */

  /* generate the roundkey */
  hashstring(key, keyblock);
  xrijndaelKeySched(keyblock, 256, 256, &st->rkks[0]);
  
  /* make a nonce */
  make_nonce(st->buf);

  /* mark the nonce with a "magic number". */
  strncpy((char *)st->buf, MAGIC, 4);

  /* encrypt the nonce with the given key */
  xrijndaelEncrypt(st->buf, &st->rkks[0]);

  /* IV is now contained in st->buf. Initialize rest of the state. */
  st->iv = 1;
  st->bufindex = 0; /* initially use bufsize to count iv bytes output */

  b->state = (void *)st;
  return 0;
}

int ccencrypt(ccrypt_stream_t *b) {
  ccrypt_state_t *st = (ccrypt_state_t *)b->state;
  word32 lbuf[8];
  char *cbuf = (char *)st->buf;
  int i;
  char c, cc;

  while (1) {
    /* handle the typical case efficiently, one block at a time */

    if (st->iv == 0 && st->bufindex == 32) {
      while (b->avail_in >= 32 && b->avail_out >= 32) {

	/* block-encrypt buffer */
	xrijndaelEncrypt(st->buf, &st->rkks[0]);
	
	/* read input to local buffer for word alignment */
	memcpy(lbuf, b->next_in, 32);
	b->next_in += 32;
	b->avail_in -= 32;
	
	/* compute ciphertext */
	for (i=0; i<8; i++) {
	  st->buf[i] ^= lbuf[i];
	}

	/* write output */
	memcpy(b->next_out, st->buf, 32);
	b->next_out += 32;
	b->avail_out -= 32;
      }
    }
    
    /* handle the general case systematically, one byte at a time */

    if (b->avail_out == 0) {
      break;
    } 

    else if (st->iv) {  /* write IV byte */
      *b->next_out = cbuf[st->bufindex];
      b->next_out++;
      b->avail_out--;
      st->bufindex++;
      if (st->bufindex == 32) {
	st->iv = 0;
      }
    } 

    else if (b->avail_in == 0) {
      break;
    }

    else {              /* encrypt one byte */
      if (st->bufindex == 32) {
	xrijndaelEncrypt(st->buf, &st->rkks[0]);
	st->bufindex = 0;
      }
      c = *b->next_in;
      b->next_in++;
      b->avail_in--;
      cc = c ^ cbuf[st->bufindex];
      cbuf[st->bufindex] = cc;
      *b->next_out = cc;
      b->next_out++;
      b->avail_out--;
      st->bufindex++;
    }
  }
  return 0;
}

int ccencrypt_end(ccrypt_stream_t *b) {
  ccrypt_state_free(b->state);
  b->state = NULL; /* guard against double free */
  return 0;
}

/* ---------------------------------------------------------------------- */
/* core functions for decryption */

int ccdecrypt_multi_init(ccrypt_stream_t *b, int n, const char **keylist, int flags) {
  word32 keyblock[8];
  ccrypt_state_t *st;
  roundkey *rkks;
  int i;
  
  b->state = NULL;

  st = malloc(sizeof(ccrypt_state_t));
  if (st == NULL) {
    return -1;
  }
  rkks = malloc(n * sizeof(roundkey));
  if (!rkks) {
    free(st);
    return -1;
  }

  st->n = n;
  st->rkks = rkks;
  st->ak = 0;

  /* generate the roundkeys */
  for (i=0; i<n; i++) {
    hashstring(keylist[i], keyblock);
    xrijndaelKeySched(keyblock, 256, 256, &st->rkks[i]);
  }

  /* Initialize rest of the state. */
  st->iv = 1;
  st->bufindex = 0;
  st->flags = flags;

  b->state = (void *)st;
  return 0;
}

int ccdecrypt_init(ccrypt_stream_t *b, const char *key, int flags) {
  return ccdecrypt_multi_init(b, 1, &key, flags);
}

int ccdecrypt(ccrypt_stream_t *b) {
  ccrypt_state_t *st = (ccrypt_state_t *)b->state;
  word32 lbuf[8];
  char *cbuf = (char *)st->buf;
  int i;
  char c, cc;
  
  while (1) {
    /* handle the typical case efficiently, one block at a time */

    if (st->iv == 0 && st->bufindex == 32) {
      while (b->avail_in >= 32 && b->avail_out >= 32) {
	
	/* block-encrypt buffer */
        xrijndaelEncrypt(st->buf, &st->rkks[st->ak]);
	memcpy(lbuf, st->buf, 32);

	/* read input */
	memcpy(st->buf, b->next_in, 32);
	b->next_in += 32;
	b->avail_in -= 32;

	/* compute plaintext */
	for (i=0; i<8; i++) {
	  lbuf[i] ^= st->buf[i];
	}

	/* write output */
	memcpy(b->next_out, lbuf, 32);
	b->next_out += 32;
	b->avail_out -= 32;
      }
    }

    /* handle the general case systematically, one byte at a time */

    if (b->avail_in == 0) {
      break;
    }

    else if (st->iv) {  /* read IV byte */
      cbuf[st->bufindex] = *b->next_in;
      b->next_in++;
      b->avail_in--;
      st->bufindex++;
      if (st->bufindex == 32) {
	st->iv = 0;
	/* find the first matching key */
	for (i=0; i<st->n; i++) {
	  /* check the "magic number" */
	  memcpy(lbuf, st->buf, 32);
	  xrijndaelDecrypt(lbuf, &st->rkks[i]);
	  if ((st->flags & CCRYPT_MISMATCH) != 0 || strncmp((char *)lbuf, MAGIC, 4) == 0) {
	    /* key matches */
	    break;
	  }
	}
	if (i<st->n) { /* matching key found */
	  st->ak = i;
	} else {       /* not found */
	  /* on error, invalidate the state so that the client cannot
	     call here again. */
	  ccrypt_state_free(b->state);
	  b->state = NULL;
	  ccrypt_errno = CCRYPT_EMISMATCH;
	  return -2;
	}
      }
    }

    else if (b->avail_out == 0) {
      break;
    }

    else {              /* decrypt one byte */
      if (st->bufindex == 32) {
	xrijndaelEncrypt(st->buf, &st->rkks[st->ak]);
	st->bufindex = 0;
      }
      cc = *b->next_in;
      b->next_in++;
      b->avail_in--;
      c = cc ^ cbuf[st->bufindex];
      cbuf[st->bufindex] = cc;
      *b->next_out = c;
      b->next_out++;
      b->avail_out--;
      st->bufindex++;
    }
  }
  return 0;
}

int ccdecrypt_end(ccrypt_stream_t *b) {
  ccrypt_state_t *st;
  
  if (b->state) {

    /* verify that we have read and verified the IV */
    st = (ccrypt_state_t *)b->state;
    if (st->iv) {
      ccrypt_state_free(b->state);
      b->state = NULL;
      ccrypt_errno = CCRYPT_EFORMAT;
      return -2;
    }
  }

  ccrypt_state_free(b->state);
  b->state = NULL;
  return 0;
}
