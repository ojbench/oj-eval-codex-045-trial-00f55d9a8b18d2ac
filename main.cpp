#include <cstdio>
#include <iostream>

using namespace std;

static const int MAXN = 500000 + 5;

struct Node {
    int key;
    unsigned int pri;
    int left;
    int right;
};

Node tree[MAXN];
int root = 0;
int nodeCount = 0;
int currentFloor = 0;
bool movingUp = true;

static unsigned int seedValue = 123456789u;

unsigned int nextPriority() {
    seedValue ^= seedValue << 13;
    seedValue ^= seedValue >> 17;
    seedValue ^= seedValue << 5;
    return seedValue;
}

int newNode(int key) {
    ++nodeCount;
    tree[nodeCount].key = key;
    tree[nodeCount].pri = nextPriority();
    tree[nodeCount].left = 0;
    tree[nodeCount].right = 0;
    return nodeCount;
}

void rotateLeft(int &nodeIndex) {
    int child = tree[nodeIndex].right;
    tree[nodeIndex].right = tree[child].left;
    tree[child].left = nodeIndex;
    nodeIndex = child;
}

void rotateRight(int &nodeIndex) {
    int child = tree[nodeIndex].left;
    tree[nodeIndex].left = tree[child].right;
    tree[child].right = nodeIndex;
    nodeIndex = child;
}

void insertNode(int &nodeIndex, int key) {
    if (nodeIndex == 0) {
        nodeIndex = newNode(key);
        return;
    }
    if (key < tree[nodeIndex].key) {
        insertNode(tree[nodeIndex].left, key);
        if (tree[tree[nodeIndex].left].pri < tree[nodeIndex].pri) {
            rotateRight(nodeIndex);
        }
    } else {
        insertNode(tree[nodeIndex].right, key);
        if (tree[tree[nodeIndex].right].pri < tree[nodeIndex].pri) {
            rotateLeft(nodeIndex);
        }
    }
}

void eraseNode(int &nodeIndex, int key) {
    if (nodeIndex == 0) {
        return;
    }
    if (key < tree[nodeIndex].key) {
        eraseNode(tree[nodeIndex].left, key);
    } else if (key > tree[nodeIndex].key) {
        eraseNode(tree[nodeIndex].right, key);
    } else {
        if (tree[nodeIndex].left == 0) {
            nodeIndex = tree[nodeIndex].right;
        } else if (tree[nodeIndex].right == 0) {
            nodeIndex = tree[nodeIndex].left;
        } else if (tree[tree[nodeIndex].left].pri < tree[tree[nodeIndex].right].pri) {
            rotateRight(nodeIndex);
            eraseNode(tree[nodeIndex].right, key);
        } else {
            rotateLeft(nodeIndex);
            eraseNode(tree[nodeIndex].left, key);
        }
    }
}

int findSuccessor(int nodeIndex, int key) {
    int answer = -1;
    while (nodeIndex != 0) {
        if (tree[nodeIndex].key > key) {
            answer = tree[nodeIndex].key;
            nodeIndex = tree[nodeIndex].left;
        } else {
            nodeIndex = tree[nodeIndex].right;
        }
    }
    return answer;
}

int findPredecessor(int nodeIndex, int key) {
    int answer = -1;
    while (nodeIndex != 0) {
        if (tree[nodeIndex].key < key) {
            answer = tree[nodeIndex].key;
            nodeIndex = tree[nodeIndex].right;
        } else {
            nodeIndex = tree[nodeIndex].left;
        }
    }
    return answer;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }

    char operation[16];
    for (int i = 0; i < n; ++i) {
        scanf("%s", operation);
        if (operation[0] == 'a' && operation[1] == 'd') {
            int x;
            scanf("%d", &x);
            insertNode(root, x);
        } else if (operation[0] == 'c') {
            int x;
            scanf("%d", &x);
            eraseNode(root, x);
        } else if (operation[0] == 'a') {
            if (root != 0) {
                int target = -1;
                if (movingUp) {
                    target = findSuccessor(root, currentFloor);
                    if (target != -1) {
                        currentFloor = target;
                    } else {
                        movingUp = false;
                        target = findPredecessor(root, currentFloor);
                        currentFloor = target;
                    }
                } else {
                    target = findPredecessor(root, currentFloor);
                    if (target != -1) {
                        currentFloor = target;
                    } else {
                        movingUp = true;
                        target = findSuccessor(root, currentFloor);
                        currentFloor = target;
                    }
                }
                eraseNode(root, target);
            }
        } else {
            printf("%d\n", currentFloor);
        }
    }

    return 0;
}
