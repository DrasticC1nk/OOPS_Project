Please follow the steps below to build and run my program but first the pre-requisits -> There are a few pre-requisits required to run this but most of them have been included. All you need is a windows machine(linux debbian support comming soon) and a c++ compiler. I would highly reccomend using the same compiler as me because of the different threading models different compilers use. 

Here is a quick guide to setup my compiler:-

1. Use this direct link to dowload the compiler with POSIX threads(release 2) >>> [text](https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-19.1.1-12.0.0-ucrt-r2/winlibs-x86_64-posix-seh-gcc-14.2.0-llvm-19.1.1-mingw-w64ucrt-12.0.0-r2.zip) 

2. Once downloaded, extract and place the folder in your desired directory and volume. 

3. To use the compiler, we can simply open the terminal within the bin directory of the extracted folder but for the ease of use and to setup a really good compiler for future, we can simply add the bin directory to the path.

4. Copy the address of the bin directory and add it to the path. For windows, simply add type 'enviroment' in the start's search bar and open the 'Edit the system variables'. 

5. Then 'Enviroment variables' -> Under 'System variables' select 'Path' -> Edit -> New -> Paste the absolute path to the bin folder and click ok. 

6. (Optional)Now we have the compiler setup. But one more QOL thing we can do is edit the name of the make program so that we can call it easily. 

7. Within the same bin folder, there should a an executable called 'mingw32-make.exe'. Change it to 'make.exe'. As this is within the same bin folder, we don't need to do anything else. 

With this our compiler is setup(highly recomended) which may solve any conflicts duirng building our project. Also this a very good compiler to be used as your main compiler. The support for POSIX threads makes it great if you wanna use some multi-threading libraries like standard threads. 

Also I highly discourage using MVS or it's compiler because I assume the threading library it uses might clashes with mine unless you explicitly change it to the one I provided. I did not test it whatsoever.

Now, to build our program, we can follow the following steps:-

1. Make your way to the project directory 'OOPS_PROJECT' within the submission zip folder after extracting. 

2. Assuming you have changed your directory to the folder mentioned in the previous step, simply write 'make' in the console/terminal and hit enter. If everything was setup prefectly, you should see everything being built. 

3. (Optional)You can use the 'make clean' command to remove all the object files and executables if you want for any reason.

4. Now, the main executable called 'maze.exe' can be found within the 'src/bin/maze.exe'. 

5. Place the maze and path file within the load folder. The name of both the file can be anything with '.txt' or '.dat'.

6. Run using 'src/bin/maze.exe <mazefile.txt/.dat> <pathfile.txt/.dat>'. Press 'esc' to terminate the animation window or simply close it. 

7. (Optional)If you wish to run multiple test cases, create a folder inside the load folder and place both of those files in the folders. One folder should hold only a single pair at a time. The load folder already contians some sample test cases. Running the 'test.bat' file after compiling will start the routine. If you do it this way, make sure there are only folders within load folder due to the way my batch routine is written. Press 'esc' after the animation completes to move on to the next test case. A log file will also be created fo convinience and will update automatically each time the routine runs. A log file with results for all the test cases already present in the load folder is available in the project directory. 

So this readme should hold all the requirements and steps required to run our small program by anyone. Still in case of any ambiguity, please contact me. 

Thank you. 

Here is a sample maze and text file format you must use.

<MazeFile>
1 0 0 0
1 0 0 0
1 0 0 0
1 1 1 1

<PathFile>
3 3 3 2 2 2