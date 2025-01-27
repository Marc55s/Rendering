#include "lorenz.h"
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
    float sensitivity = 0.02f;
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

struct Line3 {
    Vector3 start;
    Vector3 end;
};

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1800;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    // Define the camera to look into our 3d world
    Camera3D camera = {0};
    camera.position = (Vector3) {-10.0f, 5.0f, -20.0f}; // Camera position
    camera.target = (Vector3) {0.0f, 0.0f, 0.0f};      // Camera looking at point
    camera.up = (Vector3) {0.0f, 1.0f, 0.0f};          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;

    Model m = LoadModel("../models/fancycube.obj");

    double x = 0; 

    double y = 0.1;

    double z = -1; 

    double t = 0.01; 


    //Lorenz_sys *system = init(a,b,c,t,x,y,z);
    int systems_amount = 1;
    Lorenz_sys **systems = (Lorenz_sys**) MemAlloc(sizeof(Lorenz_sys*) * systems_amount);


    for (int i = 0; i < systems_amount; i++) {
        systems[i] = init(GetRandomValue(5, 20), GetRandomValue(10, 30), GetRandomValue(0, 10),t,x,y,z);
    }
    TraceLog(LOG_INFO, "Starting Parameters %d %d %d %lf %lf %lf %lf",GetRandomValue(5, 20), GetRandomValue(10, 30), GetRandomValue(0, 10),t,x,y,z);

    struct Line3 *lines = MemAlloc(sizeof(struct Line3) * 1000);

    int idx = 0;
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        camera_move(&camera);

        //DrawGrid(1000, 10);
        for (int i = 0; i < systems_amount; i++) {
            Lorenz_sys *iter = systems[i];
            Vector3 s = (Vector3){iter->x,iter->y,iter->z};
            progress(iter, 100);
            double x1 = iter->x;
            double y1 = iter->y;
            double z1 = iter->z;
            lines[idx] = (struct Line3){s,(Vector3){x1,y1,z1}};
            //DrawSphere((Vector3){x1,y1,z1}, 1, BLUE);
            //TraceLog(LOG_INFO,"Lorenz nums: %lf %lf %lf",x1,y1,z1);
        }


        /* TODO replace with buffer strategy
        * preload a buffer of given size
        * so there are always n points availible to draw
        */
        for (int i = 0; i < 1000; i++) {
            struct Line3 line = lines[i];
            DrawLine3D(line.start, line.end, PINK);
        }

        idx++;
        idx %= 1000;

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
