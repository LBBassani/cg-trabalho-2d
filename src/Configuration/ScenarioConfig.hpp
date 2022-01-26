#if !defined SCENARIO_CONFIG
#define SCENARIO_CONFIG

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "../Domain/SceneTree.hpp"
#include "../Domain/Character.hpp"
#include "../Domain/Camera.hpp"
#include "../Third-Party-Libs/rapidxml-1.13/rapidxml.hpp"

struct ParserSVG{

    static Entity* parseSVG(std::string &svgFileName) {

        Camera* scene_root = new Camera();
        scene_root->setNome("Camera");

        Entity* background = new Entity();
        background->setNome("Background");

        rapidxml::xml_document<> doc;
        rapidxml::xml_node<> * root_node = NULL;

        std::ifstream theFile (svgFileName);
        std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
        buffer.push_back('\0');

        doc.parse<0>(&buffer[0]);

        root_node = doc.first_node("svg");

        bool bigger_width_than_height = true;
        float zoom_factor, x_0, y_0, x_offset, y_offset, background_width, background_height;

        int enemy_count = 0, plataforma_count = 0;

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

                    background_width = width;
                    background_height = height;
                    x_0 = x + background_width/2;
                    y_0 = -y - background_height/2;

                }

                Entity* newBackgroundModel = new Entity();
                bigger_width_than_height ? newBackgroundModel->setShape(new Rect(width, height)) : newBackgroundModel->setShape(new Rect(height, width));
                if(colorName != "blue") newBackgroundModel->setHitbox(newBackgroundModel->getShape()); // background não deve ter hitbox 
                newBackgroundModel->setColor(getColorCode(colorName));
                newBackgroundModel->transform.position.x = x - x_0 + width/2;
                newBackgroundModel->transform.position.y = -y - y_0 - height;
                if(colorName != "blue") {
                    plataforma_count++;
                    std::string model_name = "Plataforma ";
                    model_name.append(std::to_string(plataforma_count));
                    newBackgroundModel->setNome(model_name);
                } else {
                    newBackgroundModel->setNome("Sky");    
                }
                
                background->addChild(newBackgroundModel);
                
            } else if (nodeName == "circle"){

                float   r = atof(shape_node->first_attribute("r")->value()),
                        x = atof(shape_node->first_attribute("cx")->value()),
                        y = atof(shape_node->first_attribute("cy")->value());

                Character* character = nullptr;
                if(colorName == "green"){
                    Player* player = new Player(r*2);
                    character = player;
                    character->setNome("Player");
                    scene_root->setPlayer(player);
                    background->transform.position.x = -(x - x_0 + r/2);
                } else{
                    enemy_count++;
                    std::string character_name = "Enemy ";
                    character_name.append(std::to_string(enemy_count));
                    character = new Enemy(r*2);
                    character->setNome(character_name);
                }

                character->setHitbox(new Rect(r*2, r*2));
                character->transform.position.x = x - x_0 + r/2;
                character->transform.position.y = -y - y_0;

                background->addChild(character);
            }
        }

        scene_root->addChild(background);

        // Configura zoom da tela
        scene_root->transform.scale.x = zoom_factor;
        scene_root->transform.scale.y = zoom_factor;
            
        return scene_root;
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