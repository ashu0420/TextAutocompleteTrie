#include <bits/stdc++.h>
using namespace std;

// lowercases and removes non-alphabetic chars
string normalize(const string &s) {
    string out;
    for (char c : s) if (isalpha((unsigned char)c)) out.push_back(tolower(c));
    return out;
}

// split text into lowercase word tokens
vector<string> tokenize(const string &text) {
    vector<string> tokens;
    string cur;
    for (char c : text) {
        if (isalpha((unsigned char)c)) cur.push_back(tolower(c));
        else if (!cur.empty()) { tokens.push_back(cur); cur.clear(); }
    }
    if (!cur.empty()) tokens.push_back(cur);
    return tokens;
}

struct TrieNode {
    TrieNode* children[26]{};
    bool end = false;
};

class Trie {
    TrieNode* root = new TrieNode();

    void collect(TrieNode* node, string &prefix, vector<string> &out, size_t limit) {
        if (!node || out.size() >= limit) return;
        if (node->end) out.push_back(prefix);
        for (int i = 0; i < 26 && out.size() < limit; i++) {
            if (node->children[i]) {
                prefix.push_back('a' + i);
                collect(node->children[i], prefix, out, limit);
                prefix.pop_back();
            }
        }
    }

public:
    void insert(const string &word) {
        TrieNode* cur = root;
        for (char c : word) {
            int i = c - 'a';
            if (!cur->children[i]) cur->children[i] = new TrieNode();
            cur = cur->children[i];
        }
        cur->end = true;
    }

    vector<string> autocomplete(const string &prefix, size_t limit = 200) {
        TrieNode* cur = root;
        string clean;
        for (char c : prefix) {
            if (c < 'a' || c > 'z') continue;
            clean.push_back(c);
            int i = c - 'a';
            if (!cur->children[i]) return {};
            cur = cur->children[i];
        }
        vector<string> out;
        string pref = clean;
        collect(cur, pref, out, limit);
        return out;
    }
};

class BigramModel {
    unordered_map<string, int> unigram;
    unordered_map<string, unordered_map<string,int>> bigram;
    size_t vocab_size = 0;
    long long total = 0;

public:
    void train(const vector<string> &tokens) {
        if (tokens.empty()) return;
        total += tokens.size();
        for (size_t i = 0; i < tokens.size(); i++) {
            unigram[tokens[i]]++;
            if (i + 1 < tokens.size()) bigram[tokens[i]][tokens[i+1]]++;
        }
        vocab_size = unigram.size();
    }

    double score(const string &prev, const string &cand) const {
        auto itPrev = bigram.find(prev);
        if (itPrev != bigram.end()) {
            long long countPrev = unigram.at(prev);
            long long countPair = itPrev->second.count(cand) ? itPrev->second.at(cand) : 0;
            return (countPair + 1.0) / (countPrev + vocab_size);
        }
        long long countCand = unigram.count(cand) ? unigram.at(cand) : 0;
        if (total == 0) return 1.0 / max<size_t>(1, vocab_size);
        return (countCand + 1.0) / (total + vocab_size);
    }
};

class Autocomplete {
    Trie trie;
    BigramModel model;

public:
    void load_dict(const vector<string> &words) {
        for (auto &w : words) {
            string s = normalize(w);
            if (!s.empty()) trie.insert(s);
        }
    }

    void train(const string &text) {
        model.train(tokenize(text));
    }

    vector<pair<string,double>> suggest(const string &input, size_t k = 5) {
        vector<string> tokens = tokenize(input);
        string prefix;
        for (int i = input.size()-1; i >= 0 && isalpha((unsigned char)input[i]); i--)
            prefix.insert(prefix.begin(), tolower(input[i]));

        string prev = tokens.size() >= 2 ? tokens[tokens.size()-2] : "";
        vector<string> candidates = trie.autocomplete(prefix, 1000);
        if (candidates.empty()) return {};

        struct Item { double score; string word; bool operator<(const Item &o) const { return score < o.score; } };
        priority_queue<Item> pq;
        for (auto &c : candidates)
            pq.push({model.score(prev, c), c});

        vector<pair<string,double>> out;
        for (size_t i = 0; i < k && !pq.empty(); i++) {
            auto it = pq.top(); pq.pop();
            out.push_back({it.word, it.score});
        }
        return out;
    }
};

string read_file(const string &path) {
    ifstream in(path);
    if (!in) return "";
    stringstream ss; ss << in.rdbuf();
    return ss.str();
}

vector<string> read_words(const string &path) {
    vector<string> out; string line; ifstream in(path);
    while (getline(in, line)) { string s = normalize(line); if (!s.empty()) out.push_back(s); }
    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> words = read_words("words.txt");
    string corpus = read_file("corpus.txt");

    if (words.empty()) words = {"the","a","an","to","eat","play","please","plant","code","cat","dog"};
    if (corpus.empty()) corpus = "i want to eat food. please play music. we want to plant a tree.";

    Autocomplete engine;
    engine.load_dict(words);
    engine.train(corpus);

    cout << "Type a sentence or prefix (empty line to quit)\n";
    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line) || line.empty()) break;
        auto sugg = engine.suggest(line, 5);
        if (sugg.empty()) cout << "(no suggestions)\n";
        else for (auto &p : sugg) cout << "  " << p.first << " : " << fixed << setprecision(6) << p.second << "\n";
    }
}

