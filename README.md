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
<img width="736" alt="original folder after g++ command(3)" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/3164e085-f632-4f3c-a035-423ce7f67ebf">


2. **Create a .txt file and name it testfile.txt or you can use keyfile.txt as well** (We decide to make a new file.)
3. Write “this is test file.” into testfile.txt
<img width="705" alt="testfile for _send" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/48d3e66a-8629-4465-a578-554f427a3cd1">



4. Open the terminal and go to the directory where your sample files folder is located, then run the g++ command for sender.cpp and recv.cpp
Command: **g++ sender -o sender sender.cpp** and **g++ -o recv recv.cpp**
<img width="705" alt="commands" src= "https://github.com/aseng2/CPSC351-Project1/assets/98761137/2df5208d-d78e-45f8-9254-8c823e7f02a7">


Once you are done, go back to your folder, and it should have two program debugged names: sender and recv.


5. Open two terminals:
6. First one for the sender, type: ./sender testfile.txt in terminal
7. A second one for recv:, type: ./recv in terminal 
<img width="834" alt="_send and _recv in terminal (4)" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/6df4efe6-4cc3-4f75-b7ef-b54254084b02">

If the bytes number match in both terminals, the receiver got the file, so go back and check in your samplefiles folder.


<img width="728" alt="original folder after all command executed(5)" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/54a758ea-0496-4be1-b2a7-c14fdff69fc1">

Testfile.txt__recv shows that the file was received; open it, and it should say the same sentence as testfile.txt.

<img width="728" alt="testfile for _recv" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/2b631f9a-cba9-4244-bf6a-0ee38334d38c">

This show your program worked successfully.

**Here is the Hello world version**

<img width="704" alt="HelloWorld-1" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/6385e659-7b21-403d-857f-e4117695810c">
<img width="824" alt="HelloWorld-2" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/c97f8320-4df1-4a1b-92a4-35f9687fadda">
<img width="709" alt="HelloWorld-3" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/b016cf27-c174-4fa6-9152-f6b2bdeaec22">
<img width="709" alt="HelloWorld-4" src="https://github.com/aseng2/CPSC351-Project1/assets/98761137/81213d95-d7f0-41c2-9b2a-b8af04ad93be">

## Contribution 
Anthony Seng:
Design Doc, recv.cpp and readme

Sagarkumar Patel:
sender.cpp and readme

Edmarck Sosa Pineda:
Debugging and code checking

Afnan Al Salihi:
Debugging and code checking



