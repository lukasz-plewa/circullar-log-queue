/*
 ============================================================================
 Name        : circular_log.c
 Author      : L.Plewa
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <gtkmm.h>

#define MAX_ITEMS    40
#define LINE_LENGTH  256

typedef struct circularQueue_s
{
    int     first;
    int     last;
    int     validItems;
    char*   data[MAX_ITEMS];
    pthread_mutex_t lock;
} circularQueue_t;

void mvos_webToolsInitializeQueue(circularQueue_t *theQueue);
int mvos_webToolsIsEmpty(circularQueue_t *theQueue);
int mvos_webToolsPutItem(circularQueue_t *theQueue, const char* theItemValue);
int mvos_webToolsGetItem(circularQueue_t *theQueue, char **theItemValue);
void mvos_webToolsPrintQueue(circularQueue_t *theQueue);
void mvos_webToolsRemoveQueue(circularQueue_t *theQueue);

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	circularQueue_t circular_log;
	int i;
	char line[LINE_LENGTH] = {0};

	mvos_webToolsInitializeQueue(&circular_log);

	for(i=0; i < 10000; i++)
	{
	  snprintf(line, LINE_LENGTH, "Ala ma kota to jest testowy [log][mineva]:123 %s - %s, %d", __FILE__, __FUNCTION__, i);
	  if (-1 != mvos_webToolsPutItem(&circular_log, line))
	  {
	    mvos_webToolsPrintQueue(&circular_log);
	  }
	  else
	  {
	    printf("Failed %d\n", i);
	  }
	  usleep(250000);
	}
	return EXIT_SUCCESS;
}


void mvos_webToolsRemoveQueue(circularQueue_t *theQueue)
{
  int i;
  pthread_mutex_lock(&theQueue->lock);
  theQueue->validItems  =  0;
  theQueue->first       =  0;
  theQueue->last        =  0;
  for(i=0; i<MAX_ITEMS; i++)
  {
      free(theQueue->data[i]);
  }
  pthread_mutex_unlock(&theQueue->lock);
}

void mvos_webToolsInitializeQueue(circularQueue_t *theQueue)
{
  int i;
  if (pthread_mutex_init(&theQueue->lock, NULL) != 0)
  {
      printf("\n mutex init failed\n");
      return;
  }

  pthread_mutex_lock(&theQueue->lock);
  theQueue->validItems  =  0;
  theQueue->first       =  0;
  theQueue->last        =  0;
  for(i=0; i<MAX_ITEMS; i++)
  {
    theQueue->data[i] = malloc(sizeof(char)*LINE_LENGTH);
    if( theQueue->data[i] == NULL )
    {
      printf("ERROR alocating memory for %d item\n", i);
      mvos_webToolsRemoveQueue(theQueue);
      break;
    }
  }
  pthread_mutex_unlock(&theQueue->lock);
  return;
}

int mvos_webToolsIsEmpty(circularQueue_t *theQueue)
{
  int ret = 0;
  pthread_mutex_lock(&theQueue->lock);
  if(theQueue->validItems==0)
    ret = 1;
  pthread_mutex_unlock(&theQueue->lock);
  return ret;
}

int mvos_webToolsPutItem(circularQueue_t *theQueue, const char* theItemValue)
{
  pthread_mutex_lock(&theQueue->lock);
  if(theQueue->validItems >= MAX_ITEMS)
  {
    theQueue->first = (theQueue->first+1)%MAX_ITEMS;  // queue full - rotating
  }
  else
  {
    theQueue->validItems++;
  }
  strncpy(theQueue->data[theQueue->last], theItemValue, LINE_LENGTH);
  theQueue->last = (theQueue->last+1)%MAX_ITEMS;
  pthread_mutex_unlock(&theQueue->lock);
  return 0;
}

int mvos_webToolsGetItem(circularQueue_t *theQueue, char **theItemValue)
{
  int ret = -1;
  pthread_mutex_lock(&theQueue->lock);
  if(mvos_webToolsIsEmpty(theQueue))
  {
    ret = (-1);
  }
  else
  {
    *theItemValue = theQueue->data[theQueue->first];
    theQueue->first=(theQueue->first+1)%MAX_ITEMS;
    theQueue->validItems--;
    ret = 0;
  }
  pthread_mutex_unlock(&theQueue->lock);
  return ret;
}

void mvos_webToolsPrintQueue(circularQueue_t *theQueue)
{
  int aux, aux1;
  pthread_mutex_lock(&theQueue->lock);
  aux  = theQueue->first;
  aux1 = theQueue->validItems;
  printf("\n-----\nBuffer size %d, item list:\n", theQueue->validItems);
  while(aux1>0)
  {
    printf("Element #%03d = %s\n", aux, theQueue->data[aux]);
    aux=(aux+1)%MAX_ITEMS;
    aux1--;
  }
  pthread_mutex_unlock(&theQueue->lock);
  return;
}

