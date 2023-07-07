//
// Created by airlab on 7/7/23.
//

#include "manipulator_node.h"

// This function must be implemented in the .cpp file to create
// a plugin that can be loaded at run-time
BT_REGISTER_NODES(factory)
{
    ManipulatorNode::RegisterNodes(factory);
}

namespace ManipulatorNode
{

    BT::NodeStatus IsControllerConnected()
    {
//        bool state = mycobot::MyCobot::I().IsControllerConnected();
        std::cout << "[ Connection: OK ]" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus IsPowerOn()
    {
//        bool state = mycobot::MyCobot::I().PowerOn();
        std::cout << "[ Power: ON ]" << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus SuctionInterface::on()
    {
        _on = true;
        std::cout << "SuctionInterface::on" << std::endl;
//        mycobot::MyCobot::I().SetBasicOut(SOLENOID_PIN, 0);
//        mycobot::MyCobot::I().SetBasicOut(VACUUM_PIN, 0);
//        mycobot::MyCobot::I().SleepSecond(WAIT_TIME);

        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus SuctionInterface::off()
    {
        std::cout << "SuctionInterface::off" << std::endl;

//        mycobot::MyCobot::I().SetBasicOut(SOLENOID_PIN, 1);
//        mycobot::MyCobot::I().SetBasicOut(VACUUM_PIN, 1);
//        mycobot::MyCobot::I().SleepSecond(WAIT_TIME);

        _on = false;
        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus ApproachObject::tick()
    {
        std::cout << "ApproachObject: " << this->name() << std::endl;


//        // get current coordinate
//        mycobot::Angles angles = mycobot::MyCobot::I().GetAngles();
//        std::this_thread::sleep_for(200ms);
//        mycobot::Coords coords = mycobot::MyCobot::I().GetCoords();
//        angles = mycobot::MyCobot::I().GetAngles();
//        std::cout << "[" << angles[mycobot::J1] << ", " << angles[mycobot::J2] << ", " << angles[mycobot::J3] << ", "
//                  << angles[mycobot::J4] << ", " << angles[mycobot::J5] << ", " << angles[mycobot::J6] << "]";
//
//        mycobot::Angles goal_angles = { 1, 0, 0, 0, 0, 0 };
//        mycobot::MyCobot::I().WriteAngles(goal_angles,180);
//        while (!mycobot::MyCobot::I().IsInPosition(goal_angles, false)) {
//            angles = mycobot::MyCobot::I().GetAngles();
//            std::cout << "[" << angles[mycobot::J1] << ", " << angles[mycobot::J2] << ", "
//                      << angles[mycobot::J3] << ", " << angles[mycobot::J4] << ", "
//                      << angles[mycobot::J5] << ", " << angles[mycobot::J6] << "]" << std::flush;
//            std::this_thread::sleep_for(200ms);
//        }
//
//        std::this_thread::sleep_for(5000ms);
//        mycobot::MyCobot::I().StopRobot();

        return BT::NodeStatus::SUCCESS;
    }

    BT::NodeStatus MissionCoordinator::tick()
    {
        auto msg = getInput<std::string>("message");
        if (!msg)
        {
            throw BT::RuntimeError( "missing required input [message]: ", msg.error() );
        }

        std::cout << "Robot says: " << msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }



}
