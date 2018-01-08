## Description

This page describes the usage of the SAX-SEQL software and supports our publication:

Time Series Classification by Sequence Learning in All-Subsequence Space, ICDE 2017 IEEE International Conference on Data Engineering, San Diego, Thach Le Nguyen, [Severin Gsponer](http://svgsponer.github.io), [Georgiana Ifrim](https://github.com/heerme) (Insight Centre for Data Analytics - University College Dublin)

The original SEQL software can be found here: https://github.com/heerme/seql-sequence-learner

## Installation

To compile execute following commands in the src directory:

```
mkdir -p build
cd build
mkdir -p Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../../
make
```


## How to Use

1. Convert time series data to SAX
  Usage:
 ```
 ./sax_convert [-n token_type] [-s reduction_strategy] [-N window_size] [-w word_length] [-a alphabet_size] [-i train_input] [-o train_output] [-I test_input] [-O test_output]
 ```

2. Train using ./seql_learn
  Usage:
```
./seql_learn    [-o objective_function] [-A alphabet_size] [-m minsup] [-l minpat] [-L maxpat] [-g maxgap] [-r traversal_strategy ]
                [-T #round] [-n token_type] [-c convergence_threshold] [-C regularizer_value] [-a l1_vs_l2_regularizer_weight]
                [-v verbosity] train_file model_file

Default values for parameters:
    [-o objective: 0 or 2] Objective function. Choice between logistic regression (-o 0) and squared-hinge support vector ma-
     chines (-o 2). By default set to logistic regression.
	[-A alphabet_size] Should be consistent with the input for sax_convert.
    [-g maxgap >= 0] Maximum number of consecutive gaps or wildcards allowed in a feature, e.g., a**b,
     is a feature of size 4 with any 2 characters from the input alphabet in the middle. By default
     set to 0.
    [-C regularizer value > 0] Value of the regularization parameter. By default set to 1.
    [-a alpha in [0,1]] Weight of l1 vs l2 regularizer for the elastic-net penalty. By default set to 0.2, i.e., 0.8*l1 + 0.2*l2 regularization.
    [-l minpat >= 1] Threshold on the minimum length of any feature. By default set to 1.
    [-L maxpat] Threshold on the maximum length of any feature. By default the maximum length
     is unrestricted, i.e., at most as long as the longest sequence in the training set.
    [-m minsup >= 1] Threshold on the minimum support of features, i.e., number of sequences containing
     a given feature. By default set to 1.
    [-n token type: 0 or 1] Word or character-level token. Words are delimited by white spaces. By default
     set to 1, character-level tokens.
    [-r traversal strategy: 0 or 1] Breadth First Search or Depth First Search traversal of the search tree.
     By default set to BFS.
    [-c convergence threshold >= 0] Stopping threshold based on change in aggregated score predictions.
     By default set to 0.005.
    [-T maxitr] Number of optimization iterations. By default set to the maximum between 5,000
     and the number of iterations resulting by using a convergence threshold on the aggregated
     change in score predictions.
    [-v verbosity: 1 to 5] Amount of printed detail about the training of the classifier. By default set to 1
     (light profiling information).
```
      

3. Prepare the final model using ./seql_mkmodel (this builds a trie on the features of the model for fast classification).
    Usage:
```
./seql_mkmodel [-i model_file] [-o binary_model_file] [-O predictors_file]
```
   

4. Classify using ./seql_classify (apply the learned model on new examples).
    Usage:
```
./seql_classify [-n token_type: 0 word tokens, 1 char tokens; by default set to 1] [-t classif_threshold: default 0] [-v verbosity level: default 0] test_file binary_model_file
```

5. For multiclass data:
```
./seql_multiclass [train data] [test data] [output directory] [window size] [word length] [alphabet size] 
```

## Example

For binary data:
```
./sax_convert -n 0 -s 1 -N 60 -w 16 -a 4  -i data/Coffee_TRAIN -o sax.train -I data/Coffee_TEST -O sax.test
./seql_learn -n 1 -v 1 -A 4 -d 1 sax.train seql.model
./seql_mkmodel -i seql.model -o seql.model.bin -O seql.predictor
./seql_classify -n 1 -v 0 -p seql.predictor -d 1 sax.test seql.model.bin
```

Optionally one can tune the classification threshold on the training set, to minimize the number of training errors:
```
 ./seql_classify_tune_threshold_min_errors -n 1 -v 2 data/sax.train seql.model.bin

    Best threshold:0.0746284
```

and use the best theshold for classifying the test set:
```
./seql_classify -n 1 -v 0 -t 0.0746284 -p seql.predictor -d 1 sax.test seql.model
```

Or we can do all the above steps in one line of command:
```
./sax_seql -t data/Coffee_TRAIN -T data/Coffee_TEST -d [directory for output] -n 60 -w 16 -a 4
```








