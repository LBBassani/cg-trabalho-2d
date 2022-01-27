#if !defined CHARACTER
#define CHARACTER

#include <algorithm>

#include "../Third-Party-Libs/glm/glm.hpp"

#include "HitboxMapping.hpp"
#include "MovingEntity.hpp"
#include "Shape.hpp"

struct CharacterConfigurations{
    float cabeca = 0.3;
    float tronco_height = 0.4;
    float tronco_width = 0.2;
    float braco_width = 0.2;
    float braco_height = 0.05;
    float meia_perna = 0.15;

    glm::vec3 cabeca_color = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 tronco_color = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 braco_color = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 perna_color = glm::vec3(1.0f, 0.0f, 0.0f);
};

struct Boundaries{
    MovingEntity limite_direito;
    MovingEntity limite_esquerdo;
    MovingEntity limite_superior;
    MovingEntity limite_inferior;
};

struct RotatingRect : public MovingEntity{
    virtual void act(int* keyStatus, GLdouble deltaTime){ };
};

struct Character : public MovingEntity{
    static constexpr float original_height = 30;
    float height;
    CharacterConfigurations config;
    Boundaries boundaries;

    Character(float height = Character::original_height, CharacterConfigurations config = CharacterConfigurations()){
        this->height = height;
        this->config = config;
        float width = height*config.tronco_width, boundary_size = 1;

        // Hitbox e boundaries do boneco
        setHitbox(new Rect(width, height), glm::vec2(0.0f, -height/2));

        boundaries.limite_direito.transform.position.x = width/2 + boundary_size/2;
        boundaries.limite_direito.setHitbox(new Rect(boundary_size, height), glm::vec2(0.0f, -height/2));
        this->addChild(&boundaries.limite_direito);

        boundaries.limite_esquerdo.transform.position.x = -width/2 - boundary_size/2;
        boundaries.limite_esquerdo.setHitbox(new Rect(boundary_size, height), glm::vec2(0.0f, -height/2));
        this->addChild(&boundaries.limite_esquerdo);

        boundaries.limite_inferior.transform.position.y = -boundary_size;
        boundaries.limite_inferior.setHitbox(new Rect(width, boundary_size), glm::vec2(0.0f, -height/2));
        this->addChild(&boundaries.limite_inferior);

        boundaries.limite_superior.transform.position.y = height;
        boundaries.limite_superior.setHitbox(new Rect(width, boundary_size), glm::vec2(0.0f, -height/2));
        this->addChild(&boundaries.limite_superior);


        // Monta o boneco

        // Monta o corpo do boneco
        Entity* tronco = new Entity();
        Rect* tronco_shape = new Rect(width, height*config.tronco_height);
        
        tronco->setNome("tronco");
        tronco->setShape(tronco_shape);
        tronco->setColor(config.tronco_color);
        tronco->transform.position.y = -height*config.tronco_height/2;

        this->addChild(tronco);
        
        // Monta a cabeça do boneco
        Entity* cabeca = new Entity();
        Circ* cabeca_shape = new Circ(height*config.cabeca/2);
        cabeca->setNome("cabeça");
        cabeca->setShape(cabeca_shape);
        cabeca->setColor(config.cabeca_color);
        cabeca->transform.position.y = height*config.cabeca/2 + height*config.tronco_height/2;

        this->addChild(cabeca);

        // Monta o braço do boneco
        Entity* braco = new RotatingRect();
        Rect* braco_shape = new Rect(height*config.braco_width, height*config.braco_height);
        braco->setNome("braço");
        braco->setShape(braco_shape);
        braco->setColor(config.braco_color);
        braco->transform.position.x = height*config.braco_width/2;

        this->addChild(braco);

        // Monta as pernas
        Rect* perna_shape = new Rect(height*config.braco_height, height*config.meia_perna);

        // Monta a perna direita
        Entity* coxa_dir = new RotatingRect();
        coxa_dir->setNome("coxa direita");
        coxa_dir->setShape(perna_shape);
        coxa_dir->setColor(config.perna_color);
        coxa_dir->transform.position.y = - height*config.tronco_height/2;
        coxa_dir->transform.eulerRotation.z = 180.0f;

        Entity* canela_dir = new RotatingRect();
        canela_dir->setNome("canela direita");
        canela_dir->setShape(perna_shape);
        canela_dir->setColor(config.perna_color);
        canela_dir->transform.position.y = height*config.meia_perna;
        
        coxa_dir->addChild(canela_dir);

        this->addChild(coxa_dir);

        // Monta a perna esquerda
        Entity* coxa_esq = new RotatingRect();
        coxa_esq->setNome("coxa esquerda");
        coxa_esq->setShape(perna_shape);
        coxa_esq->setColor(config.perna_color);
        coxa_esq->transform.position.y = - height*config.tronco_height/2;
        coxa_esq->transform.eulerRotation.z = 180.0f;

        Entity* canela_esq = new RotatingRect();
        canela_esq->setNome("canela esquerda");
        canela_esq->setShape(perna_shape);
        canela_esq->setColor(config.perna_color);
        canela_esq->transform.position.y = height*config.meia_perna;

        coxa_esq->addChild(canela_esq);

        this->addChild(coxa_esq);
        
    }

    virtual void do_collision(std::list<HitboxMapping> colliding_hitboxes){
        #if defined TEST
            std::cout << "Entrou em do collision" << std::endl;
        #endif
        bool collision[] = {false, false, false, false};

        HitboxMapping boundaries_hitboxes[] = {
            HitboxMapping(&boundaries.limite_direito),
            HitboxMapping(&boundaries.limite_esquerdo),
            HitboxMapping(&boundaries.limite_superior),
            HitboxMapping(&boundaries.limite_inferior)
        };

        for(auto boundary : boundaries_hitboxes){
            boundary.update_hitbox();
        }
        
        for(auto colliding_hitbox : colliding_hitboxes){

            float collisions_area[] = {
                boundaries_hitboxes[0].is_colliding(colliding_hitbox) ? boundaries_hitboxes[0].percentage_of_overlapping(colliding_hitbox) : 0,
                boundaries_hitboxes[1].is_colliding(colliding_hitbox) ? boundaries_hitboxes[1].percentage_of_overlapping(colliding_hitbox) : 0,
                boundaries_hitboxes[2].is_colliding(colliding_hitbox) ? boundaries_hitboxes[2].percentage_of_overlapping(colliding_hitbox) : 0,
                boundaries_hitboxes[3].is_colliding(colliding_hitbox) ? boundaries_hitboxes[3].percentage_of_overlapping(colliding_hitbox) : 0
            };

            const int N = sizeof(collisions_area) / sizeof(float);

            #if defined TEST
                for(auto valor : collisions_area){
                    std::cout << valor << " ";
                }
                std::cout << std::endl;
            #endif

            if(collisions_area[0] || collisions_area[1] || collisions_area[2] || collisions_area[3]) collision[(std::distance(collisions_area, std::max_element(collisions_area, collisions_area + N)))] = true;
           
        }
        
        if (collision[0]) {
            this->moveLiberty.direita = false;
            #if defined TEST
                std::cout << "Não pode mover pra direita" << std::endl;
            #endif
        } else this->moveLiberty.direita = true;

        if (collision[1]) {
            this->moveLiberty.esquerda = false;
            #if defined TEST
                std::cout << "Não pode mover pra esquerda" << std::endl;
            #endif
        } else this->moveLiberty.esquerda = true;
        
        if (collision[2]) {
            this->moveLiberty.para_cima = false;
            #if defined TEST
                std::cout << "Não pode mover pra cima" << std::endl;
            #endif
        } else this->moveLiberty.para_cima = true;

        if (collision[3]) {
            this->moveLiberty.para_baixo = false;
            #if defined TEST
                std::cout << "Não pode mover pra baixo" << std::endl;
            #endif
        } else this->moveLiberty.para_baixo = true;
    }
};

struct Player : public Character{

    Player(float height = Character::original_height) : Character(height) {
        this->is_player = true;
        this->velocity = 0.02f;
        this->x_moveConfigurations.max = 1000.0f;
        this->y_moveConfigurations.max = 500.0f;
        
        this->x_moveConfigurations.min = -1000.0f;
        this->y_moveConfigurations.min = 500.0f;

     };

    virtual void act(int* keyStatus, GLdouble deltaTime){
        if (keyStatus[(int)('a')]){
            this->x_moveConfigurations.velocity = -velocity;
        } else if (keyStatus[(int)('d')]){
            this->x_moveConfigurations.velocity = velocity;
        } else {
            this->x_moveConfigurations.velocity = 0.0f;
        }

        if (keyStatus[(int)('w')]){
            this->y_moveConfigurations.velocity = velocity;
        #if defined TEST
        } else if (keyStatus[(int)('s')]){
            this->y_moveConfigurations.velocity = -velocity;
        }else{
            this->y_moveConfigurations.velocity = 0.0f;
        }
        #else
        } else {
            this->y_moveConfigurations.velocity = -velocity;
        }
        #endif

        this->move(deltaTime);
    }

};

struct Enemy : public Character{
    Enemy(float height = Character::original_height) : Character(height){};

    virtual void act(int* keyStatus, GLdouble deltaTime){ };

};


#endif