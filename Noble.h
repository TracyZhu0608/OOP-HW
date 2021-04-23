//
//  Noble.h
//  HW07:inheritance
//
// 

#ifndef Noble_h
#define Noble_h

#include <string>
#include <iostream>
#include <vector>

namespace WarriorCraft {

    class Protector;

    class Noble {
    public:
        // constructor
        Noble(const std::string& name);

        // getters and setters
        const std::string& getName() const;
        void changeAliveStatus();
        bool getAliveStatus() const;

        void battle(Noble& enemy);  // battle between Nobles
        void die();  // change the status and set strength to 0

        // pure virtual methods
        virtual double getStrength() const = 0;
        // if the instance is a Lord, update strength of every Protector
        
        // if it is a PersonWithStrengthToFight, update itself's strength
        virtual void updateStrength(double winner_strength,double loser_strength) = 0;
        
        // if the instance is a Lord, call defend for every Protector
        // if it is a PersonWithStrengthToFight, output "Ugh!"
        virtual void defend() const = 0;

        
    private:
        std::string name;
        bool alive;
    };  // class Noble




    class Lord : public Noble {
    public:
        Lord(const std::string& name);  // constructor

        double getStrength() const;
        void updateStrength(double winner_strength, double loser_strength);   // multiply strength of each protector with the ratio
        void defend() const;
        
        bool hires(Protector& protector);
        bool fire(Protector& protector);
        
        bool remove(Protector& protector);   // attempt to remove a protector (fire/runaway)

    private:
        std::vector<Protector*> army;
    };  // class Lord




    class PersonWithStrengthToFight : public Noble {
    public:
        PersonWithStrengthToFight(const std::string& name, double strength);  // constructor

        // getter and setter
        double getStrength() const override;
        void updateStrength(double winner_strength, double loser_strength) override;

        void defend() const override;

    private:
        double strength;
    };  // class PersonWithStrengthToFight

}  // namespace WarriorCraft


#endif /* Noble_h */
