#pragma once
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <Il2Cpp.h>
#include <Tools.h>
#include <Vector3.hpp>
#include <Color.h>
#include <NEXO MODS/main.h>
#include "obfuscate.h"
#include "Unity/unity.h"
#include "Class.h"
#include "Rect.h"
#include "Bools.h"
#include <math.h>
#include <string>

inline static int g_GlHeight, g_GlWidth;
typedef char PACKAGENAME;

struct sColor {
    float line[4] = { 0.0f, 1.0f, 0.5f, 1.0f };       // Liquid Emerald
    float box[4]  = { 1.0f, 0.0f, 0.3f, 1.0f };       // Electric Ruby
};
sColor Color;

template <typename T>
struct UnityArray {
    void* klass;
    void* monitor;
    void* bounds;
    int max_length;
    T vector[64]; 
};

struct Vector2 {
    float x, y;
};

struct DictionaryEntry {
    int hashCode;
    int next;
    void* key;
    void* value;
};

struct MyDictionary {
    void* klass;
    void* monitor;
    void* buckets;
    UnityArray<DictionaryEntry>* entries;
    int count;
};

bool isOutsideScreen(ImVec2 pos, ImVec2 screen) {
    return (pos.y < 0 || pos.x > screen.x || pos.y > screen.y || pos.x < 0);
}

ImVec2 pushToScreenBorder(ImVec2 Pos, ImVec2 screen, int offset) {
    int x = (int) Pos.x;
    int y = (int) Pos.y;
    
    if (Pos.y < 0) y = -offset;
    if (Pos.x > screen.x) x = (int) screen.x + offset;
    if (Pos.y > screen.y) y = (int) screen.y + offset;
    if (Pos.x < 0) x = -offset;
    
    return ImVec2(x, y);
}

static float tm = 127 / 255.f;

void DrawCircleHealth(ImVec2 position, int health, int max_health, float radius) {
    if (max_health <= 0) return;
    float a_max = (3.14159265359f * 2.0f);
    
    ImU32 healthColor = IM_COL32(0, 255, 128, 255); 
    if (health <= (max_health * 0.6)) {
        healthColor = IM_COL32(255, 160, 0, 255); 
    }
    if (health < (max_health * 0.3)) {
        healthColor = IM_COL32(255, 0, 50, 255);  
    }
    ImGui::GetForegroundDrawList()->PathArcTo(position, radius, (-(a_max / 4.0f)) + (a_max / max_health) * (max_health - health), a_max - (a_max / 4.0f));
    ImGui::GetForegroundDrawList()->PathStroke(healthColor, ImDrawFlags_None, 3.5f);
}

void drawText2(int x, int y, long color, const char *str, float size) {
    if (!str) return;
    ImGui::GetBackgroundDrawList()->AddText(NULL, size, ImVec2(x, y), color, str);
}

void DrawRectFilled(int x, int y, int w, int h, ImColor color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void DrawRectFilledHealth(int x, int y, int w, int h, ImVec4 color) {
    ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, int T) {
    ImGui::GetBackgroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3), color);
}

bool IsVisible(void * player){
    if(player != NULL) {
        void *hitObj = NULL;
        void* camera = Camera_main();
        if (!camera) return false;
        
        Vector3 cameraLocation = Transform_GetPosition(Component_GetTransform(camera));
        void* headCollider = Player_GetHeadCollider(player);
        if (!headCollider) return false;
        
        Vector3 headLocation = Transform_GetPosition(Component_GetTransform(headCollider));
        return !Physics_Raycast(cameraLocation, headLocation, 12, &hitObj);
    }
    return false;
}

void DrawHealths(Rect box, float entityHealth, float maxHealth, int TeamID, const char* name, long color, float distance) {
    float x = box.x - (140 - box.w) / 2;
    float y = box.y;
    
    char dis[32];
    sprintf(dis, "%.0fM", distance);

    char TeamText[20];
    sprintf(TeamText, "%d", (int)TeamID);
    
    ImVec4 HPColor = ImVec4{0.0f, 1.0f, 0.5f, 0.8f}; 

    if (maxHealth > 0 && entityHealth < maxHealth) {
        float healthPercentage = entityHealth / maxHealth;
        if (healthPercentage < 0.3f) {
            HPColor = ImVec4{1.0f, 0.0f, 0.2f, 0.8f}; 
        } else if (healthPercentage < 0.6f) {
            HPColor = ImVec4{1.0f, 0.5f, 0.0f, 0.8f}; 
        }
    }

    size_t nameLen = name ? strlen(name) : 5;
    
    DrawRectFilled(x - nameLen, y - 41, 120 + nameLen * 2, 20, IM_COL32(5, 5, 10, 200)); 
    DrawRectFilled(x - nameLen, box.y - 41, 25, 20, IM_COL32(255, 0, 100, 220)); 
    
    if (strlen(TeamText) < 2) {
        drawText2(x + 6 - nameLen, box.y - 42, ImColor(255,255,255), TeamText, 18.943);
    } else {
        drawText2(x - nameLen, box.y - 42, ImColor(255,255,255), TeamText, 18.943);
    }
    
    drawText2(x + 28 - nameLen, y - 43, ImColor(255,255,255), name ? name : "Target", 18.943);
    
    float maxWidth = 120; 
    float healthBarWidth = (maxHealth > 0) ? (entityHealth * maxWidth / maxHealth) : maxWidth; 
    DrawRectFilledHealth(x - maxWidth / 2, y - 18, healthBarWidth, 5, HPColor);
    drawText2(x + 125 + nameLen, y - 43, IM_COL32(0, 255, 150, 255), dis, 18.943); 
    DrawTriangle(box.x + box.w / 2 - 10, y - 8, box.x + box.w / 2 + 15 - 10, y - 8, box.x + box.w / 2 - 2, y, IM_COL32(255, 0, 100, 255), 1);
}

static float SetFieldOfView = 0, GetFieldOfView = 0;
void DroneView() {
    if (GetFieldOfView == 0) {
        GetFieldOfView = get_fieldOfView();
    }
    if (SetFieldOfView > 0 && GetFieldOfView != 0) {
        set_fieldOfView((float)GetFieldOfView + SetFieldOfView);
    }
    if (SetFieldOfView == 0 && GetFieldOfView != 0) {
        set_fieldOfView((float)GetFieldOfView);
    }
}

void *GetClosestEnemy() {
    float shortestDistance = 9999;
    void* closestEnemy = NULL;   
    void* get_MatchGame = Curent_Match();
    if (!get_MatchGame) return NULL;
    
    void* LocalPlayer = GetLocalPlayer(get_MatchGame);
    if (LocalPlayer != NULL && Enable) {
        MyDictionary* dict = *(MyDictionary**)((uintptr_t)get_MatchGame + ListPlayer);
        if (dict != nullptr && dict->entries != nullptr) {
            void* camera = Camera_main();
            if (!camera) return NULL;
            int total = dict->count;
            for (int u = 0; u < total; u++) {
                void* Player = dict->entries->vector[u].value;       
                if (Player != NULL && Player != LocalPlayer && !get_isLocalTeam(Player) && !get_IsDieing(Player) && get_isVisible(Player) && get_MaxHP(Player)) {    
                    Vector3 PlayerPos = getPosition(Player);
                    Vector3 LocalPlayerPos = getPosition(LocalPlayer);
                    Vector3 pos2 = WorldToScreenPoint(camera, PlayerPos);     
                    bool isFov1 = isFov(Vector3(pos2.x, pos2.y), Vector3(g_GlWidth / 2, g_GlHeight / 2), Fov_Aim);
                    float distance = Vector3::Distance(LocalPlayerPos, PlayerPos);
                    if (distance < Aimdis) {
                        Vector3 targetDir = Vector3::Normalized(PlayerPos - LocalPlayerPos);
                        float angle = Vector3::Angle(targetDir, GetForward(Component_GetTransform(camera))) * 100.0;             
                        if (angle <= Fov_Aim && isFov1 && angle < shortestDistance) {                      
                            shortestDistance = angle;
                            closestEnemy = Player;
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;
}

extern bool SpeedHack;    
bool SpeedHack = false;
bool saved = false;
float savedScreenWidth = 0;
float savedScreenHeight = 0;
float active = 0.0f;
float desactive = 0.0f;

void UpdateSpeedhack() {
    if (Enable) {
        void* Simulation = GetSimulationTimer();
        if (Simulation != nullptr) {
            float FixedDeltaTime = GetTimer(Simulation);
            if(!saved) {
                active = FixedDeltaTime * 1.82f;
                desactive = FixedDeltaTime;
                saved = true;
            }
            if (SpeedHack) {
                if (FixedDeltaTime != active) {
                    SetTimer(Simulation, active);
                }
            } else {
                if (FixedDeltaTime != desactive) {
                    SetTimer(Simulation, desactive);
                }
            }
        }
    }
}

float (*get_deltaTime)();
float smoothSpeedAIM = 5; 
void *MyPlayer = NULL;
bool Drone;
const float RADAR_SIZE = 200.0f; 
const ImVec2 RADAR_CENTER(100.0f, 100.0f); 
float x, y;
float enemy1 = 2.0f;

bool RenderWatermark = false;
bool block_connections = true;

ImVec4 colorTextBackground = ImVec4(0.02f, 0.02f, 0.05f, 0.85f); 
ImVec4 colorBox          = ImVec4(1.0f, 0.0f, 0.35f, 1.0f);   
ImVec4 colorHealth       = ImVec4(0.0f, 1.0f, 0.5f, 1.0f);   
ImVec4 colorDistance     = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); 
ImVec4 colorLine         = ImVec4(0.0f, 1.0f, 0.8f, 0.6f);         
ImVec4 colorHexagono     = ImVec4(1.0f, 0.0f, 0.5f, 1.0f);
ImVec4 colorMenuText     = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 NeonColor         = ImVec4(0.0f, 1.0f, 0.7f, 1.0f);   

void* CurrentMatch = nullptr;
void* closestEnemy = nullptr;
void* LocalPlayer = nullptr;
float aimSmoothness = 0.1f;

inline void DrawAIMBOT(float screenWidth, float screenHeight) {}

inline void DrawESP(float screenWidth, float screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;
    
    // --- [1. रीयल-टाइम प्रीमियम FPS काउंटर] ---
    char fpsBuffer[32];
    sprintf(fpsBuffer, "FPS: %.1f", ImGui::GetIO().Framerate);
    draw->AddRectFilled(ImVec2(10, 10), ImVec2(110, 35), IM_COL32(10, 10, 20, 200), 6.0f);
    draw->AddRect(ImVec2(10, 10), ImVec2(110, 35), IM_COL32(0, 255, 150, 255), 6.0f, 0, 1.2f);
    draw->AddText(NULL, 16.0f, ImVec2(22, 14), IM_COL32(255, 255, 255, 255), fpsBuffer);

    int totalEnemies = 0; 
    bool enemyInFOV = false; 

    if (Aimbot) {
        void* CurrentMatch = Curent_Match();
        void* closestEnemy = GetClosestEnemy();
        void* LocalPlayer = GetLocalPlayer(CurrentMatch);

        if (closestEnemy != NULL && LocalPlayer != NULL && CurrentMatch != NULL) {
            Vector3 EnemyLocation = GetHeadPosition(closestEnemy);
            Vector3 PlayerLocation = CameraMain(LocalPlayer);
            Quaternion PlayerLook = GetRotationToLocation(EnemyLocation, 0.1f, PlayerLocation);
            bool IsScopeOn = get_IsSighting(LocalPlayer);
            bool IsFiring = get_IsFiring(LocalPlayer);
            
            if (AimWhen == 0) {
                set_aim(LocalPlayer, PlayerLook);
            } else if (AimWhen == 1 && IsFiring) {
                set_aim(LocalPlayer, PlayerLook);
            } else if (AimWhen == 2 && IsScopeOn) {
                set_aim(LocalPlayer, PlayerLook);
            }
        }
    }

    if (Enable) {
        void* current_Match = Curent_Match();
        void* local_player = GetLocalPlayer(current_Match);
        
        ImU32 electric_ruby = IM_COL32(255, 0, 90, 255);
        ImU32 liquid_emerald = IM_COL32(0, 255, 160, 255);

        if (local_player && current_Match) {
            MyDictionary* players = *(MyDictionary**)((uintptr_t)current_Match + ListPlayer);
            void* camera = Camera_main();
            if (players && players->entries && camera) {
                int total = players->count;
                for (int u = 0; u < total; u++) {
                    void* enemyPtr = players->entries->vector[u].value;

                    if (enemyPtr && enemyPtr != local_player && get_isVisible(enemyPtr) && !get_isLocalTeam(enemyPtr)) {
                        totalEnemies++;
                        Vector3 Toepos = getPosition(enemyPtr);
                        Vector3 Toeposi = WorldToScreenPoint(camera, Toepos);
                        if (Toeposi.z < 1) continue;
                        Vector3 HeadPos = Toepos + Vector3(0, 1.9f, 0);
                        Vector3 HeadPosition = WorldToScreenPoint(camera, HeadPos);
                        if (HeadPosition.z < 1) continue;
                        
                        // एनिमेटेड FOV रिंग विज़ुअल
                        draw->AddCircle(ImVec2(screenWidth / 2, screenHeight / 2), Fov_Aim, liquid_emerald, 120, 1.8f);
                        enemyInFOV = true; 

                        float distance = Vector3::Distance(getPosition(local_player), Toepos);                                             
                        float Height = abs(HeadPosition.y - Toeposi.y) * 1.09f;
                        float Width = Height * 0.55f;                                                    
                        Rect rect = Rect(HeadPosition.x - Width / 2.f, screenHeight - HeadPosition.y, Width, Height);                                                        
                        
                        // --- [2. सेंटर/एम रिंग से हेड तक कनेक्टिंग लाइन] ---
                        ImVec2 screenCenter(screenWidth / 2, screenHeight / 2);
                        ImVec2 targetHead(HeadPosition.x, screenHeight - HeadPosition.y);
                        draw->AddLine(screenCenter, targetHead, IM_COL32(255, 255, 0, 220), 1.5f);
                        draw->AddCircleFilled(targetHead, 4.0f, IM_COL32(255, 255, 0, 255)); // हेड लोकेटर नोड

                        // --- [3. टॉप-डाउन ओवरहेड लेज़र लाइन] ---
                        if (Config.ESP.Line) {
                            draw->AddLine(ImVec2(rect.x + rect.w / 2, 0), ImVec2(rect.x + rect.w / 2, rect.y), IM_COL32(0, 255, 160, 130), 1.5f);
                        }

                        // एडवांस स्लीक हेल्थ बार
                        if (Config.ESP.Health) {           
                            long clr = IM_COL32(0, 255, 150, 255); 
                            int maxHp = get_MaxHP(enemyPtr);
                            int currentHp = GetHp(enemyPtr);
                            if (maxHp > 0) {
                                if (currentHp <= (maxHp * 0.6)) clr = IM_COL32(255, 160, 0, 255);
                                if (currentHp < (maxHp * 0.3)) clr = IM_COL32(255, 0, 60, 255);
                                int xx = rect.x - 8; 
                                draw->AddRectFilled(ImVec2(xx, rect.y), ImVec2(xx + 3, rect.y + rect.h), IM_COL32(10, 10, 15, 220), 2.0f);        
                                draw->AddRectFilled(ImVec2(xx, rect.y + rect.h - (rect.h * ((float)currentHp / maxHp))), ImVec2(xx + 3, rect.y + rect.h), clr, 2.0f);
                            }
                        }

                        // --- [4. प्रीमियम कॉर्नर-कटेड ईएसपी बॉक्स] ---
                        if (Config.ESP.Box) {
                            float lW = rect.w * 0.25f;
                            float lH = rect.h * 0.25f;
                            ImVec2 topLeft(rect.x, rect.y);
                            ImVec2 bottomRight(rect.x + rect.w, rect.y + rect.h);

                            // Top-Left corner
                            draw->AddLine(topLeft, ImVec2(topLeft.x + lW, topLeft.y), electric_ruby, 2.5f);
                            draw->AddLine(topLeft, ImVec2(topLeft.x, topLeft.y + lH), electric_ruby, 2.5f);
                            // Top-Right corner
                            draw->AddLine(ImVec2(bottomRight.x, rect.y), ImVec2(bottomRight.x - lW, rect.y), electric_ruby, 2.5f);
                            draw->AddLine(ImVec2(bottomRight.x, rect.y), ImVec2(bottomRight.x, rect.y + lH), electric_ruby, 2.5f);
                            // Bottom-Left corner
                            draw->AddLine(ImVec2(rect.x, bottomRight.y), ImVec2(rect.x + lW, bottomRight.y), electric_ruby, 2.5f);
                            draw->AddLine(ImVec2(rect.x, bottomRight.y), ImVec2(rect.x, bottomRight.y - lH), electric_ruby, 2.5f);
                            // Bottom-Right corner
                            draw->AddLine(bottomRight, ImVec2(bottomRight.x - lW, bottomRight.y), electric_ruby, 2.5f);
                            draw->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y - lH), electric_ruby, 2.5f);

                            // इनर शैडो औरा प्रभाव
                            draw->AddRect(topLeft, bottomRight, IM_COL32(255, 0, 90, 40), 0, 0, 1.0f);
                        }

                        if (Config.ESP.MAFIA) {
                            monoString* Nick = get_NickName(enemyPtr);
                            std::string name = (Nick != nullptr) ? "TARGET" : "TARGET"; 
                            std::string distText = " [" + std::to_string((int)distance) + "M]";
                            std::string fullTag = name + distText;
                            
                            ImVec2 tagSize = ImGui::CalcTextSize(fullTag.c_str());
                            float posX = rect.x + (rect.w - tagSize.x) * 0.5f;
                            float posY = rect.y - 28.0f;

                            draw->AddRectFilled(ImVec2(posX - 6, posY - 2), ImVec2(posX + tagSize.x + 6, posY + tagSize.y + 2), IM_COL32(10, 10, 20, 230), 4.0f);
                            draw->AddRect(ImVec2(posX - 6, posY - 2), ImVec2(posX + tagSize.x + 6, posY + tagSize.y + 2), liquid_emerald, 4.0f, 0, 1.2f);
                            
                            draw->AddText(ImVec2(posX, posY), IM_COL32(255, 255, 255, 255), name.c_str());
                            ImVec2 nameOnlySize = ImGui::CalcTextSize(name.c_str());
                            draw->AddText(ImVec2(posX + nameOnlySize.x, posY), liquid_emerald, distText.c_str());
                        }
                    }
                }
            }
        }
    }

    if (Enable) {
        ImU32 crosshairColor = enemyInFOV ? IM_COL32(255, 255, 0, 255) : IM_COL32(0, 255, 160, 200);
        draw->AddCircleFilled(ImVec2(screenWidth / 2, screenHeight / 2), 4.0f, crosshairColor);
    }

    if (RenderWatermark) {
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImVec2 wmSize = ImVec2(240, 38);
        ImVec2 wmPos = ImVec2((displaySize.x - wmSize.x) / 2, 20); 
        ImVec2 wmEnd = ImVec2(wmPos.x + wmSize.x, wmPos.y + wmSize.y);
        
        draw->AddRectFilled(wmPos, wmEnd, IM_COL32(10, 10, 18, 230), 12.0f);
        draw->AddRect(wmPos, wmEnd, IM_COL32(255, 30, 60, 255), 12.0f, 0, 1.5f);
        
        std::string watermarkText = "NEXO MODS VIP";
        ImVec2 textSize = ImGui::CalcTextSize(watermarkText.c_str());
        ImVec2 textPos = ImVec2(wmPos.x + (wmSize.x - textSize.x) / 2, wmPos.y + (wmSize.y - textSize.y) / 2);
        draw->AddText(textPos, IM_COL32(255, 255, 255, 255), watermarkText.c_str());
    }

    if (totalEnemies > 0) {
        std::string enemyText = "ALIVE ENEMIES: " + std::to_string(totalEnemies);
        ImVec2 textSize = ImGui::CalcTextSize(enemyText.c_str());
        ImVec2 padding(16.0f, 6.0f);  
        ImVec2 center(screenWidth * 0.5f, 85.0f);

        ImVec2 boxMin(center.x - textSize.x * 0.5f - padding.x, center.y - textSize.y * 0.5f - padding.y);
        ImVec2 boxMax(center.x + textSize.x * 0.5f + padding.x, center.y + textSize.y * 0.5f + padding.y);

        draw->AddRectFilled(boxMin, boxMax, IM_COL32(15, 10, 25, 220), 6.0f);
        draw->AddRect(boxMin, boxMax, IM_COL32(255, 0, 90, 255), 6.0f, 0, 1.5f);

        ImVec2 textPos(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f);
        draw->AddText(textPos, IM_COL32(0, 255, 160, 255), enemyText.c_str());
    }
}

bool Guest;
bool (*ResetGuest)(void* _this);
bool _ResetGuest(void* _this) {
    if (Guest) { return true; }
    return ResetGuest(_this);
}

bool camxa;
float (*GetCameraHeightRateValue)(void *player);
float _GetCameraHeightRateValue(void *player) {
    if (player != nullptr) {
        if (camxa) { return 4.0f; }
    }
    return GetCameraHeightRateValue(player);
}

bool(*Bypass)(void* bypass); 
bool _Bypass(void* bypass) { return false; }

bool SniperFast = false;
bool (*_IsContinuousGen)(void *instance) = nullptr;
bool IsContinuousGen(void *instance) {
    if (instance != NULL && Enable && SniperFast) {
        return true;
    }
    return _IsContinuousGen(instance);
}

bool PcSwapAnim = false;
void (*orig_PlayChangeWeaponAnim)(void*, void*, float);
void hook_PlayChangeWeaponAnim(void* _this, void* a, float b) {
    if (Enable && PcSwapAnim) {
        return;
    }
    orig_PlayChangeWeaponAnim(_this, a, b);
}

bool Setindia = false;
int (*orig_ClientVersion)(void* _this);
int hook_ClientVersion(void* _this) {
    if (Setindia) {
        return 3;
    }
    return orig_ClientVersion(_this);
}

bool Fastswitch = false;
bool (*get_InSwapWeaponCD)(void* thiz);
bool _get_InSwapWeaponCD(void* thiz) {
    if (Fastswitch && Enable) {
        return false;
    }
    return get_InSwapWeaponCD(thiz);
}

bool AutoRun = false;
bool (*orig_get_IsFastRunning)(void *instance);
bool hook_get_IsFastRunning(void *instance) {
    if (AutoRun) {
        return true;
    }
    return orig_get_IsFastRunning(instance);
}

bool InfiniteRange = false; 
float (*orig_get_Range)(void *instance);
float hook_get_Range(void *instance) {
    if (Enable && instance && InfiniteRange) {
        return 9999.0f; 
    }
    return orig_get_Range(instance);
}
