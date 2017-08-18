# CondorTemplate
A setup for using condor on the submit.mit.edu machine.  Please run small test jobs first to make sure you aren't going to break anything before submitting large jobs!

#First time setup
1) If you have never used submit.mit.edu before, then you need to setup a few things to run when you log in.  In your home directory ($HOME), make a file called .bash_profile and fill it with the following:

#!/bin/bash
if [ -f ~/.bashrc ]; then
  . ~/.bashrc
fi

also make a .bashrc file and fill it with the following:

#!/bin/bash
source /cvmfs/cms.cern.ch/cmsset_default.sh
alias root="root -l"

These two files will load the CMSSW frameworks and let you set up a CMSSW environment, run root, etc.

2)  Make sure you have a grid certificate set up in your home directory (These usually go in a .globus directory, same ones that are used to run on CRAB).  If you do not do this, you will not be able to transfer your output files.


3)  In your /work/$USER/ area, setup a working CMSSW environment (i.e. commands: 'cmsrel *release*' and 'cmsenv') and clone this repository (using git clone) into it.  YOU MUST USE THE /work/ AREA, for some reason condor does not see the home areas.



Using the code:

Other than this readme, this repo has 5 files.  submit_template.condor should not need to be touched.  run.C can be replaced with your own code you wish to compile and run on condor.  fileList.txt is just an example additional input file.  This setup expects your C code to be have a main() function which takes at least 2 parameters: the job number and total number of jobs (so it runs as './run.exe job totalJobs *other parameters*).  As an output, this file should produce a file called 'output_*jobNumber*.root' in order for the transfer to hadoop work correctly. 

4)In submit.sh edit lines 4,5,6,7.  L4 should be the name of the C++ file containing the main() method.  L5 should be a list of any input files you wish to be transferred along with the executable to the condor node (examples: correction .root tables, lookup tables, etc.).  These files should be fairly small (not full data files!).  You do not need to transfer your entire C++ setup, because your code will be compiled and then transfered as a .exe.  L6 is the total number of jobs you wish to submit.  L7 is a list of other parameters you wish to pass in while running the code (see the last parenthetical on paragraph above).

5)In run.sh, make sure that the two awk commands on L18-19 correctly substitute the correct amount of parameters to be added after run.exe (in this example we have 4 parameters, (jobNumber totalNumberOfJobs fileList.txt 56).  If you need to pass more or less parameters and you already listed them in submit.sh, then simply follow pattern on L18-L19 to insert the correct number of parameters (adding -v *letter*=$*parameterNumber* and " "*letter* later in the line.  FINALLY CHANGE L23 TO POINT TOWARDS YOUR DESIRED OUTPUT DIRECTORY ON HADOOP AFTER THE '.EDU:2811//'.  BE WARNED THAT THIS WILL OVERWRITE ANY EXISTING FILES OF THE SAME NAME THERE.

6) run submit.sh (./submit.sh).  This will compile your code (make sure there are no errors), make a .tar.gz of all extra input files needed, and place them into a new directory which is called CondorJob_*DATE*, along with the .exe formed from the compilation.  It should also prompt for your grid certificate password, so input that.

7) cd into the new directory.  Here you may run final checks interactively using the .exe if you would like (i.e. manually running run.exe for 1 jobs, something like './run.exe 0 100 fileList.txt 56').  If you would like to procede, submit the condor job with 'condor_submit run.condor'

8) Output files should show up in your specified hadoop directory. Log, output, error files will show up in the /Logs/ subdirectory created inside this directory.  

