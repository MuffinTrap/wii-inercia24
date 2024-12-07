#include "matcap.h"

// Matrix-vector multiplication (4x4 matrix with vec3, assuming homogeneous coordinates w=1)
glm::vec3 mat4_mul_vec3(const float mat[16], glm::vec3 v) {
    glm::vec3 result;
    result.x = mat[0] * v.x + mat[4] * v.y + mat[8]  * v.z + mat[12];
    result.y = mat[1] * v.x + mat[5] * v.y + mat[9]  * v.z + mat[13];
    result.z = mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14];
    return result;
}

// Vertex Shader - Compute MatCap UVs
glm::vec2 vertex_matcap(float * view_matrix, glm::mat4 model_matrix, glm::vec3 normal) {
    // Apply the Model Matrix first (transformation from object space to world space)
    glm::vec3 transformed_normal = mat4_mul_vec3((float*)&model_matrix, normal);
    
    // Apply the View Matrix (transformation from world space to camera space)
    glm::vec3 normal_view = mat4_mul_vec3(view_matrix, transformed_normal);
    
    // Normalize and convert to UV coordinates (MatCap effect)
    glm::vec2 uv;
    uv.x = normal_view.x * 0.5f + 0.5f; // x-coordinate of UV
    uv.y = -normal_view.y * 0.5f + 0.5f; // y-coordinate of UV
    return uv;
}

bool look_at(float * view_matrix, float eye_x, float eye_y, float eye_z,
             float target_x, float target_y, float target_z,
             float up_x, float up_y, float up_z) {
    //printf("Initializing LookAt\n");

    if (eye_x == 0 && eye_y == 0 && eye_z == 0) {
        printf("Error: Camera position is invalid\n");
        return false;
    }

    float f[3] = {target_x - eye_x, target_y - eye_y, target_z - eye_z};
    float f_len = sqrtf(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
    if (f_len == 0) {
        printf("Error: Forward vector length is zero\n");
        return false;
    }

    for (int i = 0; i < 3; i++) f[i] /= f_len;

    float s[3], u[3];
    s[0] = up_y * f[2] - up_z * f[1];
    s[1] = up_z * f[0] - up_x * f[2];
    s[2] = up_x * f[1] - up_y * f[0];
    float s_len = sqrtf(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
    if (s_len == 0) {
        printf("Error: Right vector length is zero\n");
        return false;
    }

    for (int i = 0; i < 3; i++) s[i] /= s_len;

    u[0] = f[1] * s[2] - f[2] * s[1];
    u[1] = f[2] * s[0] - f[0] * s[2];
    u[2] = f[0] * s[1] - f[1] * s[0];

    //printf("Setting up view_matrix\n");

    for (int i = 0; i < 16; i++) view_matrix[i] = 0.0f;

    view_matrix[0] = s[0]; view_matrix[4] = s[1]; view_matrix[8]  = s[2];
    view_matrix[1] = u[0]; view_matrix[5] = u[1]; view_matrix[9]  = u[2];
    view_matrix[2] = -f[0]; view_matrix[6] = -f[1]; view_matrix[10] = -f[2];
    view_matrix[12] = -(s[0] * eye_x + s[1] * eye_y + s[2] * eye_z);
    view_matrix[13] = -(u[0] * eye_x + u[1] * eye_y + u[2] * eye_z);
    view_matrix[14] = f[0] * eye_x + f[1] * eye_y + f[2] * eye_z;
    view_matrix[15] = 1.0f;

    //printf("View Matrix Set\n");
    return true;
}

// Compute the Model Matrix from Transform object
glm::mat4 computeModelMatrix(const gdl::Transform& transform) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    model = glm::rotate(model, glm::radians(transform.rotationDegrees.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(transform.rotationDegrees.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(transform.rotationDegrees.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    return model;
}

// Function to calculate the matcap UV coordinates
void matcapEffect(gdl::Mesh* mesh, Camera* camera, gdl::Transform transform) {
    float view_matrix[16] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    // Compute the Model Matrix
    glm::mat4 model_matrix = computeModelMatrix(transform);

    if (camera->position.x == 0 && camera->position.y == 0 && camera->position.z == 0) return;
    
    /*printf("Camera statistics\n Position (%f,%f,%f)\n Direction (%f,%f,%f)\n Up (%f,%f,%f)\n",
        camera->position.x, camera->position.y, camera->position.z, 
        camera->direction.x, camera->direction.y, camera->direction.z, 
        camera->up.x, camera->up.y, camera->up.z);*/

    bool valid_camera = look_at(view_matrix, camera->position.x, camera->position.y, camera->position.z,
                                camera->direction.x, camera->direction.y, camera->direction.z, 
                                camera->up.x, camera->up.y, camera->up.z);

    if (!valid_camera) return;

    //printf("Vertex Count is %i\n", mesh->vertexCount);
    if (mesh->vertexCount <= 0) return;

    for (size_t i = 0; i < mesh->vertexCount; i++) {
        //printf("mc %zu / %zu ", i, mesh->vertexCount);

        size_t idx = i * 3;

        // Get the normal for the current vertex
        glm::vec3 normal = {
            mesh->normals[idx], 
            mesh->normals[idx + 1], 
            mesh->normals[idx + 2]};
        
        /*printf("= vec3(%f,%f,%f) => ", 
            mesh->normals[idx], 
            mesh->normals[idx + 1], 
            mesh->normals[idx + 2]);*/
        
        // Calculate MatCap UV
        glm::vec2 mc = vertex_matcap(view_matrix, model_matrix, normal);

        //printf("vec2(%f,%f)\n", mc.x, mc.y);

        idx = i * 2;
        mesh->uvs[idx] = mc.x;         // U coordinate for vertex i
        mesh->uvs[idx + 1] = mc.y;     // V coordinate for vertex i
    }
}