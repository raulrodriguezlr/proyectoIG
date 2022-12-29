#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define I glm::mat4(1.0)

class Model {
    
    public:
                        
        void initModel  (const char *modelFile);
        void renderModel(unsigned long mode);        
               
        virtual ~Model();
               
    private:
        
        std::vector<glm::vec3>      positions;
        std::vector<glm::vec3>      normals;
        std::vector<glm::vec2>      textureCoords;
        std::vector<unsigned short> indices;
    
        unsigned int vao;
        unsigned int vboPositions;
        unsigned int vboNormals;
        unsigned int vboTextureCoords;
        unsigned int eboIndices;

};

#endif /* MODEL_H */
