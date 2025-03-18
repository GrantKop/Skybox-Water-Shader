#include "graphics/Texture.h"

Texture::Texture(std::vector<std::string> faces, GLenum texType, GLuint texSlot, GLenum format, GLenum pixelType, GLenum minMagFilter, GLenum wrapFilter) {
    
    type = texType;
    slot = texSlot;

    stbi_set_flip_vertically_on_load(false);

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(texType, ID);

    for (GLuint i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    
}

Texture::~Texture() {}

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(type, ID);
}

void Texture::unbind() {
    glBindTexture(type, 0);
}

void Texture::deleteTexture() {
    glDeleteTextures(1, &ID);
}

void Texture::setUniform(Shader shader, const char* name, GLuint unit) {
    texUniform = glGetUniformLocation(shader.ID, name);
    shader.use();
    glUniform1i(texUniform, unit);
}
