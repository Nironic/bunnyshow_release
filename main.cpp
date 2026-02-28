#include "raylib.h"
#include "raymath.h"
#include <cstdlib>
#define RLIGHTS_IMPLEMENTATION
#include "Css/rlights.h"
//#include "assets/shaders/rlights.h"
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>



void DrawProgressBar(int x, int y, int width, int height, float progress, Color color) {
    // Рамка
    DrawRectangleLines(x, y, width, height, WHITE);
    
    // Заполнение
    int fillWidth = (width - 4) * progress;
    if (fillWidth > 0) {
        DrawRectangle(x + 2, y + 2, fillWidth, height - 4, color);
    }
}



int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    srand(time(NULL));
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "BunnyShow");
    Music menu_music = {0};
    Sound swith = {0};
    Model menu_model = {0};
    {
        //Переменная для загрузки
        int i = 0; // Сколько ресов будет столько и процентов
        //Работа с временем
        double startTime = GetTime() * 1000; // время запуска в миллисекундах
        //Инициализация аудио устройства
        InitAudioDevice();
        //Основные настройки переменные и конфиги
        float progress = 0.0f;
        int width = GetScreenWidth();
        int height = GetScreenHeight();
        std::string text1 = "Hello User";
        std::string text = "\nYou've entered my game. There will be screaming and lots of loud noises. Be careful.";
        std::string loading = "Loading";
        int fontSize = 28;
        //assets\fonts
        Font font = LoadFontEx("assets//fonts//CGXYZPC-Regular.otf", 64, 0, 250);
        Sound start_scene1 = LoadSound("assets//music//start_game.wav");
        Sound loading_fx = LoadSound("assets//music//loading.wav");
        Sound loading_fish = LoadSound("assets//music//menu_start.wav");

        //Векторы для расчета текста по Grid
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 0.02);
        Vector2 textSize2 = MeasureTextEx(font, text1.c_str(), fontSize, 0.02);
        Vector2 loading1 = MeasureTextEx(font, loading.c_str(), fontSize, 0.02);
        //Стартовая музыка
        PlaySound(start_scene1);
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
            // Функционал первой сцены
            double elapsed = (GetTime() * 1000) - startTime;
            if (progress == 1.0) {
                PlaySound(loading_fish);
                progress = 0.9;
                std::this_thread::sleep_for(std::chrono::seconds(3));
                break;
            }

            if (elapsed > 500) {
                if (i == 0){
                    menu_music = LoadMusicStream("assets//music//menu.wav");
                    i++;
                }
                if (i == 1){
                    swith = LoadSound("assets//music//swith.wav");
                    i++;
                }
                if (i == 2){
                    menu_model = LoadModel("assets//model//menu.glb");
                    i++;
                }
                progress = (float)i / 3;
            }

            // DrawTextEx требует Vector2 для позиции и float для размера
            DrawTextEx(font, text.c_str(),(Vector2){ GetScreenWidth()/2 - textSize.x/2,GetScreenHeight()/2 - textSize.y/2 + 10 },fontSize,0.02,WHITE); // Hello Users
            DrawTextEx(font, text1.c_str(),(Vector2){ GetScreenWidth()/2 - textSize2.x/2,GetScreenHeight()/2 - textSize2.y/2 },fontSize,0.02,WHITE); // Text Center
            DrawTextEx(font, loading.c_str(),(Vector2){ GetScreenWidth()/2 - loading1.x/2,(GetScreenHeight()/10 - loading1.y/2) * 10 },fontSize,0.02,WHITE); // Text Loading
            DrawProgressBar(GetScreenWidth()/2 - 200, (GetScreenHeight()/10 - loading1.y/2) * 10 + loading1.y + 10, 400, 40, progress, GREEN);
            EndDrawing();
        }
        //Разгрузка ресурсов
        UnloadSound(start_scene1);
        UnloadSound(loading_fx);
        UnloadSound(loading_fish);
        UnloadFont(font);

    }

    // Камера для 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 15.20, -8.00, -0.10 };
    camera.target = (Vector3){ 1.12, -8.59, 2.25 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    PlayMusicStream(menu_music);
    //DisableCursor();
    //Shader shader = LoadShader("assets/shaders/resources/shaders/glsl330/lighting.vs", "assets/shaders/resources/shaders/glsl330/lighting.fs");
    Shader shader = LoadShader("Css/glsl330/lighting.vs", "Css/glsl330/lighting.fs");
    for (int i = 0; i < menu_model.materialCount; i++) {
        menu_model.materials[i].shader = shader;
    }
    
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    
    // СОЗДАЕМ СВЕТ
    // Вместо WHITE используй серый
    //Light light = CreateLight(LIGHT_POINT,  (Vector3){ -12.85, -7.4, -0.18 }, Vector3Zero(), WHITE, shader);
    Light light = CreateLight(LIGHT_POINT,  (Vector3){ -12.85, -7.4, -0.18 }, Vector3Zero(), (Color){  100, 0, 0, 255 }, shader);
    //Работа с временем
    double startTime = GetTime() * 1000; // время запуска в миллисекундах
    while (!WindowShouldClose())
    {
        UpdateMusicStream(menu_music);
        double elapsed = (GetTime() * 1000) - startTime;
        
         // ===== РАНДОМНОЕ ВКЛ/ВЫКЛ ЛАМПЫ =====
        static float nextChangeTime = 2.0f;
        static bool lightState = true;
        
        float time = GetTime();
        
        if (time >= nextChangeTime) {
            lightState = !lightState;
            light.enabled = lightState;
            
            float randomDelay = 0.5f + (rand() / (float)RAND_MAX) * 2.5f;
            nextChangeTime = time + randomDelay;
            
            UpdateLightValues(shader, light);
            PlaySound(swith);
        }
        
        // Обновляем свет в шейдере
        float camPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(shader, light);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawSphereEx(light.position, 0.08f, 8, 8, light.color);
        DrawModel(menu_model, (Vector3){ 0, -10, 0 }, 1.0f, WHITE);
        
        EndMode3D();

        EndDrawing();
    }
    
    CloseAudioDevice(); // Закрываем канал аудиоустройства
    CloseWindow();
    return 0;
}