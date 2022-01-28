#if !defined SCENE_TREE
#define SCENE_TREE

#include <list>
#include "Entity.hpp"
#include "HitboxMapping.hpp"

struct SceneTree{
    Entity* root;
    std::list<HitboxMapping> static_hitbox_mapping;

    void updateHitboxMapping(){
        bool moving_cond = false; // apenas entidades que não se movem
        static_hitbox_mapping = hitboxMappingList(moving_cond);
    }
    
    std::list<HitboxMapping> hitboxMappingList(bool moving_cond){
        std::list<Entity*> entities;
        root->flattenTree(&entities);
        std::list<HitboxMapping> resp;

        for(auto entity : entities){
            if(entity->is_movable == moving_cond && entity->hitbox){
                HitboxMapping hm;
                
                if (!entity->hitbox) continue;
                hm.entity_ptr = entity;
                hm.update_hitbox();
                resp.push_back(hm);
            }
        }

        return resp;
    }

    void draw(){ 
        updateSceneTree();
        root->draw();
        #if defined TEST
        
        draw_hitboxes();
        
        #endif
    };

    void draw_hitboxes(){
        // calcula as liberdades de movimento dos objetos em cena
        bool moving_cond = true; // apenas entidades que se movem
        std::list<HitboxMapping> dynamic_hitbox_mapping = hitboxMappingList(moving_cond);

        glColor3f(1.0f, 0.0f, 0.5f);
        glPointSize(2);
        glLoadIdentity();
        glBegin (GL_POINTS);
        for(auto dynamic_hitbox : dynamic_hitbox_mapping){
            glVertex2f(dynamic_hitbox.canto_inf_esquerdo.x, dynamic_hitbox.canto_inf_esquerdo.y);
            glVertex2f(dynamic_hitbox.canto_sup_direito.x, dynamic_hitbox.canto_sup_direito.y);
        }
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin (GL_POINTS);
        for(auto static_hitbox : static_hitbox_mapping){
            glVertex2f(static_hitbox.canto_inf_esquerdo.x, static_hitbox.canto_inf_esquerdo.y);
            glVertex2f(static_hitbox.canto_sup_direito.x, static_hitbox.canto_sup_direito.y);
        }
        glEnd();
    }

    void idle(int* keyStatus, GLdouble deltaTime){
        root->idle(keyStatus, deltaTime);
    }

    void print(){
        root->print();
    }

    void updateSceneTree(){
        root->updateSelfAndChildren();

        // calcula as liberdades de movimento dos objetos em cena
        bool moving_cond = true; // apenas entidades que se movem
        std::list<HitboxMapping> dynamic_hitbox_mapping = hitboxMappingList(moving_cond);

        this->updateHitboxMapping();

        for(auto dynamic_hitbox : dynamic_hitbox_mapping){
            if (!dynamic_hitbox.entity_ptr->is_player) continue;

            std::list<HitboxMapping> collisions;
            
            bool colidiu = false;
            for(auto static_hitbox : static_hitbox_mapping){
                #if defined TEST
                    //dynamic_hitbox.is_colliding(static_hitbox) ? std::cout << "Colisão entre " << dynamic_hitbox.entity_ptr->getNome() << " e " << static_hitbox.entity_ptr->getNome() << std::endl : std::cout << "Sem colisão entre " << dynamic_hitbox.entity_ptr->getNome() << " e " << static_hitbox.entity_ptr->getNome() << std::endl;
                #endif
                if(dynamic_hitbox.is_colliding(static_hitbox)){ 
                    collisions.push_back(static_hitbox);
                    colidiu = true;
                }
            }

            for(auto another_dynamic_hitbox : dynamic_hitbox_mapping){
                if(dynamic_hitbox.entity_ptr == another_dynamic_hitbox.entity_ptr) continue;
                if(dynamic_hitbox.is_colliding(another_dynamic_hitbox)){
                    collisions.push_back(another_dynamic_hitbox);
                    colidiu = true;
                }
            }

            if (colidiu) dynamic_hitbox.entity_ptr->do_collision(collisions);
            else {
                dynamic_hitbox.entity_ptr->moveLiberty = MoveLiberty();
                #if defined TEST
                    //std::cout << "Pode mover pra onde quiser!!" << std::endl;
                #endif    
            }
            
        }

    }
};


#endif