
#pragma once


namespace TetraEngine {


    class IUpdatable {
    protected:
        IUpdatable() {

        }
        virtual void Update() = 0;
    };
    class UpdateManager {

    public:
        static void AddUpdatable();
    };
} // TetraEngine
