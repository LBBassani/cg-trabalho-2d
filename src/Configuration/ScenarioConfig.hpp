#if !defined SCENARIO_CONFIG
#define SCENARIO_CONFIG

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "../Domain/SceneTree.hpp"
#include "../Domain/Character.hpp"
#include "../Third-Party-Libs/rapidxml-1.13/rapidxml.hpp"

struct ParserSVG{

    static Entity* parseSVG(std::string &svgFileName) {

        Entity* entity = new Entity();

        Entity* background = new Entity();
        background->setNome("background");

        rapidxml::xml_document<> doc;
        rapidxml::xml_node<> * root_node = NULL;

        std::ifstream theFile (svgFileName);
        std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
        buffer.push_back('\0');

        doc.parse<0>(&buffer[0]);

        root_node = doc.first_node("svg");

        bool bigger_width_than_height = true;
        float zoom_factor, x_0, y_0, x_offset, y_offset;

        for(auto shape_node = root_node->first_node(); shape_node; shape_node = shape_node->next_sibling()){
            // Atributos de forma e cor
            std::string nodeName = shape_node->name();
            std::string colorName = shape_node->first_attribute("fill")->value();

            if(nodeName == "rect"){

                // Atributos do retangulo
                float   width = atof(shape_node->first_attribute("width")->value()), 
                        height = atof(shape_node->first_attribute("height")->value()), 
                        x = atof(shape_node->first_attribute("x")->value()), 
                        y = atof(shape_node->first_attribute("y")->value());


                if(colorName == "blue"){
                    width > height ? bigger_width_than_height = true : bigger_width_than_height = false;

                    bigger_width_than_height ? zoom_factor = 500/height : 500/width;
                    x_0 = x + width/2;
                    y_0 = -y - height/2;
                    x_offset = 500/zoom_factor;
                    y_offset = 500/zoom_factor;

                }

                Entity* newBackgroundModel = new Entity();
                bigger_width_than_height ? newBackgroundModel->setShape(new Rect(width, height)) : newBackgroundModel->setShape(new Rect(height, width));
                newBackgroundModel->setHitbox(newBackgroundModel->getShape());
                newBackgroundModel->setColor(getColorCode(colorName));
                newBackgroundModel->transform.position.x = x - x_0 + width/2;
                newBackgroundModel->transform.position.y = -y - y_0 - height;
                newBackgroundModel->setNome("Rect");
                //newBackgroundModel->transform.scale.x = zoom_factor;
                //newBackgroundModel->transform.scale.y = zoom_factor;
                
                background->addChild(newBackgroundModel);
            } else if (nodeName == "circle"){

                float   r = atof(shape_node->first_attribute("r")->value()),
                        x = atof(shape_node->first_attribute("cx")->value()),
                        y = atof(shape_node->first_attribute("cy")->value());

                Character* character = nullptr;
                if(colorName == "green"){
                    character = new Player();
                } else{
                    character = new Enemy();
                }

                character->setHitbox(new Rect(r, r));
                character->transform.position.x = x - x_0 + r/2;
                character->transform.position.y = -y - y_0 - r;
                character->setNome("Character");

                background->addChild(character);
            }
        }

        entity->addChild(background);
        
        return entity;
    }

    static glm::vec3 getColorCode(std::string colorName){
        glm::vec3 color;

        /* if (colorName == "black")
            color = glm::vec3(0.0f, 0.0f, 0.0f);
             
        else */ if(colorName == "blue")
            color = glm::vec3(0.0f, 0.0f, 1.0f);

        else if(colorName == "green")
            color = glm::vec3(0.0f, 1.0f, 0.0f);
            
        else if(colorName == "red")
            color = glm::vec3(1.0f, 0.0f, 0.0f);
        
        else
            color = glm::vec3(1.0f, 1.0f, 0.0f);
        

        return color;
    }

};

#endif