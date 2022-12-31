#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"

void configScene();
void renderScene();
void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawParedes (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSofa (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVentilador(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawMesa(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPata(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCuerpoChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTroncos(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawAlfombra(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBalda(glm::mat4 P, glm::mat4 V, glm::mat4 M);



void controladorTiempo();



void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);

// Shaders
Shaders shaders;

// Modelos
Model plane;
Model cube;
Model cylinder;
Model sphere;
Model triangle;
Model tronco;


// Viewport
int w = 1000;
int h = 1000;

// Animaciones
float desY = 0.0;
float desX = 0.0;

float rotY = 0.0;
float desZ = 0.0;

// Movimiento de camara
float fovy   = 60.0;
float alphaX =  0.0;
float alphaY =  0.0;

// Rotacion por tiempo
float RTiempo = 0;
int rotVentilador = 0;

int main() {

    // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Proyecto Final", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback(window, funKey);
    glfwSetScrollCallback   (window, funScroll);
    glfwSetCursorPosCallback(window, funCursorPos);

    // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;

}

void configScene() {

    // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);

    // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

    // Modelos
    plane.initModel("resources/models/plane.obj");
    cube.initModel("resources/models/cube.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");
    triangle.initModel("resources/models/triangle.obj");
    tronco.initModel("resources/models/wooden.OBJ");

}

void renderScene() {

    // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Indicamos los shaders a utilizar
    shaders.useShaders();

    // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

    // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x+desX,   y+desY,   z+desZ);
    glm::vec3 center(0.0+desX, 4.0+desY,  0.0+desZ);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);

    // Dibujamos el suelo
    drawSuelo(P, V, I);

    // Dibujamos las paredes y el techo
    drawParedes(P, V, I);

    // Dibujamos sofá
    glm::mat4 SSofa = glm::scale(I, glm::vec3(1.25, 1, 1));
    glm::mat4 TSofa = glm::translate(I, glm::vec3(-8, 0, -8.5));
    drawSofa(P, V, TSofa*SSofa);

    // Dibujamos ventilador (Iván)
    glm::mat4 SVentilador = glm::scale(I, glm::vec3(2.5, 1.4, 2.5));
    glm::mat4 TVentilador = glm::translate(I, glm::vec3(0, 8.1, 0));
    drawVentilador(P, V, TVentilador*SVentilador);
    if(rotVentilador%2 == 0) controladorTiempo();

    // Dibujamos chimenea (Raúl)
    glm::mat4 TChimenea = glm::translate(I, glm::vec3(0, 0, -9));
    drawChimenea(P, V, TChimenea);
    //Dibujamos balda(Raúl)
    glm::mat4 TBalda = glm::translate(I, glm::vec3(8, 5, -10));
    drawBalda(P,V,TBalda);

    // Dibujamos mesa (Iván)
    glm::mat4 TMesa = glm::translate(I, glm::vec3(-8, 0, -5));
    drawMesa(P, V, TMesa);
    // Dibujamos alfombra(Raúl)
    drawAlfombra(P,V,TMesa);

    // Dibujamos velas (Iván)

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(10.0, 1.0, 10.0));
    drawObject(plane, glm::vec3(0.0, 0.0, 1.0), P, V, M*S);

}

void drawParedes(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S1 = glm::scale(I, glm::vec3(5.0, 1.0, 10.0));
    glm::mat4 Rz90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-10, 5, 0));
    drawObject(plane, glm::vec3(1.0, 0.0, 0.0), P, V, M*T1*Rz90*S1);

    glm::mat4 S2 = glm::scale(I, glm::vec3(10.0, 1.0, 5.0));
    glm::mat4 Rx90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0, 5, -10));
    drawObject(plane, glm::vec3(1.0, 0.0, 0.0), P, V, M*T2*Rx90*S2);

    glm::mat4 T3 = glm::translate(I, glm::vec3(10, 5, 0));
    drawObject(plane, glm::vec3(1.0, 0.0, 0.0), P, V, M*T3*Rz90*S1);

    // Techo
    glm::mat4 S = glm::scale(I, glm::vec3(10.0, 1.0, 10.0));
    glm::mat4 T4 = glm::translate(I, glm::vec3(0, 10, 0));
    drawObject(plane, glm::vec3(1.0, 0.0, 0.0), P, V, M*T4*S);

}
void drawChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    //Conducto
    glm::mat4 T1 = glm::translate(I, glm::vec3(0, 6.75, -0.65));
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.5, 3.25, 0.35));
    glm::mat4 R1 =glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    drawObject(cube, glm::vec3(1, 1, 1), P, V, M*T1*S1*R1);
    //Cuerpo
    glm::mat4 T2 = glm::translate(I, glm::vec3(0, 3.25, -0.5));
    drawCuerpoChimenea(P,V,M*T2);
}
void drawCuerpoChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    //PARTE DE ARRIBA
    glm::mat4 S = glm::scale(I, glm::vec3(1.75, 0.25, 0.6));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0, 0.1));

    drawObject(cube, glm::vec3(0, 1, 0), P, V, M*T*S);
    //PARTE DE ABAJO
    glm::mat4 T1 = glm::translate(I, glm::vec3(0, -3, 0.5));
    glm::mat4 S1 = glm::scale(I, glm::vec3(2.5, 0.25, 1));
    drawObject(cube, glm::vec3(0, 1, 0), P, V, M*T1*S1);
    //PARTE IZQUIERDA
    glm::mat4 T2 = glm::translate(I, glm::vec3(-1.55, -1.25, 0.1));
    glm::mat4 T3 = glm::translate(I, glm::vec3(1.55, -1.25, 0.1));
    glm::mat4 S2 = glm::scale(I, glm::vec3(0.2, 1.5, 0.6));

    drawObject(cube, glm::vec3(0, 1, 1), P, V, M*T2*S2);
    drawObject(cube, glm::vec3(0, 1, 1), P, V, M*T3*S2);
    //PARTE TRASERA
    glm::mat4 T4 = glm::translate(I, glm::vec3(0, -1.75, -0.45));
    glm::mat4 S4 = glm::scale(I, glm::vec3(1.75, 1.5, 0.05));
    drawObject(cube, glm::vec3(0, 0, 0), P, V, M*T4*S4);
    drawTroncos(P,V,M*T1*S1);
}
void drawTroncos(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 T1 = glm::translate(I, glm::vec3(0.25, 1.5, 0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0.25, 1.5, -0.5));
    glm::mat4 T3 = glm::translate(I, glm::vec3(0.25, 2.4, -0.25));

    glm::mat4 S = glm::scale(I, glm::vec3(2, 0.5, 1));
    glm::mat4 R1 =glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));

    drawObject(tronco, glm::vec3(1, 0, 1), P, V, M*T1*R1*S);
    drawObject(tronco, glm::vec3(1, 1, 0.5), P, V, M*T2*R1*S);
    drawObject(tronco, glm::vec3(1, 0.5, 1), P, V, M*T3*R1*S);


}
void drawAlfombra(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 T = glm::translate(I, glm::vec3(1, 0, 0.5));
    glm::mat4 S = glm::scale(I, glm::vec3(1.75*1.25, 0.01, 1*1.25));
    drawObject(sphere, glm::vec3(1, 1, 1), P, V, M*T*S);
}
void drawBalda(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(2, 0.15, 0.6));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0, 0.6));
    drawObject(cube, glm::vec3(1, 1, 1), P, V, M*T*S);
    glm::mat4 S1 = glm::scale(I, glm::vec3(3, 0.15, 0.6));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-1, -2, 0.6));
    drawObject(cube, glm::vec3(1, 1, 1), P, V, M*T1*S1);
}
void drawSofa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Asiento
    glm::mat4 S = glm::scale(I, glm::vec3(1.5, 0.25, 1.0));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0.75, 0));
    drawObject(cube, glm::vec3(0.0, 1.0, 0.0), P, V, M*T*S);

    // Reposabrazos
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.25, 0.75, 0.75));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-1.25, 0.75, 0.25));
    drawObject(cube, glm::vec3(0.139, 0.069, 0.019), P, V, M*T1*S1);

    glm::mat4 T2 = glm::translate(I, glm::vec3(1.25, 0.75, 0.25));
    drawObject(cube, glm::vec3(0.139, 0.069, 0.019), P, V, M*T2*S1);

    // Respaldo
    glm::mat4 Rx90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 T3 = glm::translate(I, glm::vec3(0, 1, -0.75));
    drawObject(cube, glm::vec3(0.0, 1.0, 0.0), P, V, M*T3*Rx90*S);

    // Parte delantera
    glm::mat4 S2 = glm::scale(I, glm::vec3(1, 0.25, 0.25));
    glm::mat4 T4 = glm::translate(I, glm::vec3(0, 0.25, 0.75));
    drawObject(cube, glm::vec3(0.139, 0.069, 0.019), P, V, M*T4*S2);

}

void drawVentilador(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Cuerpo (Cilindro)
    glm::mat4 SCuerpo = glm::scale(I, glm::vec3(0.25, 0.12, 0.25));
    glm::mat4 TCuerpo = glm::translate(I, glm::vec3(0, 1, 0));
    drawObject(cylinder, glm::vec3(0.6, 0.6, 0.6), P, V, M*TCuerpo*SCuerpo);

    // Plafón (Esfera)
    glm::mat4 SPlafon = glm::scale(I, glm::vec3(0.25, 0.12, 0.25));
    glm::mat4 TPlafon = glm::translate(I, glm::vec3(0, 0.85, 0));
    drawObject(sphere, glm::vec3(0.9, 0.9, 0.9), P, V, M*TPlafon*SPlafon);

    // Sorporte Techo (Cilindro)
    glm::mat4 SSoporte = glm::scale(I, glm::vec3(0.05, 0.25, 0.05));
    glm::mat4 TSoporte = glm::translate(I, glm::vec3(0, 1.2, 0));
    drawObject(cylinder, glm::vec3(0.139, 0.069, 0.019), P, V, M*TSoporte*SSoporte);

    // Aspas (Triangulo)
    glm::mat4 RotTiempo = glm::rotate(I, glm::radians(RTiempo), glm::vec3(0, 1, 0));
    drawAspas(P, V, M*RotTiempo);

}

void drawAspas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Aspas (Triangulo)
    glm::mat4 SAspa1 = glm::scale(I, glm::vec3(0.15, 0.5, 1));
    glm::mat4 TAspa1 = glm::translate(I, glm::vec3(0, 1, 0.65));
    glm::mat4 Rx90 = glm::rotate(I, glm::radians(-90.0f), glm::vec3(1, 0, 0));
    drawObject(triangle, glm::vec3(0.139, 0.069, 0.019), P, V, M*TAspa1*Rx90*SAspa1);

    glm::mat4 TAspa2 = glm::translate(I, glm::vec3(-0.55, 1, -0.3));
    glm::mat4 Ry90Neg = glm::rotate(I, glm::radians(-120.0f), glm::vec3(0, 1, 0));
    drawObject(triangle, glm::vec3(0.139, 0.069, 0.019), P, V, M*TAspa2*Ry90Neg*Rx90*SAspa1);

    glm::mat4 TAspa3 = glm::translate(I, glm::vec3(0.55, 1, -0.3));
    glm::mat4 Ry90Pos = glm::rotate(I, glm::radians(120.0f), glm::vec3(0, 1, 0));
    drawObject(triangle, glm::vec3(0.139, 0.069, 0.019), P, V, M*TAspa3*Ry90Pos*Rx90*SAspa1);

}

void drawMesa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Patas
    glm::mat4 T1 = glm::translate(I, glm::vec3(2, 0, 0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0, 0, 1));
    glm::mat4 T3 = glm::translate(I, glm::vec3(2, 0, 1));
    drawPata(P, V, M);
    drawPata(P, V, M*T1);
    drawPata(P, V, M*T2);
    drawPata(P, V, M*T3);

    // Tablero
    glm::mat4 STablero = glm::scale(I, glm::vec3(1.5, 0.1, 0.7));
    glm::mat4 TTablero = glm::translate(I, glm::vec3(0.95, 1.15, 0.5));
    drawObject(cube, glm::vec3(0.139, 0.069, 0.019), P, V, M*TTablero*STablero);

}

void drawPata(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.10, 0.6, 0.10));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0.6, 0));
    drawObject(cube, glm::vec3(0.139, 0.069, 0.019), P, V, M*T*S);

}

void drawObject(Model &model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
    shaders.setVec3("uColor",color);
    model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    shaders.setVec3("uColor",glm::vec3(1.0, 1.0, 1.0));
    model.renderModel(GL_LINE);

}

void funFramebufferSize(GLFWwindow* window, int width, int height) {

    // Configuracion del Viewport
    glViewport(0, 0, width, height);

    // Actualizacion de w y h
    w = width;
    h = height;

}

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_UP:    desY += 0.5f;   break;
        case GLFW_KEY_DOWN:  desY -= 0.5f;   break;
        case GLFW_KEY_LEFT:  desX -= 0.5f;   break;
        case GLFW_KEY_RIGHT: desX += 0.5f;   break;
        case GLFW_KEY_LEFT_SHIFT:    break;

        case GLFW_KEY_Z:
            if(mods==GLFW_MOD_SHIFT)  desZ += desZ <   7.0f ? 0.5f : 0.0f;
            else                     desZ -= desZ > -10.0f ? 0.5f : 0.0f;
            break;
        case GLFW_KEY_V:
            if(action==GLFW_PRESS)
                rotVentilador+=1;
            break;
        default:
            desY = 0.0f;
            desX = 0.0f;
            desZ = 0.0f;

            rotY = 0.0f;
            break;
    }

}

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

    if(yoffset>0) fovy -= fovy>10.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<90.0f ? 5.0f : 0.0f;

}
void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;
    //desY=0;
    float limY = 89.0;
    alphaX = 90.0*(2.0*xpos/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*ypos/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;

}

void controladorTiempo() {
    // Giro Helices
    if(glfwGetTime()>=0.01){
        RTiempo+=6;
        glfwSetTime(0);
    }
}
