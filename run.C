#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, const char* argv[])
{
  if(argc != 5)
  {
    std::cout << "Usage: <job> <totalJobs> <fileList> <dummyVar>" << std::endl;
    return 1;
  }  

  int job = std::atoi(argv[1]);
  int totalJobs = std::atoi(argv[2]);
  std::string fList = argv[3];
  int dummy = std::atoi(argv[4]);
  std::string buffer;
  std::vector<std::string> listOfFiles;
  std::ifstream inFile(fList.data());

  if(!inFile.is_open())
  {
    std::cout << "Error opening jet file. Exiting." <<std::endl;
    return 1;
  }
  else
  {
    int line = 0;
    while(true)
    {
      inFile >> buffer;
      if(inFile.eof()) break;
      if(line%totalJobs==job) listOfFiles.push_back(buffer);
      line++;
    }
  }

  dummy = dummy+1;

  TFile * f = TFile::Open(Form("output_%d.root",job),"recreate");
  TH1D * h = new TH1D("h","h",10,0,10);
  h->Fill(1);
  h->Write();
  f->Close(); 
  return 0; 
}
