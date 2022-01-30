#if !defined TEXT
#define TEXT

#include "Entity.hpp"
#include "Shape.hpp"

void glutBitmapString(void* font, int length, const unsigned char * text){
    for(int i = 0; i < length; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text[i]);
    }
}

struct Text : public Entity{

    bool can_show = false;
    TextShape* text_shape;
    glm::vec3 scale = {0.0f, 0.0f, 0.0f};

    Text(std::string text = "Ohayo sekai"){
        this->nome = "Texto: " + text;
        this->text_shape = new TextShape();
        this->setText(text);
        this->setShape(text_shape);
        this->scale = {0.15f, 0.15f, 1.0f};
        #if defined TEST
            //this->can_show = true;
        #endif
    }

    virtual void addChild(Entity* entity){
        Entity::addChild(entity);
        if(Text* child = dynamic_cast<Text*>(entity)){
            child->shape_offset.x += this->shape_offset.x;
            child->shape_offset.y += this->shape_offset.y;
        }
    }

    virtual void setText(std::string text){
        text_shape->text = text;
    }

    virtual void set_can_show(bool can_show){
        this->can_show = can_show;

        #if defined TEST
            if(can_show) std::cout << this->getNome() << " can show " << std::endl;
        #endif 

        for (auto child : children){
            if(Text* child_text = dynamic_cast<Text*>(child)){
                child_text->set_can_show(can_show);
            }
        }
    }

    virtual bool prepare_drawing(){
        if (this->can_show){
            int text_width = 0;
            for(auto c : text_shape->text){
                text_width += glutStrokeWidth(text_shape->font, c);
            }

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glScalef(this->scale.x, this->scale.y, this->scale.z);
            glTranslatef(-text_width/2 + this->shape_offset.x, this->shape_offset.y, 0.0f);

            #if defined TEST
                // std::cout << "Imprimindo " << this->getNome() << std::endl;
            #endif
        }

        #if defined TEST
            // else std::cout << "Não pode mostrar " << this->getNome() << std::endl;
        #endif
        return this->can_show;
    }

};

struct Colliding_Text : public Text{

    Colliding_Text(std::string text = "Ohayo sekai") : Text(text){
        this->is_movable = true;
        this->is_trigger = true;
    }

    virtual void do_collision(std::list<HitboxMapping> colliding_hitbox) {
        HitboxMapping hitbox = *colliding_hitbox.begin();
        
        #if defined TEST
            // std::cout << hitbox.entity_ptr->getNome() <<" colidiu com texto!" << std::endl;
        #endif

        if(hitbox.entity_ptr->is_player) {
            this->set_can_show(true);
        }
    }

};

struct Subtext : public Text{
    Subtext(std::string text = "Good morning world") : Text(text){
        this->scale = {0.1f, 0.1f, 1.0f};
        this->shape_offset = {0.0f, -100.0f};
    }

};

struct Timed_Text : public Text{
    int timer;
    int timer_initial_value;

    Timed_Text(std::string text = "Ohayo Sekai", int timer_value = 1000.0f) : Text(text){
        this->timer = timer_value;
        this->timer_initial_value = timer_value;
        this->is_movable = true;
    }

    virtual void act(int* keyStatus, GLdouble deltaTime){
        if(this->can_show) timer -= deltaTime;
        
        #if defined TEST
            //std::cout << timer << std::endl;
        #endif
        
        if(timer <= 0){
            this->set_can_show(false);
            this->timer = this->timer_initial_value; 
        }
    }
};

struct Cascading_Timed_Text : public Timed_Text{

    virtual void set_can_show(bool can_show){
        this->can_show = can_show;
    };

    virtual void act(int* keyStatus, GLdouble deltaTime){
        bool old_can_show = this->can_show;
        if(!old_can_show) return;

        Timed_Text::act(keyStatus, deltaTime);

        bool new_can_show = this->can_show;
        if(old_can_show != new_can_show){
            Timed_Text::set_can_show(true);
            this->set_can_show(new_can_show);
        }
    }

    virtual void draw(){
        if(prepare_drawing()) Timed_Text::draw();

        for(auto child : children){
            child->draw();
        }
    }

};

#endif