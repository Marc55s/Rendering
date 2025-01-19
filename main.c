#include "raylib.h"
#include "raymath.h"

void camera_move(Camera3D *camera) {

    //TraceLog(LOG_INFO, "(%f %f %f) l = %f",camera->target.x,camera->target.y,camera->target.z,Vector3Length(camera->target));

    // Step 1: Calculate forward direction from camera position to target
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera->target, camera->position));
    float scale_movement = 1.8f;
    forward = Vector3Scale(forward, scale_movement);

    // Step 2: Handle movement along forward direction
    if (IsKeyDown(KEY_W)) {
        camera->position.x += forward.x;
        camera->position.z += forward.z;
    } else if (IsKeyDown(KEY_S)) {
        camera->position.x -= forward.x;
        camera->position.z -= forward.z;
    }

    // Step 3: Handle strafing (left-right movement)
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, (Vector3) {0, 1, 0}));
    right = Vector3Scale(right, scale_movement);
    if (IsKeyDown(KEY_A)) {
        camera->position.x -= right.x;
        camera->position.z -= right.z;
    } else if (IsKeyDown(KEY_D)) {
        camera->position.x += right.x;
        camera->position.z += right.z;
    }

    Vector3 up = Vector3Normalize(Vector3CrossProduct(forward, (Vector3) {1, 0, 0}));
    up = Vector3Scale(up, scale_movement);

    if (IsKeyDown(KEY_E)) {
        camera->position.y -= up.y;
    } else if (IsKeyDown(KEY_Q)) {
        camera->position.y += up.y;
    }


    float x_dir = 0;
    float y_dir = 0;

    if (IsKeyDown(KEY_UP)) {
        y_dir -= 1;
    } else if (IsKeyDown(KEY_DOWN)) {
        y_dir += 1;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        x_dir += 1;
    } else if (IsKeyDown(KEY_LEFT)) {
        x_dir -= 1;
    }
    float sensitivity = 0.04f;
    float yaw = -x_dir * sensitivity;
    float pitch = -y_dir * sensitivity;
    right = Vector3Normalize(right);

// Rotate the forward vector around the up axis (yaw)
    Matrix yawRotation = MatrixRotate(camera->up, yaw);
    Vector3 forwardYawed = Vector3Transform(forward, yawRotation);

// Rotate the yawed forward vector around the right axis (pitch)
    Matrix pitchRotation = MatrixRotate(right, pitch);
    Vector3 forwardRotated = Vector3Transform(forwardYawed, pitchRotation);

// Update the camera target
    camera->target = Vector3Add(camera->position, forwardRotated);

}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1100;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    // Define the camera to look into our 3d world
    Camera3D camera = {0};
    camera.position = (Vector3) {10.0f, 5.0f, 20.0f}; // Camera position
    camera.target = (Vector3) {0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;

    float z = 10;
    Model m = LoadModel("../models/fancycube.obj");
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();

        ClearBackground(BLACK);
        BeginMode3D(camera);

        camera_move(&camera);

        DrawCube((Vector3) {0, 10, 0}, 10, 10, 10, WHITE);
        DrawCubeWires((Vector3) {0, 10, 0}, 10, 10, 10, RED);
        DrawPlane((Vector3) {0, -1, 0}, (Vector2) {100, 100}, ColorTint(BLUE, GREEN));
        DrawGrid(1000, 10);

        DrawModel(m, (Vector3) {15, 10, 15}, 5, SKYBLUE);
        DrawModelWires(m, (Vector3) {15, 10, 15}, 5, RED);


        DrawSphere((Vector3) {-15, 10, 0}, 5, PURPLE);
        DrawSphereWires((Vector3) {-15, 10, 0}, 5, 50, 50, RED);

        EndMode3D();


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}