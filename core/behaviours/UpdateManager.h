
#pragma once


namespace TetraEngine {

    class IUpdatable {
    protected:
        IUpdatable() = default;
    public:
        virtual ~IUpdatable() = default;
        virtual void Update(float dt) = 0;
        virtual void LateUpdate(float dt) {}
    };
    class UpdateManager {
    std::vector<IUpdatable*> toUpdate;
    public:
        void AddUpdatable(IUpdatable* updatable);
        void RemoveUpdatable(IUpdatable* updatable);
        void UpdateAll(float dt) const;
        void LateUpdateAll(float dt) const;
        void Clear();
    };
} // TetraEngine
