// ============================================================================
// 👑 NEXO MODS VIP MENU - OB55 EDITION (ON TOP)
// ============================================================================
#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>
#include <ctime>
#include <xdl.h>
#include <KittyUtils.h>
#include <KittyMemory.h>
#include <Il2Cpp.h>
#include <SubstrateHook.h>
#include <CydiaSubstrate.h>
#include "NEXO MODS/Gui.hpp"
#include <NEXO MODS/main.h>
#include "fonts/FontAwesome6_solid.h"
#include "ImGui/Toggle.h"
#include "NEXO MODS/CyberUI.hpp"
#include "NEXO MODS/NetworkBlocker.hpp"

static float currentWidth = 720.0f;
static float currentHeight = 460.0f;
void hack_thread();
bool ImGuiOK = true;

void OpenURL(Il2CppString* url) {
    if (!url) return; 
    typedef void (*Application_OpenURL_ftn)(Il2CppString*);
    static Application_OpenURL_ftn open_url = nullptr;
    
    if (!open_url) {
        void* methodOffset = Il2CppGetMethodOffset(
            OBFUSCATE("UnityEngine.CoreModule.dll"), 
            OBFUSCATE("UnityEngine"), 
            OBFUSCATE("Application"), 
            OBFUSCATE("OpenURL"), 
            1
        );
        if (methodOffset) {
            open_url = (Application_OpenURL_ftn)methodOffset;
        }
    }
    
    if (open_url != nullptr) {
        open_url(url);
    }
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    std::thread(hack_thread).detach();
    return JNI_VERSION_1_6;
}

uintptr_t il2cpp_base = 0;
void *getRealAddr(ulong offset) {
    return reinterpret_cast<void*>(il2cpp_base + offset);
};

inline EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
inline EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);
    if (!g_IsSetup) {
        prevWidth = g_GlWidth;
        prevHeight = g_GlHeight;
        SetupImgui();
        g_IsSetup = true;
    }
    
    static bool isTouching = false;
    static float touchStartTime = 0.0f;
    
    ApplyCyberStyle();
    ImGuiIO& io = ImGui::GetIO(); 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();
    
    if (Class_Input__get_touchCount && Class_Input__GetTouch) {
        int touchCount = ((int (*)())(Class_Input__get_touchCount))();
        if (touchCount > 0) {
            UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(Class_Input__GetTouch))(0);
            float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
            io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
            if (!isTouching) {
                touchStartTime = ImGui::GetTime();
            }
            io.MouseDown[0] = true;
            isTouching = true;
        } else {
            if (isTouching && (ImGui::GetTime() - touchStartTime < 0.06f)) {
                io.MouseDown[0] = true;
            } else {
                UpdateSpeedhack();
                io.MouseDown[0] = false;
                isTouching = false;
            }  
        }
    }

    // 1. Drawing Game Visuals / ESP
    DrawESP(g_GlWidth, g_GlHeight);

    // 2. Animated HUD Watermark Banner (always visible)
    RenderTopWatermarkHUD(g_GlWidth);

    // 3. Floating Orb toggle / Main Cyber Menu
    if (!isMenuOpen) {
        // Show floating orb button when menu is closed
        RenderFloatingMenuOrb(g_GlWidth, g_GlHeight);
    } else {
        // =============================================
        // 🎮 NEXO VIP MAIN MENU WINDOW (BLACK & RED)
        // =============================================
        ImGui::SetNextWindowSize(ImVec2(currentWidth, currentHeight), ImGuiCond_FirstUseEver);
        ImGuiWindowFlags winFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
        
        ImGui::Begin("##NexoVIPMenu", nullptr, winFlags);
        
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        
        // Background Cyber Particles
        if (drawList) {
            RenderCyberParticles(drawList, winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y));
        }

        // Outer glowing window border + header rendering
        if (drawList) {
            ImVec4 accentCol = GetAccentColor(1.0f);
            ImVec4 secCol    = GetSecondaryAccent(1.0f);
            
            // Outer glowing border
            drawList->AddRect(winPos, ImVec2(winPos.x + winSize.x, winPos.y + winSize.y),
                ImGui::ColorConvertFloat4ToU32(ImVec4(accentCol.x, accentCol.y, accentCol.z, 0.45f)), 12.0f, 0, 1.5f);

            // Header Banner
            float headerH = 50.0f;
            ImVec2 hdrMin = winPos;
            ImVec2 hdrMax = ImVec2(winPos.x + winSize.x, winPos.y + headerH);
            drawList->AddRectFilled(hdrMin, hdrMax, IM_COL32(14, 10, 14, 250), 12.0f, ImDrawFlags_RoundCornersTop);
            
            // Top Accent Neon Glow Line
            drawList->AddRectFilledMultiColor(hdrMin, ImVec2(hdrMax.x, hdrMin.y + 3.0f),
                ImGui::ColorConvertFloat4ToU32(accentCol), ImGui::ColorConvertFloat4ToU32(secCol),
                ImGui::ColorConvertFloat4ToU32(secCol),    ImGui::ColorConvertFloat4ToU32(accentCol));
            
            // Brand Flame Icon
            drawList->AddText(NULL, 18.0f, ImVec2(winPos.x + 18.0f, winPos.y + 14.0f),
                ImGui::ColorConvertFloat4ToU32(accentCol), ICON_FA_FIRE);
            
            // Brand Title (NEXO MODS VIP)
            drawList->AddText(NULL, 17.0f, ImVec2(winPos.x + 44.0f, winPos.y + 14.0f),
                IM_COL32(255, 255, 255, 255), "NEXO MODS VIP");
            
            // OB55 Badge
            ImVec2 badgePos(winPos.x + 220.0f, winPos.y + 14.0f);
            drawList->AddRectFilled(badgePos, ImVec2(badgePos.x + 55.0f, badgePos.y + 22.0f), IM_COL32(255, 255, 255, 20), 4.0f);
            drawList->AddRect(badgePos, ImVec2(badgePos.x + 55.0f, badgePos.y + 22.0f),
                ImGui::ColorConvertFloat4ToU32(ImVec4(accentCol.x, accentCol.y, accentCol.z, 0.5f)), 4.0f, 0, 1.0f);
            drawList->AddText(NULL, 12.0f, ImVec2(badgePos.x + 7.0f, badgePos.y + 3.5f),
                ImGui::ColorConvertFloat4ToU32(secCol), "OB55");
            
            // Header Bottom Border
            drawList->AddLine(ImVec2(winPos.x, winPos.y + headerH),
                ImVec2(winPos.x + winSize.x, winPos.y + headerH), IM_COL32(45, 25, 35, 200), 1.0f);

            // Minimize button
            float cbW = 32.0f, cbH = 32.0f;
            ImVec2 closePos(winPos.x + winSize.x - cbW - 14.0f, winPos.y + 9.0f);
            ImGui::SetCursorScreenPos(closePos);
            if (ImGui::InvisibleButton("##CloseCyberMenuBtn", ImVec2(cbW, cbH)))
                isMenuOpen = false;
            ImU32 closeBg = ImGui::IsItemHovered() ? IM_COL32(255, 40, 60, 220) : IM_COL32(255, 255, 255, 18);
            drawList->AddRectFilled(closePos, ImVec2(closePos.x + cbW, closePos.y + cbH), closeBg, 6.0f);
            ImVec2 minSz = ImGui::CalcTextSize(ICON_FA_MINUS);
            drawList->AddText(ImVec2(closePos.x + (cbW - minSz.x)*0.5f, closePos.y + (cbH - minSz.y)*0.5f),
                IM_COL32(255, 255, 255, 255), ICON_FA_MINUS);
        }

        // Layout: sidebar + content (below header)
        float headerH2 = 50.0f;
        float sidebarWidth = 190.0f;
        float contentH = winSize.y - headerH2 - 20.0f;
        ImGui::SetCursorPos(ImVec2(10, headerH2 + 10));

        // --- LEFT NAVIGATION SIDEBAR ---
        ImGui::BeginChild("##LeftNavBar", ImVec2(sidebarWidth, contentH), true, ImGuiWindowFlags_NoScrollbar);
        ImGui::Spacing();
        if (CyberNavButton("Aimbot",       ICON_FA_CROSSHAIRS,    currentTab == 0)) currentTab = 0;
        ImGui::Spacing();
        if (CyberNavButton("Visuals",      ICON_FA_EYE,           currentTab == 1)) currentTab = 1;
        ImGui::Spacing();
        if (CyberNavButton("BLOCK (IP/Port)", ICON_FA_BAN,        currentTab == 2)) currentTab = 2;
        ImGui::Spacing();
        if (CyberNavButton("Misc / Boost", ICON_FA_BOLT,          currentTab == 3)) currentTab = 3;
        ImGui::Spacing();
        if (CyberNavButton("Themes",       ICON_FA_PALETTE,       currentTab == 4)) currentTab = 4;
        ImGui::Spacing();
        if (CyberNavButton("About",        ICON_FA_SHIELD_HALVED, currentTab == 5)) currentTab = 5;
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::SetCursorPosY(headerH2 + 10);

        // --- RIGHT CONTENT AREA ---
        ImGui::BeginChild("##TabContentArea", ImVec2(0, contentH), true);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));

        switch (currentTab) {
            case 0:
                BeginCard("AIMBOT CONFIGURATION", ICON_FA_CROSSHAIRS);
                CyberToggle("Master Aimbot Switch",   &Enable,  "Enable / disable the entire aimbot system");
                CyberToggle("Aim Prediction & Lock",  &Aimbot,  "Predict and lock onto enemy trajectory");
                CyberSlider("Aim FOV",                &Fov_Aim, 10.0f, 360.0f, "%.0f deg", ICON_FA_CROSSHAIRS);
                CyberSlider("Max Distance",           &Aimdis,  10.0f, 500.0f, "%.0f m",   ICON_FA_LOCATION_CROSSHAIRS);
                EndCard();
                break;

            case 1:
                BeginCard("ENEMY ESP OVERLAYS", ICON_FA_EYE);
                CyberToggle("Snapline Trace",         &Config.ESP.Line,   "Draw line from screen-centre to enemies");
                CyberToggle("Corner-Cut Box 2D",      &Config.ESP.Box,    "High-visibility bounding box");
                CyberToggle("Health Vital Bar",       &Config.ESP.Health, "Colour-shifting HP bar above enemies");
                CyberToggle("Name & Distance Tag",    &Config.ESP.MAFIA,  "Show name and exact distance label");
                EndCard();
                break;

            case 2: // 🔥 NEW NETWORK / IP & PORT BLOCKER SECTION
                CyberStatusCard("NETWORK INTERCEPTOR",
                                g_BlockAllNetwork ? "ALL IP/PORT BLOCKED" : "ONLINE / UNBLOCKED",
                                g_BlockAllNetwork,
                                g_BlockAllNetwork ? "All incoming & outgoing network sockets and packets dropped" : "Standard network traffic flowing normally");

                BeginCard("MASTER NETWORK & IP/PORT KILLSWITCH", ICON_FA_BAN);
                CyberToggle("BLOCK ALL IP & PORT TRAFFIC", &g_BlockAllNetwork, "Instantly blocks & drops all incoming/outgoing packets & sockets");
                EndCard();

                BeginCard("SECURITY & FILTER CONTROLS", ICON_FA_SHIELD_HALVED);
                CyberToggle("Anti-Cheat Telemetry Blocker", &g_BlockAntiCheatReports, "Blocks ban-report and telemetry upload server ports");
                CyberToggle("UDP Match Sync Freeze",       &g_BlockUdpTraffic,         "Freezes live match sync packets (Desync/Ping Freeze)");
                CyberToggle("TCP Server Logging Blocker",   &g_BlockTcpTraffic,         "Blocks crash dump & activity logging endpoints");
                CyberToggle("Log & Image Upload Blocker",   &g_BlockUploadLogs,         "Prevents screenshot and log packet uploads");
                EndCard();

                BeginCard("LIVE BLOCK STATISTICS", ICON_FA_CHART_SIMPLE);
                {
                    char statsBuf[128];
                    snprintf(statsBuf, sizeof(statsBuf), "Packets Blocked    : %llu", (unsigned long long)g_BlockedPackets.load());
                    ImGui::TextColored(GetAccentColor(1.0f), "%s", statsBuf);
                    snprintf(statsBuf, sizeof(statsBuf), "Connections Blocked: %llu", (unsigned long long)g_BlockedConnections.load());
                    ImGui::TextColored(GetSecondaryAccent(1.0f), "%s", statsBuf);
                    ImGui::Spacing();
                    if (CyberButton("Reset Packet Counters", ICON_FA_ARROWS_ROTATE, ImVec2(0, 36), false)) {
                        g_BlockedPackets = 0;
                        g_BlockedConnections = 0;
                    }
                }
                EndCard();
                break;

            case 3:
                BeginCard("PLAYER ENHANCEMENTS", ICON_FA_BOLT);
                CyberToggle("Flash SpeedHack",        &SpeedHack, "Supercharge movement speed");
                EndCard();
                BeginCard("OFFICIAL LINKS", ICON_FA_GLOBE);
                if (CyberButton("Nexo Mods Telegram Channel", ICON_FA_PAPER_PLANE, ImVec2(0, 42), true))
                    OpenURL(Il2CppString::Create("https://t.me/NEXOMODS"));
                EndCard();
                break;

            case 4: {
                BeginCard("COLOUR PALETTE PRESETS", ICON_FA_PALETTE);
                const char* themeOpts[] = { "Nexo Red (Default)","Cyber Cyan","Hyper Violet","Emerald Matrix","Sunset Gold","Dynamic RGB" };
                CyberSegmented("Theme Preset", &currentTheme, themeOpts, 6);
                EndCard();
                BeginCard("VISUAL EFFECTS", ICON_FA_WAND_MAGIC_SPARKLES);
                CyberToggle("Cyber Particles",        &enableParticles, "Animated background network nodes");
                CyberSlider("Menu Opacity",           &menuAlpha, 0.40f, 1.00f, "%.2f", ICON_FA_SLIDERS);
                EndCard();
                break;
            }

            case 5:
                BeginCard("BUILD & APP INFO", ICON_FA_SHIELD_HALVED);
                ImGui::TextColored(GetAccentColor(1.0f), "NEXO MODS VIP MENU");
                ImGui::TextDisabled("OB55 Cyber Crimson Edition (Black & Red)");
                ImGui::Separator();
                ImGui::Text("Arch    : ARM64-v8a / ARMv7");
                ImGui::Text("Engine  : Unity 3D / IL2CPP");
                ImGui::Text("Library : libNEXO.so");
                ImGui::Text("Status  : Undetected & Protected");
                ImGui::Text("Author  : NEXO MODS (ON TOP)");
                EndCard();
                break;
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::End();
    }   // end isMenuOpen else block

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}

static uintptr_t libBase;

inline void StartGUI() {
    void *ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
    if (!ptr_eglSwapBuffer) ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib64/libEGL.so", "eglSwapBuffers");
    if (!ptr_eglSwapBuffer) ptr_eglSwapBuffer = DobbySymbolResolver("libEGL.so", "eglSwapBuffers");
    if (ptr_eglSwapBuffer != nullptr) {
        DobbyHook((void *)ptr_eglSwapBuffer, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
        LOGD("GUI started successfully");
    }
}

void hack_thread() {
    sleep(5);
    pid_t pid = getpid();
    LOGD("NEXO Hack Thread initiated for PID: %i", pid);

    // Install socket blocker hooks
    InstallNetworkBlockerHooks();

    do {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        sleep(1);
    } while (il2cpp_base == 0);

    LOGD("libil2cpp.so found at address: 0x%" PRIxPTR, il2cpp_base);
    Il2CppAttach();

    void* m1 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("NewPlayerAnimationSystemComponent"), OBFUSCATE("PlayChangeWeaponAnim"), 2);
    if(m1) DobbyHook(m1, (void*)hook_PlayChangeWeaponAnim, (void**)&orig_PlayChangeWeaponAnim);

    void* m2 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW"), OBFUSCATE("GameFacade"), OBFUSCATE("ClientUsingVersion"), 0);
    if(m2) DobbyHook(m2, (void *)hook_ClientVersion, (void **)&orig_ClientVersion);

    void* m3 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_InSwapWeaponCD"), 0);
    if(m3) DobbyHook(m3, (void *)_get_InSwapWeaponCD, (void **) &get_InSwapWeaponCD);

    void* m4 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("Player"), OBFUSCATE("get_IsFastRunning"), 0);
    if(m4) DobbyHook(m4, (void *)hook_get_IsFastRunning, (void **)&orig_get_IsFastRunning);

    void* m5 = (void*)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE("COW.GamePlay"), OBFUSCATE("UGCLevelPointLight"), OBFUSCATE("get_Range"), 0);
    if(m5) DobbyHook(m5, (void*)hook_get_Range, (void**)&orig_get_Range);

    StartGUI();
}
