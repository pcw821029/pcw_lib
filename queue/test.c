#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/errno.h>
#include "lqueue.h"

static status_t queue_test (void){
    Queue   *queue;
    QData 	*qData;
    status_t     status;
    int	count;
	char uchData[128];

    queue = NULL;
    qData = NULL;
    status = EOK;

    queue = qCreate(&status, "queue_test", 0xFF);
    if (queue == NULL)
        return(status);
    
    for(count = 0; count < 0xFF; count++){
        qData = malloc(sizeof(QData));
        if (qData == NULL)
            return(ENOMEM);

        bzero(qData, sizeof(qData));
		bzero(uchData,sizeof(uchData));
		sprintf(uchData,"data count-%04d",count);
        /*todo*/
		qData->puchData = (unsigned char*)malloc(strlen(uchData)+1);
		printf("push data : [%x]%s(%d)\n",(unsigned int)qData->puchData, uchData, sizeof(qData));
		memcpy((void*)qData->puchData, (void*)uchData,strlen(uchData));
        status = qPush(queue, &qData->link);
        if (status != EOK)
            return(status);
    }

    for(count = 0; count < 0xFF; count++){
        qData = qPop(queue, &status);
        if (qData == NULL){
			return(status);
		}
		printf("pop Data : [%x]%s\n",(unsigned int)qData->puchData, qData->puchData);
		free(qData->puchData);
        free(qData);
    }

    return(EOK);
}

int main (void)
{
    status_t status;

    status = queue_test();
    printf("Queue Test: %s\n", strerror(status));
    
    exit(0);
}
