#ifndef _EEPR_H_
#define _EEPR_H_


// User defined constants
#define DATA_EE_BANKS       2
#define DATA_EE_SIZE        255
#define DATA_EE_TOTAL_SIZE  (DATA_EE_BANKS * DATA_EE_SIZE)
#define NUM_DATA_EE_PAGES   3

#define ERASE               0x4042
#define PROGRAM_ROW         0x4001
#define PROGRAM_WORD        0x4003

// Internal constants
#define	NUMBER_OF_INSTRUCTIONS_IN_PAGE  512
#define	NUMBER_OF_INSTRUCTIONS_IN_ROW   64

#define ERASE_WRITE_CYCLE_MAX           5
#define NUMBER_OF_ROWS_IN_PAGE          (NUMBER_OF_INSTRUCTIONS_IN_PAGE / NUMBER_OF_INSTRUCTIONS_IN_ROW)
#define PAGE_AVAILABLE                  1
#define PAGE_CURRENT                    0
#define PAGE_EXPIRED                    0
#define PAGE_NOT_AVAILABLE              0
#define PAGE_NOT_CURRENT                1
#define PAGE_NOT_EXPIRED                1
#define STATUS_AVAILABLE                18
#define STATUS_CURRENT                  19
#define STATUS_EXPIRED                  20

#define GetaddrNotFound() dataEEFlags.addrNotFound
#define SetaddrNotFound(x) dataEEFlags.addrNotFound = x

#define GetPageCorruptStatus() dataEEFlags.pageCorrupt
#define SetPageCorruptStatus(x) dataEEFlags.pageCorrupt = x

#define GetPageExpiredPage() dataEEFlags.expiredPage
#define SetPageExpiredPage(x) dataEEFlags.expiredPage = x

#define GetPageIllegalAddress() dataEEFlags.IllegalAddress
#define SetPageIllegalAddress(x) dataEEFlags.IllegalAddress = x

#define GetPagePackBeforeInit() dataEEFlags.packBeforeInit
#define SetPagePackBeforeInit(x) dataEEFlags.packBeforeInit = x

#define GetPagePackBeforePageFull() dataEEFlags.packBeforePageFull
#define SetPagePackBeforePageFull(x) dataEEFlags.packBeforePageFull = x

#define GetPagePackSkipped() dataEEFlags.packSkipped
#define SetPagePackSkipped(x) dataEEFlags.packSkipped = x

#define GetPageWriteError() dataEEFlags.writeError
#define SetPageWriteError(x) dataEEFlags.writeError = x

typedef union
{
  unsigned char val;
  struct
  {
    unsigned addrNotFound:1;	    // Return 0xFFFF
    unsigned expiredPage:1;	   	    // Return 0x1
    unsigned packBeforePageFull:1;	// Not a return condition
    unsigned packBeforeInit:1;		// Return 0x3
    unsigned packSkipped:1;		    // Return 0x4
    unsigned IllegalAddress:1;	    // Return 0x5
    unsigned pageCorrupt:1;		    // Return 0x6
    unsigned writeError:1;		    // Return 0x7
  };
} DATA_EE_FLAGS;

extern DATA_EE_FLAGS dataEEFlags;

extern int  ReadPMHigh(int);
extern int  ReadPMLow(int);
extern void UnlockPM(void);
extern int  WritePMHigh(int, int);
extern int  WritePMHighB(int, int);
extern int  WritePMLow(int, int);
extern int  WritePMLowB(int, int);

void            UnlockWrite         (void);
int             GetPageStatus       (unsigned char bank, unsigned volatile char page, unsigned volatile char field);
void            ErasePage           (unsigned char bank, unsigned char page);
char            IncEWCount          (unsigned char *index);
unsigned int    GetNextAvailCount   (unsigned char bank);
int             PackEE              (unsigned char bank);
unsigned char   DataEEInit          (void);
unsigned int    DataEERead          (unsigned int addr);
unsigned char   DataEEWrite         (unsigned int data, unsigned int addr);


void cond_check(void);
void eeprom_write(void);
void eeprom_read(void);

#endif 
