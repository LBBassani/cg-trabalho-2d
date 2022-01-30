#if !defined OPEN_GL_STARTER
#define OPEN_GL_STARTER

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <functional>

#include "OpenGLConfig.hpp"
#include "ScenarioConfig.hpp"
#include "../Domain/SceneTree.hpp"
#include "../Domain/Shape.hpp"
#include "../Domain/Text.hpp"

#define NUM_TECLAS_ASCII 256
#define LEFT_CLICK 256
#define RIGHT_CLICK 257
#define MOUSE_X_COORD 258
#define MOUSE_Y_COORD 259
#define PLAYER_X_COORD 260
#define PLAYER_Y_COORD 261

struct Mouse : public Entity{
    bool must_draw = true;
    glm::vec3 color_on_left_click = {0.2f, 1.0f, 0.2f};
    glm::vec3 color_on_right_click = {1.0f, 0.0f, 0.5f};
    glm::vec3 normal_color = {1.0f, 1.0f, 1.0f};

    Mouse(){
        this->setNome("Mouse");
        this->setColor(this->normal_color);
        this->setShape(new Triangle(1.0f, 1.0f));
        this->transform.eulerRotation.z = 45.0f;
        this->is_movable = true;
    }

    virtual void draw(){ if (must_draw) Entity::draw(); } // Desenha se puder ser desenhado

    virtual void update_position_on_world(int x, int y){
        glm::vec4 position_on_world = glm::inverse(parent->transform.modelMatrix)*glm::vec4(x, y, 0.0f, 1.0f);
        this->transform.position.x = position_on_world.x;
        this->transform.position.y = position_on_world.y;
    }

    virtual void act(int* keyStatus, GLdouble){
        if(keyStatus[(int) ('m')]) must_draw = !must_draw;
        
        if(keyStatus[LEFT_CLICK]) this->setColor(color_on_left_click);
        else if(keyStatus[RIGHT_CLICK]) this->setColor(color_on_right_click);
        else this->setColor(normal_color);
        
    }
};

struct OpenGLStarter{
    GLdouble framerate = 0;
    OpenGLConfig openGLConfig;
    int keyStatus[NUM_TECLAS_ASCII + 2 + 2 + 2]; // Teclas ascii + dois cliques do mouse + duas coordenadas do mouse + duas coordenadas do boneco
    SceneTree sceneTree;
    std::string sceneName;
    Mouse* mouse;
    static OpenGLStarter* instance;

    OpenGLStarter(const OpenGLConfig &config, std::string fileName){
        this->openGLConfig = OpenGLConfig(config);
        this->sceneName = fileName;
        this->makeWorld();
        /* Timed_Text* t = new Timed_Text("Ohayo", 300);
        this->sceneTree.root->addChild(t);
        t->set_can_show(true); */
    }

    virtual void makeWorld(){
        this->sceneTree.cleanSceneTree();
        this->sceneTree.root = ParserSVG::parseSVG(this->sceneName);
        this->mouse = new Mouse();
        this->sceneTree.root->addChild(this->mouse);
        this->sceneTree.root->updateSelfAndChildren();
        this->sceneTree.updateHitboxMapping();

        #if defined TEST
            sceneTree.print();
            //return 0;
        #endif

    }

    static void setInstance(OpenGLStarter* openGLStarter){
        instance = openGLStarter;
    }

    void ResetKeyStatus(){
        int i;
        //Initialize keyStatus
        for(i = 0; i < NUM_TECLAS_ASCII + 2 + 2; i++)
            keyStatus[i] = 0; 
    }

    static void keyup(unsigned char key, int x, int y){
        instance->keyStatus[(int)(tolower(key))] = 0;
    }

    static void renderScene(void){
        glClear(GL_COLOR_BUFFER_BIT);   // Limpa tela

        instance->sceneTree.draw();     // Desenha cena no buffer

        glutSwapBuffers();              // Desenha a cena do buffer na tela
        glutPostRedisplay();
    }

    static void idle(void){
        if(instance->keyStatus[(int) 'r']) instance->makeWorld();
        static GLdouble prevTime = glutGet(GLUT_ELAPSED_TIME);
        GLdouble curTime, deltaTime;
        curTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = curTime - prevTime;
        prevTime = curTime;
        instance->framerate = 1.0 / deltaTime * 1000;

        instance->sceneTree.idle(instance->keyStatus, deltaTime);
    }

    static void keyPress(unsigned char key, int, int){
        if( key == 27 )
            exit(0);

        instance->keyStatus[(int) tolower(key)] = 1;
    }

    static void mouseClick(int button, int state, int, int){
        if(button == GLUT_LEFT_BUTTON){
            if(state == GLUT_DOWN) instance->keyStatus[LEFT_CLICK] = 1; // On press
            else instance->keyStatus[LEFT_CLICK] = 0;                   // On release
        } else if(button == GLUT_RIGHT_BUTTON){
            if(state == GLUT_DOWN) instance->keyStatus[RIGHT_CLICK] = 1;  // On press
            else instance->keyStatus[RIGHT_CLICK] = 0;                  // On release
        }
        #if defined TEST
            //if(instance->keyStatus[LEFT_CLICK]) std::cout << "Clique esquerdo!!" << std::endl;
            //if(instance->keyStatus[RIGHT_CLICK]) std::cout << "Clique direito!!" << std::endl;
        #endif
    }

    static void mouseMove(int x, int y){
        #if defined TEST
            // std::cout << "Mouse em (" << instance->keyStatus[MOUSE_X_COORD] << ", " << instance->keyStatus[MOUSE_Y_COORD] << ")" << std::endl;
        #endif

        instance->mouse->update_position_on_world(x - instance->openGLConfig.width/2, - y + instance->openGLConfig.height/2);
        instance->keyStatus[MOUSE_X_COORD] = x - instance->openGLConfig.width/2;
        instance->keyStatus[MOUSE_Y_COORD] = - y + instance->openGLConfig.height/2;
        #if defined TEST
            // std::cout << "Mouse em (" << instance->mouse->transform.position.x << ", " << instance->mouse->transform.position.y << ")" << std::endl;
        #endif
    }

    void initGlut(int argc, char *argv[]){
    
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

        // Create the window.
        glutInitWindowSize(openGLConfig.width, openGLConfig.height);
        glutInitWindowPosition(150,50);
        glutCreateWindow(openGLConfig.windowName.c_str());

        glutKeyboardUpFunc(keyup);
        glutKeyboardFunc(keyPress);
        glutDisplayFunc(renderScene);
        glutIdleFunc(idle);
        glutPassiveMotionFunc(mouseMove);
        glutMouseFunc(mouseClick);

    }

    void startGlut(void){
        ResetKeyStatus();
        // The color the windows will redraw. Its done to erase the previous frame.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black, no opacity(alpha).
    
        glMatrixMode(GL_PROJECTION); // Select the projection matrix
        glOrtho(-(openGLConfig.viewingWidth/2),     // X coordinate of left edge
                (openGLConfig.viewingWidth/2),     // X coordinate of right edge
                -(openGLConfig.viewingHeight/2),     // Y coordinate of bottom edge
                (openGLConfig.viewingHeight/2),     // Y coordinate of top edge           
                -100,     // Z coordinate of the “near” plane      
                100);    // Z coordinate of the “far” plane
        glMatrixMode(GL_MODELVIEW); // Select the projection matri  
        glLoadIdentity();

        glutMainLoop();
        
    }
};

OpenGLStarter* OpenGLStarter::instance = nullptr;

#endif