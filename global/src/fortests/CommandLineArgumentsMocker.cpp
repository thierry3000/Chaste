/*

Copyright (c) 2005-2012, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <cstring>

#include "CommandLineArguments.hpp"
#include "CommandLineArgumentsMocker.hpp"

CommandLineArgumentsMocker::CommandLineArgumentsMocker(std::string newArguments)
{
    // Save the location of the real arguments to be restored later...
    mpNumOldArgs = CommandLineArguments::Instance()->p_argc;
    mpOldArgs = CommandLineArguments::Instance()->p_argv;

    boost::trim_all(newArguments); // Remove spaces/tabs at beginning and end, and any in the middle to just one space.

    std::vector<std::string> strings;
    boost::split(strings, newArguments, boost::is_any_of("\t "));

    mNumArgs = strings.size() + 1;

    mpArgs = new char* [mNumArgs];

    // Program name
    mpArgs[0] = (*mpOldArgs)[0];

    for (unsigned i=0; i<strings.size(); i++)
    {
        mpArgs[i+1] = new char[strings[i].length() + 1]; // NULL terminator
        strcpy(mpArgs[i+1], strings[i].c_str());
    }

    // Replace the original command line arguments.
    CommandLineArguments::Instance()->p_argc = &mNumArgs;
    CommandLineArguments::Instance()->p_argv= &mpArgs;
}


CommandLineArgumentsMocker::~CommandLineArgumentsMocker()
{
    for (int i=1; i<mNumArgs; i++)
    {
        delete mpArgs[i];
    }
    delete mpArgs;

    // Restore the real arguments
    CommandLineArguments::Instance()->p_argc = mpNumOldArgs;
    CommandLineArguments::Instance()->p_argv = mpOldArgs;
}

