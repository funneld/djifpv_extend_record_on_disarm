#include <time.h>

static void *guiLib = 0;
struct timespec start_ext_rec, now_ext_rec;
static uint32_t data_len = 0;
static unsigned char *queue_msg = 0;
static uint32_t (* duss_osal_msgq_receive_hook)(int param_1, uint32_t param_2, void *param_3, uint32_t *param_4) = 0;
static uint8_t prev_arm_flag = 0;
static uint8_t arm_flag = 0;
static char filter_cmd = 221; // duml msg cmdid power_status flags
