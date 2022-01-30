#if !defined TEXT
#define TEXT
#define GLT_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#define VERTEXT_IMPLEMENTATION

//#include "../Third-Party-Libs/glText/gltext.h"

#include "Entity.hpp"
#include "Shape.hpp"

void glutBitmapString(void* font, int length, const unsigned char * text){
    for(int i = 0; i < length; i++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, text[i]);
    }
}

struct Text : public Entity{

    bool can_show = false;
    TextShape text_shape;
    glm::vec3 scale = {0.0f, 0.0f, 0.0f};

    Text(std::string text = "Ohayo sekai"){
        this->nome = "Texto: " + text;
        this->setText(text);
        this->setShape(&text_shape);
        this->scale = {0.15f, 0.15f, 1.0f};
        #if defined TEST
            //this->can_show = true;
        #endif
    }

    virtual void setText(std::string text){
        text_shape.text = text;
    }

    virtual void set_can_show(bool can_show){
        this->can_show = can_show;
        for (auto child : children){
            if(Text* child_text = dynamic_cast<Text*>(child)){
                child_text->can_show = can_show;
            }
        }
    }

    virtual bool prepare_drawing(){
        if (this->can_show){
            int text_width = 0;
            for(auto c : text_shape.text){
                text_width += glutStrokeWidth(text_shape.font, c);
            }

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glScalef(this->scale.x, this->scale.y, this->scale.z);
            glTranslatef(-text_width/2 + this->shape_offset.x, this->shape_offset.y, 0.0f);
        }
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

#endif