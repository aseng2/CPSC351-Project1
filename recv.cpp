#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include "msg.h"    /* For the message struct */

using namespace std;

/* The size of the shared memory segment */
#define SHARED_MEMORY_CHUNK_SIZE 1000

/* The ids for the shared memory segment and the message queue */
int shmid, msqid;

/* The pointer to the shared memory */
void *sharedMemPtr = NULL;

/**
 * The function for receiving the name of the file
 * @return - the name of the file received from the sender
 */
string recvFileName()
{
	/* The file name received from the sender */
	string fileName;

	/* Declare an instance of the fileNameMsg struct to be used for holding the message received from the sender. */
	fileNameMsg msg;

	/* Receive the file name using msgrcv() */
	if (msgrcv(msqid, &msg, sizeof(fileNameMsg) - sizeof(long), FILE_NAME_TRANSFER_TYPE, 0) == -1)
	{
		perror("msgrcv");
		exit(1);
	}

	/* Return the received file name */
	fileName = msg.fileName;
	return fileName;
}

/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory 
 * @param msqid - the id of the shared memory
 * @param sharedMemPtr - the pointer to the shared memory
 */
void init(int& shmid, int& msqid, void*& sharedMemPtr)
{
	/* Create a file called keyfile.txt containing string "Hello world" */
	ofstream keyfile("keyfile.txt");
	keyfile << "Hello world";
	keyfile.close();

	/* Generate the key using ftok */
	key_t key = ftok("keyfile.txt", 'a');
	if (key == -1)
	{
		perror("ftok");
		exit(1);
	}

	/* Allocate a shared memory segment. The size of the segment must be SHARED_MEMORY_CHUNK_SIZE. */
	shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE, 0644 | IPC_CREAT);
	if (shmid == -1)
	{
		perror("shmget");
		exit(1);
	}

	/* Attach to the shared memory */
	sharedMemPtr = shmat(shmid, NULL, 0);
	if (sharedMemPtr == (void*)-1)
	{
		perror("shmat");
		exit(1);
	}

	/* Create a message queue */
	msqid = msgget(key, 0644 | IPC_CREAT);
	if (msqid == -1)
	{
		perror("msgget");
		exit(1);
	}
}

/**
 * The main loop
 * @param fileName - the name of the file received from the sender.
 * @return - the number of bytes received
 */
unsigned long mainLoop(const char* fileName)
{
	/* The size of the message received from the sender */
	int msgSize = -1;

	/* The number of bytes received */
	int numBytesRecv = 0;

	/* The string representing the file name received from the sender */
	string recvFileNameStr = string(fileName) + "__recv";

	/* Open the file for writing */
	FILE* fp = fopen(recvFileNameStr.c_str(), "w");

	/* Error checks */
	if (!fp)
	{
		perror("fopen");
		exit(-1);
	}

	/* Keep receiving until the sender sets the size to 0, indicating that there is no more data to send. */
	while (msgSize != 0)
	{
		/* Receive the message and get the value of the size field. */
		message msg;
		if (msgrcv(msqid, &msg, sizeof(message) - sizeof(long), SENDER_DATA_TYPE, 0) == -1)
		{
			perror("msgrcv");
			exit(1);
		}
		msgSize = msg.size;

		/* If the sender is not telling us that we are done, then get to work */
		if (msgSize != 0)
		{
			/* Count the number of bytes received */
			numBytesRecv += msgSize;

			/* Save the shared memory to file */
			if (fwrite(sharedMemPtr, sizeof(char), msgSize, fp) < 0)
			{
				perror("fwrite");
			}

			/* Tell the sender that we are ready for the next set of bytes. */
			ackMessage ack;
			ack.mtype = RECV_DONE_TYPE;
			if (msgsnd(msqid, &ack, sizeof(ack) - sizeof(long), 0) == -1)
			{
				perror("msgsnd");
				exit(1);
			}
		}
		/* We are done */
		else
		{
			/* Close the file */
			fclose(fp);
		}
	}

	return numBytesRecv;
}

/**
 * Performs cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */
void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
	/* Detach from shared memory */
	if (shmdt(sharedMemPtr) == -1)
	{
		perror("shmdt");
		exit(1);
	}
}

/**
 * Handles the exit signal
 * @param signal - the signal type
 */
void ctrlCSignal(int signal)
{
	/* Free system V resources */
	cleanUp(shmid, msqid, sharedMemPtr);
	exit(0);
}

int main(int argc, char** argv)
{
	/* Install a signal handler */
	signal(SIGINT, ctrlCSignal);

	/* Initialize */
	init(shmid, msqid, sharedMemPtr);

	/* Receive the file name from the sender */
	string fileName = recvFileName();

	/* Go to the main loop */
	fprintf(stderr, "The number of bytes received is: %lu\n", mainLoop(fileName.c_str()));

	/* Detach from shared memory segment, and deallocate shared memory and message queue */
	cleanUp(shmid, msqid, sharedMemPtr);

	return 0;
}

