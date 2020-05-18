/******************************
 * Submitted by: enter your first and last name and net ID
 * CS 3339 - Spring 2020, Texas State University
 * Project 3 Pipelining
 * Copyright 2020, all rights reserved
 * Updated by Lee B. Hinkle based on prior work by Martin Burtscher and Molly O'Neil
 ******************************/
 
#include "Stats.h"

Stats::Stats() {
  cycles = PIPESTAGES - 1; // pipeline startup cost
  flushes = 0;
  bubbles = 0;

  memops = 0;
  branches = 0;
  taken = 0;

  for(int i = IF1; i < PIPESTAGES; i++) {
    resultReg[i] = -1;
  }
}

void Stats::clock() {
  cycles++;

  // advance all pipeline flip-flops
  for(int i = WB; i > IF1; i--) {
    resultReg[i] = resultReg[i-1];
  }
  // inject a no-op in IF1
  resultReg[IF1] = -1;
}

void Stats::registerSrc(int r) {
  if (r == 0) return;
  else{
    for (int i = EXE1; i < WB; i++){
       if (resultReg[i] == r){ 
	  for (int j = i; j < WB; j++){
           bubble();
         }
       }
     }
   } 
}

void Stats::registerDest(int r) {
 resultReg[ID] = r; 
}

void Stats::flush(int count) { // count == how many ops to flush 
  for (int i = 0; i < count; i++){
      cycles++;
      flushes++;
      for (int i = WB; i > IF1; i--){
         resultReg[i] = resultReg[i-1];
      }
      resultReg[IF1] = -1;
  }
}

void Stats::bubble() {
  bubbles++;
  cycles++;
  for (int i = WB; i > EXE1; i--){
    resultReg[i] = resultReg[i-1];
  }
  resultReg[EXE1] = -1; 
}

void Stats::showPipe() {
  // this method is to assist testing and debug, please do not delete or edit
  // you are welcome to use it but remove any debug outputs before you submit
  cout << "              IF1  IF2 *ID* EXE1 EXE2 MEM1 MEM2 WB         #C      #B      #F" << endl; 
  cout << "  resultReg ";
  for(int i = 0; i < PIPESTAGES; i++) {
    cout << "  " << dec << setw(2) << resultReg[i] << " ";
  }
  cout << "   " << setw(7) << cycles << " " << setw(7) << bubbles << " " << setw(7) << flushes;
  cout << endl;
}
