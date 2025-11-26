#pragma once

namespace TetraEngine {
    class ViewProvider {

        static ViewProvider* current;
    public:
        virtual ~ViewProvider() = default;

        virtual void Enable() = 0;
        virtual void Disable() = 0;

        virtual float GetRatio() const = 0;
        virtual void SetRatio(float pRatio) = 0;
        virtual glm::mat4 GetViewMatrix() const = 0;
        virtual glm::mat4 GetProjection() const = 0;
        virtual void SetProjection(float pFovY, float pRatio, float pNearPlane, float pFarPlane) = 0;

        virtual glm::vec3 GetPosition() = 0;

        static ViewProvider* GetCurrent();
        static void SetCurrent(ViewProvider* pCurrent);

    };
} // TetraEngine