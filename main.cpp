/*******************************************************************
*                                                                  *
*  Copyright (C) 2016 Lukas Genkinger <lukas.genkinger@gmail.com>  *
*                                                                  *
*******************************************************************/



/*****************************************************************************
 *                                                                           *
 *   This program is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *  
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                           *
 ****************************************************************************/


#include <iostream>
#include <stdio.h>
#include <dirent.h>
#include <string>
#include <string.h>
#include <vector>
#include "colors.h"
#include <unistd.h>
#include <chrono>

unsigned long getFileSize(std::string path){

  FILE* fp = fopen(path.c_str(),"rb");
  fseek(fp,0L,SEEK_END);
  unsigned long size = ftell(fp);
  fclose(fp);

  return size;
}

std::vector<std::string> getFilesFromCurrDir(std::string path){
  DIR* dir;
  dirent* ent;

  std::string currPath;

  if(path.substr(path.size()-1,path.size()) != "/")
  {
   currPath = path + "/";
  }
  else
  {
    currPath = path;
  }


  std::vector<std::string> entries;
  dir = opendir(currPath.c_str());

  while ((ent = readdir(dir)))
    {
      if(ent->d_type == DT_REG)
	entries.push_back(std::string(currPath + ent->d_name));
    }
  closedir (dir);
  return entries;
}

std::vector<std::string> getFilesFromDir(std::string path){
  DIR* dir;
  dirent* ent;
  std::string currPath;

  if(path.substr(path.size()-1,path.size()) != "/")
  {
   currPath = path + "/";
  }
  else
  {
    currPath = path;
  }

  std::vector<std::string> entries;
  dir = opendir(currPath.c_str());

  while ((ent = readdir(dir))){

      if(ent->d_type == DT_REG){
        entries.push_back(std::string(currPath + ent->d_name));
        }
      else if(ent->d_type == DT_DIR){
        if((strcmp(ent->d_name,".") !=0) && (strcmp(ent->d_name,".."))!=0)
            {
                std::vector<std::string> tmp = getFilesFromDir((currPath + ent->d_name));
                entries.insert(entries.end(),tmp.begin(),tmp.end());
            }
        }
    }


  closedir (dir);
  return entries;
}

void print_usage(std::string prgrm_name){
    std::cout << "\t >> Usage: " << prgrm_name << "  [-rtv] [-s size] [-h threshold] [-p path]" << std::endl;
}

void print_verbose(std::string a, std::string b,char* col_dir ,char* col, float sz_conv,char* col_nrm){


    std::string lastdir = b.substr(0,b.find_last_of("/"));
    std::string currdir = a.substr(0,a.find_last_of("/"));

    if(currdir != lastdir)
    {
        printf("%s<%s>%s\n  |-- %s  %s[%f]\n%s",col_dir,a.substr(0,a.find_last_of("/")).c_str(),col_nrm,a.substr(a.find_last_of("/")+1,a.size()).c_str(),col,double(sz_conv),col_nrm);
    }
    else
    {
        printf("  |-- %s  %s[%f]\n%s",a.substr(a.find_last_of("/")+1,a.size()).c_str(),col,double(sz_conv),col_nrm);
    }
}



int main(int argc, char** argv){

    int option = 0;

/*FLAGS*/
    int flag_rec = 0;
    int flag_verbose = 0;
    int flag_time = 0;
    int flag_size = 2;
    int flag_exclusive = 0;
    int flag_nocol = 0;

/*OPTARGS*/
    double thresh_value = 1000000000000000.d;
    std::string p_value = "./";

/*POLL PARAMETERS*/
    while ((option = getopt (argc, argv, "s:rntveh:p:")) != -1){
        switch (option){
        case 'r':
            flag_rec = 1;
            break;
        case 's':
            if(strcmp(optarg,"b") == 0) flag_size = 1;
            if(strcmp(optarg,"k") == 0) flag_size = 2;
            if(strcmp(optarg,"m") == 0) flag_size = 3;
            if(strcmp(optarg,"g") == 0) flag_size = 4;
            break;
        case 'h':
            thresh_value = atof(optarg);
            break;
        case 'n':
            flag_nocol = 1;
            break;
        case 't':
            flag_time = 1;
            break;
        case 'p':
            p_value = std::string(optarg);
            break;
        case 'v':
            flag_verbose = 1;
            break;
        case 'e':
            flag_exclusive = 1;
            break;
        default:
            print_usage(std::string(argv[0]));
            exit(-1);
        }
}


    std::vector<std::string> ret;

    auto t_search1 = std::chrono::high_resolution_clock::now(); //TIMING

    if(flag_rec == 1){
        ret = getFilesFromDir(p_value);
    }else{
        ret = getFilesFromCurrDir(p_value);
    }

    auto t_search2 = std::chrono::high_resolution_clock::now(); //TIMING

    unsigned int total;


auto t_print1 = std::chrono::high_resolution_clock::now(); //TIMING

    for(int i=0; i < ret.size(); i++){
        unsigned long sz = getFileSize(ret.at(i));
        double sz_conv;

        switch(flag_size){

        case 1:
            sz_conv = sz;
            break;
        case 2:
            sz_conv = sz/1000.f;
            break;
        case 3:
            sz_conv = sz/1000000.f;
            break;
        case 4:
            sz_conv = sz/1000000000.f;
            break;
        default:
            exit(-2);
            break;
      }

if(flag_nocol == 1){
    if(i < 1){
        print_verbose(ret[i],"","","",sz_conv,"");
    }
    else{
        print_verbose(ret[i],ret[i-1],"","",sz_conv,"");
    }
}else if(flag_verbose == 1){
        if(i < 1){
            if(sz_conv >= thresh_value){print_verbose(ret[i],"",KYEL,KRED,sz_conv,KNRM);}
            else{print_verbose(ret[i],"",KYEL,KCYN,sz_conv,KNRM);}
        }else{
            if(sz_conv >= thresh_value){print_verbose(ret[i],ret[i-1],KYEL,KRED,sz_conv,KNRM);}
            else{print_verbose(ret[i],ret[i-1],KYEL,KCYN,sz_conv,KNRM);}
        }
}
else{
        if(i < 1){
            if(sz_conv >= thresh_value) print_verbose(ret[i],"",KYEL,KRED,sz_conv,KNRM);
        }else{
            if(sz_conv >= thresh_value) print_verbose(ret[i],ret[i-1],KYEL,KRED,sz_conv,KNRM);
        }
}
        total += sz;
    }

auto t_print2 = std::chrono::high_resolution_clock::now();

    if(flag_verbose == 1){
        std::cout << "\n\n--------------FINISHED----------------\n" << std::endl;
        std::cout << "[TOTAL NUMBER OF FILES] : " << ret.size() << std::endl;
        std::cout << "[TOTAL SIZE] : " << total/1000.f << "kB" << std::endl;
        std::cout << "[TOTAL SIZE] : " << total/1000000.f << "MB" << std::endl;
        std::cout << "[TOTAL SIZE] : " << total/1000000000.f << "GB" << std::endl;
        }


    if(flag_time == 1){
    std::cout << "\n\n--------------------------------------\n" << std::endl;
	std::cout << "[SEARCH TIME] : " << std::chrono::duration_cast<std::chrono::milliseconds>(t_search2 - t_search1).count() / 1000.f << "s" << std::endl;
    std::cout << "[PRINT TIME] : " << std::chrono::duration_cast<std::chrono::milliseconds>(t_print2 - t_print1).count() / 1000.f << "s" <<std::endl;
    std::cout << "[TOTAL ELAPSED TIME] : " << (std::chrono::duration_cast<std::chrono::milliseconds>(t_search2 - t_search1).count() + std::chrono::duration_cast<std::chrono::milliseconds>(t_print2 - t_print1).count())/1000.f << "s" << std::endl;
    }

  return 0;
}
