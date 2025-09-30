# Text Autocomplete Project (Trie + Bigram Model)

## Overview
This project implements a **text autocomplete system** using:

- **Trie (Prefix Tree)** for fast prefix search (DSA component).
- **Bigram Language Model** for context-aware ranking of suggestions (AI/NLP component).

Users can type a sentence or prefix in the **command-line interface (CLI)**, and the system suggests relevant word completions in real-time.

## Features
- Efficient **prefix search** using Trie.
- **Context-based ranking** using a probabilistic bigram model.
- Interactive **CLI demo** for testing suggestions.
- Easily **extendable** for larger dictionaries or more advanced language models (trigram, neural networks).

## Files
| File             | Description                                                   |
|------------------|---------------------------------------------------------------|
| `autocomplete.cpp` | Main C++ program implementing the Trie and bigram model.     |
| `words.txt`         | Dictionary of words to populate the Trie (one word per line).|
| `corpus.txt`        | Text corpus for training the bigram model.                   |
| `README.md`         | Project description and instructions.                       |

## Setup & Usage

1. **Clone the repository:**
   ```bash
   git clone https://github.com/ashu0420/TextAutocompleteTrie.git
   cd TextAutocompleteTrie

    Compile the C++ program:

g++ -std=c++17 -O2 autocomplete.cpp -o autocomplete

Run the program:

./autocomplete

Type a sentence or prefix:
Example input:

    > i want to ea
    Suggestions (word : score):
      eat : 0.600000
      ease : 0.200000
      eatery : 0.050000

        The last word typed is treated as the prefix.

        Suggestions are ranked based on bigram probability from corpus.txt.

    Exit: Press Enter on an empty line.

Example Usage

> please pl
Suggestions (word : score):
  play : 0.500000
  please : 0.300000

> we want to 
Suggestions (word : score):
  eat : 0.400000
  play : 0.350000
  plant : 0.150000

Technologies & Skills

    Language: C++17

    Data Structures: Trie, DFS/BFS, Priority Queue

    Algorithms: Prefix search, Ranking

    NLP/Statistics: Bigram language model, Add-one smoothing

    Interface: Command-Line Interface (CLI)

Future Enhancements

    Support trigram or neural network language models for better context.

    Add GUI or web interface for live autocomplete.

    Optimize Trie nodes to store top-k suggestions for faster retrieval.

    Extend support for larger dictionaries and corpora.

Author

Asheesh | GitHub: https://github.com/ashu0420


---

This updated `README.md` now includes your GitHub profile link, making it easy for others to view your other projects and contributions. If you need further assistance with your GitHub repository or any other aspect of your project, feel free to ask!
::contentReference[oaicite:0]{index=0}
 