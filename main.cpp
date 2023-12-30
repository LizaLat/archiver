#include <bits/stdc++.h>

using namespace std;

#define EMPTY_STRING ""

struct Forest {
    int weight;
    int root;
};
Forest forest[256];

struct Tree {
    int left, right, parent;
};
Tree tree[520];

FILE *f1;
FILE *f2;

void getMin(Forest f[], int sise, int &p1, int &p2) {
    p1 = 0;
    p2 = 1;
    if (f[p1].weight > f[p2].weight) {
        p1 = 1;
        p2 = 0;
    }
    for (int i = 0; i < sise; i++) {
        if (f[i].weight < f[p1].weight) {
            p2 = p1;
            p1 = i;
        } else if (f[i].weight > f[p1].weight && f[i].weight < f[p2].weight) {
            p2 = i;
        }
    }
}



struct Node
{
    char ch;
    int freq;
    Node *left, *right;
};

Node* getNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

struct comp
{
    bool operator()(const Node* l, const Node* r) const
    {

        return l->freq > r->freq;
    }
};

bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
    if (root == nullptr) {
        return;
    }

    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void decode(Node* root, int &index, string str)
{
    if (root == nullptr) {
        return;
    }

    if (isLeaf(root))
    {
        cout << root->ch;
        return;
    }

    index++;

    if (str[index] == '0') {
        decode(root->left, index, str);
    }
    else {
        decode(root->right, index, str);
    }
}

void buildHuffmanTree(string text)
{
    if (text == EMPTY_STRING) {
        return;
    }

    unordered_map<char, int> freq;
    for (char ch: text) {
        freq[ch]++;
    }

    priority_queue<Node*, vector<Node*>, comp> pq;

    for (auto pair: freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    while (pq.size() != 1)
    {

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top();    pq.pop();

        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    Node* root = pq.top();

    unordered_map<char, string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);

    cout << "HuffmanCodes:\n" << endl;
    for (auto pair: huffmanCode) {
        cout << pair.first << " " << pair.second << endl;
    }

    cout << "input.txt" << text << endl;

    string str;
    for (char ch: text) {
        str += huffmanCode[ch];
    }

    cout << "output.txt" << str << endl;

    if (isLeaf(root))
    {
        while (root->freq--) {
            cout << root->ch;
        }
    }
    else {
        int index = -1;
        while (index < (int)str.size() - 1) {
            decode(root, index, str);
        }
    }
}

int main()
{
    f1 = fopen("input.txt", "rb");
    //f2 = fopen("output.txt", "wb");
    freopen("output.txt", "w", stdout);
    int freq[256];
    int size_forest = 0;
    int size_tree = 0;
    for (int i = 0; i < 256; i++) {
        freq[i] = 0;
    }
    char ch;
    while (fscanf(f1, "%c", &ch) != -1) {
        freq[ch]++;
    }
    for (int i = 0; i < 256; i++) {
        if (freq[i]) {
            forest[size_forest].weight = freq[i];
            forest[size_forest].root = size_forest;
            size_forest++;
            tree[size_tree].parent = -1;
            tree[size_tree].right = -1;
            tree[size_tree].left = -1;
            size_tree++;
        }
    }
    int pos1, pos2;
    while (size_forest > 1) {
        getMin(forest, size_forest, pos1, pos2);
        tree[size_tree].left = forest[pos1].root;
        tree[size_tree].right = forest[pos2].root;
        tree[size_tree].parent = -1;
        tree[forest[pos1].root].parent = size_tree;
        tree[forest[pos2].root].parent = size_tree;
        forest[pos1].weight = forest[pos1].weight + forest[pos2].weight;
        forest[pos1].root = size_tree;
        size_forest--;
        size_tree++;
    }
    for (int i = 0; i < size_tree; i++) {
        cout << tree[i].left << " " << tree[i].right << " " << tree[i].parent << endl;
    }


    string text;
    fscanf(f1,"%s", &text);
    buildHuffmanTree(text);

    return 0;
}
