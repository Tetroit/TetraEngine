#pragma once

#include <string>
#include <iostream>
#include <utility>
#include "../ecs/ComponentManager.h"
#include "../ecs/ECS.h"

namespace TetraEngine::Tests {

    class Vegetable {
        public:

        int amount;
        std::string veggieName;

        Vegetable(int amount, std::string  veggieName) : amount(amount), veggieName(std::move(veggieName)) {}
    };

    class Square {
        public:
        float side;

        Square(float side) : side(side) {}
    };

    class Rectangle {
        public:
        float side1;
        float side2;
        Rectangle(float side1, float side2) : side1(side1), side2(side2) {}
    };

    class Salad {
        std::vector<ECS::Handle<Vegetable>> vegetables;
    public:
        float amount;
        Salad(float amount) : amount(amount) {}

        void AddVeggie(ECS::Handle<Vegetable> vegetable) {
            vegetables.push_back(vegetable);
        }

        void RemoveVeggie(ECS::Handle<Vegetable> vegetable) {
            if (auto loc = std::ranges::find(vegetables, vegetable); loc != vegetables.end()) {
                vegetables.erase(loc);
            }
            else{
                LOG_ERR("No vegetable found");
            }
        }

        std::string ListVeggies(ECS::ECS& ecs) {
            std::string res;
            int cnt = 0;
            for (auto i = vegetables.begin(); i != vegetables.end(); ) {
                auto veg = ecs.GetComponent(*i);
                if (veg == nullptr) {
                    i = vegetables.erase(i);
                    continue;
                }
                res += veg->veggieName;
                res += " ";
                ++i;
                cnt++;
            }
            if (cnt == 0)
                return res;
            res = res.substr(0, res.size() - 1);
            return res;
        }
    };
    // class Sushi {
    //     public:
    //     std::vector<ComponentHandle<Vegetable>> vegetables;
    //     int pieces;
    //     std::string name;
    //
    //     Sushi(int pieces, std::string  name) : pieces(pieces), name(std::move(name)) {}
    //     void AddVeggie(const ComponentHandle<Vegetable>& vegetable) {
    //         vegetables.push_back(vegetable);
    //     }
    //
    //     void ValidateVegetables() {
    //         for (auto loc = vegetables.begin(); loc<vegetables.end();) {
    //             if (!*loc) {
    //                 loc = vegetables.erase(loc);
    //             }
    //             else {
    //                 ++loc;
    //             }
    //         }
    //     }
    // };

    class ECSTest {
        public:
        //  static void run() {
        //
        //      auto* componentManager = new ComponentManager();
        //      uint kitchen1 = componentManager->CreateEntity();
        //      uint kitchen2 = componentManager->CreateEntity();
        //      uint kitchen3 = componentManager->CreateEntity();
        //
        //      log_assert_from(componentManager->GetEntityCount() == 3,
        //      "Create entity test SUCCESS",
        //      "Create entity test FAILED",
        //      "ECSTest");
        //
        //      componentManager->CreateComponent<Vegetable>(kitchen1, 12, "Fish");
        //      componentManager->CreateComponent<Vegetable>(kitchen2, 4, "Banana");
        //      componentManager->CreateComponent<Sushi>(kitchen2, 6, "Maki");
        //      componentManager->CreateComponent<Vegetable>(kitchen3, 5, "Cucumber");
        //
        //      auto tomato = componentManager->GetComponent<Vegetable>(kitchen1);
        //      auto name = tomato->data->veggieName;
        //
        //      log_assert_from(name == "Fish", "GetComponent test SUCCESS", "GetComponent test FAILED", "ECSTest");
        //
        //      auto cucumber = componentManager->GetComponent<Vegetable>(kitchen3);
        //
        //      auto maki = componentManager->GetComponent<Sushi>(kitchen2);
        //      maki->data->AddVeggie(tomato);
        //      log_assert_from(maki->data->vegetables.size() == 1, "Component link test 1 SUCCESS", "Component link test 1 FAILED", "ECSTest");
        //      maki->data->AddVeggie(cucumber);
        //      log_assert_from(maki->data->vegetables.size() == 2, "Component link test 2 SUCCESS", "Component link test 2 FAILED", "ECSTest");
        //
        //      bool removeRes = componentManager->RemoveComponent<Vegetable>(kitchen3);
        //      log_assert_from(removeRes, "Remove component test SUCCESS", "Remove component test FAILED", "ECSTest");
        //      auto invalid = componentManager->GetComponent<Vegetable>(kitchen3);
        //      log_assert_from(!invalid, "Invalidate component test 1 SUCCESS", "Invalidate component test 1 FAILED", "ECSTest");
        //
        //      maki->data->ValidateVegetables();
        //      log_assert_from(maki->data->vegetables.size() == 1, "Invalidate component test 2 SUCCESS",
        //          "Invalidate component test 2 FAILED", "ECSTest");
        //      componentManager->RemoveEntity(kitchen2);
        //      log_assert_from(
        //          componentManager->GetComponentCountOfType<Vegetable>() == 1,
        //          "Remove entity test 1 SUCCESS", "Remove entity test 1 FAILED", "ECSTest");
        //
        //      log_assert_from(
        //      componentManager->GetEntityCount() == 2,
        //          "Remove entity test 2 SUCCESS", "Remove entity test 2 FAILED", "ECSTest");
        // }

        static ECS::Handle<Square>* Create10Squares(ECS::ECS& ecs, const ECS::Entity* entities) {

            ECS::Handle<Square>* squares = new ECS::Handle<Square>[10];

            for (int i=0; i < 10; i++) {
                squares[i] = ecs.CreateComponent<Square>(entities[i], i);
            }
            return squares;
        }
        static ECS::Handle<Rectangle>* Create10Rectangles(ECS::ECS& ecs, const ECS::Entity* entities) {

            auto rectangles = new ECS::Handle<Rectangle>[10];

            for (int i=0; i < 10; i++) {
                rectangles[i] = ecs.CreateComponent<Rectangle>(entities[2 * i], i, i + 5);
            }
            return rectangles;
        }
        static ECS::Entity* Create20Entities(ECS::ECS& ecs) {
            auto res = new ECS::Entity[20];
            for (int i=0; i < 20; i++) {
                res[i] = ecs.CreateEntity();
            }
            return res;
        }
        static void run() {

            auto vegetableStorage = ECS::Storage<Vegetable>();
            auto tomatoH = vegetableStorage.Create(2, 1, "Tomato");
            auto potato = Vegetable(1, "Potato");
            auto potatoH = vegetableStorage.Add(0, potato);
            auto veg = vegetableStorage.Get(potatoH);
            log_test(veg->veggieName == "Potato", "ECS STORAGE ADD AND ACCESS TEST");
            veg = vegetableStorage.Get(tomatoH);
            log_test(veg->veggieName == "Tomato", "ECS STORAGE CONSTRUCTOR TEST");
            veg = vegetableStorage.GetByEntity(2);
            log_test(veg->veggieName == "Tomato", "ECS STORAGE ACCESS BY ENTITY TEST");
            vegetableStorage.Remove(potatoH);
            veg = vegetableStorage.Get(potatoH);
            log_test(veg == nullptr, "ECS STORAGE REMOVAL TEST");
            vegetableStorage.RemoveFromEntity(2);
            veg = vegetableStorage.Get(tomatoH);
            log_test(veg == nullptr, "ECS STORAGE REMOVAL BY ENTITY TEST");

            ECS::ECS ecs;
            auto kitchen1 = ecs.CreateEntity();
            log_test(ecs.ValidateEntity(kitchen1), "ECS ENTITY CREATE TEST");
            potatoH = ecs.CreateComponent<Vegetable>(kitchen1, 1, "Potato");
            log_test(ecs.GetComponent(potatoH)->veggieName == "Potato", "ECS COMPONENT CREATE TEST");
            auto kitchen2 = ecs.CreateEntity();
            auto saladH1 = ecs.CreateComponent<Salad>(kitchen1, 1.5f);
            auto cucumberH = ecs.CreateComponent<Vegetable>(kitchen2, 5, "Cucumber");
            auto salad1 = ecs.GetComponent(saladH1);
            salad1->AddVeggie(ecs.GetHandle<Vegetable>(kitchen1));
            salad1->AddVeggie(ecs.GetHandle<Vegetable>(kitchen2));
            log_test(salad1->ListVeggies(ecs)=="Potato Cucumber", "ECS COMPONENT LINK TEST");
            auto fakeCucumberH = ecs.CreateComponent<Vegetable>(kitchen1, 1, "FakeCucumber");
            log_test(!fakeCucumberH.Valid(), "ECS COMPONENT DUPLICATE TEST");
            auto potatoHCopy = potatoH;
            ecs.RemoveComponent(potatoH);
            auto emptyPotato = ecs.GetComponent(potatoHCopy);
            log_test(emptyPotato == nullptr, "ECS MISSING COMPONENT TEST");
            potatoH = ecs.GetHandle<Vegetable>(kitchen1);
            log_test(!potatoH.Valid(), "ECS MISSING HANDLE TEST");
            potatoH = ecs.GetHandle<Vegetable>(kitchen1);
            log_test(salad1->ListVeggies(ecs)=="Cucumber", "ECS INVALID COMPONENT TEST");
            ecs.RemoveComponent<Vegetable>(kitchen2);
            auto hasCucumber = ecs.HasComponent<Vegetable>(kitchen2);
            log_test(!hasCucumber , "ECS REMOVE COMPONENT BY ENTITY TEST");
            ecs.RemoveEntity(kitchen2);
            auto missingSalad = ecs.GetComponent<Salad>(kitchen2);
            log_test(missingSalad == nullptr, "ECS INVALID ENTITY TEST");

            auto entityPool = Create20Entities(ecs);
            auto s = Create10Squares(ecs, entityPool);
            auto r = Create10Rectangles(ecs, entityPool);

            std::string testOut;
            ecs.Foreach<Square>([&](Square sq) {
                testOut += std::to_string((int)(sq.side * sq.side)) + " ";
            });

            log_test(testOut == "0 1 4 9 16 25 36 49 64 81 ", "ECS FOREACH TEST");
            testOut = "";

            ecs.Foreach<Rectangle, Square>([&](Rectangle rect, Square sq) {
                testOut += (std::to_string((int)(rect.side1 * rect.side2)) + " " + std::to_string((int)(sq.side * sq.side)) + " ");
            });

            log_test(testOut == "0 0 6 4 14 16 24 36 36 64 ", "ECS FOREACH TUPLE TEST");
        }
    };
} // TetraEngine