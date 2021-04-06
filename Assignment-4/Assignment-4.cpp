//===- SVF-Teaching Assignment 4-------------------------------------//
//
//    SVF: Static Value-Flow Analysis Framework for Source Code    
//
// Copyright (C) <2013->
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===-----------------------------------------------------------------------===//

/*
 // SVF-Teaching Assignment 4: Information Flow Tracking
 //
 // 
 */
#include "Assignment-4.h"
#include <fstream>
#include <sstream>
#include <typeinfo>
using namespace SVF;
using namespace llvm;
using namespace std;

//parse and get the SrcSnk.txt filename 
// params for assign-4 test1.ll -configSrcSnk SrcSnk.txt 
static llvm::cl::opt<string> SrcSnk("configSrcSnk", llvm::cl::init(""),
                                         llvm::cl::desc("Load Source && Sink API from file"));

/// Checking alias of the variables at source and sink.
bool TaintGraphTraversal::aliasCheck(const CallBlockNode *src, const CallBlockNode *snk)
{
    const RetBlockNode* ret = src->getRetBlockNode();
    const PAGNode* actualRetPAGNode = ret->getActualRet();
    for(const PAGNode* param : snk->getActualParms()){
        if(ander->alias(actualRetPAGNode->getId(), param->getId()))
            return true;
    }
    return false;
}

// Get sources function names read from checker_source_api collected from a text file
std::set<const CallBlockNode *>& TaintGraphTraversal::identifySources()
{
    for (const CallBlockNode *cs : pag->getCallSiteSet())
    {
        const SVFFunction *fun = SVFUtil::getCallee(cs->getCallSite());
        if (checker_source_api.find(fun->getName()) != checker_source_api.end())
        {
            sources.insert(cs);
        }
    }
    return sources;
}
// Get sinks function names read from checker_sink_api collected from a text file
std::set<const CallBlockNode *>& TaintGraphTraversal::identifySinks()
{
    for (const CallBlockNode *cs : pag->getCallSiteSet())
    {
        const SVFFunction *fun = SVFUtil::getCallee(cs->getCallSite());
        if (checker_sink_api.find(fun->getName()) != checker_sink_api.end())
        {
            sinks.insert(cs);
        }
    }
    return sinks;
}

// Start taint checking. 
// There is a tainted flow from p@source to q@sink 
// if (1) alias(p,q)==true and (2) source reaches sink on ICFG.
void TaintGraphTraversal::taintChecking(){
    if (!SrcSnk.empty())
        // add Src Snk function name AIP from file
        readSrcSnkFormFile(SrcSnk);
    for(const CallBlockNode* src : identifySources()){
        for(const CallBlockNode* snk : identifySinks()){
            vector<const ICFGNode*> path;
            set<const ICFGNode*> visited;
            std::stack<const Instruction *>callstack;
            if(aliasCheck(src,snk))
                DFS(visited, path, callstack, src, snk);
        }
    }
}

/// TODO: print each path once this method is called, and
/// (1) add each path (a sequence of node IDs) as a string into std::set<std::string> paths in the format "START: 1->2->4->5->END", where -> indicate an ICFGEdge connects two ICFGNode IDs
/// (2) dump and append each program path to a `ICFGPaths.txt` in the form of ‘{ln: number cl: number, fl:name} -> {ln: number, cl: number, fl: name} -> {ln:number, cl: number, fl: name}
void TaintGraphTraversal::printICFGPath(std::vector<const ICFGNode *> &path){

}

// TODO: Implement your code to parse the two lines from `SrcSnk.txt` in the form of 
// line 1 for sources  "{ api1, api2, api3 }"
// line 2 for sinks    "{ api1, api2, api3 }" 
void TaintGraphTraversal::readSrcSnkFormFile(const string& filename){
    
}
