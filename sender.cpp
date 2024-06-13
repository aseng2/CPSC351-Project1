#include <sys/shm.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "msg.h"    /* For the message struct */

/* The size of the shared memory segment */
#define SHARED_MEMORY_CHUNK_SIZE 1000

/* The ids for the shared memory segment and the message queue */
int shmid, msqid;

/* The pointer to the shared memory */
void* sharedMemPtr;

/**
 * Sets up the shared memory segment and message queue
 * @param shmid - the id of the allocated shared memory 
 * @param msqid - the id of the allocated message queue
 */
void init(int& shmid, int& msqid, void*& sharedMemPtr)
{       
	/* create file called keyfile.txt containing string "hello world"*/
        FILE* keyfile = fopen("keyfile.txt", "w");
	if (!keyfile){
		perror("fopen");
		exit(1);
	}
	fprintf(keyfile, "Hello world");
	fclose(keyfile);

	/*generate the key using ftok*/
 	key_t key = ftok("keyfile.txt",'a');
	if (key == -1) {
	    perror("ftok");
	    exit(1);
	}
       
      /*get id of the shared memory segment */
	shmid = shmget(key, SHARED_MEMORY_CHUNK_SIZE, 0644 | IPC_CREAT);
	if (shmid == -1) {
            perror("Error shmget");
            exit(1);
	}

	/*Attach to the shared memory*/
 	sharedMemPtr = shmat(shmid, (void*)0, 0);
	if (sharedMemPtr == (char*)(-1)) {
	    perror("Error shmat");
	    exit(1);
	}


	/*Get id of the message queue */
	msqid = msgget(key, 0644 | IPC_CREAT);
	if (msqid == -1) {
	    perror("Error msgget");
	    exit(1);
	}

}

/**
 * Performs the cleanup functions
 * @param sharedMemPtr - the pointer to the shared memory
 * @param shmid - the id of the shared memory segment
 * @param msqid - the id of the message queue
 */
void cleanUp(const int& shmid, const int& msqid, void* sharedMemPtr)
{
	/*detach from shared memory*/
	if (shmdt(sharedMemPtr) == -1) {
	    perror("Error shmdt");
	    exit(1);
	}

	/*deallocate the shared memory segment*/
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
	    perror("Eerror shmctl");
	    exit(1);
	}

	/*deallocate the message que */
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
	    perror("Error msgctl");
	    exit(1);
	}
}

/**
 * The main send function
 * @param fileName - the name of the file
 * @return - the number of bytes sent
 */
unsigned long sendFile(const char* fileName)
{

	/* A buffer to store message we will send to the receiver. */
	message sndMsg; 
	
	/* A buffer to store message received from the receiver. */
	ackMessage rcvMsg;
		
	/* The number of bytes sent */
	unsigned long numBytesSent = 0;
	
	/* Open the file */
	FILE* fp =  fopen(fileName, "r");

	/* Was the file open? */
	if(!fp)
	{
		perror("Error opening file.(fopen)");
		exit(-1);
	}
	
	/* Read the whole file */
	while(!feof(fp))
	{
		/* Read at most SHARED_MEMORY_CHUNK_SIZE from the file and
 		 * store them in shared memory.  fread() will return how many bytes it has
		 * actually read. This is important; the last chunk read may be less than
		 * SHARED_MEMORY_CHUNK_SIZE.
 		 */
		if((sndMsg.size = fread(sharedMemPtr, sizeof(char), SHARED_MEMORY_CHUNK_SIZE, fp)) < 0)
		{
			perror("Error reading memory.(fread)");
			exit(-1);
		}
		
		/* count the number of bytes sent. */
		numBytesSent += sndMsg.size;


		/*Send a message to the receiver telling him that the data is ready
 		 * to be read (message of type SENDER_DATA_TYPE).
 		 */
		sndMsg.mtype = SENDER_DATA_TYPE;
		if (msgsnd(msqid, &sndMsg, sizeof(sndMsg) - sizeof(long), 0) == -1) {
			perror("msgsnd");
			exit(-1);
		}


		/*Wait until the receiver sends us a message of type RECV_DONE_TYPE telling us 
 	         * that he finished saving a chunk of memory. 
 		 */
		if (msgrcv(msqid, &rcvMsg, sizeof(rcvMsg) - sizeof(long), RECV_DONE_TYPE, 0) == -1) {
		    perror("msgrcv");
		    exit(-1);
		}


	}


	/* once we are out of the above loop, we have finished sending the file.
 	  * Lets tell the receiver that we have nothing more to send. We will do this by
 	  * sending a message of type SENDER_DATA_TYPE with size field set to 0. 	
	  */
	sndMsg.size = 0;
	if (msgsnd(msqid, &sndMsg, sizeof(sndMsg) - sizeof(long), 0) == -1) {
	    perror("msgsnd");
	    exit(-1);
	}

	/* Close the file */
	fclose(fp);

	return numBytesSent;
}

/**
 * Used to send the name of the file to the receiver
 * @param fileName - the name of the file to send
 */
void sendFileName(const char* fileName)
{
	/* Get the length of the file name */
	int fileNameSize = strlen(fileName);

	/* Make sure the file name does not exceed 
	 * the maximum buffer size in the fileNameMsg
	 * struct. If exceeds, then terminate with an error.
	 */
	if (fileNameSize > sizeof(fileNameMsg::fileName)) {
	   fprintf(stderr, "Filename is too long.\n");
	   exit(-1);
	}

	/*Create an instance of the struct representing the message
	 * containing the name of the file.
	 */
	fileNameMsg fnMsg;



	/*Set the message type FILE_NAME_TRANSFER_TYPE */
	fnMsg.mtype = FILE_NAME_TRANSFER_TYPE;


	/*Set the file name in the message */
	strcpy(fnMsg.fileName, fileName);


	/* Send the message using msgsnd */
	if (msgsnd(msqid, &fnMsg, sizeof(fnMsg) - sizeof(long), 0) == -1) {
	    perror("msgsnd");
	    exit(-1);
	}
}


int main(int argc, char** argv)
{
	
	/* Check the command line arguments */
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]);
		exit(-1);
	}
		
	/* Connect to shared memory and the message queue */
	init(shmid, msqid, sharedMemPtr);
	
	/* Send the name of the file */
        sendFileName(argv[1]);
		
	/* Send the file */
	fprintf(stderr, "The number of bytes sent is %lu\n", sendFile(argv[1]));
	
	/* Cleanup */
	cleanUp(shmid, msqid, sharedMemPtr);
		
	return 0;
}
