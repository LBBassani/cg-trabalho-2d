#if !defined HITBOX_MAPPING
#define HITBOX_MAPPING

#include "Entity.hpp"

struct HitboxMapping{
    Entity* entity_ptr;
    glm::vec2 canto_sup_direito;
    glm::vec2 canto_inf_esquerdo;

    HitboxMapping(Entity* entity = nullptr){
        entity_ptr = entity;
    }

    virtual bool is_colliding(HitboxMapping& another_hitbox){
        bool sem_colisao = 
            (another_hitbox.canto_sup_direito.x <= this->canto_inf_esquerdo.x) 
            || (another_hitbox.canto_inf_esquerdo.x >= this->canto_sup_direito.x) 
            || (another_hitbox.canto_sup_direito.y <= this->canto_inf_esquerdo.y)
            || (another_hitbox.canto_inf_esquerdo.y >= this->canto_sup_direito.y);

        return !sem_colisao;
    }

    virtual void update_hitbox(){
        if(!entity_ptr) return;

        Rect* hitbox = dynamic_cast<Rect*>(entity_ptr->hitbox);
        
        glm::vec4 canto_dir = entity_ptr->transform.getLocalModelMatrix()*glm::vec4((hitbox->width)/2 + entity_ptr->hitbox_offset.x, (hitbox->height) + entity_ptr->hitbox_offset.y, 0.0f, 1.0f);
        glm::vec4 canto_esq = entity_ptr->transform.getLocalModelMatrix()*glm::vec4(-(hitbox->width)/2 + entity_ptr->hitbox_offset.x, 0 + entity_ptr->hitbox_offset.y, 0.0f, 1.0f);

        this->canto_sup_direito = glm::vec2(canto_dir.x, canto_dir.y);
        this->canto_inf_esquerdo = glm::vec2(canto_esq.x, canto_esq.y);

    }

    virtual float percentage_of_overlapping(HitboxMapping& another_hitbox){
        float overlapping_area = (
            std::max(this->canto_inf_esquerdo.x, another_hitbox.canto_inf_esquerdo.x) 
            - std::min(this->canto_sup_direito.x, another_hitbox.canto_sup_direito.x)
        )*(
            std::max(this->canto_sup_direito.y, another_hitbox.canto_sup_direito.y)
            - std::min(this->canto_inf_esquerdo.y, another_hitbox.canto_inf_esquerdo.y)
        ),
                self_area = (this->canto_inf_esquerdo.x - this->canto_sup_direito.x)
                *(this->canto_sup_direito.y - this->canto_inf_esquerdo.y);

        return std::min(abs(overlapping_area/self_area),100.0f);

    }

    virtual void draw(){
        glLoadIdentity();
        glBegin (GL_POINTS);
        glVertex2f(this->canto_inf_esquerdo.x, this->canto_inf_esquerdo.y);
        glVertex2f(this->canto_sup_direito.x, this->canto_sup_direito.y);
        glEnd();
    }

};

#endif