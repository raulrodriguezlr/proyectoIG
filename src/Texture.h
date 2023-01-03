#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <freeImage.h>

class Texture {
    
    public:
               
        void         initTexture(const char *textureFile); 
        unsigned int getTexture(); 

        virtual ~Texture();
           
    private:
       
        unsigned int  texture;
        
        unsigned char *loadTexture(const char *textureFile, unsigned int &w, unsigned int &h);
        
};

#endif /* TEXTURE_H */
