#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

void configScene();
void renderScene();
void setLights(glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawSuelo (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawParedes (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSofa (glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVentilador(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawMesa(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTele(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawPata(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCuerpoChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTroncos(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAlfombra(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBalda(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVela(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawMueble(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawVentana(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawMecedora(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void controladorTiempoVent();
void controladorTiempoMec();

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
Model vela;
Model flame;
Model mueble;
Model tele;
Model mecedora;


// Imagenes (texturas)
Texture suelo;Texture sueloNormal;Texture sueloSpecular;

Texture paredes;Texture paredesNormal;Texture paredesSpecular;
Texture paredesVert;Texture paredesVertNormal;Texture paredesVertSpecular;
Texture paredesVertAbajo;Texture paredesVertAbajoNormal;Texture paredesVertAbajoSpecular;

Texture noEmissive;

Texture chimenea;
Texture NormChimenea;
Texture SpeChimenea;
Texture troncoDifusa;Texture troncoNormal;Texture troncoSpecular;

Texture sofa;
Texture sofaNormal;
Texture sofaSpecular;

Texture mesa;
Texture mesaNormal;
Texture mesaSpecular;

Texture velaTex;
Texture Flame;
Texture FlameNormal;
Texture FlameSpecular;

Texture muebleTex;
Texture muebleNormal;
Texture muebleSpecular;

Texture teleDif;
Texture teleSpe;
Texture teleNorm;

Texture mecedoraTex;
Texture mecedoraNormal;
Texture mecedoraSpecular;


// Luces y materiales
#define   NLD 1
#define   NLP 3
#define   NLF 2
Light     lightG;
Light     lightD[NLD];
Light     lightP[NLP];
Light     lightF[NLF];
Material  mluz;
Material sol;
Material mventilador;
Material ruby;
Material perl;
Material goldShine;
Material obsidiane;
Material cristal;

Textures texSuelo;
Textures texParedes;
Textures texParedesVert;
Textures texParedesVertAbajo;
Textures texChimenea;
Textures texTroncos;
Textures texTele;

Textures texSofa;
Textures texMesa;
Textures texVela;
Textures texFlame;

Textures texMueble;

Textures texMecedora;


// Viewport
int w = 1000;
int h = 1000;

// Animaciones
float desY = 0.0;
float desX = 0.0;

float rotY = 0.0;
float desZ = 0.0;

//****Luminosidad****
float velas=0.9;
float velasAmb=0.2;
int   contadorVelas=0;
bool  velasOn=true;

//CHIMENEA
int   contadorChimenea=0;
float chimeneaDifusa = 1;
bool  chimeneaOn=true;
float chimeneaAmb = 0.5;
//Plafon VENTILADOR
int   contadorPlafon=0;
float plafonDifusa = 1;
bool  plafonOn=true;
float plafonAmb = 0.5;
//Television
int   contadorTele=0;
float teleDifusa = 1;
bool  teleOn=true;
float teleAmb = 0.1;

//Posicion de las velas
float xv1=0.0;float yv1=0.0;float zv1=0.0;


// Movimiento de camara
float fovy   = 60.0;
float alphaX =  0.0;
float alphaY =  0.0;

// Rotacion por tiempo
float RTiempo = 0;
float RTiempoMec = 0;
float numVecesMec = 0;
int   sumRest = 0;
int   rotVentilador = 0;


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

    // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

    // Modelos
    plane.initModel("resources/models/plane.obj");
    cube.initModel("resources/models/cube.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");
    triangle.initModel("resources/models/triangle.obj");
    tronco.initModel("resources/models/wooden.obj");
    vela.initModel("resources/models/vela_1.obj");
    flame.initModel("resources/models/flame.obj");
    mueble.initModel("resources/models/bookshelf.obj");
    tele.initModel("resources/models/TV.obj");
    mecedora.initModel("resources/models/mecedora.obj");

    // Imagenes (texturas)
    suelo.initTexture("resources/textures/sueloMasPequenno.jpg");
    sueloNormal.initTexture("resources/textures/sueloMasPequennoNormal.png");
    sueloSpecular.initTexture("resources/textures/sueloMasPequennoSpecular.png");


        //INICIO TEXTURAS PAREDES
    paredes.initTexture("resources/textures/newBrickWall.jpg");
    paredesNormal.initTexture("resources/textures/newBrickWallNormal.png");
    paredesSpecular.initTexture("resources/textures/newBrickWallSpecular.png");

    paredesVert.initTexture("resources/textures/newBrickWall200.jpg");
    paredesVertNormal.initTexture("resources/textures/newBrickWall200Normal.jpg");
    paredesVertSpecular.initTexture("resources/textures/newBrickWall200Specular.png");

    paredesVertAbajo.initTexture("resources/textures/newBrickWall400.jpg");
    paredesVertAbajoNormal.initTexture("resources/textures/newBrickWall400Normal.png");
    paredesVertAbajoNormal.initTexture("resources/textures/newBrickWall400Specular.png");

        //FINAL TEXTURAS PAREDES

    chimenea.initTexture("resources/textures/imgWallDiffuse.png");
    SpeChimenea.initTexture("resources/textures/imgWallSpecular.png");
    NormChimenea.initTexture("resources/textures/imgWallNormal.png");
    troncoDifusa.initTexture("resources/textures/WoodenDifusa.jpeg");
    troncoSpecular.initTexture("resources/textures/WoodenSpecular.jpeg");
    troncoNormal.initTexture("resources/textures/WoodenNormal.jpeg");


    noEmissive.initTexture("resources/textures/imgNoEmissive.png");

    sofa.initTexture("resources/textures/sofa.jpg");
    sofaNormal.initTexture("resources/textures/sofaNormal.png");
    sofaSpecular.initTexture("resources/textures/sofaSpecular.png");

    mesa.initTexture("resources/textures/maderaGris.jpg");
    mesaNormal.initTexture("resources/textures/mesaNormal.png");
    mesaSpecular.initTexture("resources/textures/mesaSpecular.png");

    velaTex.initTexture("resources/textures/vela.png");
    Flame.initTexture("resources/textures/llama.png");
    FlameNormal.initTexture("resources/textures/llamaNormal.png");
    FlameSpecular.initTexture("resources/textures/llamaSpecular.png");

    teleDif.initTexture("resources/textures/programa1.jpeg");
    teleSpe.initTexture("resources/textures/programa1Specular.png");
    teleNorm.initTexture("resources/textures/programa1Normal.png");

    muebleTex.initTexture("resources/textures/mueble.jpg");
    muebleNormal.initTexture("resources/textures/muebleNormal.png");
    muebleSpecular.initTexture("resources/textures/muebleSpecular.png");

    mecedoraTex.initTexture("resources/textures/mecedora.png");
    mecedoraNormal.initTexture("resources/textures/mecedoraNormal.png");
    mecedoraSpecular.initTexture("resources/textures/mecedoraSpecular.png");


    // Luz ambiental global
    lightG.ambient = glm::vec3(0.2, 0.2, 0.2);

    // ****Luces direccionales****
    lightD[0].direction = glm::vec3(0.0, 0.0, 1.0);
    lightD[0].ambient   = glm::vec3( 0.0, 0.0, 0.0);
    lightD[0].diffuse   = glm::vec3( 0.0, 0.0, 0.0);
    lightD[0].specular  = glm::vec3( 0.0, 0.0, 0.0);




    //***** Luces posicionales******

    //CAMBIAR Y AÑADIR LUCES Posicionales, las Posicionales son las que  pierden intensidad a lo largo del recorrido por el resto igual que focales
    lightP[0].position    = glm::vec3(10.0, 3.0, 10.0);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;



    //***** Luces focales*****

    //CAMBIAR Y AÑADIR LUCES FOCALES, las focales son las que no pierden intensidad a lo largo del recorrido por el resto igual que posicionales

    // *****Materiales*****
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    sol.ambient   = glm::vec4( 0.329412, 0.223529, 0.027451,1.0);
    sol.diffuse   = glm::vec4( 0.780392, 0.568627, 0.113725, 1.0);
    sol.specular  = glm::vec4(0.992157, 0.941176, 0.807843, 1.0);
    sol.emissive  = glm::vec4(1,1,0,1);
    sol.shininess = 27.897;

    mventilador.ambient   = glm::vec4( 0.05f,0.05f,0.05f,1.0f);
    mventilador.diffuse   = glm::vec4( 0.5f,0.5f,0.5f,1.0f);
    mventilador.specular  = glm::vec4(0.7f,0.7f,0.7f,1.0f);
    mventilador.shininess = 10.0f;

    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 1.0);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 1.0);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 1.0);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.0);
    ruby.shininess = 76.8;

    perl.ambient   = glm::vec4(0.25f, 0.20725f, 0.20725f, 0.922f );
    perl.diffuse   = glm::vec4(1.0f, 0.829f, 0.829f, 0.922f);
    perl.specular  = glm::vec4(0.296648f, 0.296648f, 0.296648f, 0.922f);
    perl.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    perl.shininess = 11.264f;

    goldShine.ambient   = glm::vec4( 0.24725f, 0.1995f, 0.0745f, 1.0f);
    goldShine.diffuse   = glm::vec4(0.65164f, 0.30648f, 0.12648f, 1.0f);
    goldShine.specular  = glm::vec4(0.628281f, 0.555802f, 0.366065f, 1.0f);
    goldShine.emissive  = glm::vec4(1,0.62,0,1);
    goldShine.shininess = 51.2f;

    obsidiane.ambient   = glm::vec4( 0.05375f, 0.05f, 0.06625f, 0.82f);
    obsidiane.diffuse   = glm::vec4(0.18275f, 0.17f, 0.22525f, 0.82f);
    obsidiane.specular  = glm::vec4(0.332741f, 0.328634f, 0.346435f, 0.82f );
    obsidiane.emissive  = glm::vec4(0,0,0,0);
    obsidiane.shininess = 38.4f;

    cristal.ambient   = glm::vec4(0.25f, 0.20725f, 0.20725f, 0.5f );
    cristal.diffuse   = glm::vec4(1.0f, 0.829f, 0.829f, 0.5f);
    cristal.specular  = glm::vec4(0.296648f, 0.296648f, 0.296648f, 0.5f);
    cristal.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    cristal.shininess = 10.0f;


    texSuelo.diffuse   = suelo.getTexture();
    texSuelo.specular  = sueloSpecular.getTexture();
    texSuelo.emissive  = noEmissive.getTexture();
    texSuelo.normal    = sueloNormal.getTexture();
    texSuelo.shininess = 10.0;

    texParedes.diffuse   = paredes.getTexture();
    texParedes.specular  = paredesSpecular.getTexture();
    texParedes.emissive  = noEmissive.getTexture();
    texParedes.normal    = paredesNormal.getTexture();
    texParedes.shininess = 10.0;

    texParedesVert.diffuse   = paredesVert.getTexture();
    texParedesVert.specular  = paredesVertSpecular.getTexture();
    texParedesVert.emissive  = noEmissive.getTexture();
    texParedesVert.normal    = paredesVertNormal.getTexture();
    texParedesVert.shininess = 10.0;

    texParedesVertAbajo.diffuse  = paredesVertAbajo.getTexture();
    texParedesVertAbajo.specular  = paredesVertAbajoSpecular.getTexture();
    texParedesVertAbajo.emissive  = noEmissive.getTexture();
    texParedesVertAbajo.normal    = paredesVertAbajoNormal.getTexture();
    texParedesVertAbajo.shininess = 10.0;


    texChimenea.diffuse  = chimenea.getTexture();
    texChimenea.specular  = SpeChimenea.getTexture();
    texChimenea.emissive  = noEmissive.getTexture();
    texChimenea.normal    = NormChimenea.getTexture();
    texChimenea.shininess = 10.0;

    texTroncos.diffuse  = troncoDifusa.getTexture();
    texTroncos.specular  = troncoSpecular.getTexture();
    texTroncos.emissive  = noEmissive.getTexture();
    texTroncos.normal    = troncoNormal.getTexture();
    texTroncos.shininess = 10.0;

    texSofa.diffuse  = sofa.getTexture();
    texSofa.specular  = sofaSpecular.getTexture();
    texSofa.emissive  = noEmissive.getTexture();
    texSofa.normal    = sofaNormal.getTexture();
    texSofa.shininess = 10.0;

    texMesa.diffuse  = mesa.getTexture();
    texMesa.specular  = mesaSpecular.getTexture();
    texMesa.emissive  = noEmissive.getTexture();
    texMesa.normal    = mesaNormal.getTexture();
    texMesa.shininess = 10.0;

    texVela.diffuse  = velaTex.getTexture();
    texVela.specular  = velaTex.getTexture();
    texVela.emissive  = velaTex.getTexture();
    texVela.normal    = velaTex.getTexture();
    texVela.shininess = 10.0;

    texTele.diffuse  = teleDif.getTexture();
    texTele.specular  = teleSpe.getTexture();
    texTele.emissive  = noEmissive.getTexture();
    texTele.normal  = teleNorm.getTexture();
    texTele.shininess  = 10;

    texMueble.diffuse  = muebleTex.getTexture();
    texMueble.specular  = muebleSpecular.getTexture();
    texMueble.emissive  = noEmissive.getTexture();
    texMueble.normal    = muebleNormal.getTexture();
    texMueble.shininess = 10.0;

    texFlame.diffuse=Flame.getTexture();
    texFlame.specular=FlameSpecular.getTexture();
    texFlame.emissive=noEmissive.getTexture();
    texFlame.normal=FlameNormal.getTexture();
    texFlame.shininess = 10.0;

    texMecedora.diffuse = mecedoraTex.getTexture();
    texMecedora.specular = mecedoraSpecular.getTexture();
    texMecedora.emissive = noEmissive.getTexture();
    texMecedora.normal = mecedoraNormal.getTexture();
    texMecedora.shininess = 10.0;


}

void renderScene() {

    //Algunas luces tenemos que meterlas dentro del bucle en render Scene porque se encienden y se apagan t0do el rato
    //Vela 1
    lightP[1].position    = glm::vec3(xv1, yv1, zv1);
    lightP[1].ambient     = glm::vec3(velasAmb, velasAmb, velasAmb);
    lightP[1].diffuse     = glm::vec3(velas, velas, velas);
    lightP[1].specular    = glm::vec3(velas, velas, velas);
    lightP[1].c0          = 1.00;
    lightP[1].c1          = 0.6;
    lightP[1].c2          = 0.3;
    //Chimenea
    lightP[2].position    = glm::vec3(0, 0.5, -9.2);
    lightP[2].ambient     = glm::vec3(chimeneaAmb, chimeneaAmb, chimeneaAmb);
    lightP[2].diffuse     = glm::vec3(chimeneaDifusa, chimeneaDifusa, chimeneaDifusa);
    lightP[2].specular    = glm::vec3(chimeneaDifusa, chimeneaDifusa, chimeneaDifusa);
    lightP[2].c0          = 1.00;
    lightP[2].c1          = 0.8;
    lightP[2].c2          = 0.45;

    //LUZ DEL VENTILADOR
    lightF[0].direction   = glm::vec3( 0.0, -9, 0.0);
    lightF[0].position    = glm::vec3(0,  9,  0.0);
    lightF[0].ambient     = glm::vec3( plafonAmb,  plafonAmb,  plafonAmb);
    lightF[0].diffuse     = glm::vec3( plafonDifusa,  plafonDifusa,  plafonDifusa);
    lightF[0].specular    = glm::vec3( plafonDifusa,  plafonDifusa,  plafonDifusa);
    lightF[0].innerCutOff = 5.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 25.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.20;
    lightF[0].c2          = 0.1;
    //Con la siguiente linea le damos una ligera iluminacion al material del plafon cuando esta encendida
    mventilador.emissive  = glm::vec4(plafonDifusa-plafonAmb/2,plafonDifusa-plafonAmb/2,plafonDifusa-plafonAmb/2,plafonDifusa-plafonAmb/2);
    //LUZ TELE
    lightF[1].position    = glm::vec3(-9.2, 2.2, -3.5);
    lightF[1].direction   = glm::vec3( 9.2, 1, 0.0);
    lightF[1].ambient     = glm::vec3( teleAmb, teleAmb,teleAmb);
    lightF[1].diffuse     = glm::vec3( teleDifusa,  teleDifusa,  teleDifusa);
    lightF[1].specular    = glm::vec3( teleDifusa-3*teleAmb,  teleDifusa-3*teleAmb,  teleDifusa-3*teleAmb);
    lightF[1].innerCutOff = 21.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 25.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.20;
    lightF[1].c2          = 0.1;


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
    float x = 7.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 7.0f*glm::sin(glm::radians(alphaY));
    float z = 7.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x+desX,   y+desY,   z+desZ);
    glm::vec3 center(0.0+desX, 4.0+desY,  0.0+desZ);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

    // Fijamos las luces
    setLights(P, V);

    // Dibujamos el suelo
    drawSuelo(P, V, I);

    // Dibujamos las paredes y el techo
    drawParedes(P, V, I);

    // Dibujamos sofá
    glm::mat4 SSofa = glm::scale(I, glm::vec3(1.25, 1, 1));
    glm::mat4 TSofa = glm::translate(I, glm::vec3(-3, 0, -3.5));
    glm::mat4 Ry270 = glm::rotate(I, glm::radians(270.0f), glm::vec3(0, 1, 0));

    drawSofa(P, V, TSofa*SSofa*Ry270);

    // Dibujamos ventilador
    glm::mat4 SVentilador = glm::scale(I, glm::vec3(2.5, 1.4, 2.5));
    glm::mat4 TVentilador = glm::translate(I, glm::vec3(0, 8, 0));
    drawVentilador(P, V, TVentilador*SVentilador);
    if(rotVentilador%2 == 0) controladorTiempoVent();

    // Dibujamos chimenea
    glm::mat4 TChimenea = glm::translate(I, glm::vec3(0, 0, -9));
    drawChimenea(P, V, TChimenea);

    //Dibujamos balda
    glm::mat4 TBalda = glm::translate(I, glm::vec3(8, 5, -10));
    drawBalda(P,V,TBalda);

    // Dibujamos mesa
    glm::mat4 TMesa = glm::translate(I, glm::vec3(-8.6, 0, -4.5));
    drawMesa(P, V, TMesa*Ry270);

    //Dibujamos Tele
    glm::mat4 TTele = glm::translate(I, glm::vec3(-9.2, 1.3, -3.5));

    drawTele(P,V,TTele*Ry270);

    // Dibujamos alfombra
    glm::mat4 Ry90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
    drawAlfombra(P,V,TMesa*Ry90);

    // Dibujamos velas
    glm::mat4 TVela = glm::translate(I, glm::vec3(8, 5.1, -9.8));
    drawVela(P,V,I*TVela);
    xv1=8;yv1=5.5;zv1=-9.8;

    // Dibujamos mueble con libros
    glm::mat4 TMueble = glm::translate(I, glm::vec3(9.1, 0, 0));
    drawMueble(P, V, TMueble);

    // Dibujamos ventana
    glm::mat4 SVentana = glm::scale(I, glm::vec3(1.5, 1.5, 1));
    glm::mat4 TVentana = glm::translate(I, glm::vec3(-10, 2.5, 3));
    drawVentana(P, V, TVentana*Ry90*SVentana);

    // Dibujamos mecedora
    glm::mat4 Ry200 = glm::rotate(I, glm::radians(200.0f), glm::vec3(0, 1, 0));
    glm::mat4 TMecedora = glm::translate(I, glm::vec3(7, 0, -5));
    drawMecedora(P, V, TMecedora*Ry200);
    if(rotVentilador%2 != 0) controladorTiempoMec();

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);
//DIBUJA LAS ESFERITAS
/*
    for(int i=0; i<NLD; i++) {
        glm::mat4 M = glm::translate(I,lightD[i].position) * glm::scale(I,glm::vec3(4));
        drawObjectMat(sphere, mluz, P, V, M);
    }
*/
    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.021));
        drawObjectMat(sphere, mluz, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectMat(sphere, mluz, P, V, M);
        }

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(10.0, 1.0, 10.0));
    drawObjectTex(plane, texSuelo, P, V, M*S);

}

void drawParedes(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    //Pared izq ventana
    glm::mat4 S3 = glm::scale(I, glm::vec3(5.0, 1.0, 3.5));
    glm::mat4 Rz90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-10, 5, 6.5));
    drawObjectTex(plane, texParedes, P, V, M*T1*Rz90*S3);

    //Pared dcha ventana
    glm::mat4 S4 = glm::scale(I, glm::vec3(5.0, 1.0, 4.8));
    glm::mat4 T5 = glm::translate(I, glm::vec3(-10, 5, -5.2));
    drawObjectTex(plane, texParedes, P, V, M*T5*Rz90*S4);

    //Pared arriba ventana
    glm::mat4 S5 = glm::scale(I, glm::vec3(2.2, 1.0, 1.8));
    glm::mat4 T6 = glm::translate(I, glm::vec3(-10, 7.8, 1.2));
    drawObjectTex(plane, texParedesVert, P, V, M*T6*Rz90*S5);
    //Pared abajo ventana
    glm::mat4 S6 = glm::scale(I, glm::vec3(1.1, 1.0, 1.8));
    glm::mat4 T7 = glm::translate(I, glm::vec3(-10, 1.1, 1.2));
    drawObjectTex(plane, texParedesVert, P, V, M*T7*Rz90*S6);

    glm::mat4 S2 = glm::scale(I, glm::vec3(5.0, 8.0, 10.0));
    glm::mat4 Rx90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0, 5, -10));
    drawObjectTex(plane, texParedes, P, V, M*T2*Rz90*Rx90*S2);

    glm::mat4 S1 = glm::scale(I, glm::vec3(5.0, 1.0, 10.0));
    glm::mat4 T3 = glm::translate(I, glm::vec3(10, 5, 0));
    drawObjectTex(plane, texParedes, P, V, M*T3*Rz90*S1);

    // Techo
    glm::mat4 S = glm::scale(I, glm::vec3(10.0, 1.0, 10.0));
    glm::mat4 T4 = glm::translate(I, glm::vec3(0, 10, 0));
    drawObjectTex(plane,texSuelo, P, V, M*T4*S);

}
void drawChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //Conducto
    glm::mat4 T1 = glm::translate(I, glm::vec3(0, 6.75, -0.65));
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.5, 3.25, 0.35));
    glm::mat4 R1 =glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    drawObjectTex(cube,texChimenea, P, V, M*T1*S1*R1);

    //Cuerpo
    glm::mat4 T2 = glm::translate(I, glm::vec3(0, 3.25, -0.5));
    drawCuerpoChimenea(P,V,M*T2);
}
void drawCuerpoChimenea(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //PARTE DE ARRIBA
    glm::mat4 S = glm::scale(I, glm::vec3(1.75, 0.25, 0.6));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0, 0.1));

    drawObjectTex(cube,texChimenea, P, V, M*T*S);

    //PARTE DE ABAJO
    glm::mat4 T1 = glm::translate(I, glm::vec3(0, -3, 0.5));
    glm::mat4 S1 = glm::scale(I, glm::vec3(2.5, 0.25, 1));
    drawObjectTex(cube,texChimenea, P, V, M*T1*S1);

    //PARTE IZQUIERDA
    glm::mat4 T2 = glm::translate(I, glm::vec3(-1.55, -1.25, 0.1));
    glm::mat4 T3 = glm::translate(I, glm::vec3(1.55, -1.25, 0.1));
    glm::mat4 S2 = glm::scale(I, glm::vec3(0.2, 1.5, 0.6));

    drawObjectTex(cube,texChimenea, P, V, M*T2*S2);
    drawObjectTex(cube, texChimenea, P, V, M*T3*S2);

    //PARTE TRASERA
    glm::mat4 T4 = glm::translate(I, glm::vec3(0, -1.75, -0.45));
    glm::mat4 S4 = glm::scale(I, glm::vec3(1.75, 1.5, 0.05));
    drawObjectTex(cube, texChimenea, P, V, M*T4*S4);
    drawTroncos(P,V,M*T1*S1);

}
void drawTroncos(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 T1 = glm::translate(I, glm::vec3(0.25, 1.5, 0));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0.25, 1.5, -0.5));
    glm::mat4 T3 = glm::translate(I, glm::vec3(0.25, 2.4, -0.25));

    glm::mat4 S = glm::scale(I, glm::vec3(2, 0.5, 1));
    glm::mat4 R1 =glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));

    //DIBUJAMOS LA LEÑA
    drawObjectTex(tronco, texTroncos, P, V, M*T1*R1*S);
    drawObjectTex(tronco,texTroncos, P, V, M*T2*R1*S);
    drawObjectTex(tronco, texTroncos, P, V, M*T3*R1*S);

    //DIBUJAMOS LA LLAMA
    glm::mat4 Tll = glm::translate(I, glm::vec3(0, 2.4, -0.25));

    glm::mat4 SLL = glm::scale(I, glm::vec3(0.5, 0.8, 0.5));
    if(chimeneaOn)
        drawObjectMat(flame, goldShine, P, V, M*Tll*SLL);




}
void drawAlfombra(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 T = glm::translate(I, glm::vec3(-1, 0, 2));
    glm::mat4 S = glm::scale(I, glm::vec3(1.75*1.25, 0.01, 1*1.25));
    drawObjectMat(sphere, ruby, P, V, M*T*S);

}
void drawBalda(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2, 0.15, 0.6));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0, 0.6));
    drawObjectMat(cube, perl, P, V, M*T*S);
    glm::mat4 S1 = glm::scale(I, glm::vec3(3, 0.15, 0.6));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-1, -2, 0.6));
    drawObjectMat(cube, perl, P, V, M*T1*S1);

}
void drawSofa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Asiento
    glm::mat4 S = glm::scale(I, glm::vec3(1.5, 0.25, 1.0));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0.75, 0));
    drawObjectTex(cube, texSofa, P, V, M*T*S);

    // Reposabrazos
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.25, 0.75, 0.75));
    glm::mat4 T1 = glm::translate(I, glm::vec3(-1.25, 0.75, 0.25));
    drawObjectTex(cube, texSofa, P, V, M*T1*S1);

    glm::mat4 T2 = glm::translate(I, glm::vec3(1.25, 0.75, 0.25));
    drawObjectTex(cube, texSofa, P, V, M*T2*S1);

    // Respaldo
    glm::mat4 Rx90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 T3 = glm::translate(I, glm::vec3(0, 1, -0.75));
    drawObjectTex(cube, texSofa, P, V, M*T3*Rx90*S);

    // Parte delantera
    glm::mat4 S2 = glm::scale(I, glm::vec3(1, 0.25, 0.25));
    glm::mat4 T4 = glm::translate(I, glm::vec3(0, 0.25, 0.75));
    drawObjectTex(cube, texSofa, P, V, M*T4*S2);

}

void drawVentilador(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Cuerpo (Cilindro)
    glm::mat4 SCuerpo = glm::scale(I, glm::vec3(0.25, 0.12, 0.25));
    glm::mat4 TCuerpo = glm::translate(I, glm::vec3(0, 1, 0));
    drawObjectMat(cylinder, ruby, P, V, M*TCuerpo*SCuerpo);

    // Plafón (Esfera)
    glm::mat4 SPlafon = glm::scale(I, glm::vec3(0.25, 0.12, 0.25));
    glm::mat4 TPlafon = glm::translate(I, glm::vec3(0, 0.85, 0));
    drawObjectMat(sphere, mventilador, P, V, M*TPlafon*SPlafon);

    // Sorporte Techo (Cilindro)
    glm::mat4 SSoporte = glm::scale(I, glm::vec3(0.05, 0.25, 0.05));
    glm::mat4 TSoporte = glm::translate(I, glm::vec3(0, 1.2, 0));
    drawObjectMat(cylinder, ruby, P, V, M*TSoporte*SSoporte);

    // Aspas (Triangulo)
    glm::mat4 RotTiempo = glm::rotate(I, glm::radians(RTiempo), glm::vec3(0, 1, 0));
    drawAspas(P, V, M*RotTiempo);

}

void drawAspas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    // Aspas (Triangulo)
    glm::mat4 SAspa1 = glm::scale(I, glm::vec3(0.15, 0.5, 1));
    glm::mat4 TAspa1 = glm::translate(I, glm::vec3(0, 1, 0.65));
    glm::mat4 Rx90 = glm::rotate(I, glm::radians(-90.0f), glm::vec3(1, 0, 0));
    drawObjectMat(triangle, ruby, P, V, M*TAspa1*Rx90*SAspa1);

    glm::mat4 TAspa2 = glm::translate(I, glm::vec3(-0.55, 1, -0.3));
    glm::mat4 Ry90Neg = glm::rotate(I, glm::radians(-120.0f), glm::vec3(0, 1, 0));
    drawObjectMat(triangle, ruby, P, V, M*TAspa2*Ry90Neg*Rx90*SAspa1);

    glm::mat4 TAspa3 = glm::translate(I, glm::vec3(0.55, 1, -0.3));
    glm::mat4 Ry90Pos = glm::rotate(I, glm::radians(120.0f), glm::vec3(0, 1, 0));
    drawObjectMat(triangle, ruby, P, V, M*TAspa3*Ry90Pos*Rx90*SAspa1);

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
    drawObjectTex(cube, texMesa, P, V, M*TTablero*STablero);

}
void drawTele(glm::mat4 P, glm::mat4 V, glm::mat4 M) {


    glm::mat4 Ry =glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
    glm::mat4 Rx =glm::rotate(I, glm::radians(180.0f), glm::vec3(1, 0, 0));
    glm::mat4 Rz =glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 0, 1));


    glm::mat4 R90x =glm::rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));

    glm::mat4 S = glm::scale(I, glm::vec3(1.5, 1.5, 1.5));

    drawObjectMat(tele,obsidiane , P, V, M*S*Ry);

    glm::mat4 T = glm::translate(I, glm::vec3(0, 1, -0.8));
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.75, 1, 0.53));
    if(teleOn)
        drawObjectTex(plane,texTele,P,V,M*T*R90x*S1);
    //    drawObjectTex(plane,texTele,P,V,M*Rz*T*R90x*S1);

}


void drawPata(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.10, 0.6, 0.10));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0.6, 0));
    drawObjectTex(cube, texMesa, P, V, M*T*S);

}

void drawVela(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2, 2, 2));
    drawObjectMat(vela, ruby, P, V, M*S);
    glm::mat4 S1 = glm::scale(I, glm::vec3(0.09, 0.09, 0.09));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 0.2, 0));
    if(velasOn== true)
        drawObjectMat(flame, goldShine, P, V, M*T*S1);


}

void drawMueble(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 0.5, 0.5));
    glm::mat4 Ry =glm::rotate(I, glm::radians(180.0f), glm::vec3(0, 1, 0));
    glm::mat4 T = glm::translate(I, glm::vec3(0, 1.3, 0));
    drawObjectTex(mueble, texMueble, P, V, M*T*Ry*S);

}

void drawVentana(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.1, 1.2, 0.1));
    glm::mat4 T1 = glm::translate(I, glm::vec3(0, 1, 0));
    drawObjectMat(cube, perl, P, V, M*T1*S);

    glm::mat4 Rz =glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    glm::mat4 T2 = glm::translate(I, glm::vec3(1.1, 2.1, 0));
    drawObjectMat(cube, perl, P, V, M*T2*Rz*S);

    glm::mat4 T3 = glm::translate(I, glm::vec3(1.1, -0.1, 0));
    drawObjectMat(cube, perl, P, V, M*T3*Rz*S);

    glm::mat4 T4 = glm::translate(I, glm::vec3(2.2, 1, 0));
    drawObjectMat(cube, perl, P, V, M*T4*S);

    // Cristal
    glm::mat4 SCristal = glm::scale(I, glm::vec3(1, 1, 0.02));
    glm::mat4 TCristal = glm::translate(I, glm::vec3(1.1, 1, 0));
    glDepthMask(GL_FALSE);
        drawObjectMat(cube, cristal, P, V, M*TCristal*SCristal);
    glDepthMask(GL_TRUE);

}

void drawMecedora(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(3.5, 3.5, 3.5));
    glm::mat4 RotTiempo = glm::rotate(I, glm::radians(RTiempoMec), glm::vec3(0, 0, 1));
    drawObjectTex(mecedora, texMecedora, P, V, M*RotTiempo*S);

}

void drawObjectMat(Model &model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model &model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    model.renderModel(GL_FILL);

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
        case GLFW_KEY_UP:    desY += desY <   7.0f ? 0.5f : 0.0f;   break;//FLECHA ARRIBA MUEVE LA CAMARA Y EL CENTER 0.5 EN EL EJE POSITIVO DE LAS Y
        case GLFW_KEY_DOWN:  desY -= desY >   -7.0f ? 0.5f : 0.0f;   break;//FLECHA ABAJO MUEVE LA CAMARA Y EL CENTER 0.5 EN EL EJE NEGATIVO DE LAS Y
        case GLFW_KEY_LEFT:  desX -= desX >  -7.0f ? 0.5f : 0.0f;   break;//FLECHA IZQUIERDA MUEVE LA CAMARA Y EL CENTER 0.5 EN EL EJE NEGATIVO DE LAS X
        case GLFW_KEY_RIGHT: desX += desX <   7.0f ? 0.5f : 0.0f;   break;//FLECHA DERECHA MUEVE LA CAMARA Y EL CENTER 0.5 EN EL EJE POSITIVO DE LAS X
        case GLFW_KEY_LEFT_SHIFT:    break;
        case GLFW_KEY_SPACE://SI PULSAMOS EL ESPACIO ENCENDEMOS Y APAGAMOS LAS VELAS
            if(action==GLFW_PRESS) {
                contadorVelas+=1;
                if (contadorVelas % 2 == 1) {
                    velas = 0;
                    velasOn=false;
                    velasAmb = 0;
                }else{velas = 0.9;
                    velasOn=true;
                    velasAmb = 0.2;}

            }
            break;
        case GLFW_KEY_C://SI PULSAMOS C ENCENDEMOS Y APAGAMOS LA CHIMENEA

            if(action==GLFW_PRESS) {
                contadorChimenea+=1;
                if (contadorChimenea % 2 == 1) {
                    chimeneaDifusa = 0;
                    chimeneaOn=false;
                    chimeneaAmb = 0;
                }else{chimeneaDifusa = 1;
                    chimeneaOn=true;
                    chimeneaAmb = 0.5;}

            }
            break;
        case GLFW_KEY_T://SI PULSAMOS T ENCENDEMOS Y APAGAMOS LA Tele

            if(action==GLFW_PRESS) {
                contadorTele+=1;
                if (contadorTele % 2 == 1) {
                    teleDifusa = 0;
                    teleOn=false;
                    teleAmb = 0;
                }else{teleDifusa = 1;
                    teleOn=true;
                    teleAmb = 0.1;}

            }
            break;


        case GLFW_KEY_Z://PULSAR Z +SHIFT IZQUIERDO  MUEVE LA CAMARA Y EL CENTER 0.5 EN EL EJE POSITIVO DE LAS Z, SIN SHIFT EN EL NEGATIVO
            if(mods==GLFW_MOD_SHIFT)  desZ += desZ <   5.0f ? 0.5f : 0.0f;
            else                     desZ -= desZ > -7.0f ? 0.5f : 0.0f;
            break;
        case GLFW_KEY_V://PULSAR V ENCIENDE Y APAGA EL VENTILADOR
            if(mods==GLFW_MOD_SHIFT){
                if(action==GLFW_PRESS) {
                    contadorPlafon+=1;
                    if (contadorPlafon % 2 == 1) {
                        plafonDifusa = 0;
                        plafonOn=false;
                        plafonAmb = 0;
                    }else{plafonDifusa = 1;
                        plafonOn=true;
                        plafonAmb = 0.5;}

                }

            }else
                if(action==GLFW_PRESS)
                    rotVentilador+=1;
            break;
        default: //CUALQUIER OTRO RESETEA EL EYE Y CENTER
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

void controladorTiempoVent() {
    // Giro Helices
    if(glfwGetTime()>=0.01){
        RTiempo+=6;
        if(sumRest == 0) {
            RTiempoMec += 0.1;
            numVecesMec += 1;
            if(numVecesMec > 100) {
                numVecesMec = 0;
                sumRest = 1;
            }
        }
        else if(sumRest == 1) {
            RTiempoMec -= 0.1;
            numVecesMec += 1;
            if(numVecesMec > 100) {
                numVecesMec = 0;
                sumRest = 0;
            }
        }
        glfwSetTime(0);
    }
}

void controladorTiempoMec() {
    if(glfwGetTime()>=0.01){
        if(sumRest == 0) {
            RTiempoMec += 0.1;
            numVecesMec += 1;
            if(numVecesMec > 100) {
                numVecesMec = 0;
                sumRest = 1;
            }
        }
        else if(sumRest == 1) {
            RTiempoMec -= 0.1;
            numVecesMec += 1;
            if(numVecesMec > 100) {
                numVecesMec = 0;
                sumRest = 0;
            }
        }
        glfwSetTime(0);
    }
}
