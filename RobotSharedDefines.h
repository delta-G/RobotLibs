#ifndef ROBOT_SHARED_DEFINES_H_
#define ROBOT_SHARED_DEFINES_H_


#define BASE_ESP_BAUD 115200

#define ROBOT_COM_BAUD 115200
#define ARM_BOARD_BAUD 115200

/////////////   DRIVE MODES


enum DriveModeEnum {
	DRIVE,
	ARM,
	MINE,
	AUTO,
	NUMBER_OF_MODES
};

/////////////   BOOT TIMING

#define RMB_BOOT_INIT_WAIT 3000
#define ARM_BOOT_INIT_WAIT 3250
#define ARM_BOOT_TIMEOUT   7000




#define START_OF_PACKET '<'
#define END_OF_PACKET '>'

#define RADIO_BUFFER_SIZE 64

#define XBOX_RAW_BUFFER_SIZE 14

#define ROBOT_DATA_DUMP_SIZE 22

#define ARM_DUMP_SIZE 22

#define ROBOT_IP 192, 168, 1, 75
#define ROBOT_PORT 1234

#define BASE_STATION_IP 192, 168, 1, 74
#define MY_DEFAULT_GATEWAY 192, 168, 1, 1
#define MY_NETMASK 255, 255, 255, 0


#ifndef NUM_ELEMENTS
#define NUM_ELEMENTS(x) ((sizeof(x)) / (sizeof(x[0])))
#endif


//enum ButtonMaskEnum {
//		BUT_UP = 0x0001,
//		BUT_RIGHT = 0x0008,
//		BUT_DOWN = 0x0002,
//		BUT_LEFT = 0x0004,
//		BUT_BACK = 0x0020,
//		BUT_START = 0x0010,
//		BUT_L3 = 0x0040,
//		BUT_R3 = 0x0080,
//		BUT_L2 = 0,
//		BUT_R2 = 0,
//		BUT_L1 = 0x0100,
//		BUT_R1 = 0x0200,
//
//		BUT_B = 0x2000,
//		BUT_A = 0x1000,
//		BUT_X = 0x4000,
//		BUT_Y = 0x8000,
//
//		BUT_XBOX = 0x0400,
//		BUT_SYNC = 0x0800,
//};

//enum ButtonMaskEnum {
//		BUT_UP = 0x0100,
//		BUT_RIGHT = 0x0800,
//		BUT_DOWN = 0x0200,
//		BUT_LEFT = 0x0400,
//		BUT_BACK = 0x2000,
//		BUT_START = 0x1000,
//		BUT_L3 = 0x4000,
//		BUT_R3 = 0x8000,
//		BUT_L2 = 0,
//		BUT_R2 = 0,
//		BUT_L1 = 0x0001,
//		BUT_R1 = 0x0002,
//
//		BUT_B = 0x0020,
//		BUT_A = 0x0010,
//		BUT_X = 0x0040,
//		BUT_Y = 0x0080,
//
//		BUT_XBOX = 0x0004,
//		BUT_SYNC = 0x0008,
//};


//enum HatEnum {
//        LeftHatX = 0,
//        LeftHatY = 1,
//        RightHatX = 2,
//        RightHatY = 3,
//};

//typedef union _ControllerUnion {

//			uint8_t rawBuffer[14];
//			struct {

//				uint16_t checkBytes;
//				uint16_t buttonState;
//				uint8_t leftTrigger;
//				uint8_t rightTrigger;
//				int16_t hatValues[4];

//			} values;

//		} ControllerUnion;





///////////////////////  ROBOT MAIN BRAIN PHRASES


#define RMB_STARTUP_STRING 		"<E-RMB-Active>"
#define RMB_START_RESPONSE		"<RMB-Responds>"
#define HBOR_STRING     		"<RMB HBoR>"
#define HEARTBEAT_STRING		"<RMB HB>"

#define RMB_ARM_TEST_STRING		"<B>"


//////////////////////   COM BOARD STRINGS
#define COM_CONNECT_STRING		"<ECONNECT>"
#define COM_START_STRING		"<ESTART>"

#define BAD_COMMAND_STRING		"<Bad Command>"

//////////////////////   ARM BOARD STRINGS

#define ARM_BAD_EEPROM 			"<ARM_EEPROM_BAD>"
#define ARM_INIT_COMPLETE 		"<ARM_GOOD>"
#define ARM_CONNECT_RESPONSE	"<ARM_CONNECTED>"

#define ARM_NO_NEW_DATA			"<nr>"
#define ARM_MOVEMENT_DONE		"<ARM_MD>"
#define ARM_MOVING		"<AM>"
#define ARM_READY		"<AR>"


#endif /* ROBOT_SHARED_DEFINES_H_ */
