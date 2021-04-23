//
//  Protector.h
//  HW07:inheritance

//

#ifndef Warrior_h
#define Warrior_h

#include <string>
#include <iostream>
#include <vector>


namespace WarriorCraft {

    class Lord;

    class Protector {
    public:
        // constructor
        Protector(const std::string& name, double strength);

        // getters and setters
        const std::string& getName() const;
        double getStrength() const;
        void setStrength(double theStrength);
        void setLord(Lord* nobleptr);
        Lord* getLord() const;

        // runaway
        bool runaway();

        // pure virtual defend method
        virtual void defend() const = 0;

    private:
        std::string name;
        double strength;
        Lord* lord;
    };  // class Protector



    class Warrior : public Protector {
    public:
        using Protector::Protector;
        void defend() const override;
    };  // class Warrior

    class Wizard : public Protector {
    public:
        using Protector::Protector;
        void defend() const override;
    };  // class Wizard

    class Archer : public Warrior {
    public:
        using Warrior::Warrior;
        void defend() const override;
    };  // class Archer

    class Swordsman : public Warrior {
    public:
        using Warrior::Warrior;
        void defend() const override;
    };  // class Swordsmen

}  // namespace WarriorCraft

#endif /* Protector_h */
