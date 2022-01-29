#if !defined CHARACTER
#define CHARACTER

#include <algorithm>

#include "../Third-Party-Libs/glm/glm.hpp"
#include "../Third-Party-Libs/glm/gtx/vector_angle.hpp"

#include "HitboxMapping.hpp"
#include "MovingEntity.hpp"
#include "Shape.hpp"
#include "Shot.hpp"

#define MOUSE_LEFT_CLICK 256
#define MOUSE_RIGHT_CLICK 257
#define MOUSE_X_COORD 258
#define MOUSE_Y_COORD 259
#define PLAYER_X_COORD 260
#define PLAYER_Y_COORD 261

struct CharacterConfigurations{
    float cabeca = 0.3;
    float tronco_height = 0.4;
    float tronco_width = 0.2;
    float braco_width = 0.2;
    float braco_height = 0.05;
    float meia_perna = 0.15;

    glm::vec3 cabeca_color = glm::vec3(0.0f, 0.8f, 0.2f);
    glm::vec3 tronco_color = glm::vec3(0.0f, 0.8f, 0.2f);
    glm::vec3 braco_color = glm::vec3(1.0f, 1.0f, 0.2f);
    glm::vec3 perna_color = glm::vec3(1.0f, 0.0f, 0.2f);
};

struct Boundaries{
    Entity* limite_direito;
    glm::vec2 dist_direito;
    Entity* limite_esquerdo;
    glm::vec2 dist_esquerdo;
    Entity* limite_superior;
    glm::vec2 dist_superior;
    Entity* limite_inferior;
    glm::vec2 dist_inferior;
};

struct RotatingRect : public MovingEntity{
    virtual void act(int* keyStatus, GLdouble deltaTime){ };
};

struct Braco : public MovingEntity{
    
    glm::vec2 target_position = {0.0f, 0.0f};
    float height, width;
    float max_angle, min_angle;
    bool player_braco = false;

    Braco(){
        this->angular_moveConfigurations.velocity = 0.1f;
    }
    
    virtual void act(int* keyStatus, GLdouble deltaTime){
        
        if(this->player_braco) this->target_position = {keyStatus[MOUSE_X_COORD], keyStatus[MOUSE_Y_COORD]};
        else this->target_position = {keyStatus[PLAYER_X_COORD], keyStatus[PLAYER_Y_COORD]};

        this->move(deltaTime);
    }

    virtual void flip(bool direita){
        if(
            (this->transform.eulerRotation.z < 90.0f && direita)
            || (this->transform.eulerRotation.z > 90.0f && !direita)
        ) return;
        float angle_to_rotate = 2*(90 - this->transform.eulerRotation.z);
        this->transform.eulerRotation.z += angle_to_rotate;
    }

    virtual void move(GLdouble deltaTime){
        
        // Coloca o braço pra rotacionar até o mouse
        glm::vec4   braco_origem = {this->transform.position.x, this->transform.position.y + height/2, 0.0f, 1.0f},
                    target = {target_position.x, target_position.y, 0.0f, 1.0f};
        
        braco_origem = this->transform.modelMatrix*braco_origem;
        
        float   angle_1 = atan2(target.y - braco_origem.y, target.x - braco_origem.x)*180/glm::pi<float>(),
                angle_2 = this->transform.eulerRotation.z,
                target_angle = angle_1 - angle_2;
        int signal = target_angle >= 0 ? 1 : -1;
        float new_angle = this->transform.eulerRotation.z + signal*angular_moveConfigurations.velocity*deltaTime;

        #if defined TEST
            //std::cout << "Angulo calculado: " << angle_2 << ", Angulo real: " << this->transform.eulerRotation.z << std::endl;
            //std::cout << "Braço no angulo " << this->transform.eulerRotation.z << " precisa rodar " << std::min(this->max_angle, std::max(new_angle, min_angle)) << " pra tentar alcançar " << target_angle << std::endl; 
        #endif

        this->transform.eulerRotation.z = std::min(this->max_angle, std::max(new_angle, min_angle));

    }
};

struct Character : public MovingEntity{
    static constexpr float original_height = 30;
    float height;
    CharacterConfigurations config;
    Boundaries boundaries;
    Braco* braco;
    Shot* shot = nullptr;
    int shot_cooldown = 1000;
    
    bool can_jump = true;

    Character(float height = Character::original_height, CharacterConfigurations config = CharacterConfigurations()){
        this->is_character = true;
        this->height = height;
        this->config = config;
        float width = height*config.tronco_width;

        // Hitbox do boneco
        setHitbox(new Rect(width, height), glm::vec2(0.0f, -height/2));

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
        this->braco = new Braco();
        this->braco->width = height*config.braco_width;
        this->braco->height = height*config.braco_height;
        Rect* braco_shape = new Rect(this->braco->width, this->braco->height);
        braco->setNome("braço");
        braco->setShape(braco_shape, {this->braco->width/2, 0.0f});
        braco->setColor(config.braco_color);
        braco->min_angle = -45.0f;
        braco->max_angle = 45.0f;

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

        perna_shape = new Rect(height*config.braco_height, height*config.meia_perna);

        Entity* canela_dir = new RotatingRect();
        canela_dir->setNome("canela direita");
        canela_dir->setShape(perna_shape);
        canela_dir->setColor(config.perna_color);
        canela_dir->transform.position.y = height*config.meia_perna;
        
        coxa_dir->addChild(canela_dir);

        this->addChild(coxa_dir);

        // Monta a perna esquerda

        perna_shape = new Rect(height*config.braco_height, height*config.meia_perna);
        
        Entity* coxa_esq = new RotatingRect();
        coxa_esq->setNome("coxa esquerda");
        coxa_esq->setShape(perna_shape);
        coxa_esq->setColor(config.perna_color);
        coxa_esq->transform.position.y = - height*config.tronco_height/2;
        coxa_esq->transform.eulerRotation.z = 180.0f;

        perna_shape = new Rect(height*config.braco_height, height*config.meia_perna);

        Entity* canela_esq = new RotatingRect();
        canela_esq->setNome("canela esquerda");
        canela_esq->setShape(perna_shape);
        canela_esq->setColor(config.perna_color);
        canela_esq->transform.position.y = height*config.meia_perna;

        coxa_esq->addChild(canela_esq);

        this->addChild(coxa_esq);
        
    }

    virtual void create_boundaries(){
        if (!parent) return;

        float width = height*config.tronco_width, boundary_size = 1;


        boundaries.dist_direito.x = width/2 + boundary_size/2;
        boundaries.limite_direito = new Entity();
        boundaries.limite_direito->setHitbox(new Rect(boundary_size, height - boundary_size), glm::vec2(boundaries.dist_direito.x, -height/2));
        boundaries.limite_direito->setNome("limite direito");
        boundaries.limite_direito->is_movable = true;
        this->addChild(boundaries.limite_direito);

        boundaries.dist_esquerdo.x = -width/2 - boundary_size/2;
        boundaries.limite_esquerdo = new Entity();
        boundaries.limite_esquerdo->setHitbox(new Rect(boundary_size, height - boundary_size), glm::vec2(boundaries.dist_esquerdo.x, -height/2));
        boundaries.limite_esquerdo->setNome("limite esquerdo");
        boundaries.limite_esquerdo->is_movable = true;
        this->addChild(boundaries.limite_esquerdo);

        boundaries.dist_inferior.y = -boundary_size;
        boundaries.limite_inferior = new Entity();
        boundaries.limite_inferior->setHitbox(new Rect(width - boundary_size, boundary_size), glm::vec2(0.0f, boundaries.dist_inferior.y -height/2));
        boundaries.limite_inferior->setNome("limite inferior");
        boundaries.limite_inferior->is_movable = true;
        this->addChild(boundaries.limite_inferior);

        boundaries.dist_superior.y = height;
        boundaries.limite_superior = new Entity();
        boundaries.limite_superior->setHitbox(new Rect(width - boundary_size, boundary_size), glm::vec2(0.0f, boundaries.dist_superior.y -height/2));
        boundaries.limite_superior->setNome("limite superior");
        boundaries.limite_superior->is_movable = true;
        this->addChild(boundaries.limite_superior);
    }

    virtual void do_collision(std::list<HitboxMapping> colliding_hitboxes){
        #if defined TEST
            //std::cout << "Entrou em do collision" << std::endl;
        #endif
        bool collision[] = {false, false, false, false};

        HitboxMapping* boundaries_hitboxes[] = {
            new HitboxMapping(boundaries.limite_direito),
            new HitboxMapping(boundaries.limite_esquerdo),
            new HitboxMapping(boundaries.limite_superior),
            new HitboxMapping(boundaries.limite_inferior)
        };

        for (auto boundary : boundaries_hitboxes){
            boundary->update_hitbox();
        }
        
        #if defined TEST
            /* std::cout << colliding_hitboxes.begin()->entity_ptr->getNome() << " p1: (" << colliding_hitboxes.begin()->canto_inf_esquerdo.x << ", " << colliding_hitboxes.begin()->canto_inf_esquerdo.y << ") p2: (" << colliding_hitboxes.begin()->canto_sup_direito.x << ", " << colliding_hitboxes.begin()->canto_sup_direito.y << ") "<< std::endl;
            exit (0); */
        #endif

        for(auto colliding_hitbox : colliding_hitboxes){

            float collisions_area[] = {
                boundaries_hitboxes[0]->is_colliding(colliding_hitbox) ? boundaries_hitboxes[0]->percentage_of_overlapping(colliding_hitbox) : 0,
                boundaries_hitboxes[1]->is_colliding(colliding_hitbox) ? boundaries_hitboxes[1]->percentage_of_overlapping(colliding_hitbox) : 0,
                boundaries_hitboxes[2]->is_colliding(colliding_hitbox) ? boundaries_hitboxes[2]->percentage_of_overlapping(colliding_hitbox) : 0,
                boundaries_hitboxes[3]->is_colliding(colliding_hitbox) ? boundaries_hitboxes[3]->percentage_of_overlapping(colliding_hitbox) : 0
            };

            #if defined TEST
                /* std::cout << boundaries_hitboxes[0]->is_colliding(colliding_hitbox) << std::endl;
                std::cout << boundaries_hitboxes[1]->is_colliding(colliding_hitbox) << std::endl;
                std::cout << boundaries_hitboxes[2]->is_colliding(colliding_hitbox) << std::endl;
                std::cout << boundaries_hitboxes[3]->is_colliding(colliding_hitbox) << std::endl; */
            #endif

            const int N = sizeof(collisions_area) / sizeof(float);
            #if defined TEST
                /* for(auto valor : collisions_area){
                    std::cout << valor << " ";
                }
                std::cout << std::endl; */
            #endif

            if(collisions_area[0] || collisions_area[1] || collisions_area[2] || collisions_area[3]) collision[(std::distance(collisions_area, std::max_element(collisions_area, collisions_area + N)))] = true;
           
        }
        
        if (collision[0]) {
            this->moveLiberty.direita = false;
            #if defined TEST
                //std::cout << "Não pode mover pra direita" << std::endl;
            #endif
        } else this->moveLiberty.direita = true;

        if (collision[1]) {
            this->moveLiberty.esquerda = false;
            #if defined TEST
                //std::cout << "Não pode mover pra esquerda" << std::endl;
            #endif
        } else this->moveLiberty.esquerda = true;
        
        if (collision[2]) {
            this->moveLiberty.para_cima = false;
            this->can_jump = false;
            #if defined TEST
                //std::cout << "Não pode mover pra cima" << std::endl;
            #endif
        } else this->moveLiberty.para_cima = true;

        if (collision[3]) {
            this->moveLiberty.para_baixo = false;
            this->can_jump = true;
            this->y_moveConfigurations.max = this->transform.position.y + 3*height;
            #if defined TEST
                //std::cout << "Não pode mover pra baixo" << std::endl;
            #endif
        } else this->moveLiberty.para_baixo = true;
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){
        MovingEntity::act(keyStatus,  deltaTime);

        if(keyStatus[(int) 'p']) return;

        this->shot_cooldown -= deltaTime;
    }

    virtual void move(GLdouble deltaTime){
        MovingEntity::move(deltaTime);

        if(
            this->transform.position.y + this->y_moveConfigurations.velocity*deltaTime >= this->y_moveConfigurations.max // Se chegou na altura máxima ou
            || (this->y_moveConfigurations.velocity < 0 && moveLiberty.para_baixo)                                       // Se estava caindo e não terminou de cair ainda
        ) 
                this->can_jump = false; 
    }

    virtual void do_shot(GLdouble deltaTime){
        if (this->shot_cooldown > 0) return;

        this->shot_cooldown = 1000;
        if(this->is_player) this->shot = new PlayerShot(this->braco->height);
        else this->shot = new EnemyShot(this->braco->height);

        this->shot->transform.eulerRotation.z = this->braco->transform.eulerRotation.z;
        this->shot->transform.position.x = this->transform.position.x;
        this->shot->shape_offset = {this->braco->width + this->braco->height, this->braco->height};
        this->shot->hitbox_offset = {this->braco->width - this->braco->height, this->braco->height};
        this->shot->transform.position.y = this->transform.position.y;
        this->shot->x_moveConfigurations.velocity = 2*this->velocity;
        this->parent->addChild(this->shot);
    }

};

struct Player : public Character{


    Player(float height = Character::original_height) : Character(height) {
        this->is_player = true;
        this->braco->player_braco = true;
        this->velocity = 0.02f;
        this->x_moveConfigurations.max = 1000.0f;
        this->y_moveConfigurations.max = 500.0f;
        
        this->x_moveConfigurations.min = -1000.0f;
        this->y_moveConfigurations.min = 500.0f;
    }

    virtual void move(GLdouble deltaTime){
        Character::move(deltaTime);

        // Anima as perninhas

        // Se mudar de direção, muda o braço de direção
        /* if(this->x_moveConfigurations.velocity > 0.0f){
            this->braco->transform.position.x = this->braco->width/2;
            this->braco->transform.eulerRotation.z = 180.f;
            this->braco->transform.position.y = this->braco->height;
        } else if (this->x_moveConfigurations.velocity < 0.0f){
            this->braco->transform.position.x = -this->braco->width/2;
            this->braco->transform.eulerRotation.z = 0.0f;
            this->braco->transform.position.y = 0.0f;
        } */
        
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){
        Character::act(keyStatus, deltaTime);

        if (keyStatus[(int)('a')]){
            this->x_moveConfigurations.velocity = -velocity;
        } else if (keyStatus[(int)('d')]){
            this->x_moveConfigurations.velocity = velocity;
        } else {
            this->x_moveConfigurations.velocity = 0.0f;
        }

        #if defined TEST
            if (keyStatus[(int)('w')]){
                this->y_moveConfigurations.velocity = height*3/1000;
                //std::cout << this->y_moveConfigurations.velocity << std::endl;
            } else if (keyStatus[(int)('s')]){
                this->y_moveConfigurations.velocity = -height*3/1000;
            }else{
                this->y_moveConfigurations.velocity = 0.0f;
            }
        #else
            if(keyStatus[MOUSE_RIGHT_CLICK] && this->can_jump){
                this->y_moveConfigurations.velocity = height*3/1000;
            } else {
                this->y_moveConfigurations.velocity = -height*3/1000;;
            }
        #endif

            if(keyStatus[MOUSE_LEFT_CLICK]){
                this->do_shot(deltaTime);
            }

        this->move(deltaTime);

        glm::vec4 real_player_coords = this->transform.modelMatrix*glm::vec4(this->transform.position.x,this->transform.position.y, 0.0f, 1.0f);
        
        keyStatus[PLAYER_X_COORD] = (int) real_player_coords.x;
        keyStatus[PLAYER_Y_COORD] = (int) -real_player_coords.y;

        #if defined TEST
            //std::cout << "Player no ponto: " << keyStatus[PLAYER_X_COORD] << ", " << keyStatus[PLAYER_Y_COORD] << std::endl;
        #endif
    }

};

struct Enemy : public Character{
    Enemy(float height = Character::original_height) : Character(height){};

    virtual void act(int* keyStatus, GLdouble deltaTime){ };

};


#endif