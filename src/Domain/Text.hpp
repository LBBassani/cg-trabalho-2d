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

    Text(std::string text){
        this->setText(text);
        this->setShape(&text_shape);
        #if defined TEST
            this->can_show = true;
        #endif
    }    

    virtual void setText(std::string text){
        text_shape.text = text;
    }

    virtual void draw(){
        if (!this->can_show) return;

        int text_width = 0;
        for(auto c : text_shape.text){
            text_width += glutStrokeWidth(text_shape.font, c);
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glScalef(0.1f, 0.1f, 1.0f);
        glTranslatef(-text_width/2, 0.0f, 0.0f);
        Model::draw();


    }

};

#endif