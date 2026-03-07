#include "raylib.h" // RayLib
#include "raymath.h" // RayMath
#include <cstdlib> // Random
#define RLIGHTS_IMPLEMENTATION // RayLib init shaders
#include "assets//shaders//rlights.h" // RayLib link shaders
#include <string> // String C++
#include <thread> // Setting main thread
#include <chrono> // Sleep Time
#include <vector>
#include <sstream>
#include <libre.h>

std::string version = "0.0.0.2";
ResourcesManager data;

//Scene1 --> Загрузка игры (первоначальный экран)
void scene1(){
    {
        std::vector<std::vector<std::string>> recource; // Дата с ресурсами которые будут подгружаться на меню и игру в целом
        recource.insert(recource.end(), {
            {"model" ,"menu", "assets//model//menu.glb"},
            {"model" ,"player", "assets//model//player.glb"},
            {"model" ,"maps", "assets//model//maps.glb"},
            {"music", "menu", "assets//music//menu.wav"},
            {"sound", "swith", "assets//music//swith.wav"},
            {"shader", "shaders", "assets//shaders//glsl330/lighting.vs", "assets//shaders///glsl330/lighting.fs"},
        });
        int i = 0; // Временная переменная для загрузки ресурсов
        double startTime = GetTime() * 1000; // Замер времени (В миллисекундах)
        float progress = 0.0f; // Прогресс загрузки в float
        int width = GetScreenWidth(); // Ширина экрана
        int height = GetScreenHeight(); // Высота экрана
        std::string text1 = "Hello User"; // Центрированный текст (Приведствие)
        std::string text = "\nYou've entered my game. There will be screaming and lots of loud noises. Be careful."; // Центрированный текст (Приведствие)
        std::string loading = "Loading"; // Текст загрузки
        int fontSize = 28; // Размер шрифта
        Font font = LoadFontEx("assets//fonts//CGXYZPC-Regular.otf", 64, 0, 250); //Подгрузка шрифта
        Sound start_scene1 = LoadSound("assets//music//start_game.wav"); //Загрузка стартового звука
        Sound loading_fx = LoadSound("assets//music//loading.wav"); // Звук загрузки
        Sound loading_fish = LoadSound("assets//music//menu_start.wav"); //Звук окончания загрузки

        //Векторы для расчета текста по Grid (Узнаем сколько пикселей составляет каждый текст чтобы расположить все в структурном порядке)
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 0.02);
        Vector2 textSize2 = MeasureTextEx(font, text1.c_str(), fontSize, 0.02);
        Vector2 loading1 = MeasureTextEx(font, loading.c_str(), fontSize, 0.02);
        PlaySound(start_scene1); //Воспроизведение стартового звука
        while (!WindowShouldClose()) { // Если окно закроется то Break
            BeginDrawing(); // Начинаем отрисовку
            ClearBackground(BLACK); // Заливаем все черным цветом
            // ------------------------------------------------------------------------------------------
            // Функционал сцены
            // ------------------------------------------------------------------------------------------
            double elapsed = (GetTime() * 1000) - startTime; // Замер времени в миллисекундах
            if (progress == 1.0) { // Если прогресс завершился то:
                PlaySound(loading_fish); // Играем окончание загрузки
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Ждем 3 секунды для звука
                break; // Останавливаем цикл
            }

            if (elapsed > 500) { //Если время больше пол секунды то:
                //Поменять всю концепцию загрузки данных. Чтобы не надо было прописывать все вручную. 
                /*
                if (i == 0){
                    data.AddMusic("menu", "assets//music//menu.wav");
                    i++;
                }
                if (i == 1){
                    //swith = LoadSound("assets//music//swith.wav"); // Загрузка звука для лампочки
                    data.AddSound("swith", "assets//music//swith.wav");
                    i++;
                }
                if (i == 2){
                    data.AddModel("menu", "assets//model//menu.glb");
                    i++;
                }
                if (i == 3){
                    data.AddShader("shaders", "assets//shaders//glsl330/lighting.vs", "assets//shaders///glsl330/lighting.fs");
                    i++;
                }
                if (i == 4) {
                    data.AddModel("player", "assets//model//player.glb");
                    i++;
                }
                if (i == 5) {
                    data.AddModel("player", "assets//model//maps.glb");
                    i++;
                }
                */
                if (recource[i][0] == "model"){
                    data.AddModel(recource[i][1], recource[i][2].c_str());
                    i++;
                }
                if (recource[i][0] == "music"){
                    data.AddMusic(recource[i][1], recource[i][2].c_str());
                    i++;
                }
                if (recource[i][0] == "sound"){
                    data.AddSound(recource[i][1], recource[i][2].c_str());
                    i++;
                }
                if (recource[i][0] == "shader"){
                    data.AddShader(recource[i][1], recource[i][2].c_str(), recource[i][3].c_str());
                    i++;
                }
                progress = (float)i / recource.size();
            }

            //Отрисовка текста
            DrawTextEx(font, text.c_str(),(Vector2){ GetScreenWidth()/2 - textSize.x/2,GetScreenHeight()/2 - textSize.y/2 + 10 },fontSize,0.02,WHITE); // Hello Users
            DrawTextEx(font, text1.c_str(),(Vector2){ GetScreenWidth()/2 - textSize2.x/2,GetScreenHeight()/2 - textSize2.y/2 },fontSize,0.02,WHITE); // Text Center
            DrawTextEx(font, loading.c_str(),(Vector2){ GetScreenWidth()/2 - loading1.x/2,(GetScreenHeight()/10 - loading1.y/2) * 10 },fontSize,0.02,WHITE); // Text Loading
            DrawProgressBar(GetScreenWidth()/2 - 200, (GetScreenHeight()/10 - loading1.y/2) * 10 + loading1.y + 10, 400, 40, progress, GREEN); // Отрисовка ProgressBar
            EndDrawing(); // Закончили отрисовку
        }
        //Разгрузка ресурсов
        UnloadSound(start_scene1);
        UnloadSound(loading_fx);
        UnloadSound(loading_fish);
        UnloadFont(font);
    }
}

void menu(){
    int width = GetScreenWidth(); // Ширина экрана
    int height = GetScreenHeight(); // Высота экрана
    // Камера для 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 15.20, -8.00, -0.10 };
    camera.target = (Vector3){ 1.12, -8.59, 2.25 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Music* menu_music = data.GetMusic("menu");
    PlayMusicStream(*menu_music); // Запускаем стрим по меню музыке
    //DisableCursor(); // Выключаем курсор (Использовалось для тестов)

    //Подгрузка шейдеров
    Model* menu_model = data.GetModel("menu");
    Shader* shader = data.GetShader("shaders");
    Sound* swith = data.GetSound("swith");
    if (menu_model){
        for (int i = 0; i < menu_model->materialCount; i++) {
            menu_model->materials[i].shader = *shader;
        }
    }
    shader->locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(*shader, "viewPos");
    int ambientLoc = GetShaderLocation(*shader, "ambient");
    float ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    SetShaderValue(*shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    Light light = CreateLight(LIGHT_POINT,  (Vector3){ -12.85, -7.4, -0.18 }, Vector3Zero(), (Color){  100, 0, 0, 255 }, *shader); // Создание лампы

    double startTime = GetTime() * 1000; // Замер времени (В миллисекундах)
    int fontSize = 70; // Размер шрифта
    Font font = LoadFontEx("assets//fonts//Deadline_0.otf", 64, 0, 250); //Подгрузка шрифта
    Font font_button = LoadFontEx("assets//fonts//Vertiger.otf", 64, 0, 250); // Загрузка шрифта для кнопок
    std::string text = "BunnyShow";
    //Работа по Grid
    float button_y = 100;
    float x_button = (width * 2.0) / 100.0;
    float y_button = (((float)height * 5.f) / 100.f) + 200 + 50;
    float button_x = 350;
    //Button
    //Button Play Solo
    Button play_solo = CreateButton(font_button);
    play_solo.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    play_solo.text = "PLAY";
    y_button += button_y;
    //Button Setting
    Button setting = CreateButton(font_button);
    setting.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    setting.text = "SETTING";
    y_button += button_y;
    //Button Exit
    Button exits = CreateButton(font_button);
    exits.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    exits.text = "EXIT";
    // Text Format
    int textSize = MeasureText((std::string("BunnyShow Fix ") + version).c_str(), 20);


    while (!WindowShouldClose())
    {
        UpdateMusicStream(*menu_music); // Обновляем каждый кадр музыку
        double elapsed = (GetTime() * 1000) - startTime; // Расчет времени в миссисикундах

        //Работа с лампой. Включение/Выключение на технологии Rand (random)
        static float nextChangeTime = 2.0f;
        static bool lightState = true;
        
        float time = GetTime();
        
        if (time >= nextChangeTime) {
            lightState = !lightState;
            light.enabled = lightState;
            
            float randomDelay = 0.5f + (rand() / (float)RAND_MAX) * 2.5f;
            nextChangeTime = time + randomDelay;
            
            UpdateLightValues(*shader, light);
            PlaySound(*swith);
        }

        //Обновление кнопок
        UpdateButton(play_solo);
        UpdateButton(setting);
        UpdateButton(exits);
        
        // Проверяем нажатие
        if (exits.isPressed) {
            break;
        }

        
        // Обновляем свет в шейдере
        float camPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(*shader, shader->locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(*shader, light);


        BeginDrawing(); // Начало отрисовка
        ClearBackground(BLACK); // Заливаем все черным

        BeginMode3D(camera); // Включаем 3д режим
            DrawSphereEx(light.position, 0.08f, 8, 8, light.color); // Рисуем сферу и присоединяем к ней шейдер света
            DrawModel(*menu_model, (Vector3){ 0, -10, 0 }, 1.0f, WHITE); // Рисуем модель меню (menu.glb)
        EndMode3D(); // Заканчиваем 3д отрисовку
        //Рисуем кнопки
        DrawButton(play_solo);
        DrawButton(setting);
        DrawButton(exits);
        //Рисуем текст
        DrawTextEx(font, text.c_str(),(Vector2){ ((float)width * 2.0f) / 100.f, ((float)height * 5.f) / 100.f },fontSize,0.02,WHITE);
        DrawText("BunnyShow Fix 0.0.0.2", (width - textSize),  (height - ((height * 1.f) / 100.f) * 2.f), 20, WHITE);

        EndDrawing(); // Заканчиваем 2д отрисовку
    }
}

int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE); //FullScrenn мод
    srand(time(NULL)); // Запуск таймера от времени (В противном случае собьется и будет работать одинаково)
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "BunnyShow"); // Инициализация окна
    InitAudioDevice(); // Инициализация аудио

    //Переменные в которых находиться дата, модели, музыка, шейдеры

    
    Music menu_music = {0};
    Sound swith = {0};
    Shader shader = {0};

    scene1(); // Запуск сцены загрузки
    menu(); // Запуск сцены меню


    //UnloadModel(menu_model);
    UnloadSound(swith);
    UnloadMusicStream(menu_music);
    UnloadShader(shader);
    CloseAudioDevice(); // Закрываем канал аудиоустройства
    CloseWindow(); // Закрываем окно
    return 0; // Возвращаем 0 из программы
}
