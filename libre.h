#pragma once
#include "raylib.h"
#include <string>
#include <vector>

class ResourcesManager {
private:
    struct Resource {
        std::string name;
        std::string type;
        Model model;
        Sound sound;
        Music music;
        Texture2D texture;
        Shader shader;
        Font font;
    };
    
    std::vector<Resource> resources;

public:
    // Конструктор
    ResourcesManager() = default;
    
    // Деструктор
    ~ResourcesManager() {
        UnloadAll();
    }
    
    // Добавление ресурсов
    void AddModel(const std::string& name, const char* path) {
        Resource res;
        res.name = name;
        res.type = "model";
        res.model = LoadModel(path);
        resources.push_back(res);
    }
    
    void AddSound(const std::string& name, const char* path) {
        Resource res;
        res.name = name;
        res.type = "sound";
        res.sound = LoadSound(path);
        resources.push_back(res);
    }
    
    void AddMusic(const std::string& name, const char* path) {
        Resource res;
        res.name = name;
        res.type = "music";
        res.music = LoadMusicStream(path);
        resources.push_back(res);
    }
    void AddShader(const std::string& name, const char* path1, const char* path2) {
        Resource res;
        res.name = name;
        res.type = "shader";
        res.shader = LoadShader(path1, path2);
        resources.push_back(res);
    }
    
    // Получение ресурсов
    Model* GetModel(const std::string& name) {
        for (auto& res : resources) {
            if (res.name == name && res.type == "model") {
                return &res.model;
            }
        }
        return nullptr;
    }
    
    Sound* GetSound(const std::string& name) {
        for (auto& res : resources) {
            if (res.name == name && res.type == "sound") {
                return &res.sound;
            }
        }
        return nullptr;
    }
    
    Music* GetMusic(const std::string& name) {
        for (auto& res : resources) {
            if (res.name == name && res.type == "music") {
                return &res.music;
            }
        }
        return nullptr;
    }
    Shader* GetShader(const std::string& name) {
        for (auto& res : resources) {
            if (res.name == name && res.type == "shader") {
                return &res.shader;
            }
        }
        return nullptr;
    }
    Font* GetFont(const std::string& name) {
        for (auto& res : resources) {
            if (res.name == name && res.type == "font") {
                return &res.font;
            }
        }
        return nullptr;
    }
    // Очистка всех ресурсов
    void UnloadAll() {
        for (auto& res : resources) {
            if (res.type == "model") UnloadModel(res.model);
            else if (res.type == "sound") UnloadSound(res.sound);
            else if (res.type == "music") UnloadMusicStream(res.music);
            else if (res.type == "texture") UnloadTexture(res.texture);
            else if (res.type == "shader") UnloadShader(res.shader);
            else if (res.type == "font") UnloadShader(res.shader);
        }
        resources.clear();
    }
    
    // Проверка существования
    bool Exists(const std::string& name) {
        for (auto& res : resources) {
            if (res.name == name) return true;
        }
        return false;
    }
    
    // Получить количество ресурсов
    size_t Count() const {
        return resources.size();
    }
};


//Структуры
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
    Font font;
};

Button CreateButton(Font defaultFont) {  // передаём шрифт
    Button btn = {0};
    btn.normalColor = DARKGRAY;
    btn.hoverColor = RED;
    btn.textColor = WHITE;
    btn.fontSize = 50;
    btn.isHovered = false;
    btn.isPressed = false;
    btn.font = defaultFont;  // сохраняем шрифт в кнопке
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
    Color currentColor = btn.isHovered ? btn.hoverColor : btn.normalColor;
    
    DrawRectangleRec(btn.rect, currentColor);
    DrawRectangleLinesEx(btn.rect, 2, BLACK);
    
    // ИСПОЛЬЗУЕМ ШРИФТ ИЗ КНОПКИ
    Vector2 textSize = MeasureTextEx(btn.font, btn.text, btn.fontSize, 2);
    int textX = btn.rect.x + btn.rect.width/2 - textSize.x/2;
    int textY = btn.rect.y + btn.rect.height/2 - textSize.y/2;
    
    // Рисуем текст СВОИМ ШРИФТОМ
    DrawTextEx(btn.font, btn.text, (Vector2){ (float)textX, (float)textY }, 
               btn.fontSize, 2, btn.textColor);
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

//Split
std::vector<std::string> split(const std::string &str, char delim) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}