#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "extend_record_on_disarm.h"

uint32_t duss_osal_msgq_receive(int param_1, uint32_t param_2, void *param_3, uint32_t *param_4){

	duss_osal_msgq_receive_hook = dlsym (RTLD_NEXT, "duss_osal_msgq_receive");
		if (duss_osal_msgq_receive_hook == 0){
			if(!guiLib)guiLib = dlopen("/system/lib/libduml_osal.so", 1);
			duss_osal_msgq_receive_hook = dlsym (guiLib, "duss_osal_msgq_receive");
			if (duss_osal_msgq_receive_hook == 0)
			{
				printf("dlsym: %s\n", dlerror());
			}
		}

	uint32_t ret = duss_osal_msgq_receive_hook(param_1, param_2, param_3, param_4);

	if(param_2 == 1000000){	//event client 1000000. using it as "unique" identifier
		queue_msg = (unsigned char *)param_3;

		if(*queue_msg == filter_cmd){ // 1st byte == cmdid 221
			data_len = (uint32_t)*(queue_msg + 12);	//12-15th byte int32 msg length
		}
	}

	if(param_2 == 0){	//msg data sent to event client id 0 right after id 1000000
		if(*queue_msg == filter_cmd){

			arm_flag = *((uint8_t *)param_3);  //1st byte == arm flag. 8 == armed, 0 == disarmed

			if((prev_arm_flag - arm_flag) == 8){ //start timer when switching from armed->disarmed
				clock_gettime(CLOCK_MONOTONIC, &start);
			}

			clock_gettime(CLOCK_MONOTONIC, &now);

			if((now.tv_sec - start.tv_sec) < 15){
				*((uint8_t *)param_3) = 8; //inject custom arm flag.
				printf("EXTENDING RECORD DISARM TIME... \n");
			}
			
			prev_arm_flag = arm_flag;
		}
	}

	return ret;
}