# SVNFile-System
The addition, deletion, modification, viewing of files and version control


1. Operation mode:
Create the Test folder for the run path of this program. Copy "softwaretest.exe" from the "SoftwareTest\bin\Debug" directory into the Test folder and rename it "svn" to double-click to run.

2. After running, type "create", and the "_svn_" folder will be created under the test folder, which will hold the version information. When the program runs, an empty folder called "Version0" is created in the "_svn_" folder without holding any information. It is a logical version. Starting with version 1, a "Version" folder is created after each "commit" to hold files that are different from the previous version, thereby reducing the amount of space consumed. When you type the "quit" command, there writes a "information.txt" in the folder _svn_ to save some variables that were used when the program was running. When you run "svn.exe" next time, you can read the above information and return to the original state of the previous version. All uncommitted information will not be recovered.

3. Instead of using the "quit" command, simply close the "svn.exe" to exit the program. The program will not save the information, and the next time it runs, it must delete the "_svn_" folder before running "svn.exe".
