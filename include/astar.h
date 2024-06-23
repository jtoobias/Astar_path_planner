#include <cstdint>
#include <cmath>
class Node
{
    private:
        int tCost;

    public:
        
        int64_t id;
        Node* parent;
        uint32_t stepCost;
        uint32_t gCost;
        uint32_t hCost;
        uint32_t x_index;
        uint32_t y_index;
        bool checked;


        Node(){
            this->checked = false;
            this->parent = nullptr;
        }

        // For End node on init
        Node(int64_t id, uint32_t x_idx,uint32_t y_idx) : 
            id(id),x_index(x_idx),y_index(y_idx)
            {
            this->parent = nullptr;
            this->checked = false;
        }
        // For start node on init
        Node(int64_t id,Node &goalNode, uint32_t x_idx,uint32_t y_idx,uint32_t gCost) : 
            id(id),x_index(x_idx),y_index(y_idx), gCost(gCost)
            {
            this->parent = nullptr;
            this->checked = true;
            this->stepCost = 0;
            this->hCost = this->calculate_h_cost(goalNode);
            this->set_tCost();
        }

        void set_tCost(){
            this->tCost = gCost + hCost;
        }
        uint32_t get_tCost(){
            return this->tCost;
        }

        uint32_t get_gCost(){
            if (this->parent->id == 0){
                this->gCost = this->stepCost;
            } else {
                this->gCost = this->parent->gCost + this->stepCost;
            }
            return this->gCost;
        }
        // double calculate_h_cost(Node &end_node);


        double calculate_h_cost(Node &end_node){
            double diff_x = end_node.x_index-this->x_index;
            double diff_y = end_node.y_index-this->y_index;
            this->hCost = hypot(diff_x,diff_y);
            return this->hCost;
        }
};