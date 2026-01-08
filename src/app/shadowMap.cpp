#include <shadowMap.h>

void ShadowMap::init(int resolution){
    this->resolution = resolution;
    glGenFramebuffers(1, &fbo);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Filtering & Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::update(glm::vec3 sunDir, Camera *playerCam) {
    // 1. Determine where the shadow box should focus
    // Shifting it forward slightly so shadows don't cut off right at the camera lens
    glm::vec3 focusPoint = playerCam->position;// + (playerCam->direction * 10.0f);

    // 2. Calculate Sun Position (Moving it back along the light ray)
    float distance = 40.0f; 
    this->position = focusPoint - (glm::normalize(sunDir) * distance);
    
    // 3. View Matrix
    glm::mat4 lightView = glm::lookAt(this->position, focusPoint, glm::vec3(0.0f, 1.0f, 0.0f));

    // 4. Projection Matrix (The Cuboid)
    float size = 20.0f; 
    float near_p = 1.0f;
    float far_p = 100.0f;
    glm::mat4 lightProjection = glm::ortho(-size, size, -size, size, near_p, far_p);

    this->matrix = lightProjection * lightView;
}

void ShadowMap::bind() {
    glViewport(0, 0, resolution, resolution);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}