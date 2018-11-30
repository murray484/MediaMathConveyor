/*
 *
 *  Debugging and option flags set in input file or command-line
 *
 */

#define MAX_FLAGS 31

typedef enum { INT, FLT, CHAR, STRING, ON, OFF } DataType;

typedef struct {
    char *token;
    char cl_switch;
    DataType type;
    void *data;
} Flag;

extern Flag GlobalFlags[MAX_FLAGS];

extern char ACISerialPortName[];
extern char *SerialPortName;
extern char InputFileName[];
extern int SetTermFlag;
extern int VolatileCWSType[6];
extern int SpeedCWSType, ForceCWSType, DeltaCWSType;
extern int ViewVolatile[6];   
extern int UseACIPort; 

extern double SerialCommandRes(char*);
