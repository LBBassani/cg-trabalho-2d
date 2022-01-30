#if !defined SCENARIO_CONFIG
#define SCENARIO_CONFIG

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "../Domain/SceneTree.hpp"
#include "../Domain/Character.hpp"
#include "../Domain/Camera.hpp"
#include "../Domain/Text.hpp"
#include "../Third-Party-Libs/rapidxml-1.13/rapidxml.hpp"

struct ParserSVG{

    static Entity* parseSVG(std::string &svgFileName) {

        Camera* scene_root = new Camera();
        scene_root->setNome("Camera");

        Entity* background = new Entity();
        background->setNome("Background");

        Entity* game_won_message = game_won_message = new Colliding_Text("You won!!");
        game_won_message->addChild(new Subtext("Press r to restart"));

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
                    
                    Entity* parede = new Entity();
                    // Hitbox das paredes do cenário
                    parede->setNome("Limite lateral esquerdo");
                    parede->transform.position.x = newBackgroundModel->transform.position.x;
                    parede->transform.position.y = newBackgroundModel->transform.position.y;
                    parede->setHitbox(new Rect(1.0f, height), glm::vec2(-width/2, 0.0f));
                    background->addChild(parede);

                    parede = new Entity();
                    parede->setNome("Limite lateral direito");
                    parede->transform.position.x = newBackgroundModel->transform.position.x;
                    parede->transform.position.y = newBackgroundModel->transform.position.y;
                    parede->setHitbox(new Rect(1.0f, height), glm::vec2(width/2, 0.0f));
                    background->addChild(parede);

                    parede = new Entity();
                    parede->setNome("Limite superior");
                    parede->transform.position.x = newBackgroundModel->transform.position.x;
                    parede->transform.position.y = newBackgroundModel->transform.position.y;
                    parede->setHitbox(new Rect(width, 1.0f), glm::vec2(0.0f, height));
                    background->addChild(parede);

                    parede = new Entity();
                    parede->setNome("Limite inferior");
                    parede->transform.position.x = newBackgroundModel->transform.position.x;
                    parede->transform.position.y = newBackgroundModel->transform.position.y;
                    parede->setHitbox(new Rect(width, 1.0f), glm::vec2(0.0f, -1.0f));
                    background->addChild(parede);

                    game_won_message->transform.position.x = newBackgroundModel->transform.position.x;
                    game_won_message->transform.position.y = newBackgroundModel->transform.position.y;
                    game_won_message->setHitbox(new Rect(10.0f, 10.0f), glm::vec2(width/2 - 10.0f, 0.0f));

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
                    character = new Enemy(r*2);
                    enemy_count++;
                    std::string character_name = "Enemy ";
                    character_name.append(std::to_string(enemy_count));
                    character->setNome(character_name);
                }

                character->transform.position.x = x - x_0 + r/2;
                character->transform.position.y = -y - y_0;

                character->y_moveConfigurations.min = character->transform.position.y - r*2;
                character->y_moveConfigurations.max = character->transform.position.y + r*6; // Pula 3x a própria altura
 
                background->addChild(character);
                character->create_boundaries();
            }
        }

        background->addChild(game_won_message);
        scene_root->addChild(background);

        // Configura zoom da tela
        scene_root->transform.scale.x = zoom_factor;
        scene_root->transform.scale.y = zoom_factor;
        #if defined TEST
            std::cout << zoom_factor << std::endl;
        #endif

        // Textos no inicio
        Cascading_Timed_Text* t_1 = new Cascading_Timed_Text("Hello", 2000);
        scene_root->addChild(t_1);
        Cascading_Timed_Text* t_2 = new Cascading_Timed_Text(" ", 500);
        t_1->addChild(t_2);

        Cascading_Timed_Text* t_3 = new Cascading_Timed_Text("", 2000); // Timer para ativar o próximo
        t_2->addChild(t_3);

        Timed_Text* t_4 = new Timed_Text("", 2000); // Timer para ficar ativo
        Subtext* sub = new Subtext("You can move on 'a' 'd'");
        t_4->addChild(sub);
        t_2->addChild(t_4);

        t_2 = t_3;
        t_3 = new Cascading_Timed_Text(" ", 500); // Intervalo
        t_2->addChild(t_3);

        t_2 = t_3;
        t_3 = new Cascading_Timed_Text("", 2000);  // Timer para ativar o próximo
        t_2->addChild(t_3);

        t_4 = new Timed_Text("", 2000); // Timer para ficar ativo
        sub = new Subtext("You can jump on mouse right click"); 
        t_4->addChild(sub);
        t_2->addChild(t_4);

        t_2 = t_3;
        t_3 = new Cascading_Timed_Text(" ", 500); // Intervalo
        t_2->addChild(t_3);

        t_2 = t_3;
        t_3 = new Cascading_Timed_Text("", 2000);  // Timer para ativar o próximo
        t_2->addChild(t_3);

        t_4 = new Timed_Text("", 2000); // Timer para ficar ativo
        sub = new Subtext("You can shoot on mouse left click");
        t_4->addChild(sub);
        t_2->addChild(t_4);

        t_2 = t_3;
        t_3 = new Cascading_Timed_Text(" ", 500); // Intervalo
        t_2->addChild(t_3);

        t_2 = t_3;
        t_3 = new Cascading_Timed_Text("", 2000);  // Timer para ativar o próximo
        t_2->addChild(t_3);

        t_4 = new Timed_Text("", 2000); // Timer para ficar ativo
        sub = new Subtext("You can pause on 'p'");
        t_4->addChild(sub);
        t_2->addChild(t_4);

        t_1->set_can_show(true);
            
        return scene_root;
    }

    static glm::vec3 getColorCode(std::string colorName){
        glm::vec3 color;

        if (colorName == "black")
            color = glm::vec3(0.0f, 0.0f, 0.0f);
             
        else if(colorName == "blue")
            color = glm::vec3(0.2f, 0.5f, 1.0f);

        else if(colorName == "green")
            color = glm::vec3(0.0f, 0.2f, 0.0f);
            
        else if(colorName == "red")
            color = glm::vec3(1.0f, 0.0f, 0.0f);
        
        else
            color = glm::vec3(1.0f, 1.0f, 0.0f);
        

        return color;
    }

};

#endif