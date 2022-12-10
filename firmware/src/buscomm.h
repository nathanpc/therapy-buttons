/**
 * buscomm.h
 * Bus communication abstraction layer.
 * 
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef BUSCOMM_H
#define	BUSCOMM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include <stdbool.h>
	
// Some definitions.
#define FRAME_MAX_LEN 250
#define ARG_MAX_LEN   15
#define ARGS_MAX      5

// Frame of data.
typedef struct {
	uint8_t addr;
	char command[ARG_MAX_LEN + 1];
	char args[ARGS_MAX][ARG_MAX_LEN + 1];
	uint8_t num_args;
} comms_frame_t;

// Command parsing stages.
typedef enum {
	COMMS_STAGE_READY,
	COMMS_STAGE_ADDR,
	COMMS_STAGE_COMMAND,
	COMMS_STAGE_NEWARG,
	COMMS_STAGE_ARG,
	COMMS_STAGE_FINISHED
} comms_stage_t;

// Initialization
void Comms_Initialize(uint8_t addr);

// Receiving
void Comms_ReceiveChar(char c);
void Comms_ParseFrame(void);

// External Handlers
extern void Comms_HandleCommand(const comms_frame_t *frame);

// Replying
void Comms_AddrReplyStart(uint8_t addr);
void Comms_ReplyStart(void);
void Comms_ReplyEnd(void);
void Comms_AddrReply(uint8_t addr, const char *reply);
void Comms_Reply(const char *reply);

// Error Handling
void Comms_ResetRXBuffer(void);
void Comms_DiscardFrame(void);

// Getters and Setters
void Comms_SetOurAddress(uint8_t addr, bool persist);
const char* Comms_GetAddrStr(void);

// Debugging
void Comms_DebugPrintFrame(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BUSCOMM_H */

