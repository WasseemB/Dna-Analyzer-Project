//
// Created by Wasseem Bazbaz on 29/02/2020.
//

#include "Dup.h"
#include <iostream>
#include "../Model/DnaSequence.h"
#include "../DnaContainer.h"
#include "../Model/RealDnaSequence.h"

using namespace std;
const string Dup::s_HELP = "dup <#seq> [@<new_seq_name>]";
const string Dup::s_INFO = "duplicates the sequence.\n "
                           "If a new name is not provided, then it will be based on the name of <seq>,"
                           " suffixed by _1 (or _2, _3, ... if the name is already taken).";

static bool isDigit(const string &line) {
    char *p;
    strtol(line.c_str(), &p, 10);
    return *p == 0;
}


string getSubSequenceName(string seq_name) {

    seq_name.append("_");
    size_t count = 1;
    string new_seq_name = seq_name + (to_string(count));
    RealDnaSequence *res = DnaContainer::findByString(new_seq_name);
    while (res != NULL) {
        count++;
        new_seq_name = seq_name + (to_string(count));
        res = DnaContainer::findByString(new_seq_name);
    }
    return new_seq_name;
}

void Dup::run(vector<string> args) {
    try {
        RealDnaSequence *metaDnaSequence = NULL;
        const string &seq = args[1].substr(1);
        string seq_name;
        size_t seq_id = strtoul(seq.c_str(), NULL, 0);
        RealDnaSequence *new_sequence = DnaContainer::findById(seq_id);
        switch (args.size()) {
            case 2 :
                seq_name = getSubSequenceName(new_sequence->getName());
                break;
            case 3:
                seq_name = args[2].substr(1);
                break;
        }
        metaDnaSequence = new RealDnaSequence(new DnaSequence(new_sequence->getDnaSequence()->getSequence()),
                                              seq_name);
//        maybe one is added but not the other check it later.
        DnaContainer::addDna(metaDnaSequence->getId(), metaDnaSequence);
        DnaContainer::addDna(metaDnaSequence->getName(), metaDnaSequence);

    }
    catch (invalid_argument &invalid_argument) {
        cout << invalid_argument.what() << ", no data was added" << endl;
    }
}

int Dup::parse(vector<string> args) {
    int res = False;
    switch (args.size()) {
        case 2 :
            if (args[1][0] == '#') {
                const string &seq = args[1].substr(1);
                if (isDigit(seq)) {
                    res = True;
                } else {
                    cout << "after # is not a digit" << endl;
                }
            } else {
                cout << s_HELP << endl;
            }
            break;
        case 3:
            if (args[2][0] == '@') {
                res = True;
            } else {
                cout << s_HELP << endl;
            }
            break;
        default:
            cout << s_HELP << endl;
    }
    return res;

}

std::string Dup::getHelp() {
    return Dup::s_HELP;
}

std::string Dup::getInfo() {
    return Dup::s_INFO;
}
