/*
 *	RAPL Command Neumonics
 */

# define	MOVE	"move"
# define	RES		"res"

int RobErr;

/*
 *	RAPL Error Messages\
 */

static char *rapl_error[] = {
	"Axis #1 (waist) out of range.",
	"Axis #2 (shoulder) out of range.",
	"Axis #3 (elbow) out of range.",
	"Axis #4 (wrist) out of range.",
	"Axis #5 (waist) out of range.",
	"Axis #6 (waist) out of range.",
	"Axis #7 (waist) out of range.",
	"Axis #8 (waist) out of range.",
	"Illegal Operand - an operand in the command line is not acceptable.",
	"End of line not found.",
	"Symbol Undefined.",
	"Location Undefined.",
	"Command Error.",
	"Argument Missing.",
	"Bad Syntax.",
	"Precision Point Not Legal.",
	"I/O Address Error.",
	"Program Not Found.",
	"Illegal Argument.",
	"Line Not Found.",
	"Argument Too Big.",
	"No Room Left.",
	"Table Full.",
	"Bad Line Number.",
	"Illegal A/D Channel.",
	"Line Too Long.",
	"End of File Not Found.",
	"Program Exists.",
	"Line Exists.",
	"Transform Illegal - Attempt to move to an impossible position.",
	"Reserved.",
	"Reserved.",
	"Joint #1 (wasist) Home Failed.",
	"Joint #2 (shoulder) Home Failed.",
	"Joint #3 (elbow) Home Failed.",
	"Joint #4 (wrist) Home Failed.",
	"Joint #5 (gripper) Home Failed.",
	"Joint #6 Home Failed.",
	"Joint #7 Home Failed.",
	"Joint #8 Home Failed.",
	"Arm Power has been switched off.",
	"Robot has not been homed.",
	"Joint #1 (waist) Out of Range.",
	"Joint #2 (shoulder) Out of Range.",
	"Joint #3 (elbow) Out of Range.",
	"Joint #4 (wrist) Out of Range.",
	"Joint #5 (gripper) Out of Range.",
	"Checksum Failure - memory value is incorrect.",
	"Reserved.",
	"Memory Not Allocated.",
	"Ambigouous Command.",
	"Serial Channel #0 Timeout.",
	"Serial Channel #0 CTS Timeout.",
	"Serial Channel #1 Timeout.",
	"Serial Channel #1 CTS Timeout.",
	"Reserved.",
	"Reserved.",
	"Reserved.",
	"Calibration Checksum Error - Robot must be recalibrated.",
	"Reserved.",
	"Cannot use Priviledged command.",
	"Illegal Channel.",
	"Remote Communication Error.",
	"Reserved.",
	"No Return Level.",
	"Bad Axis Number - This axis doesn't exist.",
	"Not Available.",
	"Axis Slot Alread Used.",
	"Straight Line Error.",
	"Invaild torque value.",
	"Speed is too fast or less than zero",
	"Invalid variable name.",
	"Invalid location name.",
	"Gripper opening distance is invalid.",
};

#define BAD_TORQUE 	69
#define BAD_SPEED  	70
#define BAD_VAR		71
#define BAD_LOC		73
#define BAD_GRIP	74
#define MAX_ROB_ERR 	74

#define MAX_TORQUE	60 	/* maximum torque */
#define MAX_SPEED	60 	/* maximum speed */
#define MAX_GRIP	2 	/* widest grip opening */

#define LOC_LEN		8	/* max length of a location name */
#define VAR_LEN		8	/* max length of a variable name */
