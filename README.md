# pattern_matching

## Description

String Pattern Matching Algorithm implementation. The algorithm is designed to efficiently search for multiple patterns (words) in a given text. It utilizes a tree-based DFA (Deterministic Finite Automaton) to determine matches simultaneously, enabling the detection of more than one match at a time. The algorithm has a linear time complexity, making it suitable for processing large texts efficiently.

## functions and fils 

The repository consists of two main files:

### `slist.c`

This file contains the implementation of a double-linked list class, which is utilized by the pattern-matching algorithm.

**Structs:**
- `dbllist_node`: Represents a node in the double-linked list.
- `dbllist`: Represents the list itself.

**Functions:**
- `dbllist_init`: Initializes the list.
- `dbllist_destroy`: Frees the list and its associated memory.
- `dbllist_append`: Adds a new node to the end of the list.
- `dbllist_prepend`: Adds a new node to the start of the list.
- `dbllist_remove`: Removes a specified node from the list.

### `pattern-matching.c`

This file contains the implementation of the String Pattern Matching Algorithm.

**Structs:**
- `pm_labeled_edge`: Represents an arrow in the pattern-matching algorithm, connecting nodes.
- `pm_state`: Represents a node in the pattern-matching algorithm.
- `pm_match`: Represents a match found in the text.
- `pm`: Represents the tree structure used by the algorithm.

**Functions:**
- `pm_init`: Initializes the pattern-matching tree.
- `pm_addstring`: Adds a pattern (word) to the tree.
- `pm_makeFSM`: Constructs the fail paths in the pattern-matching tree.
- `pm_goto_set`: Creates links between nodes (arrows) based on specific labels.
- `pm_goto_get`: Retrieves a link with a given label.
- `pm_fsm_search`: Searches for matches in the text using the pattern-matching tree.
- `pm_destroy`: Frees the memory allocated for the nodes in the pattern-matching tree.

## Usage

To use this algorithm, follow these steps:

1. Include the necessary files (`slist.c` and `pattern-matching.c`) in your project.
2. Initialize the pattern-matching tree using `pm_init`.
3. Add patterns (words) to the tree using `pm_addstring` for each pattern.
4. Construct the fail paths in the tree using `pm_makeFSM`.
5. Perform the pattern matching on a given text using `pm_fsm_search`.
6. Retrieve the matches found in the text from the resulting double-linked list (`dbllist`) created by the algorithm.

## Input

The input to the algorithm consists of patterns (words) that need to be searched for in a text. These patterns are added to the pattern-matching tree using the `pm_addstring` function. The text to be searched is provided as input to the `pm_fsm_search` function.

## Output

The output of the algorithm is a double-linked list (`dbllist`) containing instances of `pm_match`. Each `pm_match` instance represents a match found in the text. You can iterate through the list to access and process the matches as needed.
