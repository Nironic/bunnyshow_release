#include "raylib.h" // RayLib
#include "raymath.h" // RayMath
#include <cstdlib> // Random
#define RLIGHTS_IMPLEMENTATION // RayLib init shaders
#include "Css/rlights.h" // RayLib link shaders
#include <vector> // Vectors C++
#include <string> // String C++
#include <thread> // Setting main thread
#include <chrono> // Sleep Time
#include <iostream>

//Button
struct Button {
    Rectangle rect;      // x, y, width, height
    Color normalColor;   // цвет когда не наведена
    Color hoverColor;    // цвет при наведении
    Color textColor;     // цвет текста
    const char* text;    // текст на кнопке
    int fontSize;        // размер шрифта
    bool isHovered;      // состояние (наведена или нет)
    bool isPressed;      // была ли нажата в этом кадре
};

Button CreateButton() {
    Button btn = {0};  // всё заполняется позже
    
    // Можно задать значения по умолчанию
    btn.normalColor = DARKGRAY;
    btn.hoverColor = RED;
    btn.textColor = WHITE;
    btn.fontSize = 20;
    btn.isHovered = false;
    btn.isPressed = false;
    
    return btn;
}

// Обновления кнопок
void UpdateButton(Button &btn) { 
    Vector2 mouse = GetMousePosition();
    
    // Проверяем наведение
    btn.isHovered = CheckCollisionPointRec(mouse, btn.rect);
    
    // Проверяем нажатие
    btn.isPressed = btn.isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

//Рисуем кнопку
void DrawButton(Button btn) {
    // Выбираем цвет в зависимости от наведения
    Color currentColor = btn.isHovered ? btn.hoverColor : btn.normalColor;
    
    // Рисуем кнопку
    DrawRectangleRec(btn.rect, currentColor);
    DrawRectangleLinesEx(btn.rect, 2, BLACK);
    
    // Рисуем текст
    int textWidth = MeasureText(btn.text, btn.fontSize);
    int textX = btn.rect.x + btn.rect.width/2 - textWidth/2;
    int textY = btn.rect.y + btn.rect.height/2 - btn.fontSize/2;
    
    DrawText(btn.text, textX, textY, btn.fontSize, btn.textColor);
}

//Progress Bar отрисовка
void DrawProgressBar(int x, int y, int width, int height, float progress, Color color) {
    // Рамка
    DrawRectangleLines(x, y, width, height, WHITE);
    
    // Заполнение
    int fillWidth = (width - 4) * progress;
    if (fillWidth > 0) {
        DrawRectangle(x + 2, y + 2, fillWidth, height - 4, color);
    }
}


//Scene1 --> Загрузка игры (первоначальный экран)
void scene1(Music &menu_music, Sound &swith, Model &menu_model, Shader &shader){
    {
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
                if (i == 0){
                    menu_music = LoadMusicStream("assets//music//menu.wav"); // Загрузка музыки для меню
                    i++;
                }
                if (i == 1){
                    swith = LoadSound("assets//music//swith.wav"); // Загрузка звука для лампочки
                    i++;
                }
                if (i == 2){
                    menu_model = LoadModel("assets//model//menu.glb"); // Загрузка модели меню
                    i++;
                }
                if (i == 3){
                    shader = LoadShader("Css/glsl330/lighting.vs", "Css/glsl330/lighting.fs");
                    i++;
                }
                progress = (float)i / 4;
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

void menu(Music &menu_music, Sound &swith, Model &menu_model, Shader &shader){
    int width = GetScreenWidth(); // Ширина экрана
    int height = GetScreenHeight(); // Высота экрана
    // Камера для 3D
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 15.20, -8.00, -0.10 };
    camera.target = (Vector3){ 1.12, -8.59, 2.25 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    PlayMusicStream(menu_music); // Запускаем стрим по меню музыке
    //DisableCursor(); // Выключаем курсор (Использовалось для тестов)

    //Подгрузка шейдеров
    for (int i = 0; i < menu_model.materialCount; i++) {
        menu_model.materials[i].shader = shader;
    }
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    SetShaderValue(shader, ambientLoc, ambient, SHADER_UNIFORM_VEC4);
    Light light = CreateLight(LIGHT_POINT,  (Vector3){ -12.85, -7.4, -0.18 }, Vector3Zero(), (Color){  100, 0, 0, 255 }, shader); // Создание лампы

    double startTime = GetTime() * 1000; // Замер времени (В миллисекундах)
    int fontSize = 50; // Размер шрифта
    Font font = LoadFontEx("assets//fonts//CGXYZPC-Regular.otf", 64, 0, 250); //Подгрузка шрифта
    std::string text = "BunnyShow";
    //Работа по Grid
    float button_y = 150;
    float x_button = (width * 2.0) / 100.0;
    float y_button = (((float)height * 5.f) / 100.f) + 200 + 50;
    float button_x = 200;
    //Button
    //Button Play Solo
    Button play_solo = CreateButton();
    play_solo.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    play_solo.text = "PLAY SOLO";
    y_button += button_y;
    //Button Play Solo
    Button play_frends = CreateButton();
    play_frends.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    play_frends.text = "PLAY FRENDS";
    y_button += button_y;
    //Button Play Solo
    Button setting = CreateButton();
    setting.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    setting.text = "SETTING";
    y_button += button_y;
    //Button Play Solo
    Button exits = CreateButton();
    exits.rect = (Rectangle){ x_button, y_button, button_x, button_y };
    exits.text = "EXIT";
    // Text Format
    int textSize = MeasureText("BunnyShow Fix 0.0.0.2", 20);


    while (!WindowShouldClose())
    {
        UpdateMusicStream(menu_music); // Обновляем каждый кадр музыку
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
            
            UpdateLightValues(shader, light);
            PlaySound(swith);
        }

        //Обновление кнопок
        UpdateButton(play_solo);
        UpdateButton(play_frends);
        UpdateButton(setting);
        UpdateButton(exits);
        
        // Проверяем нажатие
        if (exits.isPressed) {
            break;
        }

        
        // Обновляем свет в шейдере
        float camPos[3] = { camera.position.x, camera.position.y, camera.position.z };
        SetShaderValue(shader, shader.locs[SHADER_LOC_VECTOR_VIEW], camPos, SHADER_UNIFORM_VEC3);
        UpdateLightValues(shader, light);


        BeginDrawing(); // Начало отрисовка
        ClearBackground(BLACK); // Заливаем все черным

        BeginMode3D(camera); // Включаем 3д режим
            DrawSphereEx(light.position, 0.08f, 8, 8, light.color); // Рисуем сферу и присоединяем к ней шейдер света
            DrawModel(menu_model, (Vector3){ 0, -10, 0 }, 1.0f, WHITE); // Рисуем модель меню (menu.glb)
        EndMode3D(); // Заканчиваем 3д отрисовку
        //Рисуем кнопки
        DrawButton(play_solo);
        DrawButton(play_frends);
        DrawButton(setting);
        DrawButton(exits);
        //Рисуем текст
        DrawTextEx(font, text.c_str(),(Vector2){ ((float)width * 2.0f) / 100.f, ((float)height * 5.f) / 100.f },fontSize,0.02,WHITE); // Hello Users
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
    Model menu_model = {0};
    Shader shader = {0};

    scene1(menu_music, swith, menu_model, shader); // Запуск сцены загрузки
    menu(menu_music, swith, menu_model, shader); // Запуск сцены меню


    UnloadModel(menu_model);
    UnloadSound(swith);
    UnloadMusicStream(menu_music);
    UnloadShader(shader);
    CloseAudioDevice(); // Закрываем канал аудиоустройства
    CloseWindow(); // Закрываем окно
    return 0; // Возвращаем 0 из программы
}