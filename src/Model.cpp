#include "Model.h"

//-----------------------------------------------------------------------------------------------------
// Lee los atributos del modelo de un fichero de texto y los almacena en los vectores correspondientes
//-----------------------------------------------------------------------------------------------------
void Model::initModel(const char *modelFile) {
   
 // Importa el modelo mediante la librería Assimp
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(modelFile,  
        aiProcess_Triangulate |     
        aiProcess_JoinIdenticalVertices | 
        aiProcess_PreTransformVertices  |
        aiProcess_GenSmoothNormals | 
        aiProcess_CalcTangentSpace | 
        aiProcess_GenUVCoords);
    if(!scene) {
        std::cout << "El fichero " << modelFile << " no se puede abrir." << std::endl;
        std::cin.get();
        exit(1);
    }
  
 // Extrae los atributos (posiciones, normales, texturas, indices) del modelo importado
    aiNode *node = scene->mRootNode;
    aiMesh *mesh = scene->mMeshes[node->mMeshes[0]];
    for(int i=0; i<mesh->mNumVertices; i++) {
        positions.push_back(glm::vec3(mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z));
        normals.push_back(glm::normalize(glm::vec3(mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z)));
        if(mesh->mTextureCoords[0]) textureCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y));
        else                        textureCoords.push_back(glm::vec2(0.0f, 0.0f));
    }
    for(int i=0; i<mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(int j=0; j<face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

 // Crea un VAO con los atributos del modelo
    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vboPositions);
    glGenBuffers(1,&vboNormals);
    glGenBuffers(1,&vboTextureCoords);
    glGenBuffers(1,&eboIndices);
    glBindVertexArray(vao);
     // Posiciones
        glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*positions.size(), &(positions.front()), GL_STATIC_DRAW);  
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(0);
     // Normales   
        glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &(normals.front()), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(1);
     // Texturas
        glBindBuffer(GL_ARRAY_BUFFER, vboTextureCoords);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*textureCoords.size(), &(textureCoords.front()), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(2);
     // Índices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices.size(), &(indices.front()), GL_STATIC_DRAW);
    glBindVertexArray(0);
    
}

//--------------------------------
// Renderiza el VAO con el modelo
//--------------------------------
void Model::renderModel(unsigned long mode) {
    
    glPolygonMode(GL_FRONT_AND_BACK, mode);
    glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void *)0);
    glBindVertexArray(0);
    
}

//-----------------------------------
// Destructor de la clasede la clase 
//-----------------------------------
Model::~Model() {
  
    glDeleteBuffers(1,&vao);
    glDeleteBuffers(1,&vboPositions);
    glDeleteBuffers(1,&vboNormals);
    glDeleteBuffers(1,&vboTextureCoords);
    glDeleteBuffers(1,&eboIndices);
    
}
