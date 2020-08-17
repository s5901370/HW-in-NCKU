/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      // cout<<"A"<<endl;
      mtest.reset(toSizeT(b));
      
      #else
      // cout<<"B"<<endl;
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
  vector<string> options;
  
  // cout<<"A"<<endl; 
  if(!CmdExec::lexOptions(option,options))
    return CMD_EXEC_ERROR;
  
  bool doA = false,gotN = false,gotS = false;
  int num,arr,input;
  // cout<<"A"<<endl; 
  if(options.size()>0){
    int n = options.size();
    for(int i=0;i < n;++i){
      if(!myStrNCmp("-Array",options[i],2)){//"-a"
        if(doA){
          if(gotS)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
        }
        if(i+1>=n)
          return CmdExec::errorOption(CMD_OPT_MISSING,options[i]);
        doA = true;
      }
      else if(myStr2Int(options[i],input)){//is a number
        if(gotS && gotN){
          // cout<<"A"<<endl;
          return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
        }
        if(doA){
          // cout<<"A"<<endl;
          if(gotS){
            gotN = true;
            num = input;
            // cout<<num<<endl;
            if(num <= 0)
              return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
            continue;
          }
          else {
            // cout<<"A"<<endl;
            gotS = true;
            arr = input;
            if(arr <= 0)
              return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
            continue;
          }
          if(i+1>=n)
            return CmdExec::errorOption(CMD_OPT_MISSING,options[i]);
        }
        if(gotN)
          return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
        gotN = true;
        num = input;
        if(num <= 0)
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
      }
      else{
        if(doA && gotS && gotN)
          return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
      }
    }
  }
  else
    return CmdExec::errorOption(CMD_OPT_MISSING,"");
  // cout<<"A"<<endl; 

  // int ar;

  //   if(options.size() == 2)
  //     return CmdExec::errorOption(CMD_OPT_MISSING,"-a");
  //   else if(options.size() > 2){
  //     if(!myStr2Int(options[2],ar))
  //       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[2]);
  //     else if(options[2] == "0")
  //       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[2]);
  //     else if(options.size()>3)
  //       return CmdExec::errorOption(CMD_OPT_EXTRA,options[3]);
  //   }
  // }
  // else{
  //   if(options.size()>1)
  //     return CmdExec::errorOption(CMD_OPT_EXTRA,options[1]);
  //   if(options[0] == "0")
  //     return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  // }
  // cout<<"A"<<endl;  //

  try{
    if(doA){
      mtest.newArrs(num,arr);
      // cout<<"A"<<endl;
    }
    else{ 
      mtest.newObjs(num);}
  }catch(bad_alloc&){
    // cout<<"A"<<endl;
    return CMD_EXEC_ERROR;
  }
  // cout<<"A"<<endl;


   return CMD_EXEC_DONE;
  
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
  vector<string> options;
  bool doI = false,doR = false,doA = false,gotO = false,gotN = false;
  size_t Id,Rid;
  int input,number;
  size_t oSize = mtest.getObjListSize();
  size_t aSize = mtest.getArrListSize();
  if(!lexOptions(option,options))
    return CMD_EXEC_ERROR;
  int n = options.size();
  if(options.size()>0){
    for(int i =0;i<n;++i){
      if(!myStrNCmp("-Index",options[i],2)){
        if(doI){
          if(gotO)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
        }
        if(doR){
          if(gotN)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
        }
        if(i+1>=n)
          return CmdExec::errorOption(CMD_OPT_MISSING,options[i]);
        doI = true;
      }
      else if(!myStrNCmp("-Random",options[i],2)){
        if(doI){
          if(gotO)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
        }
        if(doR){
          if(gotN)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
        }
        if(i+1>=n)
          return CmdExec::errorOption(CMD_OPT_MISSING,options[i]);
        doR = true;
      }
      else if(myStr2Int(options[i],input)){
        if(doI){
          if(gotO)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          if(input < 0)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          gotO = true;
          Id = (size_t)input;
          number = i;
          continue;
        }
        else if(doR){
          if(gotN)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          if(input < 0)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          gotN = true;
          Rid = (size_t)input;
          number = i;
          continue;
        }
        else
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
      }
      else if(!myStrNCmp("-Array",options[i],2)){
        if(n == 1)
          return CmdExec::errorOption(CMD_OPT_MISSING,options[i]);
        if(doA){
          if(doR){
            if(gotN)
              return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          }
          else if(doI){
            if(gotO)
              return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          }
          else//-a -a
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
        }
        else{
          if(doR && !gotN)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
          if(doI && !gotO)
            return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
        }
        doA = true;
      }
      else
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[i]);
    }
  }
  else
    return CmdExec::errorOption(CMD_OPT_MISSING,"");

  
  if(doI){
    if(doA){
      if(Id<aSize)
        mtest.deleteArr(Id);
      else{ 
        cerr<<"Size of array list ("<<aSize<<") is <= "<<Id<<"!!"<<endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[number]);
      } //n<0 
    }
    else{
      if(Id<oSize)
        mtest.deleteObj(Id);
      else{
          cerr<<"Size of object list ("<<aSize<<") is <= "<<Id<<"!!"<<endl;
          return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[number]);
      }
    }
  }
  else if(doR){
    if(doA){
      if(aSize == 0){
        cerr<<"Size of array list is 0!!"<<endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
      } 
      for(size_t i=0;i<Rid ;++i)
        mtest.deleteArr(rnGen(aSize));
    }
    else{
      if(oSize == 0){
        cerr<<"Size of object list is 0!!"<<endl;
        return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
      }
      for(size_t i=0;i<Rid ;++i)
        mtest.deleteObj(rnGen(oSize));
    }
  }



  // int n;
  // if(options.size()>1){
  //     if(!myStr2Int(options[1],n))
  //       return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
  //     else{
  //       if(doI){
  //           if(options.size()>2){//do arr

  //             if(!myStrNCmp("-Array",options[2],2)){
  //               if(n>=0 && n<aSize)
  //                 mtest.deleteArr(n);
  //               else if(n >= aSize){
  //                 cerr<<"Size of array list ("<<aSize<<") is <= "<<n<<"!!"<<endl;
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
  //               } //n<0 
  //               else
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
  //             } 
  //             else{//options[2]!= -a
  //               if(!myStrNCmp("-Index",options[2],2) || !myStrNCmp("-Random",options[2],2))
  //                 return CmdExec::errorOption(CMD_OPT_EXTRA,options[2]);
  //               else
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[2]);
  //             }

  //           }
  //           else{ // size = 2 ,do obj
  //             if(n>=0 && n<oSize)
  //               mtest.deleteObj(n);
  //             else if(n >= aSize){
  //                 cerr<<"Size of object list ("<<aSize<<") is <= "<<n<<"!!"<<endl;
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
  //             } //n<0 
  //             else
  //               return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[1]);
              
  //           }
  //       }
  //       else if(doR){
  //           if(options.size()>2){ //do arr
  //             if(!myStrNCmp("-Array",options[2],2)){
  //               if(aSize == 0){
  //                 cerr<<"Size of array list is 0!!"<<endl;
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  //               }
  //               // RandomNumGen gen;
                
  //               for(int i=0;i<n ;++i){
  //                 // int g = gen(aSize);
  //                 mtest.deleteArr(rnGen(aSize));
  //               }

  //             }
  //             else{
  //               if(!myStrNCmp("-Index",options[2],2) || !myStrNCmp("-Random",options[2],2))
  //                 return CmdExec::errorOption(CMD_OPT_EXTRA,options[2]);
  //               else
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[2]);
  //             }
  //           }
  //           else{ // size = 2, do obj
  //             if(oSize == 0){
  //                 cerr<<"Size of object list is 0!!"<<endl;
  //                 return CmdExec::errorOption(CMD_OPT_ILLEGAL,options[0]);
  //             }
  //             // RandomNumGen gen;
  //             for(int i=0;i<n ;++i){
  //               // int g = gen(oSize);
  //               mtest.deleteObj(rnGen(oSize));
  //             }
  //           }
  //       }
  //     }
          
  // }
  // else// size = 1
  //     return CmdExec::errorOption(CMD_OPT_MISSING,options[0]);
    
  

   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


