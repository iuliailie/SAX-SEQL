/*
 * Authors: Georgiana Ifrim (georgiana.ifrim@gmail.com)
 *		    Thach Le Nguyen (thach.lenguyen@insight-centre.org)
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation.
 *
 */

#ifndef SEQL_CLASSIFY_H_
#define SEQL_CLASSIFY_H_


#include <vector>
#include <string>
#include <map>
#include "mmap.h"
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <cmath>
//#include "common.h"
#include "common_string_symbol.h"
#include "darts.h"
#include "sys/time.h"
#include "mytrie.h"



class SEQLClassifier
{
private:

	Trie trie;


    MeCab::Mmap<char> mmap;
    double *alpha;
    double bias;
    Darts::DoubleArray da;
    std::vector <int>  result;
    std::vector <stx::string_symbol> doc;
    std::map <std::string, double> rules;
    std::map <std::string, int> rules_and_ids;

    bool userule;
    int oov_docs;

    void project (std::string prefix,
                  unsigned int pos,
                  size_t trie_pos,
                  size_t str_pos,
                  bool token_type);


public:

    SEQLClassifier(): userule(false), oov_docs(0) {};

    double getBias();

    int getOOVDocs();

    void setRule(bool t);

    bool open (const char *file, double threshold);

    bool load_mytrie (const char *file, double threshold);

    // Compute the area under the ROC curve.
    double calcROC( std::vector< std::pair<double, int> >& forROC );

    // Compute the area under the ROC50 curve.
    // Fixes the number of negatives to 50.
    // Stop computing curve after seeing 50 negatives.
    double calcROC50( std::vector< std::pair<double, int> >& forROC );

    double classify (const char *line, bool token_type);

    double classify_with_mytrie(const char *line,double max_distance);

    std::ostream &printRules (std::ostream &os);

    std::ostream &printIds (std::ostream &os);
};




#endif /* SEQL_CLASSIFY_H_ */
