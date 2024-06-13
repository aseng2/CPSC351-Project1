# CPSC351 Section 1 Summer 24 Project1
## Group Members:
- Anthony Seng (Section 1) Email: aseng6825@csu.fullerton.edu
- Sagarkumar Patel (Section 1) Email: s_patel@csu.fullerton.edu
- Edmarck Sosa Pineda (Section 1) Email: e.sosa@csu.fullerton.edu
- Afnan Al Salihi (Section 1) Email: aalsalihi@csu.fullerton.edu

## Explanation
Explanation of Sender and Receiver:
	The program’s main purpose is to depict shared memory and message queues between Sender (sender.cpp) and Receiver (recv.cpp). Shared memory enables information communication between processes by reading/writing data to/from a shared memory region. Message queue allows information communication between processes by exchanging messages utilizing a queue. 

 ## How to run the program

1. Make sure all the cpp files are in the same folder.
<img width="728" alt="original folder after all command executed(5)" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/54a758ea-0496-4be1-b2a7-c14fdff69fc1">


2. **Create a .txt file and name it testfile.txt or you can use keyfile.txt as well** (We decide to make a new file.)
3. Write “this is test file.” into testfile.txt
<img width="728" alt="testfile for _recv" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/3734f364-e191-4c08-9ff8-0a1829299a58">


4. Open the terminal and go to the directory where your sample files folder is located, then run the g++ command for sender.cpp and recv.cpp
Command: **g++ sender -o sender sender.cpp** and **g++ -o recv recv.cpp**
<img width="834" alt="_send and _recv in terminal (4)" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/6df4efe6-4cc3-4f75-b7ef-b54254084b02">



Once you are done, go back to your folder, and it should have two program debugged names: sender and recv.


5. Open two terminals:
6. First one for the sender, type: ./sender testfile.txt in terminal
7. A second one for recv:, type: ./recv in terminal 


If the bytes number match in both terminals, the receiver got the file, so go back and check in your samplefiles folder.


Testfile.txt__recv shows that the file was received; open it, and it should say the same sentence as testfile.txt.


This show your program worked successfully. 

