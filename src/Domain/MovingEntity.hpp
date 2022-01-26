#if !defined MOVING_ENTITY
#define MOVING_ENTITY

#include "GL/gl.h"

#include "SceneTree.hpp"

struct MoveConfigurations{
    float max;
    float min;
    float velocity;

    MoveConfigurations(float max = 0.0f, float min = 0.0f, float velocity = 0.0f){
        this->max = max;
        this->min = min;
        this->velocity = velocity;
    }
};

struct MovingEntity : public Entity{


    MoveConfigurations x_moveConfigurations;
    MoveConfigurations y_moveConfigurations;
    MoveConfigurations angular_moveConfigurations;    

    MovingEntity(MoveConfigurations x_config = MoveConfigurations(), MoveConfigurations y_config = MoveConfigurations(), MoveConfigurations ang_config = MoveConfigurations()){
        this->is_movable = true;

        this->x_moveConfigurations = x_config;
        this->y_moveConfigurations = y_config;
        this->angular_moveConfigurations = ang_config;
    }

    /*
     * @brief Função que deve decidir a ação que a entidade irá tomar com base na tecla
     * @param key : tecla que foi acionada
     */
    virtual void act(int* keyStatus, GLdouble deltaTime) = 0;

    /*
     * @brief Função genérica de movimento de translação
     */
    virtual void move(GLdouble deltaTime){

        // Calcula novas posições
        float   new_x_position = this->transform.position.x + x_moveConfigurations.velocity * deltaTime,
                new_y_position = this->transform.position.y + y_moveConfigurations.velocity * deltaTime;
        
        // Atualiza a posição em x se estiver nos limites
        if (new_x_position > x_moveConfigurations.min 
            && new_x_position < x_moveConfigurations.max) 
                this->transform.position.x = new_x_position;

        // Atualiza a posição em y se estiver nos limites
        if (new_y_position > y_moveConfigurations.min 
            && new_y_position < y_moveConfigurations.max) 
                this->transform.position.y = new_y_position;
        
    }

    /*
     * @brief Função genérica de movimento de rotação
     */
    virtual void rotate(GLdouble deltaTime){

        // Calcula nova rotação
        float new_angular_rotation = this->transform.eulerRotation.z + angular_moveConfigurations.velocity * deltaTime;

        // Atualiza a rotação se estiver nos limites
        if (new_angular_rotation > angular_moveConfigurations.min
            && new_angular_rotation < angular_moveConfigurations.max)
                this->transform.eulerRotation.z = new_angular_rotation;

    }

};

#endif