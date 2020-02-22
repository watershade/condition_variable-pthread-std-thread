# condition_variable-pthread-std-thread
There are two source file:
  1、condvar.cpp  : from c++11, it import  std::thread, mutex and condition_variable (and future also). This file demostate how to use them.
  2、pcondvar.cpp : it is a demo to show how to use pthread_cond_xx   
  
  Makefile for this two file.
  
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  How to use them
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  This two file use g++ to compile, gcc should be ok. Please use them in linux (include WSL on win10). Make sue you have install g++. If you don't please use "sudo apt-get install build-essential" on ubuntu/debian. Or other analogous cammond in other linux distribute version.

Just use make to make two file and it will generate condvar and pcondvar file. Just use "./condvar" or "./pcondvar" to run them.
Or you can just make one file like: "make condvar" or "make pcondvar"
