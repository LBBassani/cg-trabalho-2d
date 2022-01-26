#if !defined CAMERA
#define CAMERA

#include "MovingEntity.hpp"
#include "Character.hpp"

#include "../Third-Party-Libs/glm/glm.hpp"

struct Camera : public MovingEntity{

    bool is_auto = true;
    Player* player_to_follow = nullptr;
    bool is_there_a_player_to_follow = false;
    int auto_key_last_value = 0;

    Camera(float velocity = 0.02f){
        this->velocity = velocity;
    }

    void setPlayer(Player* player){
        
        if(player){
            this->player_to_follow = player;
            this->is_there_a_player_to_follow = true;
        }
    }

    void removePlayer(){
        this->is_there_a_player_to_follow = false;
    }

    Player* getPlayer(){
        if(is_there_a_player_to_follow){
            return player_to_follow;
        }
        return nullptr;
    }

    virtual void setAuto(bool will_be_auto){
        this->is_auto = will_be_auto;
        if(this->is_auto){
            this->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        }
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){

        if (keyStatus[(int) ('c')] != auto_key_last_value){
            auto_key_last_value = keyStatus[(int) ('c')]; //debouncing
            if(keyStatus[(int) ('c')])
                setAuto(!is_auto);
                is_auto ? std::cout << "Camera configurada para automática" << std::endl : std::cout << "Camera configurada para manual" << std::endl;
        }

        if(is_auto){
            if(is_there_a_player_to_follow){
                float   new_x_position = player_to_follow->transform.position.x,
                        new_y_position = player_to_follow->transform.position.y;

                if(new_x_position > this->x_moveConfigurations.min && new_x_position < this->x_moveConfigurations.max)
                    this->transform.position.x = new_x_position;
                
                if(new_y_position > this->y_moveConfigurations.min && new_y_position < this->y_moveConfigurations.max)
                    this->transform.position.y = new_y_position;

            }
        }else{
            if (keyStatus[(int)('j')]){
                this->x_moveConfigurations.velocity = -velocity;
            } else if (keyStatus[(int)('l')]){
                this->x_moveConfigurations.velocity = velocity;
            } else {
                this->x_moveConfigurations.velocity = 0.0f;
            }

            if (keyStatus[(int)('i')]){
                this->y_moveConfigurations.velocity = velocity;
            } else if (keyStatus[(int)('k')]){
                this->y_moveConfigurations.velocity = -velocity;
            } else {
                this->y_moveConfigurations.velocity = 0.0f;
            }

            this->move(deltaTime);
        }
    }

};

#endif