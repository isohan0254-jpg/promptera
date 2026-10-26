#pragma once

#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include "imgui.h"
#include "imgui_internal.h"
#include "fonts/IconsFontAwesome6.h"

// ============================================================================
// 🎨 DYNAMIC THEME SYSTEM & COLOR CONSTANTS (NEXO BLACK & RED THEME DEFAULT)
// ============================================================================
enum ThemePreset {
    THEME_CRIMSON_RED = 0,  // NEXO Blood Red & Obsidian Black (Default!)
    THEME_CYBER_CYAN,       // Neon Cyan / Ice Blue
    THEME_HYPER_VIOLET,     // Cyberpunk Purple / Neon Violet
    THEME_MATRIX_EMERALD,   // Liquid Green / Neon Mint
    THEME_SUNSET_GOLD,      // Amber Gold / Flame
    THEME_RAINBOW_WAVE      // Dynamic RGB Wave
};

inline int currentTheme = THEME_CRIMSON_RED;
inline float menuAlpha = 0.95f;
inline bool enableParticles = true;
inline bool enableGlowPulse = true;
inline bool isMenuOpen = true;
inline float menuAnimProgress = 1.0f;
inline ImVec2 menuFloatOrbPos = ImVec2(90.0f, 200.0f);
inline int currentTab = 0;
inline float animTabPillY = 0.0f;

// Color Helpers
inline ImVec4 GetAccentColor(float alpha = 1.0f) {
    float time = (float)ImGui::GetTime();
    switch (currentTheme) {
        case THEME_CRIMSON_RED:
            return ImVec4(1.00f, 0.12f, 0.26f, alpha); // #FF1E42 Neon Blood Red
        case THEME_CYBER_CYAN:
            return ImVec4(0.00f, 0.88f, 1.00f, alpha);
        case THEME_HYPER_VIOLET:
            return ImVec4(0.68f, 0.35f, 1.00f, alpha);
        case THEME_MATRIX_EMERALD:
            return ImVec4(0.05f, 0.95f, 0.55f, alpha);
        case THEME_SUNSET_GOLD:
            return ImVec4(1.00f, 0.68f, 0.15f, alpha);
        case THEME_RAINBOW_WAVE: {
            float hue = fmodf(time * 0.15f, 1.0f);
            float r, g, b;
            ImGui::ColorConvertHSVtoRGB(hue, 0.85f, 1.0f, r, g, b);
            return ImVec4(r, g, b, alpha);
        }
        default:
            return ImVec4(1.00f, 0.12f, 0.26f, alpha);
    }
}

inline ImVec4 GetSecondaryAccent(float alpha = 1.0f) {
    float time = (float)ImGui::GetTime();
    switch (currentTheme) {
        case THEME_CRIMSON_RED:
            return ImVec4(0.80f, 0.04f, 0.16f, alpha); // #CC0A29 Deep Crimson
        case THEME_CYBER_CYAN:
            return ImVec4(0.40f, 0.22f, 1.00f, alpha);
        case THEME_HYPER_VIOLET:
            return ImVec4(1.00f, 0.20f, 0.70f, alpha);
        case THEME_MATRIX_EMERALD:
            return ImVec4(0.00f, 0.75f, 0.90f, alpha);
        case THEME_SUNSET_GOLD:
            return ImVec4(1.00f, 0.30f, 0.10f, alpha);
        case THEME_RAINBOW_WAVE: {
            float hue = fmodf(time * 0.15f + 0.28f, 1.0f);
            float r, g, b;
            ImGui::ColorConvertHSVtoRGB(hue, 0.85f, 1.0f, r, g, b);
            return ImVec4(r, g, b, alpha);
        }
        default:
            return ImVec4(0.80f, 0.04f, 0.16f, alpha);
    }
}

inline ImU32 GetAccentU32(float alpha = 1.0f) {
    return ImGui::ColorConvertFloat4ToU32(GetAccentColor(alpha));
}

inline ImU32 GetSecondaryU32(float alpha = 1.0f) {
    return ImGui::ColorConvertFloat4ToU32(GetSecondaryAccent(alpha));
}

// ============================================================================
// 🌌 BACKGROUND CYBER PARTICLE SYSTEM
// ============================================================================
struct CyberParticle {
    ImVec2 pos;
    ImVec2 vel;
    float radius;
    float alpha;
    float pulseSpeed;
};

static std::vector<CyberParticle> s_CyberParticles;

inline void RenderCyberParticles(ImDrawList* drawList, ImVec2 min, ImVec2 max) {
    if (!enableParticles || !drawList) return;
    
    if (s_CyberParticles.empty()) {
        s_CyberParticles.resize(32);
        for (auto& p : s_CyberParticles) {
            p.pos = ImVec2(min.x + (float)(rand() % (int)fmaxf(1.0f, max.x - min.x)),
                           min.y + (float)(rand() % (int)fmaxf(1.0f, max.y - min.y)));
            p.vel = ImVec2(((rand() % 100) / 100.0f - 0.5f) * 20.0f,
                           ((rand() % 100) / 100.0f - 0.5f) * 20.0f);
            p.radius = 1.2f + (rand() % 25) / 10.0f;
            p.alpha = 0.30f + (rand() % 45) / 100.0f;
            p.pulseSpeed = 1.2f + (rand() % 20) / 10.0f;
        }
    }
    
    float dt = ImGui::GetIO().DeltaTime;
    float time = (float)ImGui::GetTime();
    ImVec4 colA = GetAccentColor(1.0f);
    
    for (size_t i = 0; i < s_CyberParticles.size(); i++) {
        auto& p = s_CyberParticles[i];
        p.pos.x += p.vel.x * dt;
        p.pos.y += p.vel.y * dt;
        
        if (p.pos.x < min.x) p.pos.x = max.x;
        if (p.pos.x > max.x) p.pos.x = min.x;
        if (p.pos.y < min.y) p.pos.y = max.y;
        if (p.pos.y > max.y) p.pos.y = min.y;
        
        float a = p.alpha * (0.6f + 0.4f * sinf(time * p.pulseSpeed + (float)i));
        ImU32 pCol = ImColor(colA.x, colA.y, colA.z, a * 0.50f);
        drawList->AddCircleFilled(p.pos, p.radius, pCol);
        
        for (size_t j = i + 1; j < s_CyberParticles.size(); j++) {
            auto& p2 = s_CyberParticles[j];
            float dx = p.pos.x - p2.pos.x;
            float dy = p.pos.y - p2.pos.y;
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist < 55.0f) {
                float lineA = (1.0f - dist / 55.0f) * 0.20f * a;
                drawList->AddLine(p.pos, p2.pos, ImColor(colA.x, colA.y, colA.z, lineA), 1.0f);
            }
        }
    }
}

// ============================================================================
// 🎨 MODERN CYBERPUNK THEME STYLING (BLACK & RED)
// ============================================================================
inline void ApplyCyberStyle() {
    ImGuiStyle *style = &ImGui::GetStyle();
    style->WindowPadding = ImVec2(0, 0);
    style->WindowRounding = 12.0f;
    style->FramePadding = ImVec2(10, 8);
    style->ItemSpacing = ImVec2(8, 8);
    style->ItemInnerSpacing = ImVec2(8, 6);
    style->FrameRounding = 8.0f;
    style->ChildRounding = 10.0f;
    style->PopupRounding = 10.0f;
    style->ScrollbarRounding = 10.0f;
    style->ScrollbarSize = 5.0f;
    style->WindowBorderSize = 0.0f;
    style->FrameBorderSize = 0.0f;
    style->ChildBorderSize = 1.0f;

    ImVec4* colors = style->Colors;
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.08f, menuAlpha); // Deep Obsidian Black
    colors[ImGuiCol_ChildBg]                = ImVec4(0.09f, 0.08f, 0.11f, 0.90f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.07f, 0.10f, 0.96f);
    colors[ImGuiCol_Border]                 = ImVec4(0.30f, 0.10f, 0.15f, 0.40f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_Text]                   = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.52f, 0.60f, 1.00f);
    
    ImVec4 accent = GetAccentColor(1.0f);
    ImVec4 accentDim = GetAccentColor(0.25f);
    ImVec4 accentHover = GetAccentColor(0.85f);
    
    colors[ImGuiCol_FrameBg]                = ImVec4(0.14f, 0.11f, 0.15f, 0.90f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.20f, 0.14f, 0.20f, 0.90f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.16f, 0.24f, 0.90f);
    
    colors[ImGuiCol_Button]                 = accentDim;
    colors[ImGuiCol_ButtonHovered]          = accentHover;
    colors[ImGuiCol_ButtonActive]           = accent;
    
    colors[ImGuiCol_Header]                 = accentDim;
    colors[ImGuiCol_HeaderHovered]          = ImVec4(accent.x, accent.y, accent.z, 0.40f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(accent.x, accent.y, accent.z, 0.60f);
    
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.06f, 0.06f, 0.08f, 0.30f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.30f, 0.15f, 0.20f, 0.60f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = accent;
    colors[ImGuiCol_ScrollbarGrabActive]    = accent;
    
    colors[ImGuiCol_CheckMark]              = accent;
    colors[ImGuiCol_SliderGrab]             = accent;
    colors[ImGuiCol_SliderGrabActive]       = accentHover;
    
    colors[ImGuiCol_Separator]              = ImVec4(accent.x * 0.4f, accent.y * 0.4f, accent.z * 0.4f, 0.35f);
    colors[ImGuiCol_SeparatorHovered]       = accent;
    colors[ImGuiCol_SeparatorActive]        = accent;
    
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered]      = accent;
    colors[ImGuiCol_ResizeGripActive]       = accent;
}

// ============================================================================
// 🎛️ CUSTOM ANIMATED WIDGETS (ZERO TEXT OVERLAPPING)
// ============================================================================

// 1. Sleek Card Container (Uses ChannelsSplit to render background behind all contents without layout collapse)
inline void BeginCard(const char* title, const char* icon = nullptr) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->ChannelsSplit(2);
    dl->ChannelsSetCurrent(1); // Render elements into foreground channel

    ImGui::Spacing();
    ImGui::BeginGroup();
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec4 accent = GetAccentColor(1.0f);
    ImU32 accentCol = ImGui::ColorConvertFloat4ToU32(accent);
    
    // Left glowing indicator tag
    dl->AddRectFilled(ImVec2(p.x + 2.0f, p.y + 2.0f), ImVec2(p.x + 5.5f, p.y + ImGui::GetTextLineHeight() + 2.0f), accentCol, 2.0f);
    
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
    if (icon && strlen(icon) > 0) {
        ImGui::TextColored(accent, "%s", icon);
        ImGui::SameLine();
    }
    ImGui::TextColored(ImVec4(0.96f, 0.96f, 0.98f, 1.00f), "%s", title);
    
    ImGui::Spacing();
    ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(accent.x, accent.y, accent.z, 0.25f));
    ImGui::Separator();
    ImGui::PopStyleColor();
    ImGui::Spacing();
}

inline void EndCard() {
    ImGui::Spacing();
    ImGui::EndGroup();
    
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    
    // Card bounds expansion for padding
    min.x -= 6.0f;
    min.y -= 6.0f;
    max.x += 6.0f;
    max.y += 6.0f;
    
    ImVec4 accent = GetAccentColor(1.0f);
    dl->ChannelsSetCurrent(0); // Render card background into background channel
    
    // Glass card background (Dark obsidian glass with subtle red tint)
    dl->AddRectFilled(min, max, IM_COL32(16, 12, 16, 235), 10.0f);
    // Subtle glowing card border
    dl->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.25f)), 10.0f, 0, 1.0f);
    
    dl->ChannelsMerge();
    ImGui::Spacing();
}

// 2. Animated Glowing Toggle Switch (Dynamically sized with zero text overlap)
inline bool CyberToggle(const char* label, bool* state, const char* desc = nullptr) {
    if (!state) return false;
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    float availWidth = ImGui::GetContentRegionAvail().x;
    float lineH = ImGui::GetTextLineHeight();
    float rowHeight = desc ? (lineH * 2.0f + 16.0f) : (lineH + 16.0f);
    if (rowHeight < 38.0f) rowHeight = 38.0f;
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImRect bb(p, ImVec2(p.x + availWidth, p.y + rowHeight));
    
    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    if (pressed) {
        *state = !*state;
        ImGui::MarkItemEdited(id);
    }

    // Per-toggle smooth animation state
    static std::map<ImGuiID, float> animMap;
    float target = *state ? 1.0f : 0.0f;
    if (animMap.find(id) == animMap.end()) {
        animMap[id] = target;
    }
    animMap[id] = ImLerp(animMap[id], target, g.IO.DeltaTime * 14.0f);
    float anim = animMap[id];

    ImDrawList* dl = window->DrawList;
    
    // Row background hover highlight
    if (hovered || anim > 0.05f) {
        ImU32 rowBg = hovered ? IM_COL32(255, 255, 255, 12) : IM_COL32(255, 255, 255, 4);
        dl->AddRectFilled(bb.Min, bb.Max, rowBg, 6.0f);
    }

    // Text Color & Position (Separated cleanly so description never collides)
    ImVec4 accent = GetAccentColor(1.0f);
    ImU32 titleCol = ImLerp(IM_COL32(230, 230, 240, 255), IM_COL32(255, 255, 255, 255), anim);
    
    float textX = bb.Min.x + 8.0f;
    float titleY = desc ? (bb.Min.y + 4.0f) : (bb.Min.y + (rowHeight - lineH) * 0.5f);
    
    dl->AddText(ImVec2(textX, titleY), titleCol, label);
    
    if (desc) {
        float descY = titleY + lineH + 3.0f;
        dl->AddText(ImVec2(textX, descY), IM_COL32(140, 145, 165, 220), desc);
    }

    // Switch Track Geometry
    float swWidth = 50.0f;
    float swHeight = 24.0f;
    float swRadius = swHeight * 0.5f;
    ImVec2 swPos(bb.Max.x - swWidth - 8.0f, bb.Min.y + (rowHeight - swHeight) * 0.5f);
    
    // Color Interpolation (Off: Dark Slate, On: Vibrant Crimson Red)
    ImVec4 offCol = ImVec4(0.18f, 0.16f, 0.20f, 1.0f);
    ImVec4 onCol = accent;
    ImVec4 trackCol = ImLerp(offCol, onCol, anim);
    ImU32 trackU32 = ImGui::ColorConvertFloat4ToU32(trackCol);
    
    // Draw Track
    dl->AddRectFilled(swPos, ImVec2(swPos.x + swWidth, swPos.y + swHeight), trackU32, swRadius);
    
    // Glowing border when active
    if (anim > 0.05f) {
        ImU32 glowCol = ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, anim * 0.40f));
        dl->AddRect(swPos, ImVec2(swPos.x + swWidth, swPos.y + swHeight), glowCol, swRadius, 0, 2.0f);
    }

    // Knob Geometry & Smooth Gliding Movement
    float knobRadius = swRadius - 2.5f;
    float knobMinX = swPos.x + swRadius;
    float knobMaxX = swPos.x + swWidth - swRadius;
    float knobX = ImLerp(knobMinX, knobMaxX, anim);
    ImVec2 knobCenter(knobX, swPos.y + swRadius);
    
    // Knob Soft Shadow
    dl->AddCircleFilled(ImVec2(knobCenter.x + 1.0f, knobCenter.y + 1.5f), knobRadius, IM_COL32(0, 0, 0, 90), 24);
    
    // Knob Body (White)
    dl->AddCircleFilled(knobCenter, knobRadius, IM_COL32(255, 255, 255, 255), 24);
    
    // Knob Inner Accent Core
    if (anim > 0.1f) {
        dl->AddCircleFilled(knobCenter, knobRadius * 0.45f * anim, ImGui::ColorConvertFloat4ToU32(accent), 16);
    }
    
    return pressed;
}

// 3. Animated Glowing Custom Slider (Properly aligned with no text/track overlaps)
inline bool CyberSlider(const char* label, float* value, float min, float max, const char* format = "%.0f", const char* icon = nullptr) {
    if (!value) return false;
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    float availWidth = ImGui::GetContentRegionAvail().x;
    float lineH = ImGui::GetTextLineHeight();
    float totalHeight = lineH + 34.0f;
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImRect bb(p, ImVec2(p.x + availWidth, p.y + totalHeight));
    
    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id)) return false;

    ImDrawList* dl = window->DrawList;
    ImVec4 accent = GetAccentColor(1.0f);
    ImVec4 secAccent = GetSecondaryAccent(1.0f);

    // 1. Top row: Label & Icon
    float textX = bb.Min.x + 6.0f;
    float topY = bb.Min.y + 2.0f;
    if (icon && strlen(icon) > 0) {
        dl->AddText(ImVec2(textX, topY), ImGui::ColorConvertFloat4ToU32(accent), icon);
        ImVec2 iconSz = ImGui::CalcTextSize(icon);
        textX += iconSz.x + 8.0f;
    }
    dl->AddText(ImVec2(textX, topY), IM_COL32(230, 230, 240, 255), label);

    // Value Formatted Badge on Top Right
    char valBuf[64];
    snprintf(valBuf, sizeof(valBuf), format, *value);
    ImVec2 valSize = ImGui::CalcTextSize(valBuf);
    float badgeW = valSize.x + 16.0f;
    float badgeH = lineH + 4.0f;
    ImVec2 badgePos(bb.Max.x - badgeW - 6.0f, topY - 1.0f);
    
    dl->AddRectFilled(badgePos, ImVec2(badgePos.x + badgeW, badgePos.y + badgeH), IM_COL32(24, 18, 24, 220), 5.0f);
    dl->AddRect(badgePos, ImVec2(badgePos.x + badgeW, badgePos.y + badgeH), ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.45f)), 5.0f, 0, 1.0f);
    dl->AddText(ImVec2(badgePos.x + 8.0f, badgePos.y + (badgeH - valSize.y) * 0.5f), ImGui::ColorConvertFloat4ToU32(accent), valBuf);

    // 2. Track & Thumb Geometry (Cleanly placed below label row)
    float trackH = 8.0f;
    float trackY = topY + lineH + 12.0f;
    float trackMinX = bb.Min.x + 8.0f;
    float trackMaxX = bb.Max.x - 8.0f;
    float trackWidth = trackMaxX - trackMinX;
    
    ImRect trackBb(ImVec2(trackMinX, trackY - 10.0f), ImVec2(trackMaxX, trackY + 18.0f));
    
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(trackBb, id, &hovered, &held);
    
    bool changed = false;
    if (held) {
        float mouseX = g.IO.MousePos.x;
        float t = (mouseX - trackMinX) / trackWidth;
        t = ImClamp(t, 0.0f, 1.0f);
        float newVal = min + (max - min) * t;
        if (*value != newVal) {
            *value = newVal;
            changed = true;
            ImGui::MarkItemEdited(id);
        }
    }

    // Animation state for thumb expansion on hover
    static std::map<ImGuiID, float> animMap;
    float targetAnim = (hovered || held) ? 1.0f : 0.0f;
    if (animMap.find(id) == animMap.end()) animMap[id] = targetAnim;
    animMap[id] = ImLerp(animMap[id], targetAnim, g.IO.DeltaTime * 12.0f);
    float anim = animMap[id];

    float fraction = ImClamp((*value - min) / (max - min), 0.0f, 1.0f);
    float knobX = trackMinX + trackWidth * fraction;

    // Track Background
    dl->AddRectFilled(ImVec2(trackMinX, trackY), ImVec2(trackMaxX, trackY + trackH), IM_COL32(28, 22, 28, 255), 4.0f);
    
    // Progress Gradient Fill
    if (fraction > 0.005f) {
        dl->AddRectFilledMultiColor(
            ImVec2(trackMinX, trackY),
            ImVec2(knobX, trackY + trackH),
            ImGui::ColorConvertFloat4ToU32(secAccent),
            ImGui::ColorConvertFloat4ToU32(accent),
            ImGui::ColorConvertFloat4ToU32(accent),
            ImGui::ColorConvertFloat4ToU32(secAccent)
        );
    }

    // Glowing Thumb Knob
    float baseRadius = 8.0f;
    float thumbRadius = baseRadius + anim * 2.5f;
    ImVec2 thumbCenter(knobX, trackY + trackH * 0.5f);
    
    // Outer Glow Ring
    dl->AddCircleFilled(thumbCenter, thumbRadius + 3.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.35f + anim * 0.25f)), 24);
    // Outer Border
    dl->AddCircle(thumbCenter, thumbRadius + 1.0f, ImGui::ColorConvertFloat4ToU32(accent), 24, 2.0f);
    // Thumb Body
    dl->AddCircleFilled(thumbCenter, thumbRadius, IM_COL32(255, 255, 255, 255), 24);
    // Inner Accent Dot
    dl->AddCircleFilled(thumbCenter, thumbRadius * 0.45f, ImGui::ColorConvertFloat4ToU32(accent), 16);

    return changed;
}

// 4. Dedicated Status Card (For Network Block & Anti-Cheat display)
inline void CyberStatusCard(const char* title, const char* statusText, bool isDanger, const char* subtitle = nullptr) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return;

    float availWidth = ImGui::GetContentRegionAvail().x;
    float lineH = ImGui::GetTextLineHeight();
    float totalH = subtitle ? (lineH * 2.0f + 20.0f) : (lineH + 20.0f);
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImRect bb(p, ImVec2(p.x + availWidth, p.y + totalH));
    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, window->GetID(title))) return;

    ImDrawList* dl = window->DrawList;
    ImVec4 statusCol = isDanger ? ImVec4(1.00f, 0.15f, 0.25f, 1.0f) : ImVec4(0.10f, 0.90f, 0.45f, 1.0f);
    ImU32 statusU32 = ImGui::ColorConvertFloat4ToU32(statusCol);

    // Card Body
    dl->AddRectFilled(bb.Min, bb.Max, IM_COL32(18, 14, 18, 235), 8.0f);
    dl->AddRect(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(ImVec4(statusCol.x, statusCol.y, statusCol.z, 0.35f)), 8.0f, 0, 1.0f);

    // Pulsing Status Dot
    float time = (float)ImGui::GetTime();
    float pulse = (sinf(time * 3.5f) + 1.0f) * 0.5f;
    float dotX = bb.Min.x + 16.0f;
    float dotY = bb.Min.y + (subtitle ? (lineH * 0.5f + 8.0f) : (totalH * 0.5f));
    
    dl->AddCircleFilled(ImVec2(dotX, dotY), 4.5f + pulse * 2.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(statusCol.x, statusCol.y, statusCol.z, 0.35f)), 16);
    dl->AddCircleFilled(ImVec2(dotX, dotY), 4.0f, statusU32, 16);

    // Title Text
    dl->AddText(ImVec2(dotX + 14.0f, bb.Min.y + 6.0f), IM_COL32(240, 240, 245, 255), title);

    // Status Tag on Right
    ImVec2 tagSz = ImGui::CalcTextSize(statusText);
    float tagW = tagSz.x + 16.0f;
    float tagH = lineH + 4.0f;
    ImVec2 tagMin(bb.Max.x - tagW - 8.0f, bb.Min.y + 5.0f);
    ImVec2 tagMax(tagMin.x + tagW, tagMin.y + tagH);
    
    dl->AddRectFilled(tagMin, tagMax, ImGui::ColorConvertFloat4ToU32(ImVec4(statusCol.x, statusCol.y, statusCol.z, 0.20f)), 5.0f);
    dl->AddRect(tagMin, tagMax, ImGui::ColorConvertFloat4ToU32(ImVec4(statusCol.x, statusCol.y, statusCol.z, 0.60f)), 5.0f, 0, 1.0f);
    dl->AddText(ImVec2(tagMin.x + 8.0f, tagMin.y + (tagH - tagSz.y) * 0.5f), statusU32, statusText);

    // Subtitle
    if (subtitle) {
        dl->AddText(ImVec2(dotX + 14.0f, bb.Min.y + lineH + 8.0f), IM_COL32(140, 145, 165, 220), subtitle);
    }
}

// 5. Animated Segmented Button Selector
inline bool CyberSegmented(const char* label, int* selected, const char* const items[], int count) {
    if (!selected || count <= 0) return false;

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);

    float availWidth = ImGui::GetContentRegionAvail().x;
    float lineH = ImGui::GetTextLineHeight();
    float totalHeight = lineH + 38.0f;
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImRect bb(p, ImVec2(p.x + availWidth, p.y + totalHeight));

    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id)) return false;

    ImDrawList* dl = window->DrawList;
    ImVec4 accent = GetAccentColor(1.0f);

    // Label
    dl->AddText(ImVec2(bb.Min.x + 6.0f, bb.Min.y + 2.0f), IM_COL32(210, 215, 230, 255), label);

    // Segment Box
    float segBoxY = bb.Min.y + lineH + 6.0f;
    float segBoxH = 30.0f;
    float segBoxMinX = bb.Min.x + 4.0f;
    float segBoxMaxX = bb.Max.x - 4.0f;
    float segBoxW = segBoxMaxX - segBoxMinX;
    float itemW = segBoxW / (float)count;

    // Container Background
    dl->AddRectFilled(ImVec2(segBoxMinX, segBoxY), ImVec2(segBoxMaxX, segBoxY + segBoxH), IM_COL32(20, 16, 22, 255), 6.0f);
    dl->AddRect(ImVec2(segBoxMinX, segBoxY), ImVec2(segBoxMaxX, segBoxY + segBoxH), IM_COL32(45, 30, 40, 200), 6.0f, 0, 1.0f);

    // Animation state for sliding pill
    static std::map<ImGuiID, float> animMap;
    float targetX = (float)(*selected);
    if (animMap.find(id) == animMap.end()) animMap[id] = targetX;
    animMap[id] = ImLerp(animMap[id], targetX, g.IO.DeltaTime * 14.0f);
    float animX = animMap[id];

    // Draw Sliding Pill
    float pillMinX = segBoxMinX + animX * itemW + 2.0f;
    float pillMaxX = pillMinX + itemW - 4.0f;
    dl->AddRectFilled(ImVec2(pillMinX, segBoxY + 2.0f), ImVec2(pillMaxX, segBoxY + segBoxH - 2.0f), ImGui::ColorConvertFloat4ToU32(accent), 5.0f);

    bool changed = false;
    for (int i = 0; i < count; i++) {
        ImVec2 itemMin(segBoxMinX + i * itemW, segBoxY);
        ImVec2 itemMax(itemMin.x + itemW, segBoxY + segBoxH);
        
        ImGuiID itemId = window->GetID((std::string(label) + "_seg_" + std::to_string(i)).c_str());
        bool hovered, held;
        bool clicked = ImGui::ButtonBehavior(ImRect(itemMin, itemMax), itemId, &hovered, &held);
        
        if (clicked && *selected != i) {
            *selected = i;
            changed = true;
            ImGui::MarkItemEdited(id);
        }

        // Text
        ImVec2 txtSize = ImGui::CalcTextSize(items[i]);
        ImVec2 txtPos(itemMin.x + (itemW - txtSize.x) * 0.5f, itemMin.y + (segBoxH - txtSize.y) * 0.5f);
        
        ImU32 txtCol = (*selected == i) ? IM_COL32(10, 10, 14, 255) : (hovered ? IM_COL32(255, 255, 255, 255) : IM_COL32(180, 185, 200, 255));
        dl->AddText(txtPos, txtCol, items[i]);
    }

    return changed;
}

// 6. Animated Sidebar Nav Button
inline bool CyberNavButton(const char* label, const char* icon, bool selected, ImVec2 size = ImVec2(0, 46)) {
    if (size.x <= 0) size.x = ImGui::GetContentRegionAvail().x;
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImRect bb(p, ImVec2(p.x + size.x, p.y + size.y));

    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool clicked = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Animation state per button
    static std::map<ImGuiID, float> animMap;
    float target = selected ? 1.0f : (hovered ? 0.35f : 0.0f);
    if (animMap.find(id) == animMap.end()) animMap[id] = target;
    animMap[id] = ImLerp(animMap[id], target, g.IO.DeltaTime * 14.0f);
    float anim = animMap[id];

    ImDrawList* dl = window->DrawList;
    ImVec4 accent = GetAccentColor(1.0f);

    // Highlight Background
    if (anim > 0.01f) {
        ImVec4 bgCol = ImLerp(ImVec4(0.12f, 0.10f, 0.14f, 0.0f), ImVec4(accent.x * 0.22f, accent.y * 0.22f, accent.z * 0.22f, 0.85f), anim);
        dl->AddRectFilled(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(bgCol), 8.0f);
        
        // Active Indicator Stripe on Left
        if (selected) {
            float barH = size.y * 0.6f;
            float barY = bb.Min.y + (size.y - barH) * 0.5f;
            dl->AddRectFilled(ImVec2(bb.Min.x + 2.0f, barY), ImVec2(bb.Min.x + 5.5f, barY + barH), ImGui::ColorConvertFloat4ToU32(accent), 2.0f);
        }
    }

    // Icon and label positioning
    float iconX = bb.Min.x + 12.0f + (anim * 3.0f);
    float textX = iconX + 26.0f;
    float textY = bb.Min.y + (size.y - ImGui::GetTextLineHeight()) * 0.5f;

    ImVec4 iconCol = ImLerp(ImVec4(0.55f, 0.55f, 0.65f, 1.0f), accent, anim);
    ImVec4 textCol = ImLerp(ImVec4(0.70f, 0.70f, 0.80f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), anim);

    dl->AddText(ImVec2(iconX, textY), ImGui::ColorConvertFloat4ToU32(iconCol), icon);
    dl->AddText(ImVec2(textX, textY), ImGui::ColorConvertFloat4ToU32(textCol), label);

    return clicked;
}

// 7. Animated Action Button
inline bool CyberButton(const char* label, const char* icon = nullptr, ImVec2 size = ImVec2(0, 40), bool primary = false) {
    if (size.x <= 0) size.x = ImGui::GetContentRegionAvail().x;
    
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiID id = window->GetID(label);
    
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImRect bb(p, ImVec2(p.x + size.x, p.y + size.y));

    ImGui::ItemSize(bb, 0.0f);
    if (!ImGui::ItemAdd(bb, id)) return false;

    bool hovered, held;
    bool clicked = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    // Animation state
    static std::map<ImGuiID, float> animMap;
    float target = held ? 1.0f : (hovered ? 0.6f : 0.0f);
    if (animMap.find(id) == animMap.end()) animMap[id] = target;
    animMap[id] = ImLerp(animMap[id], target, g.IO.DeltaTime * 14.0f);
    float anim = animMap[id];

    ImDrawList* dl = window->DrawList;
    ImVec4 accent = GetAccentColor(1.0f);
    ImVec4 secAccent = GetSecondaryAccent(1.0f);

    if (primary) {
        // Gradient Button
        dl->AddRectFilledMultiColor(
            bb.Min, bb.Max,
            ImGui::ColorConvertFloat4ToU32(accent),
            ImGui::ColorConvertFloat4ToU32(secAccent),
            ImGui::ColorConvertFloat4ToU32(secAccent),
            ImGui::ColorConvertFloat4ToU32(accent)
        );
        if (hovered) {
            dl->AddRectFilled(bb.Min, bb.Max, IM_COL32(255, 255, 255, 30), 8.0f);
        }
    } else {
        // Outline Glass Button
        ImU32 bgCol = ImGui::ColorConvertFloat4ToU32(ImLerp(ImVec4(0.14f, 0.10f, 0.15f, 0.85f), ImVec4(accent.x * 0.3f, accent.y * 0.3f, accent.z * 0.3f, 0.95f), anim));
        dl->AddRectFilled(bb.Min, bb.Max, bgCol, 8.0f);
        dl->AddRect(bb.Min, bb.Max, ImGui::ColorConvertFloat4ToU32(ImLerp(ImVec4(0.30f, 0.15f, 0.20f, 0.7f), accent, anim)), 8.0f, 0, 1.2f);
    }

    // Content centering
    std::string fullText = (icon && strlen(icon) > 0) ? (std::string(icon) + "  " + label) : label;
    ImVec2 txtSize = ImGui::CalcTextSize(fullText.c_str());
    ImVec2 txtPos(bb.Min.x + (size.x - txtSize.x) * 0.5f, bb.Min.y + (size.y - txtSize.y) * 0.5f);
    
    ImU32 txtCol = primary ? IM_COL32(10, 10, 14, 255) : (hovered ? IM_COL32(255, 255, 255, 255) : IM_COL32(230, 235, 245, 255));
    dl->AddText(txtPos, txtCol, fullText.c_str());

    return clicked;
}

// ============================================================================
// 📱 FLOATING MENU ORB (EXPAND / MINIMIZE)
// ============================================================================
inline void RenderFloatingMenuOrb(int glWidth, int glHeight) {
    float time = (float)ImGui::GetTime();
    float pulse = (sinf(time * 3.5f) + 1.0f) * 0.5f;
    ImVec4 accent = GetAccentColor(1.0f);
    
    float orbRadius = 26.0f;
    ImVec2 orbCenter = menuFloatOrbPos;

    ImGui::SetNextWindowPos(ImVec2(orbCenter.x - orbRadius - 10, orbCenter.y - orbRadius - 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2((orbRadius + 10) * 2, (orbRadius + 10) * 2), ImGuiCond_Always);
    ImGui::Begin("##FloatingOrb", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 screenCenter(orbCenter.x, orbCenter.y);

    // Outer Pulsing Glow Rings
    float glowRadius1 = orbRadius + 3.0f + pulse * 5.0f;
    float glowRadius2 = orbRadius + 7.0f + pulse * 9.0f;
    dl->AddCircle(screenCenter, glowRadius2, ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.15f * (1.0f - pulse))), 36, 1.5f);
    dl->AddCircle(screenCenter, glowRadius1, ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.35f * (1.0f - pulse * 0.5f))), 36, 2.0f);

    // Orb Body
    dl->AddCircleFilled(screenCenter, orbRadius, IM_COL32(14, 10, 14, 240), 36);
    dl->AddCircle(screenCenter, orbRadius, ImGui::ColorConvertFloat4ToU32(accent), 36, 2.2f);

    // Center Icon (Fiery Neon Red Flame)
    const char* icon = ICON_FA_FIRE;
    ImVec2 iconSize = ImGui::CalcTextSize(icon);
    dl->AddText(ImVec2(screenCenter.x - iconSize.x * 0.5f, screenCenter.y - iconSize.y * 0.5f), ImGui::ColorConvertFloat4ToU32(accent), icon);

    // Touch Interaction
    ImGuiID orbId = ImGui::GetID("##OrbButton");
    ImRect orbBb(ImVec2(screenCenter.x - orbRadius, screenCenter.y - orbRadius), ImVec2(screenCenter.x + orbRadius, screenCenter.y + orbRadius));
    bool hovered, held;
    bool clicked = ImGui::ButtonBehavior(orbBb, orbId, &hovered, &held);

    if (held && ImGui::IsMouseDragging(0, 2.0f)) {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        menuFloatOrbPos.x += delta.x;
        menuFloatOrbPos.y += delta.y;
        menuFloatOrbPos.x = ImClamp(menuFloatOrbPos.x, orbRadius + 10.0f, (float)glWidth - orbRadius - 10.0f);
        menuFloatOrbPos.y = ImClamp(menuFloatOrbPos.y, orbRadius + 10.0f, (float)glHeight - orbRadius - 10.0f);
    } else if (clicked) {
        isMenuOpen = !isMenuOpen;
    }

    ImGui::End();
}

// ============================================================================
// 🌈 ANIMATED TOP WATERMARK HUD BANNER (NEXO MODS VIP)
// ============================================================================
inline void RenderTopWatermarkHUD(int glWidth) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) return;

    float time = (float)ImGui::GetTime();
    float pulse = (sinf(time * 2.8f) + 1.0f) * 0.5f;
    ImVec4 accent = GetAccentColor(1.0f);
    ImVec4 secAccent = GetSecondaryAccent(1.0f);

    float barW = 460.0f;
    float barH = 34.0f;
    float barX = (glWidth - barW) * 0.5f;
    float barY = 12.0f;

    ImVec2 minPos(barX, barY);
    ImVec2 maxPos(barX + barW, barY + barH);

    // Glass Background
    draw->AddRectFilled(minPos, maxPos, IM_COL32(12, 10, 14, 235), 8.0f);
    
    // Top Glowing Accent Line
    draw->AddRectFilledMultiColor(
        minPos,
        ImVec2(maxPos.x, minPos.y + 2.5f),
        ImGui::ColorConvertFloat4ToU32(accent),
        ImGui::ColorConvertFloat4ToU32(secAccent),
        ImGui::ColorConvertFloat4ToU32(secAccent),
        ImGui::ColorConvertFloat4ToU32(accent)
    );

    // Glowing Border
    draw->AddRect(minPos, maxPos, ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.4f + pulse * 0.3f)), 8.0f, 0, 1.2f);

    // Pulsing Status Dot
    float dotX = barX + 16.0f;
    float dotY = barY + barH * 0.5f;
    draw->AddCircleFilled(ImVec2(dotX, dotY), 4.0f + pulse * 1.5f, ImGui::ColorConvertFloat4ToU32(ImVec4(accent.x, accent.y, accent.z, 0.4f)), 16);
    draw->AddCircleFilled(ImVec2(dotX, dotY), 3.5f, ImGui::ColorConvertFloat4ToU32(accent), 16);

    // Brand Name (NEXO MODS VIP)
    draw->AddText(NULL, 15.0f, ImVec2(dotX + 12.0f, barY + 8.5f), IM_COL32(255, 255, 255, 255), "NEXO MODS VIP");
    
    // Dot Separator
    draw->AddCircleFilled(ImVec2(dotX + 122.0f, dotY), 2.0f, IM_COL32(140, 120, 140, 200));

    // OB55 Tag
    draw->AddText(NULL, 14.0f, ImVec2(dotX + 132.0f, barY + 9.0f), ImGui::ColorConvertFloat4ToU32(secAccent), "OB55 ULTRA");

    // Live FPS & Anti-Cheat Status
    char statsBuf[64];
    snprintf(statsBuf, sizeof(statsBuf), "FPS: %.0f  |  PROTECTED", ImGui::GetIO().Framerate);
    ImVec2 statsSize = ImGui::CalcTextSize(statsBuf);
    draw->AddText(NULL, 14.0f, ImVec2(maxPos.x - statsSize.x - 16.0f, barY + 9.0f), ImGui::ColorConvertFloat4ToU32(accent), statsBuf);
}
