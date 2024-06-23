#include <iostream>
#include <vector>

#include "../include/astar.h"


    // Define an alias for a 2D vector of integers
using IntMatrix = std::vector<std::vector<int> >; 

const int ROWS = 10;
const int COLS = 10;
const int BOARD_SIZE = ROWS*COLS;

// enum CellType { EMPTY, START, END, OBSTACLE};
enum CellCost { EMPTY_C = 10, START_C = 1, END_C = 0, OBSTACLE_C = 1000000};

bool explore_surrounding_nodes(std::vector<Node> &nodes,Node &currentNode,Node &goalNode,IntMatrix &board,uint64_t* count);

void printBoard(const IntMatrix& board,Node &currentNode) {
    char symbol = ' ';
    Node* tmpNode = nullptr;
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            switch (board[x][y]) {
                case EMPTY_C:
                    symbol = ' ';
                    break;
                case OBSTACLE_C:
                    symbol = '+';
                    break;
                case START_C:
                    symbol = 'S';
                    break;
                case END_C:
                    symbol = 'E';
                    break;
                default:
                    symbol = '!';
                    break;

            }
            tmpNode = &currentNode;
            // std::cout << tmpNode->id << ' ';
            for (int i = 0; i < currentNode.id;i++){
                if (symbol == 'S' || symbol == 'E') break;
                if (tmpNode == nullptr){
                    break;
                }
                
                if (tmpNode->x_index == x && tmpNode->y_index == y){
                    symbol ='P';
                    break;
                } else {
                    if (tmpNode->parent == nullptr) break;
                    // std::cout << tmpNode->id << ' ' << std::endl;
                    tmpNode = tmpNode->parent;
                    if (tmpNode->id == 0) break;
                }
            }
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}

void printBoard(const IntMatrix& board) {
    char symbol = ' ';
    for (int x = 0; x < COLS; x++) {
        for (int y = 0; y < ROWS; y++) {
            switch (board[x][y]) {
                case EMPTY_C:
                    symbol = ' ';
                    break;
                case OBSTACLE_C:
                    symbol = '+';
                    break;
                case START_C:
                    symbol = 'S';
                    break;
                case END_C:
                    symbol = 'E';
                    break;
                default:
                    symbol = '!';
                    break;

            }
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}

int main() {
    // Initialize the board with empty spaces
    // std::vector<std::vector<char>> board(ROWS, std::vector<char>(COLS, EMPTY));
    std::vector<Node> nodes;
    nodes.reserve((ROWS*COLS)); // Reserve space for 100 nodes to avoid reallocation

    uint64_t node_id = 0;
    
    IntMatrix board(ROWS, std::vector<int>(COLS, EMPTY_C));
    int start_x_index = 0, start_y_index = 0;
    int end_x_index = COLS-1, end_y_index = ROWS-1;
    int count = 0;
    // Set start point, end point, and obstacles
    board[start_x_index][start_y_index] = START_C; // Start point at top-left corner
    board[end_x_index][end_y_index] = END_C; // End point at bottom-right corner

    // Add some obstacles
    board[1][1] = OBSTACLE_C;
    board[2][1] = OBSTACLE_C;
    board[1][0] = OBSTACLE_C;
    board[3][1] = OBSTACLE_C;
    board[3][2] = OBSTACLE_C;
    board[3][3] = OBSTACLE_C;
    board[3][4] = OBSTACLE_C;
    board[3][5] = OBSTACLE_C;
    board[3][6] = OBSTACLE_C;

    board[end_x_index-2][end_y_index] = OBSTACLE_C;
    board[end_x_index-2][end_y_index-1] = OBSTACLE_C;
    board[end_x_index-2][end_y_index-2] = OBSTACLE_C;
    board[end_x_index-2][end_y_index-3] = OBSTACLE_C;
    board[end_x_index-2][end_y_index-4] = OBSTACLE_C;
    board[end_x_index-1][end_y_index-4] = OBSTACLE_C;
    board[end_x_index-1][end_y_index-5] = OBSTACLE_C;
    board[end_x_index-1][end_y_index-6] = OBSTACLE_C;

    // Print the board
    printBoard(board);

    // Search board starting at Start and find the best path to End.
    Node endNode(-1,end_x_index,end_y_index);
    Node startNode(node_id,endNode,start_x_index,start_y_index,0);
    int currentNodeIndex = 0;
    nodes.push_back(startNode);
    node_id ++;
    double fValue = 100000000;


    while (count < BOARD_SIZE){

        if (explore_surrounding_nodes(nodes,nodes[currentNodeIndex],endNode,board,&node_id)){
            std::cout << "Goal Found" << std::endl;
            nodes[currentNodeIndex].checked = true;
                // print final path
            printBoard(board,nodes.back());
            break;
        } else {
            fValue = 1000000;
            nodes[currentNodeIndex].checked = true;
                // search for the next lowest cost cell to investigate
            for (int i = 0; i < nodes.size(); i++){
                if ((nodes[i].get_tCost() < fValue) && (nodes[i].checked == false)){
                    fValue = nodes[i].get_tCost();
                    currentNodeIndex = i;
                }
            }
            count++;
        }
    }
    
    return 0;
}

bool explore_surrounding_nodes(std::vector<Node> &nodes,Node &currentNode,Node &goalNode,IntMatrix &board,uint64_t* count){
    Node tmpNode,selectedNode;
    Node* priorNode;
    int row = -1, col = -1,tmpCost = 1000000000;
    bool nodeFound = false;
        // Loop through all the surronding cells, and find the lowest cost cell
    for (int i = 0; i < 8;i++){
        switch (i) {
            case 0:
                row = currentNode.y_index + 1;
                col = currentNode.x_index;
                break;
            case 1:
                row = currentNode.y_index + 1;
                col = currentNode.x_index + 1;
                break;
            case 2:
                row = currentNode.y_index;
                col = currentNode.x_index + 1;
                break;
            case 3:
                row = currentNode.y_index - 1;
                col = currentNode.x_index + 1;
                break;
            case 4:
                row = currentNode.y_index - 1;
                col = currentNode.x_index - 1;
                break;
            case 5:
                row = currentNode.y_index + 1;
                col = currentNode.x_index - 1;
                break;
            case 6:
                row = currentNode.y_index;
                col = currentNode.x_index - 1;
                break;
            case 7:
                row = currentNode.y_index - 1;
                col = currentNode.x_index;
                break;
            default:
                std::cout << "MISTAKE in index" << std::endl;
                break;
        }

                // Make sure only check valid cells
        if (row >= 0 && col >= 0 && row < ROWS && col < COLS){
                // Check if this cell is already a node?
            for (Node node : nodes){
                if (node.x_index == col && node.y_index == row){
                    priorNode = &node;
                    nodeFound = true;
                }
            }
                // Save all cells with the current node as a parrent
            tmpNode.parent = &currentNode;
            tmpNode.stepCost = board[col][row];
            tmpNode.x_index = col;
            tmpNode.y_index = row;
            tmpNode.calculate_h_cost(goalNode);
            tmpNode.get_gCost();
            tmpNode.set_tCost();
                // This cell has already been found prior
            if (nodeFound){
                    // update the prior cell with new data
                if (priorNode->get_tCost() > tmpNode.get_tCost()){
                    // Check if this actually updates the node.
                    *priorNode = tmpNode;
                } 
                nodeFound = false;
            } else {
                tmpNode.id = *count;
                    // Increment the node count
                (*count)++;
                // Add new node
                nodes.push_back(tmpNode);

                // Check if the goal is found: If so add it and return true
                if (tmpNode.x_index == goalNode.x_index && tmpNode.y_index == goalNode.y_index){
                    return true;
                }
            }
        }
    }
    return false;
}