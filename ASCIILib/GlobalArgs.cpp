#include "GlobalArgs.h"

#include <algorithm>


vector<string> ascii::GlobalArgs::args;

void ascii::GlobalArgs::Add(string arg)
{
    args.push_back(arg);
}

void ascii::GlobalArgs::AddList(vector<string> argList)
{
    for (int i = 0; i < argList.size(); ++i)
    {
        args.push_back(argList[i]);
    }
}

void ascii::GlobalArgs::Remove(string arg)
{
    args.erase(remove(args.begin(), args.end(), arg));
}

void ascii::GlobalArgs::Clear()
{
    args.clear();
}

bool ascii::GlobalArgs::Enabled(string arg)
{
    return find(args.begin(), args.end(), arg) != args.end();
}
