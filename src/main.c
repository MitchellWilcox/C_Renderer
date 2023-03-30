#include "raylib.h"
#include "raymath.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif


#include "second.h"

int main() 
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;
		

    InitWindow(screenWidth, screenHeight, "raylib");

    Camera camera = { 0 };
    camera.position = (Vector3){ 20.0f, 20.0f, 18.0f };
    camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    //LoadGltf model
    Model m_TRex = LoadModel("resources/models/gltf/TREX.glb");
    // Texture2D texture = LoadTexture("resources/models/iqm/guytex.png");
    // SetMaterialTexture(&m_TRex.materials[0], MATERIAL_MAP_DIFFUSE, texture);

    // Load basic lighting shader
    Shader s_lightShader = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

    s_lightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(s_lightShader, "viewPos");


    int ambientLoc = GetShaderLocation(s_lightShader, "ambient");
    SetShaderValue(s_lightShader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    
    for(int i=0;i<m_TRex.materialCount;++i)
    {
        m_TRex.materials[i].shader = s_lightShader;
    }

    Light lights[MAX_LIGHTS] = { 0 };
    lights[0] = CreateLight(LIGHT_POINT, (Vector3){ 5, 5 , 5 }, Vector3Zero(), YELLOW, s_lightShader);
    lights[1] = CreateLight(LIGHT_POINT, (Vector3){ -6, 6 , -6}, Vector3Zero(), RED, s_lightShader);

    //printf(model.meshCount);


    // Load gltf model animations
    // unsigned int animsCount = 0;
    // unsigned int animIndex = 0;
    // unsigned int animCurrentFrame = 0;
    // ModelAnimation *modelAnimations = LoadModelAnimations("resources/models/gltf/TREX.glb", &animsCount);

    SetCameraMode(camera, CAMERA_ORBITAL);

    Vector3 position  = { 0,0,0 };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(s_lightShader, s_lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);

                DrawModel(m_TRex, position, 1.0f, WHITE);

                for (int i = 0; i < 2; i++)
                {
                    DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
                    DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
                }

                DrawGrid(20, 1.0f);

            EndMode3D();

            DrawText("This is a raylib example", 10, 40, 20, DARKGRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    //UnloadTexture(texture);                     // Unload texture
    //UnloadTexture(bumpMap);                     // Unload texture
    UnloadShader(s_lightShader);   // Unload shader
    UnloadModel(m_TRex);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}