#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/VertexArrayObject.h"
#include "input/DetectInput.h"
#include "graphics/Texture.h"

std::vector<Vertex> skyboxVerts = {
    {Vertex{glm::vec3(-50.0f, -50.0f,  50.0f)}},
    {Vertex{glm::vec3( 50.0f, -50.0f,  50.0f)}},
    {Vertex{glm::vec3( 50.0f,  50.0f,  50.0f)}},
    {Vertex{glm::vec3(-50.0f,  50.0f,  50.0f)}},

    {Vertex{glm::vec3(-50.0f, -50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f, -50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f,  50.0f, -50.0f)}},
    {Vertex{glm::vec3(-50.0f,  50.0f, -50.0f)}},

    {Vertex{glm::vec3(-50.0f,  50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f,  50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f,  50.0f,  50.0f)}},
    {Vertex{glm::vec3(-50.0f,  50.0f,  50.0f)}},

    {Vertex{glm::vec3(-50.0f, -50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f, -50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f, -50.0f,  50.0f)}},
    {Vertex{glm::vec3(-50.0f, -50.0f,  50.0f)}},

    {Vertex{glm::vec3( 50.0f, -50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f,  50.0f, -50.0f)}},
    {Vertex{glm::vec3( 50.0f,  50.0f,  50.0f)}},
    {Vertex{glm::vec3( 50.0f, -50.0f,  50.0f)}},

    {Vertex{glm::vec3(-50.0f, -50.0f, -50.0f)}},
    {Vertex{glm::vec3(-50.0f,  50.0f, -50.0f)}},
    {Vertex{glm::vec3(-50.0f,  50.0f,  50.0f)}},
    {Vertex{glm::vec3(-50.0f, -50.0f,  50.0f)}}
};

std::vector<GLuint> skyboxIndices = {
    2, 1, 0, 0, 3, 2,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    14, 13, 12, 12, 15, 14,
    18, 17, 16, 16, 19, 18,
    20, 21, 22, 22, 23, 20
};

std::vector<std::string> skyboxFaces = {
    "../../assets/skybox/posx.jpg",
    "../../assets/skybox/negx.jpg",
    "../../assets/skybox/posy.jpg",
    "../../assets/skybox/negy.jpg",
    "../../assets/skybox/posz.jpg",
    "../../assets/skybox/negz.jpg" 
};

const int gridSize = 100;

std::vector<Vertex> waterVertices;
std::vector<GLuint> waterIndices;

void createWaterObject() {
    for (int x = 0; x < gridSize; x++) {
        for (int z = 0; z < gridSize; z++) {
            float xPos = -50.0f + (x / float(gridSize - 1)) * 100.0f;
            float zPos = -50.0f + (z / float(gridSize - 1)) * 100.0f;

            waterVertices.push_back({
                glm::vec3(xPos, 1.0f, zPos),
                glm::vec3(0.0f, 1.0f, 0.0f),
                glm::vec2(x / float(gridSize - 1), z / float(gridSize - 1))
            });

            if (x < gridSize - 1 && z < gridSize - 1) {
                int topLeft = x * gridSize + z;
                int topRight = (x + 1) * gridSize + z;
                int bottomLeft = x * gridSize + (z + 1);
                int bottomRight = (x + 1) * gridSize + (z + 1);

                waterIndices.push_back(topLeft);
                waterIndices.push_back(bottomLeft);
                waterIndices.push_back(bottomRight);
                waterIndices.push_back(topLeft);
                waterIndices.push_back(bottomRight);
                waterIndices.push_back(topRight);
            }
        }
    }
}

GLuint screenFBO;
GLuint screenTexture;

void createFrameBufferObject() {
    glGenFramebuffers(1, &screenFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);

    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "ERROR: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


int main() {

    initGLFW(3, 3);

    GLFWwindow* window = nullptr;
    if (!createWindow(window, "Final Project CS457", 800, 600)) {
        glfwTerminate();
        return -1;
    }

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    createFrameBufferObject();
    createWaterObject();

    Shader skyboxShader("../../shaders/skybox.vert", "../../shaders/skybox.frag");
    Texture skybox(skyboxFaces, GL_TEXTURE_CUBE_MAP, 0, GL_RGB, GL_UNSIGNED_BYTE);
    skybox.setUniform(skyboxShader, "skybox", 0);

    VertexArrayObject skyboxVAO;
    skyboxVAO.bind();
    skyboxVAO.addVertexBuffer(skyboxVerts);
    skyboxVAO.addElementBuffer(skyboxIndices);
    skyboxVAO.addAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    VertexArrayObject waterVAO;
    waterVAO.bind();
    waterVAO.addVertexBuffer(waterVertices);
    waterVAO.addElementBuffer(waterIndices);
    waterVAO.addAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    waterVAO.addAttribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    waterVAO.addAttribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    Shader waterShader("../../shaders/water.vert", "../../shaders/water.frag");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    float deltaTime = 0.0f;	// Time between current frame and last frame
    float lastFrame = 0.0f; // Time of last frame
    float time = 0.0f;

    // Main render loop
    while (!glfwWindowShouldClose(window)) {

        time += 0.01f;

        deltaTime = glfwGetTime() - lastFrame;
        lastFrame += deltaTime;

        processInput(window, &camera, deltaTime);

        camera.updateCameraMatrix(0.1f, 1000.0f, window);

        // Screenspace rendering
        glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox rendering 1st pass
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        skyboxShader.use();
        skyboxShader.setMat4("cameraMatrix", camera.cameraMatrix);

        skybox.bind();
        skyboxVAO.bind();
        glDrawElements(GL_TRIANGLES, skyboxIndices.size(), GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Default render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox rendering 2nd pass
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        skyboxShader.use();
        skyboxShader.setMat4("cameraMatrix", camera.cameraMatrix);

        skybox.bind();
        skyboxVAO.bind();
        glDrawElements(GL_TRIANGLES, skyboxIndices.size(), GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        
        // Water rendering
        waterShader.use();
        waterShader.setFloat("time", time);
        waterShader.setVec3("viewPos", camera.position);
        waterShader.setMat4("cameraMatrix", camera.cameraMatrix);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        waterShader.setInt("screenTexture", 1);

        waterVAO.bind();
        glDrawElements(GL_TRIANGLES, waterIndices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    waterVAO.deleteBuffers();
    skyboxVAO.deleteBuffers();
    skyboxShader.deleteShader();
    waterShader.deleteShader();

    glfwTerminate();

    return 0;
}
