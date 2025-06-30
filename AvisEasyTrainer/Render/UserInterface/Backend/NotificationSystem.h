#pragma once

namespace UI {
    namespace NotificationSystem {


        enum class Position { TopLeft, TopRight, TopCenter, BottomRight, Auto };
        enum class Type { Info, Warning, Error, Success };

        struct Notification {
            std::string message;
            float duration = 3.0f;
            float timeCreated = 0.0f;
            Position position = Position::TopRight;
            Type type = Type::Info;
        };

        inline std::mutex notificationMutex;
        inline std::queue<Notification> notificationQueue;
        inline std::vector<Notification> activeNotifications;

        inline float GetTimeNow() {
            using namespace std::chrono;
            return duration<float>(steady_clock::now().time_since_epoch()).count();
        }

        inline Position GetAutoResolvedPosition() {
            const ImVec2 menu = menuBox.pos;
            const ImVec2 screen = GetParentWindowSize();
            const bool topHalf = menu.y < screen.y / 2;
            const bool rightHalf = menu.x > screen.x / 2;

            if (topHalf && rightHalf)  return Position::TopLeft;
            if (topHalf && !rightHalf) return Position::TopRight;
            if (!topHalf && rightHalf) return Position::BottomRight;
            return Position::TopCenter;
        }

        inline void Push(const std::string& msg, Position pos = Position::TopRight, float duration = 3.0f, Type type = Type::Info) {
            std::lock_guard lock(notificationMutex);
            if (pos == Position::Auto)
                pos = GetAutoResolvedPosition();
            notificationQueue.push({ msg, duration, GetTimeNow(), pos, type });
        }

        inline float EaseOutBack(float t, float s = 1.70158f) {
            t -= 1.0f;
            return t * t * ((s + 1.0f) * t + s) + 1.0f;
        }
        inline float EaseInBack(float t, float s = 1.70158f) {
            return t * t * ((s + 1.0f) * t - s);
        }

        inline ImVec2 GetAnimatedPosition(ImVec2 finalPos, Position pos, float elapsed, float totalDuration, float animDuration) {
            const float slideAmount = 50.0f;
            ImVec2 offset(0, 0);

            if (elapsed < animDuration) {
                float eased = EaseOutBack(elapsed / animDuration);
                switch (pos) {
                case Position::TopLeft:   offset.x = -slideAmount * (1.f - eased); break;
                case Position::TopRight:
                case Position::BottomRight: offset.x = +slideAmount * (1.f - eased); break;
                case Position::TopCenter: offset.y = -slideAmount * (1.f - eased); break;
                default: break;
                }
            }
            else if (elapsed > totalDuration - animDuration) {
                float eased = EaseInBack((elapsed - (totalDuration - animDuration)) / animDuration);
                switch (pos) {
                case Position::TopLeft:   offset.x = -slideAmount * eased; break;
                case Position::TopRight:
                case Position::BottomRight: offset.x = +slideAmount * eased; break;
                case Position::TopCenter: offset.y = -slideAmount * eased; break;
                default: break;
                }
            }

            return ImVec2(finalPos.x + offset.x, finalPos.y + offset.y);
        }

        inline void Render() {

            using  namespace Render::Draw;

            constexpr float Padding = 10.0f;
            constexpr float BoxWidth = 300.0f;
            constexpr float Spacing = 5.0f;
            constexpr float BarHeight = 4.0f;
            constexpr float AnimDuration = 0.4f;

            {
                std::lock_guard lock(notificationMutex);
                while (!notificationQueue.empty()) {
                    activeNotifications.push_back(notificationQueue.front());
                    notificationQueue.pop();
                }
            }

            const float now = GetTimeNow();

            // Remove expired notifications
            activeNotifications.erase(
                std::remove_if(activeNotifications.begin(), activeNotifications.end(),
                    [now](const Notification& n) { return now - n.timeCreated > n.duration; }),
                activeNotifications.end()
            );

            std::map<Position, float> positionStacks;

            for (auto& notif : activeNotifications) {
                const ImVec2 screen = GetParentWindowSize();

                float wrapWidth = BoxWidth - 24.0f;
                ImVec2 textSize = ImGui::CalcTextSize(notif.message.c_str(), nullptr, true, wrapWidth);
                float contentHeight = textSize.y + 20.0f;
                float totalHeight = contentHeight + BarHeight;

                float& offset = positionStacks[notif.position];

                ImVec2 basePos;
                switch (notif.position) {
                case Position::TopLeft:
                    basePos = { Padding, Padding + offset };
                    break;
                case Position::TopRight:
                    basePos = { screen.x - BoxWidth - Padding, Padding + offset };
                    break;
                case Position::TopCenter:
                    basePos = { (screen.x - BoxWidth) * 0.5f, Padding };
                    offset = totalHeight; // Only one notification stacked at TopCenter
                    break;
                case Position::BottomRight:
                    basePos = { screen.x - BoxWidth - Padding, screen.y - Padding - totalHeight - offset };
                    break;
                default: continue;
                }

                ImVec2 animPos = GetAnimatedPosition(basePos, notif.position, now - notif.timeCreated, notif.duration, AnimDuration);

                ImU32 bgColor = IM_COL32(30, 30, 30, 220);
                ImU32 textColor = IM_COL32(255, 255, 255, 255);
                ImU32 progressColor = IM_COL32(255, 255, 255, 200);

                switch (notif.type) {
                case Type::Info:    progressColor = IM_COL32(180, 180, 255, 200); break;
                case Type::Warning: progressColor = IM_COL32(255, 200, 0, 200); break;
                case Type::Error:   progressColor = IM_COL32(255, 50, 50, 200); break;
                case Type::Success: progressColor = IM_COL32(0, 220, 100, 200); break;
                }

                DrawRect(animPos, { BoxWidth, totalHeight }, bgColor, 8.0f);

                ImVec2 textPos = { animPos.x + 12.0f, animPos.y + 10.0f };
                AddWrappedText(notif.message, textPos, wrapWidth, textColor);

                float progress = 1.0f - (now - notif.timeCreated) / notif.duration;
                float progressWidth = BoxWidth * std::clamp(progress, 0.0f, 1.0f);
                DrawRect({ animPos.x, animPos.y + totalHeight - BarHeight }, { progressWidth, BarHeight }, progressColor, 4.0f);

                offset += totalHeight + Spacing;
            }
        }

        // Simple helper shortcuts
        inline void NotifyInfo(const std::string& msg, float duration = 3.0f) { Push(msg, Position::Auto, duration, Type::Info); }
        inline void NotifyWarning(const std::string& msg, float duration = 3.0f) { Push(msg, Position::Auto, duration, Type::Warning); }
        inline void NotifyError(const std::string& msg, float duration = 3.0f) { Push(msg, Position::Auto, duration, Type::Error); }
        inline void NotifySuccess(const std::string& msg, float duration = 3.0f) { Push(msg, Position::Auto, duration, Type::Success); }
        inline void NotifyAt(const std::string& msg, Position pos, Type type = Type::Info, float duration = 3.0f) { Push(msg, pos, duration, type); }
    }

}