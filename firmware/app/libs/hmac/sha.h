/**************************** sha.h ****************************/
/******************* See RFC 4634 for details ******************/
#ifndef _SHA_H_
#define _SHA_H_

/*
 *  Description:
 *      This file implements the Secure Hash Signature Standard
 *      algorithms as defined in the National Institute of Standards
 *      and Technology Federal Information Processing Standards
 *      Publication (FIPS PUB) 180-1 published on April 17, 1995, 180-2
 *      published on August 1, 2002, and the FIPS PUB 180-2 Change
 *      Notice published on February 28, 2004.
 *
 *      A combined document showing all algorithms is available at
 *              http://csrc.nist.gov/publications/fips/
 *              fips180-2/fips180-2withchangenotice.pdf
 *
 *      The five hashes are defined in these sizes:
 *              SHA-1           20 byte / 160 bit
 *              SHA-224         28 byte / 224 bit
 *              SHA-256         32 byte / 256 bit
 *              SHA-384         48 byte / 384 bit
 *              SHA-512         64 byte / 512 bit
 */

#include <stdint.h>
/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typedef the following:
 *    name              meaning
 *  uint64_t         unsigned 64 bit integer
 *  uint32_t         unsigned 32 bit integer
 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
 *  int_least16_t    integer of >= 16 bits
 *
 */

#ifndef _SHA_enum_
#define _SHA_enum_
/*
 *  All SHA functions return one of these values.
 */
enum
{
  shaSuccess = 0,
  shaNull,			/* Null pointer parameter */
  shaInputTooLong,		/* input data too long */
  shaStateError,		/* called Input after FinalBits or Result */
  shaBadParam			/* passed a bad parameter */
};
#endif /* _SHA_enum_ */

/*
 *  These constants hold size information for each of the SHA
 *  hashing operations
 */
enum
{
  SHA1_Message_Block_Size = 64, SHA224_Message_Block_Size = 64,
  SHA256_Message_Block_Size = 64, SHA384_Message_Block_Size = 128,
  SHA512_Message_Block_Size = 128,
  USHA_Max_Message_Block_Size = SHA512_Message_Block_Size,

  SHA1HashSize = 20, SHA224HashSize = 28, SHA256HashSize = 32,
  SHA384HashSize = 48, SHA512HashSize = 64,
  USHAMaxHashSize = SHA512HashSize,

  SHA1HashSizeBits = 160, SHA224HashSizeBits = 224,
  SHA256HashSizeBits = 256, SHA384HashSizeBits = 384,
  SHA512HashSizeBits = 512, USHAMaxHashSizeBits = SHA512HashSizeBits
};

/*
 *  These constants are used in the USHA (unified sha) functions.
 */
typedef enum SHAversion
{
  SHA1, SHA224, SHA256, SHA384, SHA512
} SHAversion;

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation.
 */
typedef struct SHA1Context
{
  uint32_t Intermediate_Hash[SHA1HashSize / 4];	/* Message Digest */

  uint32_t Length_Low;		/* Message length in bits */
  uint32_t Length_High;		/* Message length in bits */

  int_least16_t Message_Block_Index;	/* Message_Block array index */
  /* 512-bit message blocks */
  uint8_t Message_Block[SHA1_Message_Block_Size];

  int Computed;			/* Is the digest computed? */
  int Corrupted;		/* Is the digest corrupted? */
} SHA1Context;

/*
 *  This structure will hold context information for the SHA-256
 *  hashing operation.
 */
typedef struct SHA256Context
{
  uint32_t Intermediate_Hash[SHA256HashSize / 4];	/* Message Digest */

  uint32_t Length_Low;		/* Message length in bits */
  uint32_t Length_High;		/* Message length in bits */

  int_least16_t Message_Block_Index;	/* Message_Block array index */
  /* 512-bit message blocks */
  uint8_t Message_Block[SHA256_Message_Block_Size];

  int Computed;			/* Is the digest computed? */
  int Corrupted;		/* Is the digest corrupted? */
} SHA256Context;

/*
 *  This structure will hold context information for the SHA-512
 *  hashing operation.
 */
typedef struct SHA512Context
{
#ifdef USE_32BIT_ONLY
  uint32_t Intermediate_Hash[SHA512HashSize / 4];	/* Message Digest  */
  uint32_t Length[4];		/* Message length in bits */
#else				/* !USE_32BIT_ONLY */
  uint64_t Intermediate_Hash[SHA512HashSize / 8];	/* Message Digest */
  uint64_t Length_Low, Length_High;	/* Message length in bits */ // cppcheck-suppress misra-c2012-12.3
#endif				/* USE_32BIT_ONLY */
  int_least16_t Message_Block_Index;	/* Message_Block array index */
  /* 1024-bit message blocks */
  uint8_t Message_Block[SHA512_Message_Block_Size];

  int Computed;			/* Is the digest computed? */
  int Corrupted;		/* Is the digest corrupted? */
} SHA512Context;

/*
 *  This structure will hold context information for the SHA-224
 *  hashing operation. It uses the SHA-256 structure for computation.
 */
typedef struct SHA256Context SHA224Context;

/*
 *  This structure will hold context information for the SHA-384
 *  hashing operation. It uses the SHA-512 structure for computation.
 */
typedef struct SHA512Context SHA384Context;

/*
 *  This structure holds context information for all SHA
 *  hashing operations.
 */
typedef struct USHAContext
{
  int whichSha;			/* which SHA is being used */
  union // cppcheck-suppress misra-c2012-19.2
  {
    SHA1Context sha1Context;
    SHA224Context sha224Context;
    SHA256Context sha256Context;
    SHA384Context sha384Context;
    SHA512Context sha512Context;
  } ctx; // cppcheck-suppress misra-c2012-19.2
} USHAContext;

/*
 *  This structure will hold context information for the HMAC
 *  keyed hashing operation.
 */
typedef struct HMACContext
{
  int whichSha;			/* which SHA is being used */
  int hashSize;			/* hash size of SHA being used */
  int blockSize;		/* block size of SHA being used */
  USHAContext shaContext;	/* SHA context */
  unsigned char k_opad[USHA_Max_Message_Block_Size];
  /* outer padding - key XORd with opad */
} HMACContext;

/*
 *  Function Prototypes
 */

/* SHA-1 */
extern int SHA1Reset (SHA1Context *); // cppcheck-suppress misra-c2012-8.2
extern int SHA1Input (SHA1Context *, const uint8_t * bytes,  // cppcheck-suppress misra-c2012-8.2
		      unsigned int bytecount); // cppcheck-suppress misra-c2012-8.2
extern int SHA1FinalBits (SHA1Context *, const uint8_t bits, // cppcheck-suppress misra-c2012-8.2
			  unsigned int bitcount); // cppcheck-suppress misra-c2012-8.2
extern int SHA1Result (SHA1Context *, uint8_t Message_Digest[SHA1HashSize]); // cppcheck-suppress misra-c2012-8.2

/* SHA-224 */
extern int SHA224Reset (SHA224Context *); // cppcheck-suppress misra-c2012-8.2
extern int SHA224Input (SHA224Context *, const uint8_t * bytes, // cppcheck-suppress misra-c2012-8.2
			unsigned int bytecount); // cppcheck-suppress misra-c2012-8.2
extern int SHA224FinalBits (SHA224Context *, const uint8_t bits, // cppcheck-suppress misra-c2012-8.2
			    unsigned int bitcount); // cppcheck-suppress misra-c2012-8.2
extern int SHA224Result (SHA224Context *, // cppcheck-suppress misra-c2012-8.2
			 uint8_t Message_Digest[SHA224HashSize]); // cppcheck-suppress misra-c2012-8.2

/* SHA-256 */
extern int SHA256Reset (SHA256Context *); // cppcheck-suppress misra-c2012-8.2
extern int SHA256Input (SHA256Context *, const uint8_t * bytes, // cppcheck-suppress misra-c2012-8.2
			unsigned int bytecount); // cppcheck-suppress misra-c2012-8.2
extern int SHA256FinalBits (SHA256Context *, const uint8_t bits, // cppcheck-suppress misra-c2012-8.2
			    unsigned int bitcount); // cppcheck-suppress misra-c2012-8.2
extern int SHA256Result (SHA256Context *, // cppcheck-suppress misra-c2012-8.2
			 uint8_t Message_Digest[SHA256HashSize]); // cppcheck-suppress misra-c2012-8.2

/* SHA-384 */
extern int SHA384Reset (SHA384Context *); // cppcheck-suppress misra-c2012-8.2
extern int SHA384Input (SHA384Context *, const uint8_t * bytes, // cppcheck-suppress misra-c2012-8.2
			unsigned int bytecount); // cppcheck-suppress misra-c2012-8.2
extern int SHA384FinalBits (SHA384Context *, const uint8_t bits, // cppcheck-suppress misra-c2012-8.2
			    unsigned int bitcount); // cppcheck-suppress misra-c2012-8.2
extern int SHA384Result (SHA384Context *, // cppcheck-suppress misra-c2012-8.2
			 uint8_t Message_Digest[SHA384HashSize]); // cppcheck-suppress misra-c2012-8.2

/* SHA-512 */
extern int SHA512Reset (SHA512Context *); // cppcheck-suppress misra-c2012-8.2
extern int SHA512Input (SHA512Context *, const uint8_t * bytes, // cppcheck-suppress misra-c2012-8.2
			unsigned int bytecount); // cppcheck-suppress misra-c2012-8.2
extern int SHA512FinalBits (SHA512Context *, const uint8_t bits, // cppcheck-suppress misra-c2012-8.2
			    unsigned int bitcount); // cppcheck-suppress misra-c2012-8.2
extern int SHA512Result (SHA512Context *, // cppcheck-suppress misra-c2012-8.2
			 uint8_t Message_Digest[SHA512HashSize]); // cppcheck-suppress misra-c2012-8.2

/* Unified SHA functions, chosen by whichSha */
extern int USHAReset (USHAContext *, SHAversion whichSha); // cppcheck-suppress misra-c2012-8.2
extern int USHAInput (USHAContext *, // cppcheck-suppress misra-c2012-8.2
		      const uint8_t * bytes, unsigned int bytecount); // cppcheck-suppress misra-c2012-8.2
extern int USHAFinalBits (USHAContext *,  // cppcheck-suppress misra-c2012-8.2
			  const uint8_t bits, unsigned int bitcount); // cppcheck-suppress misra-c2012-8.2
extern int USHAResult (USHAContext *, // cppcheck-suppress misra-c2012-8.2
		       uint8_t Message_Digest[USHAMaxHashSize]); // cppcheck-suppress misra-c2012-8.2
extern int USHABlockSize (enum SHAversion whichSha); // cppcheck-suppress misra-c2012-8.2
extern int USHAHashSize (enum SHAversion whichSha); // cppcheck-suppress misra-c2012-8.2
extern int USHAHashSizeBits (enum SHAversion whichSha); // cppcheck-suppress misra-c2012-8.2

/*
 * HMAC Keyed-Hashing for Message Authentication, RFC2104,
 * for all SHAs.
 * This interface allows a fixed-length text input to be used.
 */
extern int hmac (SHAversion whichSha,	/* which SHA algorithm to use */
		 const unsigned char *text,	/* pointer to data stream */
		 int text_len,	/* length of data stream */
		 const unsigned char *key,	/* pointer to authentication key */
		 int key_len,	/* length of authentication key */
		 uint8_t digest[USHAMaxHashSize]);	/* caller digest to fill in */

/*
 * HMAC Keyed-Hashing for Message Authentication, RFC2104,
 * for all SHAs.
 * This interface allows any length of text input to be used.
 */
extern int hmacReset (HMACContext * ctx, enum SHAversion whichSha,
		      const unsigned char *key, int key_len);
extern int hmacInput (HMACContext * ctx, const unsigned char *text,
		      int text_len);

extern int hmacFinalBits (HMACContext * ctx, const uint8_t bits,
			  unsigned int bitcount);
extern int hmacResult (HMACContext * ctx, uint8_t digest[USHAMaxHashSize]);

#endif /* _SHA_H_ */
